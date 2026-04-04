#ifndef QVIRT_LIBVIRT_CONNECTIONWORKER_H
#define QVIRT_LIBVIRT_CONNECTIONWORKER_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QAtomicInt>
#include <QSet>
#include <QList>
#include <QString>
#include <QtGlobal>

#ifdef LIBVIRT_FOUND
#include <libvirt/libvirt.h>
#ifdef _WIN32
#undef state
#endif
#else
typedef void *virConnectPtr;
#endif

namespace QVirt {

struct DomainStats {
    QString name;
    int state;
    quint64 maxMemory;
    quint64 currentMemory;
    int vcpuCount;
    quint64 cpuTime;
    float cpuUsage;
};

struct PollResult {
    bool alive;
    QList<DomainStats> domainStats;
    QSet<QString> currentDomainNames;
    bool initialLoadDone;
};

class ConnectionWorker : public QObject
{
    Q_OBJECT

public:
    explicit ConnectionWorker(virConnectPtr conn, QObject *parent = nullptr);
    ~ConnectionWorker() override;

    void requestStop();

    void setPrevCpuTime(quint64 cpuTime) { m_prevCpuTime = cpuTime; }
    void setPrevIdleTime(quint64 idleTime) { m_prevIdleTime = idleTime; }

    quint64 prevCpuTime() const { return m_prevCpuTime; }
    quint64 prevIdleTime() const { return m_prevIdleTime; }
    int lastCpuUsage() const { return m_lastCpuUsage; }

signals:
    void tickFinished(const PollResult &result);
    void connectionLost();

public slots:
    void doInitialLoad();
    void doTick();
    void stop();

private:
    DomainStats collectDomainStats(virDomainPtr domain, qint64 prevCpuTime, qint64 prevTimestamp, float prevCpuUsage);
    void checkAlive();

    virConnectPtr m_conn;
    QAtomicInt m_stopped;

    qint64 m_prevCpuTime;
    qint64 m_prevIdleTime;
    int m_lastCpuUsage;

    qint64 m_tickCounter;
};

} // namespace QVirt

#endif // QVIRT_LIBVIRT_CONNECTIONWORKER_H
