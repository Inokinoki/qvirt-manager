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

#include <QtTest>
#include "../../src/devices/Device.h"
#include "../../src/devices/DiskDevice.h"
#include "../../src/devices/NetworkDevice.h"
#include "../../src/devices/ControllerDevice.h"
#include "../../src/devices/TPMDevice.h"
#include "../../src/devices/HostDevice.h"
#include "../../src/devices/FileSystemDevice.h"
#include "../../src/devices/WatchdogDevice.h"
#include "../../src/devices/RNGDevice.h"
#include "../../src/devices/SmartcardDevice.h"
#include "../../src/devices/MemballoonDevice.h"

using namespace QVirt;

/**
 * @brief Unit tests for Device classes
 */
class TestDevices : public QObject
{
    Q_OBJECT

private slots:
    void testDiskDevice();
    void testDiskDeviceXML();
    void testNetworkDevice();
    void testNetworkDeviceXML();
    void testControllerDevice();
    void testControllerDeviceXML();
    void testDeviceProperties();
    void testTPMDevice();
    void testTPMDeviceXML();
    void testHostDevice();
    void testHostDeviceXML();
    void testFileSystemDevice();
    void testFileSystemDeviceXML();
    void testWatchdogDevice();
    void testWatchdogDeviceXML();
    void testRNGDevice();
    void testRNGDeviceXML();
    void testSmartcardDevice();
    void testSmartcardDeviceXML();
    void testMemballoonDevice();
    void testMemballoonDeviceXML();
};

void TestDevices::testDiskDevice()
{
    DiskDevice disk;
    disk.setDevice(DiskDevice::DeviceType::Disk);
    disk.setDiskType(DiskDevice::DiskType::File);
    disk.setDriverType("qcow2");
    disk.setSource("/var/lib/libvirt/images/test.qcow2");
    disk.setBus(DiskDevice::BusType::Virtio);
    disk.setTarget("vda");

    QCOMPARE(disk.device(), DiskDevice::DeviceType::Disk);
    QCOMPARE(disk.diskType(), DiskDevice::DiskType::File);
    QCOMPARE(disk.driverType(), QString("qcow2"));
    QCOMPARE(disk.source(), QString("/var/lib/libvirt/images/test.qcow2"));
    QCOMPARE(disk.bus(), DiskDevice::BusType::Virtio);
    QCOMPARE(disk.target(), QString("vda"));
}

void TestDevices::testDiskDeviceXML()
{
    DiskDevice disk;
    disk.setDevice(DiskDevice::DeviceType::Disk);
    disk.setDiskType(DiskDevice::DiskType::File);
    disk.setDriverType("qcow2");
    disk.setSource("/var/lib/libvirt/images/test.qcow2");
    disk.setBus(DiskDevice::BusType::Virtio);
    disk.setTarget("vda");

    QString xml = disk.toXML();
    QVERIFY(xml.contains("<disk"));
    QVERIFY(xml.contains("type='file'"));
    QVERIFY(xml.contains("device='disk'"));
    QVERIFY(xml.contains("dev='vda'"));
    QVERIFY(xml.contains("bus='virtio'"));
}

void TestDevices::testNetworkDevice()
{
    NetworkDevice network;
    network.setNetworkType(NetworkDevice::NetworkType::Network);
    network.setSource("default");
    network.setModel(NetworkDevice::ModelType::Virtio);
    network.setMacAddress("52:54:00:00:00:01");

    QCOMPARE(network.networkType(), NetworkDevice::NetworkType::Network);
    QCOMPARE(network.source(), QString("default"));
    QCOMPARE(network.model(), NetworkDevice::ModelType::Virtio);
    QCOMPARE(network.macAddress(), QString("52:54:00:00:00:01"));
}

void TestDevices::testNetworkDeviceXML()
{
    NetworkDevice network;
    network.setNetworkType(NetworkDevice::NetworkType::Network);
    network.setSource("default");
    network.setModel(NetworkDevice::ModelType::Virtio);

    QString xml = network.toXML();
    QVERIFY(xml.contains("<interface"));
    QVERIFY(xml.contains("type='network'"));
    QVERIFY(xml.contains("network='default'"));
}

void TestDevices::testControllerDevice()
{
    ControllerDevice controller;
    controller.setControllerType(ControllerDevice::ControllerType::USB);
    controller.setIndex(0);
    controller.setUSBModel(ControllerDevice::USBModel::QemuXhci);

    QCOMPARE(controller.controllerType(), ControllerDevice::ControllerType::USB);
    QCOMPARE(controller.index(), 0);
    QCOMPARE(controller.usbModel(), ControllerDevice::USBModel::QemuXhci);
}

void TestDevices::testControllerDeviceXML()
{
    ControllerDevice controller;
    controller.setControllerType(ControllerDevice::ControllerType::USB);
    controller.setIndex(0);
    controller.setUSBModel(ControllerDevice::USBModel::QemuXhci);

    QString xml = controller.toXML();
    QVERIFY(xml.contains("<controller"));
    QVERIFY(xml.contains("type='usb'"));
}

void TestDevices::testDeviceProperties()
{
    // Test base Device class properties
    // Note: Device is an abstract class, so we test through a concrete device
    DiskDevice disk;
    disk.setAlias("test-alias");

    QCOMPARE(disk.alias(), QString("test-alias"));
}

void TestDevices::testTPMDevice()
{
    TPMDevice tpm;
    tpm.setModel(TPMDevice::TPMModel::TIS);
    tpm.setBackend(TPMDevice::TPMBackend::Emulator);
    tpm.setVersion(TPMDevice::TPMVersion::V2_0);

    QCOMPARE(tpm.model(), TPMDevice::TPMModel::TIS);
    QCOMPARE(tpm.backend(), TPMDevice::TPMBackend::Emulator);
    QCOMPARE(tpm.version(), TPMDevice::TPMVersion::V2_0);
}

void TestDevices::testTPMDeviceXML()
{
    TPMDevice tpm;
    tpm.setModel(TPMDevice::TPMModel::TIS);
    tpm.setBackend(TPMDevice::TPMBackend::Emulator);
    tpm.setVersion(TPMDevice::TPMVersion::V2_0);

    QString xml = tpm.toXML();
    QVERIFY(xml.contains("<tpm"));
    QVERIFY(xml.contains("model='tpm-tis'"));
    QVERIFY(xml.contains("type='emulator'"));
    QVERIFY(xml.contains("version='2.0'"));
}

void TestDevices::testHostDevice()
{
    HostDevice host;
    host.setHostDeviceType(HostDevice::HostDeviceType::USB);
    host.setUsbBus(1);
    host.setUsbDevice(2);

    QCOMPARE(host.hostDeviceType(), HostDevice::HostDeviceType::USB);
    QCOMPARE(host.usbBus(), 1);
    QCOMPARE(host.usbDevice(), 2);
}

void TestDevices::testHostDeviceXML()
{
    HostDevice host;
    host.setHostDeviceType(HostDevice::HostDeviceType::USB);
    host.setUsbMode(HostDevice::USBMode::Bus);
    host.setUsbBus(1);
    host.setUsbDevice(2);

    QString xml = host.toXML();
    QVERIFY(xml.contains("<hostdev"));
    QVERIFY(xml.contains("type='usb'"));
    QVERIFY(xml.contains("<source"));
    QVERIFY(xml.contains("bus='1'"));
    QVERIFY(xml.contains("device='2'"));
}

void TestDevices::testFileSystemDevice()
{
    FileSystemDevice fs;
    fs.setDriverType(FileSystemDevice::DriverType::VirtioFS);
    fs.setSourcePath("/host/path");
    fs.setTargetDir("/guest/path");
    fs.setMountTag("mytag");

    QCOMPARE(fs.driverType(), FileSystemDevice::DriverType::VirtioFS);
    QCOMPARE(fs.sourcePath(), QString("/host/path"));
    QCOMPARE(fs.targetDir(), QString("/guest/path"));
    QCOMPARE(fs.mountTag(), QString("mytag"));
}

void TestDevices::testFileSystemDeviceXML()
{
    FileSystemDevice fs;
    fs.setDriverType(FileSystemDevice::DriverType::VirtioFS);
    fs.setSourcePath("/host/path");
    fs.setTargetDir("/guest/path");

    QString xml = fs.toXML();
    QVERIFY(xml.contains("<filesystem"));
    QVERIFY(xml.contains("type='virtiofs'"));
    QVERIFY(xml.contains("dir='/host/path'"));
}

void TestDevices::testWatchdogDevice()
{
    WatchdogDevice watchdog;
    watchdog.setModel(WatchdogDevice::WatchdogModel::I6300ESB);
    watchdog.setAction(WatchdogDevice::WatchdogAction::Reset);

    QCOMPARE(watchdog.model(), WatchdogDevice::WatchdogModel::I6300ESB);
    QCOMPARE(watchdog.action(), WatchdogDevice::WatchdogAction::Reset);
}

void TestDevices::testWatchdogDeviceXML()
{
    WatchdogDevice watchdog;
    watchdog.setModel(WatchdogDevice::WatchdogModel::I6300ESB);
    watchdog.setAction(WatchdogDevice::WatchdogAction::Reset);

    QString xml = watchdog.toXML();
    QVERIFY(xml.contains("<watchdog"));
    QVERIFY(xml.contains("model='i6300esb'"));
    QVERIFY(xml.contains("action='reset'"));
}

void TestDevices::testRNGDevice()
{
    RNGDevice rng;
    rng.setModel(RNGDevice::RNGModel::Virtio);
    rng.setBackend(RNGDevice::RNGBackend::Random);
    rng.setSourcePath("/dev/urandom");

    QCOMPARE(rng.model(), RNGDevice::RNGModel::Virtio);
    QCOMPARE(rng.backend(), RNGDevice::RNGBackend::Random);
    QCOMPARE(rng.sourcePath(), QString("/dev/urandom"));
}

void TestDevices::testRNGDeviceXML()
{
    RNGDevice rng;
    rng.setModel(RNGDevice::RNGModel::Virtio);
    rng.setBackend(RNGDevice::RNGBackend::Random);
    rng.setSourcePath("/dev/urandom");

    QString xml = rng.toXML();
    QVERIFY(xml.contains("<rng"));
    QVERIFY(xml.contains("model='virtio'"));
    QVERIFY(xml.contains("type='random'"));
    QVERIFY(xml.contains("/dev/urandom"));
}

void TestDevices::testSmartcardDevice()
{
    SmartcardDevice smartcard;
    smartcard.setMode(SmartcardDevice::SmartcardMode::Host);
    smartcard.setType(SmartcardDevice::SmartcardType::SpiceVMC);

    QCOMPARE(smartcard.mode(), SmartcardDevice::SmartcardMode::Host);
    QCOMPARE(smartcard.type(), SmartcardDevice::SmartcardType::SpiceVMC);
}

void TestDevices::testSmartcardDeviceXML()
{
    SmartcardDevice smartcard;
    smartcard.setMode(SmartcardDevice::SmartcardMode::Host);
    smartcard.setType(SmartcardDevice::SmartcardType::SpiceVMC);

    QString xml = smartcard.toXML();
    QVERIFY(xml.contains("<smartcard"));
    QVERIFY(xml.contains("mode='host'"));
    QVERIFY(xml.contains("spicevmc"));
}

void TestDevices::testMemballoonDevice()
{
    MemballoonDevice balloon;
    balloon.setModel(MemballoonDevice::MemballoonModel::Virtio);
    balloon.setAutodeflate(true);
    balloon.setPeriod(5);

    QCOMPARE(balloon.model(), MemballoonDevice::MemballoonModel::Virtio);
    QCOMPARE(balloon.autodeflate(), true);
    QCOMPARE(balloon.period(), 5);
}

void TestDevices::testMemballoonDeviceXML()
{
    MemballoonDevice balloon;
    balloon.setModel(MemballoonDevice::MemballoonModel::Virtio);
    balloon.setAutodeflate(true);

    QString xml = balloon.toXML();
    QVERIFY(xml.contains("<memballoon"));
    QVERIFY(xml.contains("model='virtio'"));
    QVERIFY(xml.contains("autodeflate='on'"));
}

QTEST_MAIN(TestDevices)
#include "test_devices.moc"
