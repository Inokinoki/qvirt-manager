#include "ConnectionWorker.h"
#include <QDebug>
#include <QDateTime>
#include <cstring>

#ifdef LIBVIRT_FOUND
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>
#ifdef _WIN32
#undef state
#endif
#endif

namespace QVirt {

ConnectionWorker::ConnectionWorker(virConnectPtr conn, QObject *parent)
    : QObject(parent)
    , m_conn(conn)
    , m_stopped(false)
    , m_prevCpuTime(0)
    , m_prevIdleTime(0)
    , m_lastCpuUsage(0)
    , m_tickCounter(0)
{
}

ConnectionWorker::~ConnectionWorker() {}

void ConnectionWorker::requestStop()
{
    m_stopped.storeRelaxed(true);
}

void ConnectionWorker::stop()
{
    m_stopped.storeRelaxed(true);
}

DomainStats ConnectionWorker::collectDomainStats(virDomainPtr domain, qint64 prevCpuTime,
                                                  qint64 prevTimestamp, float prevCpuUsage)
{
    DomainStats stats;
    stats.cpuUsage = prevCpuUsage;

#ifdef LIBVIRT_FOUND
    const char *name = virDomainGetName(domain);
    stats.name = name ? QString::fromUtf8(name) : QString();

    virDomainInfo info;
    int ret = virDomainGetInfo(domain, &info);
    if (ret < 0) {
        stats.state = VIR_DOMAIN_NOSTATE;
        return stats;
    }

    stats.state = info.state;
    stats.maxMemory = info.maxMem;
    stats.currentMemory = info.memory;
    stats.vcpuCount = info.nrVirtCpu;
    stats.cpuTime = info.cpuTime;

    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (prevTimestamp > 0 && currentTime > prevTimestamp) {
        quint64 cpuTimeDelta = info.cpuTime - prevCpuTime;
        qint64 timeDelta = currentTime - prevTimestamp;
        if (timeDelta > 100 && stats.vcpuCount > 0) {
            float usage = (static_cast<float>(cpuTimeDelta) / 1000000.0f) /
                          static_cast<float>(timeDelta) / stats.vcpuCount * 100.0f;
            float maxUsage = stats.vcpuCount * 100.0f;
            if (usage < 0.0f) usage = 0.0f;
            if (usage > maxUsage) usage = maxUsage;
            stats.cpuUsage = usage;
        }
    }
#else
    Q_UNUSED(domain);
    Q_UNUSED(prevCpuTime);
    Q_UNUSED(prevTimestamp);
    Q_UNUSED(prevCpuUsage);
#endif
    return stats;
}

void ConnectionWorker::doInitialLoad()
{
#ifdef LIBVIRT_FOUND
    if (m_stopped.loadRelaxed() || !m_conn) {
        return;
    }

    PollResult result;
    result.alive = true;
    result.initialLoadDone = true;

    int numDomains = virConnectNumOfDomains(m_conn);
    int numInactiveDomains = virConnectNumOfDefinedDomains(m_conn);
    if (numDomains < 0) numDomains = 0;
    if (numInactiveDomains < 0) numInactiveDomains = 0;

    QSet<QString> &names = result.currentDomainNames;

    if (numDomains > 0) {
        int *domainIds = new int[numDomains];
        int actual = virConnectListDomains(m_conn, domainIds, numDomains);
        if (actual >= 0) {
            for (int i = 0; i < actual; i++) {
                virDomainPtr domainPtr = virDomainLookupByID(m_conn, domainIds[i]);
                if (domainPtr) {
                    DomainStats ds = collectDomainStats(domainPtr, 0, 0, 0.0f);
                    if (!ds.name.isEmpty()) {
                        names.insert(ds.name);
                        result.domainStats.append(ds);
                    }
                    virDomainFree(domainPtr);
                }
            }
        }
        delete[] domainIds;
    }

    if (numInactiveDomains > 0) {
        char **domainNamesArr = new char*[numInactiveDomains];
        int actual = virConnectListDefinedDomains(m_conn, domainNamesArr, numInactiveDomains);
        if (actual >= 0) {
            for (int i = 0; i < actual; i++) {
                QString domainName = QString::fromUtf8(domainNamesArr[i]);
                if (!names.contains(domainName)) {
                    virDomainPtr domainPtr = virDomainLookupByName(m_conn, domainNamesArr[i]);
                    if (domainPtr) {
                        DomainStats ds = collectDomainStats(domainPtr, 0, 0, 0.0f);
                        ds.name = domainName;
                        names.insert(ds.name);
                        result.domainStats.append(ds);
                        virDomainFree(domainPtr);
                    }
                }
                free(domainNamesArr[i]);
            }
        }
        delete[] domainNamesArr;
    }

    emit tickFinished(result);
#endif
}

void ConnectionWorker::doTick()
{
#ifdef LIBVIRT_FOUND
    if (m_stopped.loadRelaxed() || !m_conn) {
        return;
    }

    m_tickCounter++;

    if (virConnectIsAlive(m_conn) != 1) {
        emit connectionLost();
        return;
    }

    PollResult result;
    result.alive = true;
    result.initialLoadDone = false;

    int numDomains = virConnectNumOfDomains(m_conn);
    int numInactiveDomains = virConnectNumOfDefinedDomains(m_conn);
    if (numDomains < 0) numDomains = 0;
    if (numInactiveDomains < 0) numInactiveDomains = 0;

    QSet<QString> &names = result.currentDomainNames;

    if (numDomains > 0) {
        int *domainIds = new int[numDomains];
        int actual = virConnectListDomains(m_conn, domainIds, numDomains);
        if (actual >= 0) {
            for (int i = 0; i < actual; i++) {
                virDomainPtr domainPtr = virDomainLookupByID(m_conn, domainIds[i]);
                if (domainPtr) {
                    DomainStats ds = collectDomainStats(domainPtr, 0, 0, 0.0f);
                    if (!ds.name.isEmpty()) {
                        names.insert(ds.name);
                        result.domainStats.append(ds);
                    }
                    virDomainFree(domainPtr);
                }
            }
        }
        delete[] domainIds;
    }

    if (numInactiveDomains > 0) {
        char **domainNamesArr = new char*[numInactiveDomains];
        int actual = virConnectListDefinedDomains(m_conn, domainNamesArr, numInactiveDomains);
        if (actual >= 0) {
            for (int i = 0; i < actual; i++) {
                QString domainName = QString::fromUtf8(domainNamesArr[i]);
                if (!names.contains(domainName)) {
                    virDomainPtr domainPtr = virDomainLookupByName(m_conn, domainNamesArr[i]);
                    if (domainPtr) {
                        DomainStats ds = collectDomainStats(domainPtr, 0, 0, 0.0f);
                        ds.name = domainName;
                        names.insert(ds.name);
                        result.domainStats.append(ds);
                        virDomainFree(domainPtr);
                    }
                }
                free(domainNamesArr[i]);
            }
        }
        delete[] domainNamesArr;
    }

    emit tickFinished(result);
#endif
}

} // namespace QVirt
