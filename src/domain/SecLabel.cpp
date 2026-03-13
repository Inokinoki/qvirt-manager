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

#include "SecLabel.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

namespace QVirt {

SecLabel::SecLabel(QObject *parent)
    : BaseObject(parent)
{
}

void SecLabel::setType(SecLabelType type)
{
    if (m_type != type) {
        m_type = type;
        emit configChanged();
    }
}

void SecLabel::setModel(SecLabelModel model)
{
    if (m_model != model) {
        m_model = model;
        emit configChanged();
    }
}

void SecLabel::setLabel(const QString &label)
{
    if (m_label != label) {
        m_label = label;
        emit configChanged();
    }
}

void SecLabel::setRelabel(bool enabled)
{
    if (m_relabel != enabled) {
        m_relabel = enabled;
        emit configChanged();
    }
}

QString SecLabel::toXML() const
{
    if (isEmpty()) {
        return QString();
    }

    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(false);
    writer.writeStartElement("seclabel");
    writer.writeAttribute("type", typeToString(m_type));
    writer.writeAttribute("model", modelToString(m_model));

    if (!m_relabel) {
        writer.writeAttribute("relabel", "no");
    }

    if (!m_label.isEmpty()) {
        writer.writeCharacters(m_label);
    }

    writer.writeEndElement();
    return xml;
}

bool SecLabel::fromXML(const QString &xml)
{
    QXmlStreamReader reader(xml);
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == QLatin1String("seclabel")) {
                QString typeStr = reader.attributes().value("type").toString();
                m_type = stringToType(typeStr);
                
                QString modelStr = reader.attributes().value("model").toString();
                m_model = stringToModel(modelStr);
                
                QString relabel = reader.attributes().value("relabel").toString();
                m_relabel = (relabel != QLatin1String("no"));
                
                m_label = reader.readElementText();
            }
        }
        
        if (reader.isEndElement() && reader.name() == QLatin1String("seclabel")) {
            break;
        }
    }
    
    return !reader.hasError();
}

bool SecLabel::isEmpty() const
{
    return m_type == SecLabelType::Dynamic && m_model == SecLabelModel::SELinux &&
           m_label.isEmpty() && m_relabel;
}

QString SecLabel::typeToString(SecLabelType type)
{
    switch (type) {
        case SecLabelType::Static: return QStringLiteral("static");
        case SecLabelType::None: return QStringLiteral("none");
        default: return QStringLiteral("dynamic");
    }
}

SecLabel::SecLabelType SecLabel::stringToType(const QString &str)
{
    if (str == QLatin1String("static")) return SecLabelType::Static;
    if (str == QLatin1String("none")) return SecLabelType::None;
    return SecLabelType::Dynamic;
}

QString SecLabel::modelToString(SecLabelModel model)
{
    switch (model) {
        case SecLabelModel::AppArmor: return QStringLiteral("apparmor");
        case SecLabelModel::DAC: return QStringLiteral("dac");
        default: return QStringLiteral("selinux");
    }
}

SecLabel::SecLabelModel SecLabel::stringToModel(const QString &str)
{
    if (str == QLatin1String("apparmor")) return SecLabelModel::AppArmor;
    if (str == QLatin1String("dac")) return SecLabelModel::DAC;
    return SecLabelModel::SELinux;
}

} // namespace QVirt
