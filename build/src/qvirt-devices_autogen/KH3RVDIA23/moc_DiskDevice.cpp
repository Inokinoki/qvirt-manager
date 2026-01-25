/****************************************************************************
** Meta object code from reading C++ file 'DiskDevice.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "devices/DiskDevice.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DiskDevice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QVirt__DiskDevice_t {
    QByteArrayData data[30];
    char stringdata0[216];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__DiskDevice_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__DiskDevice_t qt_meta_stringdata_QVirt__DiskDevice = {
    {
QT_MOC_LITERAL(0, 0, 17), // "QVirt::DiskDevice"
QT_MOC_LITERAL(1, 18, 8), // "DiskType"
QT_MOC_LITERAL(2, 27, 4), // "File"
QT_MOC_LITERAL(3, 32, 5), // "Block"
QT_MOC_LITERAL(4, 38, 3), // "Dir"
QT_MOC_LITERAL(5, 42, 7), // "Network"
QT_MOC_LITERAL(6, 50, 6), // "Volume"
QT_MOC_LITERAL(7, 57, 10), // "DeviceType"
QT_MOC_LITERAL(8, 68, 4), // "Disk"
QT_MOC_LITERAL(9, 73, 5), // "CDROM"
QT_MOC_LITERAL(10, 79, 6), // "Floppy"
QT_MOC_LITERAL(11, 86, 3), // "LUN"
QT_MOC_LITERAL(12, 90, 7), // "BusType"
QT_MOC_LITERAL(13, 98, 4), // "None"
QT_MOC_LITERAL(14, 103, 3), // "IDE"
QT_MOC_LITERAL(15, 107, 4), // "SCSI"
QT_MOC_LITERAL(16, 112, 6), // "Virtio"
QT_MOC_LITERAL(17, 119, 4), // "SATA"
QT_MOC_LITERAL(18, 124, 3), // "USB"
QT_MOC_LITERAL(19, 128, 2), // "SD"
QT_MOC_LITERAL(20, 131, 3), // "FDC"
QT_MOC_LITERAL(21, 135, 9), // "CacheMode"
QT_MOC_LITERAL(22, 145, 7), // "Default"
QT_MOC_LITERAL(23, 153, 12), // "Writethrough"
QT_MOC_LITERAL(24, 166, 9), // "Writeback"
QT_MOC_LITERAL(25, 176, 10), // "Directsync"
QT_MOC_LITERAL(26, 187, 6), // "Unsafe"
QT_MOC_LITERAL(27, 194, 6), // "IOMode"
QT_MOC_LITERAL(28, 201, 7), // "Threads"
QT_MOC_LITERAL(29, 209, 6) // "Native"

    },
    "QVirt::DiskDevice\0DiskType\0File\0Block\0"
    "Dir\0Network\0Volume\0DeviceType\0Disk\0"
    "CDROM\0Floppy\0LUN\0BusType\0None\0IDE\0"
    "SCSI\0Virtio\0SATA\0USB\0SD\0FDC\0CacheMode\0"
    "Default\0Writethrough\0Writeback\0"
    "Directsync\0Unsafe\0IOMode\0Threads\0"
    "Native"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__DiskDevice[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       5,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, alias, flags, count, data
       1,    1, 0x2,    5,   39,
       7,    7, 0x2,    4,   49,
      12,   12, 0x2,    8,   57,
      21,   21, 0x2,    6,   73,
      27,   27, 0x2,    3,   85,

 // enum data: key, value
       2, uint(QVirt::DiskDevice::DiskType::File),
       3, uint(QVirt::DiskDevice::DiskType::Block),
       4, uint(QVirt::DiskDevice::DiskType::Dir),
       5, uint(QVirt::DiskDevice::DiskType::Network),
       6, uint(QVirt::DiskDevice::DiskType::Volume),
       8, uint(QVirt::DiskDevice::DeviceType::Disk),
       9, uint(QVirt::DiskDevice::DeviceType::CDROM),
      10, uint(QVirt::DiskDevice::DeviceType::Floppy),
      11, uint(QVirt::DiskDevice::DeviceType::LUN),
      13, uint(QVirt::DiskDevice::BusType::None),
      14, uint(QVirt::DiskDevice::BusType::IDE),
      15, uint(QVirt::DiskDevice::BusType::SCSI),
      16, uint(QVirt::DiskDevice::BusType::Virtio),
      17, uint(QVirt::DiskDevice::BusType::SATA),
      18, uint(QVirt::DiskDevice::BusType::USB),
      19, uint(QVirt::DiskDevice::BusType::SD),
      20, uint(QVirt::DiskDevice::BusType::FDC),
      22, uint(QVirt::DiskDevice::CacheMode::Default),
      13, uint(QVirt::DiskDevice::CacheMode::None),
      23, uint(QVirt::DiskDevice::CacheMode::Writethrough),
      24, uint(QVirt::DiskDevice::CacheMode::Writeback),
      25, uint(QVirt::DiskDevice::CacheMode::Directsync),
      26, uint(QVirt::DiskDevice::CacheMode::Unsafe),
      22, uint(QVirt::DiskDevice::IOMode::Default),
      28, uint(QVirt::DiskDevice::IOMode::Threads),
      29, uint(QVirt::DiskDevice::IOMode::Native),

       0        // eod
};

void QVirt::DiskDevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::DiskDevice::staticMetaObject = { {
    &Device::staticMetaObject,
    qt_meta_stringdata_QVirt__DiskDevice.data,
    qt_meta_data_QVirt__DiskDevice,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::DiskDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::DiskDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__DiskDevice.stringdata0))
        return static_cast<void*>(this);
    return Device::qt_metacast(_clname);
}

int QVirt::DiskDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Device::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
