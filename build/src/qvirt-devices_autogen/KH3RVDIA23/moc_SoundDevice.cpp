/****************************************************************************
** Meta object code from reading C++ file 'SoundDevice.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "devices/SoundDevice.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SoundDevice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QVirt__SoundDevice_t {
    QByteArrayData data[10];
    char stringdata0[76];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__SoundDevice_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__SoundDevice_t qt_meta_stringdata_QVirt__SoundDevice = {
    {
QT_MOC_LITERAL(0, 0, 18), // "QVirt::SoundDevice"
QT_MOC_LITERAL(1, 19, 10), // "SoundModel"
QT_MOC_LITERAL(2, 30, 4), // "None"
QT_MOC_LITERAL(3, 35, 4), // "ICH6"
QT_MOC_LITERAL(4, 40, 4), // "ICH9"
QT_MOC_LITERAL(5, 45, 4), // "AC97"
QT_MOC_LITERAL(6, 50, 6), // "ES1370"
QT_MOC_LITERAL(7, 57, 4), // "SB16"
QT_MOC_LITERAL(8, 62, 9), // "PCSpeaker"
QT_MOC_LITERAL(9, 72, 3) // "USB"

    },
    "QVirt::SoundDevice\0SoundModel\0None\0"
    "ICH6\0ICH9\0AC97\0ES1370\0SB16\0PCSpeaker\0"
    "USB"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__SoundDevice[] = {

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
       2, uint(QVirt::SoundDevice::SoundModel::None),
       3, uint(QVirt::SoundDevice::SoundModel::ICH6),
       4, uint(QVirt::SoundDevice::SoundModel::ICH9),
       5, uint(QVirt::SoundDevice::SoundModel::AC97),
       6, uint(QVirt::SoundDevice::SoundModel::ES1370),
       7, uint(QVirt::SoundDevice::SoundModel::SB16),
       8, uint(QVirt::SoundDevice::SoundModel::PCSpeaker),
       9, uint(QVirt::SoundDevice::SoundModel::USB),

       0        // eod
};

void QVirt::SoundDevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::SoundDevice::staticMetaObject = { {
    &Device::staticMetaObject,
    qt_meta_stringdata_QVirt__SoundDevice.data,
    qt_meta_data_QVirt__SoundDevice,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::SoundDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::SoundDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__SoundDevice.stringdata0))
        return static_cast<void*>(this);
    return Device::qt_metacast(_clname);
}

int QVirt::SoundDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Device::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
