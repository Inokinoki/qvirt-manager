/****************************************************************************
** Meta object code from reading C++ file 'VideoDevice.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "devices/VideoDevice.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoDevice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QVirt__VideoDevice_t {
    QByteArrayData data[10];
    char stringdata0[73];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__VideoDevice_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__VideoDevice_t qt_meta_stringdata_QVirt__VideoDevice = {
    {
QT_MOC_LITERAL(0, 0, 18), // "QVirt::VideoDevice"
QT_MOC_LITERAL(1, 19, 10), // "VideoModel"
QT_MOC_LITERAL(2, 30, 4), // "None"
QT_MOC_LITERAL(3, 35, 3), // "QXL"
QT_MOC_LITERAL(4, 39, 6), // "Cirrus"
QT_MOC_LITERAL(5, 46, 5), // "VMVGA"
QT_MOC_LITERAL(6, 52, 6), // "Virtio"
QT_MOC_LITERAL(7, 59, 3), // "VGA"
QT_MOC_LITERAL(8, 63, 4), // "QXL3"
QT_MOC_LITERAL(9, 68, 4) // "QXL4"

    },
    "QVirt::VideoDevice\0VideoModel\0None\0"
    "QXL\0Cirrus\0VMVGA\0Virtio\0VGA\0QXL3\0QXL4"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__VideoDevice[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       1,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, alias, flags, count, data
       1,    1, 0x2,    8,   19,

 // enum data: key, value
       2, uint(QVirt::VideoDevice::VideoModel::None),
       3, uint(QVirt::VideoDevice::VideoModel::QXL),
       4, uint(QVirt::VideoDevice::VideoModel::Cirrus),
       5, uint(QVirt::VideoDevice::VideoModel::VMVGA),
       6, uint(QVirt::VideoDevice::VideoModel::Virtio),
       7, uint(QVirt::VideoDevice::VideoModel::VGA),
       8, uint(QVirt::VideoDevice::VideoModel::QXL3),
       9, uint(QVirt::VideoDevice::VideoModel::QXL4),

       0        // eod
};

void QVirt::VideoDevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::VideoDevice::staticMetaObject = { {
    &Device::staticMetaObject,
    qt_meta_stringdata_QVirt__VideoDevice.data,
    qt_meta_data_QVirt__VideoDevice,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::VideoDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::VideoDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__VideoDevice.stringdata0))
        return static_cast<void*>(this);
    return Device::qt_metacast(_clname);
}

int QVirt::VideoDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Device::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
