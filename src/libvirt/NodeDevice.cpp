/*
 * QVirt-Manager
 *
 * Copyright (C) 2025-2026 Inoki <veyx.shaw@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "NodeDevice.h"
#include "Connection.h"
#include <QDebug>

namespace QVirt {

NodeDevice::NodeDevice(Connection *conn, virNodeDevicePtr device)
    : BaseObject(conn)
    , m_connection(conn)
    , m_device(device)
{
    const char *name = virNodeDeviceGetName(m_device);
    if (name) {
        m_name = QString::fromUtf8(name);
    }

    const char *parent = virNodeDeviceGetParent(m_device);
    if (parent) {
        m_parent = QString::fromUtf8(parent);
    }

    // Get and cache XML description
    char *xml = virNodeDeviceGetXMLDesc(m_device, 0);
    if (xml) {
        m_cachedXmlDesc = QString::fromUtf8(xml);
        m_xmlFetched = true;
        free(xml);
    }

    qDebug() << "Created NodeDevice wrapper for" << m_name;
}

NodeDevice::~NodeDevice()
{
    if (m_device) {
        virNodeDeviceFree(m_device);
        m_device = nullptr;
    }
}

QString NodeDevice::getXMLDesc(unsigned int flags)
{
    if (!m_device) {
        return QString();
    }

    // Return cached XML if already fetched (avoid repeated remote calls)
    if (m_xmlFetched && !m_cachedXmlDesc.isEmpty()) {
        return m_cachedXmlDesc;
    }

    char *xml = virNodeDeviceGetXMLDesc(m_device, flags);
    if (!xml) {
        return QString();
    }

    QString xmlStr = QString::fromUtf8(xml);
    free(xml);

    // Cache the XML for future calls
    m_cachedXmlDesc = xmlStr;
    m_xmlFetched = true;
    return xmlStr;
}

void NodeDevice::updateInfo()
{
    if (!m_device) {
        return;
    }

    // Refresh and cache XML
    char *xml = virNodeDeviceGetXMLDesc(m_device, 0);
    if (xml) {
        m_cachedXmlDesc = QString::fromUtf8(xml);
        m_xmlFetched = true;
        free(xml);
    }
}

} // namespace QVirt
