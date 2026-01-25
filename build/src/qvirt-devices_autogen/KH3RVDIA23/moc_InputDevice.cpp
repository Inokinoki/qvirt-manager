/****************************************************************************
** Meta object code from reading C++ file 'InputDevice.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "devices/InputDevice.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'InputDevice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QVirt__InputDevice_t {
    QByteArrayData data[12];
    char stringdata0[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__InputDevice_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__InputDevice_t qt_meta_stringdata_QVirt__InputDevice = {
    {
QT_MOC_LITERAL(0, 0, 18), // "QVirt::InputDevice"
QT_MOC_LITERAL(1, 19, 9), // "InputType"
QT_MOC_LITERAL(2, 29, 4), // "None"
QT_MOC_LITERAL(3, 34, 6), // "Tablet"
QT_MOC_LITERAL(4, 41, 5), // "Mouse"
QT_MOC_LITERAL(5, 47, 8), // "Keyboard"
QT_MOC_LITERAL(6, 56, 7), // "BusType"
QT_MOC_LITERAL(7, 64, 3), // "USB"
QT_MOC_LITERAL(8, 68, 6), // "Virtio"
QT_MOC_LITERAL(9, 75, 3), // "PS2"
QT_MOC_LITERAL(10, 79, 3), // "Xen"
QT_MOC_LITERAL(11, 83, 5) // "i8042"

    },
    "QVirt::InputDevice\0InputType\0None\0"
    "Tablet\0Mouse\0Keyboard\0BusType\0USB\0"
    "Virtio\0PS2\0Xen\0i8042"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__InputDevice[] = {

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
       1,    1, 0x2,    4,   24,
       6,    6, 0x2,    6,   32,

 // enum data: key, value
       2, uint(QVirt::InputDevice::InputType::None),
       3, uint(QVirt::InputDevice::InputType::Tablet),
       4, uint(QVirt::InputDevice::InputType::Mouse),
       5, uint(QVirt::InputDevice::InputType::Keyboard),
       2, uint(QVirt::InputDevice::BusType::None),
       7, uint(QVirt::InputDevice::BusType::USB),
       8, uint(QVirt::InputDevice::BusType::Virtio),
       9, uint(QVirt::InputDevice::BusType::PS2),
      10, uint(QVirt::InputDevice::BusType::Xen),
      11, uint(QVirt::InputDevice::BusType::i8042),

       0        // eod
};

void QVirt::InputDevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::InputDevice::staticMetaObject = { {
    &Device::staticMetaObject,
    qt_meta_stringdata_QVirt__InputDevice.data,
    qt_meta_data_QVirt__InputDevice,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::InputDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::InputDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__InputDevice.stringdata0))
        return static_cast<void*>(this);
    return Device::qt_metacast(_clname);
}

int QVirt::InputDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Device::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
