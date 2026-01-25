/****************************************************************************
** Meta object code from reading C++ file 'GraphicsDevice.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "devices/GraphicsDevice.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GraphicsDevice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QVirt__GraphicsDevice_t {
    QByteArrayData data[11];
    char stringdata0[92];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__GraphicsDevice_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__GraphicsDevice_t qt_meta_stringdata_QVirt__GraphicsDevice = {
    {
QT_MOC_LITERAL(0, 0, 21), // "QVirt::GraphicsDevice"
QT_MOC_LITERAL(1, 22, 12), // "GraphicsType"
QT_MOC_LITERAL(2, 35, 4), // "None"
QT_MOC_LITERAL(3, 40, 3), // "VNC"
QT_MOC_LITERAL(4, 44, 5), // "SPICE"
QT_MOC_LITERAL(5, 50, 3), // "SDL"
QT_MOC_LITERAL(6, 54, 3), // "RTP"
QT_MOC_LITERAL(7, 58, 10), // "ListenType"
QT_MOC_LITERAL(8, 69, 7), // "Address"
QT_MOC_LITERAL(9, 77, 7), // "Network"
QT_MOC_LITERAL(10, 85, 6) // "Socket"

    },
    "QVirt::GraphicsDevice\0GraphicsType\0"
    "None\0VNC\0SPICE\0SDL\0RTP\0ListenType\0"
    "Address\0Network\0Socket"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__GraphicsDevice[] = {

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
       1,    1, 0x2,    5,   24,
       7,    7, 0x2,    4,   34,

 // enum data: key, value
       2, uint(QVirt::GraphicsDevice::GraphicsType::None),
       3, uint(QVirt::GraphicsDevice::GraphicsType::VNC),
       4, uint(QVirt::GraphicsDevice::GraphicsType::SPICE),
       5, uint(QVirt::GraphicsDevice::GraphicsType::SDL),
       6, uint(QVirt::GraphicsDevice::GraphicsType::RTP),
       2, uint(QVirt::GraphicsDevice::ListenType::None),
       8, uint(QVirt::GraphicsDevice::ListenType::Address),
       9, uint(QVirt::GraphicsDevice::ListenType::Network),
      10, uint(QVirt::GraphicsDevice::ListenType::Socket),

       0        // eod
};

void QVirt::GraphicsDevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::GraphicsDevice::staticMetaObject = { {
    &Device::staticMetaObject,
    qt_meta_stringdata_QVirt__GraphicsDevice.data,
    qt_meta_data_QVirt__GraphicsDevice,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::GraphicsDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::GraphicsDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__GraphicsDevice.stringdata0))
        return static_cast<void*>(this);
    return Device::qt_metacast(_clname);
}

int QVirt::GraphicsDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Device::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
