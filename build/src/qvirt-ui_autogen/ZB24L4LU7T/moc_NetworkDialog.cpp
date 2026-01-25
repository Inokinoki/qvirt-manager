/****************************************************************************
** Meta object code from reading C++ file 'NetworkDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/dialogs/NetworkDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NetworkDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QVirt__NetworkDialog_t {
    QByteArrayData data[11];
    char stringdata0[155];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__NetworkDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__NetworkDialog_t qt_meta_stringdata_QVirt__NetworkDialog = {
    {
QT_MOC_LITERAL(0, 0, 20), // "QVirt::NetworkDialog"
QT_MOC_LITERAL(1, 21, 17), // "onNetworkSelected"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 14), // "onNetworkStart"
QT_MOC_LITERAL(4, 55, 13), // "onNetworkStop"
QT_MOC_LITERAL(5, 69, 15), // "onNetworkDelete"
QT_MOC_LITERAL(6, 85, 16), // "onNetworkRefresh"
QT_MOC_LITERAL(7, 102, 15), // "onCreateNetwork"
QT_MOC_LITERAL(8, 118, 9), // "onRefresh"
QT_MOC_LITERAL(9, 128, 22), // "onContextMenuRequested"
QT_MOC_LITERAL(10, 151, 3) // "pos"

    },
    "QVirt::NetworkDialog\0onNetworkSelected\0"
    "\0onNetworkStart\0onNetworkStop\0"
    "onNetworkDelete\0onNetworkRefresh\0"
    "onCreateNetwork\0onRefresh\0"
    "onContextMenuRequested\0pos"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__NetworkDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    1,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,   10,

       0        // eod
};

void QVirt::NetworkDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onNetworkSelected(); break;
        case 1: _t->onNetworkStart(); break;
        case 2: _t->onNetworkStop(); break;
        case 3: _t->onNetworkDelete(); break;
        case 4: _t->onNetworkRefresh(); break;
        case 5: _t->onCreateNetwork(); break;
        case 6: _t->onRefresh(); break;
        case 7: _t->onContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QVirt::NetworkDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_QVirt__NetworkDialog.data,
    qt_meta_data_QVirt__NetworkDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::NetworkDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::NetworkDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__NetworkDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int QVirt::NetworkDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
struct qt_meta_stringdata_QVirt__CreateNetworkDialog_t {
    QByteArrayData data[7];
    char stringdata0[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__CreateNetworkDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__CreateNetworkDialog_t qt_meta_stringdata_QVirt__CreateNetworkDialog = {
    {
QT_MOC_LITERAL(0, 0, 26), // "QVirt::CreateNetworkDialog"
QT_MOC_LITERAL(1, 27, 8), // "validate"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 13), // "onModeChanged"
QT_MOC_LITERAL(4, 51, 5), // "index"
QT_MOC_LITERAL(5, 57, 20), // "onDHCPEnabledChanged"
QT_MOC_LITERAL(6, 78, 5) // "state"

    },
    "QVirt::CreateNetworkDialog\0validate\0"
    "\0onModeChanged\0index\0onDHCPEnabledChanged\0"
    "state"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__CreateNetworkDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x08 /* Private */,
       3,    1,   30,    2, 0x08 /* Private */,
       5,    1,   33,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    6,

       0        // eod
};

void QVirt::CreateNetworkDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CreateNetworkDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->validate(); break;
        case 1: _t->onModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onDHCPEnabledChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QVirt::CreateNetworkDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_QVirt__CreateNetworkDialog.data,
    qt_meta_data_QVirt__CreateNetworkDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::CreateNetworkDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::CreateNetworkDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__CreateNetworkDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int QVirt::CreateNetworkDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
