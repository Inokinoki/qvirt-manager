/****************************************************************************
** Meta object code from reading C++ file 'Device.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "devices/Device.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Device.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QVirt__Device_t {
    QByteArrayData data[30];
    char stringdata0[238];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__Device_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__Device_t qt_meta_stringdata_QVirt__Device = {
    {
QT_MOC_LITERAL(0, 0, 13), // "QVirt::Device"
QT_MOC_LITERAL(1, 14, 13), // "configChanged"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 10), // "DeviceType"
QT_MOC_LITERAL(4, 40, 4), // "None"
QT_MOC_LITERAL(5, 45, 4), // "Disk"
QT_MOC_LITERAL(6, 50, 7), // "Network"
QT_MOC_LITERAL(7, 58, 10), // "Controller"
QT_MOC_LITERAL(8, 69, 5), // "Input"
QT_MOC_LITERAL(9, 75, 8), // "Graphics"
QT_MOC_LITERAL(10, 84, 5), // "Sound"
QT_MOC_LITERAL(11, 90, 5), // "Video"
QT_MOC_LITERAL(12, 96, 10), // "HostDevice"
QT_MOC_LITERAL(13, 107, 10), // "FileSystem"
QT_MOC_LITERAL(14, 118, 9), // "Smartcard"
QT_MOC_LITERAL(15, 128, 11), // "RedirDevice"
QT_MOC_LITERAL(16, 140, 3), // "TPM"
QT_MOC_LITERAL(17, 144, 3), // "RNG"
QT_MOC_LITERAL(18, 148, 5), // "Panic"
QT_MOC_LITERAL(19, 154, 5), // "VSock"
QT_MOC_LITERAL(20, 160, 8), // "Watchdog"
QT_MOC_LITERAL(21, 169, 6), // "Serial"
QT_MOC_LITERAL(22, 176, 8), // "Parallel"
QT_MOC_LITERAL(23, 185, 7), // "Console"
QT_MOC_LITERAL(24, 193, 7), // "Channel"
QT_MOC_LITERAL(25, 201, 6), // "Memory"
QT_MOC_LITERAL(26, 208, 5), // "ShMem"
QT_MOC_LITERAL(27, 214, 5), // "IOMMU"
QT_MOC_LITERAL(28, 220, 6), // "PStore"
QT_MOC_LITERAL(29, 227, 10) // "Memballoon"

    },
    "QVirt::Device\0configChanged\0\0DeviceType\0"
    "None\0Disk\0Network\0Controller\0Input\0"
    "Graphics\0Sound\0Video\0HostDevice\0"
    "FileSystem\0Smartcard\0RedirDevice\0TPM\0"
    "RNG\0Panic\0VSock\0Watchdog\0Serial\0"
    "Parallel\0Console\0Channel\0Memory\0ShMem\0"
    "IOMMU\0PStore\0Memballoon"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__Device[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       1,   20, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // enums: name, alias, flags, count, data
       3,    3, 0x2,   26,   25,

 // enum data: key, value
       4, uint(QVirt::Device::DeviceType::None),
       5, uint(QVirt::Device::DeviceType::Disk),
       6, uint(QVirt::Device::DeviceType::Network),
       7, uint(QVirt::Device::DeviceType::Controller),
       8, uint(QVirt::Device::DeviceType::Input),
       9, uint(QVirt::Device::DeviceType::Graphics),
      10, uint(QVirt::Device::DeviceType::Sound),
      11, uint(QVirt::Device::DeviceType::Video),
      12, uint(QVirt::Device::DeviceType::HostDevice),
      13, uint(QVirt::Device::DeviceType::FileSystem),
      14, uint(QVirt::Device::DeviceType::Smartcard),
      15, uint(QVirt::Device::DeviceType::RedirDevice),
      16, uint(QVirt::Device::DeviceType::TPM),
      17, uint(QVirt::Device::DeviceType::RNG),
      18, uint(QVirt::Device::DeviceType::Panic),
      19, uint(QVirt::Device::DeviceType::VSock),
      20, uint(QVirt::Device::DeviceType::Watchdog),
      21, uint(QVirt::Device::DeviceType::Serial),
      22, uint(QVirt::Device::DeviceType::Parallel),
      23, uint(QVirt::Device::DeviceType::Console),
      24, uint(QVirt::Device::DeviceType::Channel),
      25, uint(QVirt::Device::DeviceType::Memory),
      26, uint(QVirt::Device::DeviceType::ShMem),
      27, uint(QVirt::Device::DeviceType::IOMMU),
      28, uint(QVirt::Device::DeviceType::PStore),
      29, uint(QVirt::Device::DeviceType::Memballoon),

       0        // eod
};

void QVirt::Device::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Device *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->configChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Device::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Device::configChanged)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::Device::staticMetaObject = { {
    &BaseObject::staticMetaObject,
    qt_meta_stringdata_QVirt__Device.data,
    qt_meta_data_QVirt__Device,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::Device::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::Device::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__Device.stringdata0))
        return static_cast<void*>(this);
    return BaseObject::qt_metacast(_clname);
}

int QVirt::Device::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void QVirt::Device::configChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
