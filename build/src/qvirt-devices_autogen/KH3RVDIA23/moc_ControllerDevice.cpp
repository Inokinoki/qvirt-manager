/****************************************************************************
** Meta object code from reading C++ file 'ControllerDevice.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "devices/ControllerDevice.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ControllerDevice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QVirt__ControllerDevice_t {
    QByteArrayData data[25];
    char stringdata0[227];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__ControllerDevice_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__ControllerDevice_t qt_meta_stringdata_QVirt__ControllerDevice = {
    {
QT_MOC_LITERAL(0, 0, 23), // "QVirt::ControllerDevice"
QT_MOC_LITERAL(1, 24, 14), // "ControllerType"
QT_MOC_LITERAL(2, 39, 4), // "None"
QT_MOC_LITERAL(3, 44, 3), // "USB"
QT_MOC_LITERAL(4, 48, 3), // "PCI"
QT_MOC_LITERAL(5, 52, 4), // "SCSI"
QT_MOC_LITERAL(6, 57, 4), // "SATA"
QT_MOC_LITERAL(7, 62, 3), // "IDE"
QT_MOC_LITERAL(8, 66, 12), // "VirtioSerial"
QT_MOC_LITERAL(9, 79, 4), // "CCID"
QT_MOC_LITERAL(10, 84, 8), // "IEEE1394"
QT_MOC_LITERAL(11, 93, 8), // "USBModel"
QT_MOC_LITERAL(12, 102, 9), // "Piix3Uhci"
QT_MOC_LITERAL(13, 112, 9), // "Piix4Uhci"
QT_MOC_LITERAL(14, 122, 4), // "Ehci"
QT_MOC_LITERAL(15, 127, 9), // "Ich9Ehci1"
QT_MOC_LITERAL(16, 137, 9), // "Ich9Uhci1"
QT_MOC_LITERAL(17, 147, 9), // "Ich9Uhci2"
QT_MOC_LITERAL(18, 157, 9), // "Ich9Uhci3"
QT_MOC_LITERAL(19, 167, 10), // "VT8231Uhci"
QT_MOC_LITERAL(20, 178, 13), // "VT82C686BUhci"
QT_MOC_LITERAL(21, 192, 7), // "PCIOhci"
QT_MOC_LITERAL(22, 200, 7), // "NecXhci"
QT_MOC_LITERAL(23, 208, 9), // "TegraUhci"
QT_MOC_LITERAL(24, 218, 8) // "QemuXhci"

    },
    "QVirt::ControllerDevice\0ControllerType\0"
    "None\0USB\0PCI\0SCSI\0SATA\0IDE\0VirtioSerial\0"
    "CCID\0IEEE1394\0USBModel\0Piix3Uhci\0"
    "Piix4Uhci\0Ehci\0Ich9Ehci1\0Ich9Uhci1\0"
    "Ich9Uhci2\0Ich9Uhci3\0VT8231Uhci\0"
    "VT82C686BUhci\0PCIOhci\0NecXhci\0TegraUhci\0"
    "QemuXhci"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__ControllerDevice[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       2,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, alias, flags, count, data
       1,    1, 0x2,    9,   24,
      11,   11, 0x2,   14,   42,

 // enum data: key, value
       2, uint(QVirt::ControllerDevice::ControllerType::None),
       3, uint(QVirt::ControllerDevice::ControllerType::USB),
       4, uint(QVirt::ControllerDevice::ControllerType::PCI),
       5, uint(QVirt::ControllerDevice::ControllerType::SCSI),
       6, uint(QVirt::ControllerDevice::ControllerType::SATA),
       7, uint(QVirt::ControllerDevice::ControllerType::IDE),
       8, uint(QVirt::ControllerDevice::ControllerType::VirtioSerial),
       9, uint(QVirt::ControllerDevice::ControllerType::CCID),
      10, uint(QVirt::ControllerDevice::ControllerType::IEEE1394),
       2, uint(QVirt::ControllerDevice::USBModel::None),
      12, uint(QVirt::ControllerDevice::USBModel::Piix3Uhci),
      13, uint(QVirt::ControllerDevice::USBModel::Piix4Uhci),
      14, uint(QVirt::ControllerDevice::USBModel::Ehci),
      15, uint(QVirt::ControllerDevice::USBModel::Ich9Ehci1),
      16, uint(QVirt::ControllerDevice::USBModel::Ich9Uhci1),
      17, uint(QVirt::ControllerDevice::USBModel::Ich9Uhci2),
      18, uint(QVirt::ControllerDevice::USBModel::Ich9Uhci3),
      19, uint(QVirt::ControllerDevice::USBModel::VT8231Uhci),
      20, uint(QVirt::ControllerDevice::USBModel::VT82C686BUhci),
      21, uint(QVirt::ControllerDevice::USBModel::PCIOhci),
      22, uint(QVirt::ControllerDevice::USBModel::NecXhci),
      23, uint(QVirt::ControllerDevice::USBModel::TegraUhci),
      24, uint(QVirt::ControllerDevice::USBModel::QemuXhci),

       0        // eod
};

void QVirt::ControllerDevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::ControllerDevice::staticMetaObject = { {
    &Device::staticMetaObject,
    qt_meta_stringdata_QVirt__ControllerDevice.data,
    qt_meta_data_QVirt__ControllerDevice,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::ControllerDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::ControllerDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__ControllerDevice.stringdata0))
        return static_cast<void*>(this);
    return Device::qt_metacast(_clname);
}

int QVirt::ControllerDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Device::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
