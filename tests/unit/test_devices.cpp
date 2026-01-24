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

QTEST_MAIN(TestDevices)
#include "test_devices.moc"
