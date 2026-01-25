/****************************************************************************
** Meta object code from reading C++ file 'StoragePoolDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/dialogs/StoragePoolDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'StoragePoolDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QVirt__StoragePoolDialog_t {
    QByteArrayData data[17];
    char stringdata0[234];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__StoragePoolDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__StoragePoolDialog_t qt_meta_stringdata_QVirt__StoragePoolDialog = {
    {
QT_MOC_LITERAL(0, 0, 24), // "QVirt::StoragePoolDialog"
QT_MOC_LITERAL(1, 25, 14), // "onPoolSelected"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 16), // "onVolumeSelected"
QT_MOC_LITERAL(4, 58, 11), // "onPoolStart"
QT_MOC_LITERAL(5, 70, 10), // "onPoolStop"
QT_MOC_LITERAL(6, 81, 12), // "onPoolDelete"
QT_MOC_LITERAL(7, 94, 13), // "onPoolRefresh"
QT_MOC_LITERAL(8, 108, 14), // "onVolumeCreate"
QT_MOC_LITERAL(9, 123, 14), // "onVolumeDelete"
QT_MOC_LITERAL(10, 138, 16), // "onVolumeDownload"
QT_MOC_LITERAL(11, 155, 14), // "onVolumeUpload"
QT_MOC_LITERAL(12, 170, 13), // "onVolumeClone"
QT_MOC_LITERAL(13, 184, 12), // "onVolumeWipe"
QT_MOC_LITERAL(14, 197, 9), // "onRefresh"
QT_MOC_LITERAL(15, 207, 22), // "onContextMenuRequested"
QT_MOC_LITERAL(16, 230, 3) // "pos"

    },
    "QVirt::StoragePoolDialog\0onPoolSelected\0"
    "\0onVolumeSelected\0onPoolStart\0onPoolStop\0"
    "onPoolDelete\0onPoolRefresh\0onVolumeCreate\0"
    "onVolumeDelete\0onVolumeDownload\0"
    "onVolumeUpload\0onVolumeClone\0onVolumeWipe\0"
    "onRefresh\0onContextMenuRequested\0pos"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__StoragePoolDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    0,   86,    2, 0x08 /* Private */,
       5,    0,   87,    2, 0x08 /* Private */,
       6,    0,   88,    2, 0x08 /* Private */,
       7,    0,   89,    2, 0x08 /* Private */,
       8,    0,   90,    2, 0x08 /* Private */,
       9,    0,   91,    2, 0x08 /* Private */,
      10,    0,   92,    2, 0x08 /* Private */,
      11,    0,   93,    2, 0x08 /* Private */,
      12,    0,   94,    2, 0x08 /* Private */,
      13,    0,   95,    2, 0x08 /* Private */,
      14,    0,   96,    2, 0x08 /* Private */,
      15,    1,   97,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,   16,

       0        // eod
};

void QVirt::StoragePoolDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<StoragePoolDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onPoolSelected(); break;
        case 1: _t->onVolumeSelected(); break;
        case 2: _t->onPoolStart(); break;
        case 3: _t->onPoolStop(); break;
        case 4: _t->onPoolDelete(); break;
        case 5: _t->onPoolRefresh(); break;
        case 6: _t->onVolumeCreate(); break;
        case 7: _t->onVolumeDelete(); break;
        case 8: _t->onVolumeDownload(); break;
        case 9: _t->onVolumeUpload(); break;
        case 10: _t->onVolumeClone(); break;
        case 11: _t->onVolumeWipe(); break;
        case 12: _t->onRefresh(); break;
        case 13: _t->onContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QVirt::StoragePoolDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_QVirt__StoragePoolDialog.data,
    qt_meta_data_QVirt__StoragePoolDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::StoragePoolDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::StoragePoolDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__StoragePoolDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int QVirt::StoragePoolDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
struct qt_meta_stringdata_QVirt__CreateVolumeDialog_t {
    QByteArrayData data[5];
    char stringdata0[58];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__CreateVolumeDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__CreateVolumeDialog_t qt_meta_stringdata_QVirt__CreateVolumeDialog = {
    {
QT_MOC_LITERAL(0, 0, 25), // "QVirt::CreateVolumeDialog"
QT_MOC_LITERAL(1, 26, 8), // "validate"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 15), // "onFormatChanged"
QT_MOC_LITERAL(4, 52, 5) // "index"

    },
    "QVirt::CreateVolumeDialog\0validate\0\0"
    "onFormatChanged\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__CreateVolumeDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x08 /* Private */,
       3,    1,   25,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,

       0        // eod
};

void QVirt::CreateVolumeDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CreateVolumeDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->validate(); break;
        case 1: _t->onFormatChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QVirt::CreateVolumeDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_QVirt__CreateVolumeDialog.data,
    qt_meta_data_QVirt__CreateVolumeDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::CreateVolumeDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::CreateVolumeDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__CreateVolumeDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int QVirt::CreateVolumeDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
