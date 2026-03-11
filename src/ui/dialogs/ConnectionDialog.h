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

#ifndef QVIRT_UI_DIALOGS_CONNECTIONDIALOG_H
#define QVIRT_UI_DIALOGS_CONNECTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>

namespace QVirt {

// Forward declaration
struct ConnectionInfo;

/**
 * @brief Dialog for adding a new libvirt connection
 *
 * Supports local and remote connections with SSH/TLS/TCP transports
 */
class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget *parent = nullptr);
    explicit ConnectionDialog(const ConnectionInfo &info, QWidget *parent = nullptr);
    ~ConnectionDialog() override;

    QString uri() const { return m_uri; }
    QString originalURI() const { return m_originalURI; }
    bool autoconnect() const { return m_autoconnect; }
    QString sshKeyPath() const { return m_sshKeyPath; }
    QString sshUsername() const { return m_sshUsername; }
    QString sshPassword() const { return m_sshPassword; }
    bool savePassword() const { return m_savePassword; }
    bool isEditMode() const { return m_isEditMode; }

private slots:
    void validateAndAccept();
    void updateURI();
    void testConnection();
    void onRemoteToggled(bool enabled);
    void onTransportChanged(int index);

private:
    void setupUI();
    void loadConnectionInfo(const ConnectionInfo &info);
    QString buildURI() const;
    bool testConnection(const QString &testUri);

    // Basic connection UI
    QLineEdit *m_uriEdit;
    QComboBox *m_typeCombo;
    QCheckBox *m_autoconnectCheck;
    QPushButton *m_btnOK;
    QPushButton *m_btnCancel;

    // Remote connection UI
    QGroupBox *m_remoteGroup;
    QCheckBox *m_remoteCheck;
    QComboBox *m_transportCombo;
    QLineEdit *m_hostnameEdit;
    QLineEdit *m_usernameEdit;
    QLineEdit *m_portEdit;
    QLineEdit *m_sshKeyPathEdit;
    QPushButton *m_browseKeyPathBtn;
    QCheckBox *m_savePasswordCheck;
    QPushButton *m_testConnectionBtn;

    QString m_uri;
    QString m_originalURI;  // For edit mode - stores original URI
    bool m_autoconnect;
    QString m_sshKeyPath;
    QString m_sshUsername;
    QString m_sshPassword;
    bool m_savePassword;
    bool m_isEditMode;
};

} // namespace QVirt

#endif // QVIRT_UI_DIALOGS_CONNECTIONDIALOG_H
