/*
 * QVirt-Manager
 *
 * Copyright (C) 2025-2026 The QVirt-Manager Developers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef QVIRT_UI_DIALOGS_ADDHARDWAREDIALOG_H
#define QVIRT_UI_DIALOGS_ADDHARDWAREDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

#include "../../libvirt/Domain.h"
#include "../../devices/Device.h"

namespace QVirt {

/**
 * @brief Add Hardware Dialog
 *
 * Dialog to add new devices to a virtual machine
 */
class AddHardwareDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddHardwareDialog(Domain *domain, QWidget *parent = nullptr);
    ~AddHardwareDialog() override = default;

    Device* getCreatedDevice() const { return m_createdDevice; }

private slots:
    void onDeviceTypeChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void onAccepted();

private:
    void setupUI();
    void setupDeviceList();
    void setupDevicePages();

    Domain *m_domain;
    Device *m_createdDevice = nullptr;

    // UI components
    QListWidget *m_deviceList;
    QStackedWidget *m_pageStack;
    QDialogButtonBox *m_buttonBox;

    // Device type enum
    enum class DeviceType {
        Storage,
        Network,
        Controller,
        Input,
        Graphics,
        Video,
        Sound,
        HostDevice,
        Filesystem,
        TPM,
        RNG
    };
};

/**
 * @brief Base class for device configuration pages
 */
class DeviceConfigPage : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceConfigPage(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual Device* createDevice() = 0;
    virtual bool validate() { return true; }
};

/**
 * @brief Storage device configuration page
 */
class StorageDevicePage : public DeviceConfigPage
{
    Q_OBJECT

public:
    explicit StorageDevicePage(QWidget *parent = nullptr);

    Device* createDevice() override;
    bool validate() override;

private:
    void setupUI();

    QComboBox *m_deviceTypeCombo;
    QComboBox *m_busTypeCombo;
    QLineEdit *m_pathEdit;
    QSpinBox *m_sizeSpin;
    QComboBox *m_formatCombo;
    QCheckBox *m_readonlyCheck;
};

/**
 * @brief Network device configuration page
 */
class NetworkDevicePage : public DeviceConfigPage
{
    Q_OBJECT

public:
    explicit NetworkDevicePage(QWidget *parent = nullptr);

    Device* createDevice() override;
    bool validate() override;

private:
    void setupUI();

    QComboBox *m_networkTypeCombo;
    QComboBox *m_sourceCombo;
    QComboBox *m_modelCombo;
    QLineEdit *m_macEdit;
};

/**
 * @brief Controller device configuration page
 */
class ControllerDevicePage : public DeviceConfigPage
{
    Q_OBJECT

public:
    explicit ControllerDevicePage(QWidget *parent = nullptr);

    Device* createDevice() override;
    bool validate() override;

private:
    void setupUI();

    QComboBox *m_controllerTypeCombo;
    QComboBox *m_modelCombo;
    QSpinBox *m_indexSpin;
};

/**
 * @brief Input device configuration page
 */
class InputDevicePage : public DeviceConfigPage
{
    Q_OBJECT

public:
    explicit InputDevicePage(QWidget *parent = nullptr);

    Device* createDevice() override;
    bool validate() override;

private:
    void setupUI();

    QComboBox *m_inputTypeCombo;
    QComboBox *m_busCombo;
};

/**
 * @brief Graphics device configuration page
 */
class GraphicsDevicePage : public DeviceConfigPage
{
    Q_OBJECT

public:
    explicit GraphicsDevicePage(QWidget *parent = nullptr);

    Device* createDevice() override;
    bool validate() override;

private:
    void setupUI();

    QComboBox *m_graphicsTypeCombo;
    QCheckBox *m_autoPortCheck;
    QSpinBox *m_portSpin;
    QLineEdit *m_listenAddressEdit;
    QComboBox *m_keymapCombo;
};

/**
 * @brief Video device configuration page
 */
class VideoDevicePage : public DeviceConfigPage
{
    Q_OBJECT

public:
    explicit VideoDevicePage(QWidget *parent = nullptr);

    Device* createDevice() override;
    bool validate() override;

private:
    void setupUI();

    QComboBox *m_modelCombo;
    QSpinBox *m_vramSpin;
    QSpinBox *m_headsSpin;
    QCheckBox *m_accel3DCheck;
};

/**
 * @brief Sound device configuration page
 */
class SoundDevicePage : public DeviceConfigPage
{
    Q_OBJECT

public:
    explicit SoundDevicePage(QWidget *parent = nullptr);

    Device* createDevice() override;
    bool validate() override;

private:
    void setupUI();

    QComboBox *m_modelCombo;
};

} // namespace QVirt

#endif // QVIRT_UI_DIALOGS_ADDHARDWAREDIALOG_H
