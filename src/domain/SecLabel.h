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

#ifndef QVIRT_DOMAIN_SECLABEL_H
#define QVIRT_DOMAIN_SECLABEL_H

#include "../core/BaseObject.h"
#include <QString>

namespace QVirt {

/**
 * @brief Security labeling configuration
 *
 * Represents SELinux/AppArmor security labels
 */
class SecLabel : public BaseObject
{
    Q_OBJECT

public:
    enum class SecLabelType {
        Dynamic,
        Static,
        None
    };
    Q_ENUM(SecLabelType)

    enum class SecLabelModel {
        SELinux,
        AppArmor,
        DAC
    };
    Q_ENUM(SecLabelModel)

    explicit SecLabel(QObject *parent = nullptr);
    ~SecLabel() override = default;

    SecLabelType type() const { return m_type; }
    void setType(SecLabelType type);

    SecLabelModel model() const { return m_model; }
    void setModel(SecLabelModel model);

    QString label() const { return m_label; }
    void setLabel(const QString &label);

    bool relabel() const { return m_relabel; }
    void setRelabel(bool enabled);

    QString toXML() const;
    bool fromXML(const QString &xml);

    bool isEmpty() const;

    static QString typeToString(SecLabelType type);
    static SecLabelType stringToType(const QString &str);
    static QString modelToString(SecLabelModel model);
    static SecLabelModel stringToModel(const QString &str);

signals:
    void configChanged();

private:
    SecLabelType m_type = SecLabelType::Dynamic;
    SecLabelModel m_model = SecLabelModel::SELinux;
    QString m_label;
    bool m_relabel = true;
};

} // namespace QVirt

#endif // QVIRT_DOMAIN_SECLABEL_H
