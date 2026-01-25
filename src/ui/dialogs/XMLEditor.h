/*
 * QVirt-Manager
 *
 * Copyright (C) 2025-2026 Inoki <veyx.shaw@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version).
 */

#ifndef QVIRT_UI_DIALOGS_XMLEDITOR_H
#define QVIRT_UI_DIALOGS_XMLEDITOR_H

#include <QDialog>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCheckBox>

#include <libvirt/libvirt.h>

// Windows.h defines 'state' as a macro which breaks our code
#ifdef _WIN32
#undef state
#endif

namespace QVirt {

/**
 * @brief XML Editor Dialog
 *
 * Dialog for editing libvirt XML directly
 */
class XMLEditor : public QDialog
{
    Q_OBJECT

public:
    explicit XMLEditor(QWidget *parent = nullptr);
    ~XMLEditor() override = default;

    void setXML(const QString &xml);
    QString getXML() const;

    void setTitle(const QString &title);
    void setDescription(const QString &description);

    void setReadOnly(bool readOnly);
    void setValidateXML(bool validate);

private slots:
    void validate();
    void onTextChanged();
    void onFormatXML();
    void onReset();

private:
    void setupUI();
    bool isValidXML(const QString &xml);

    QPlainTextEdit *m_editor;
    QLabel *m_descriptionLabel;
    QLabel *m_statusLabel;
    QPushButton *m_btnValidate;
    QPushButton *m_btnFormat;
    QPushButton *m_btnReset;
    QPushButton *m_btnOK;
    QPushButton *m_btnCancel;

    QString m_originalXML;
    bool m_validateXML;
};

/**
 * @brief VM XML Editor Dialog
 *
 * Specialized XML editor for VM configuration
 */
class VMXMLEditor : public XMLEditor
{
    Q_OBJECT

public:
    explicit VMXMLEditor(const QString &vmName, QWidget *parent = nullptr);
    ~VMXMLEditor() override = default;

    void setDomainXML(const QString &xml, virDomainPtr domain);
    QString getDomainXML() const;

private:
    QString m_vmName;
};

} // namespace QVirt

#endif // QVIRT_UI_DIALOGS_XMLEDITOR_H
