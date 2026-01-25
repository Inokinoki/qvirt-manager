/****************************************************************************
** Meta object code from reading C++ file 'SnapshotDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/dialogs/SnapshotDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SnapshotDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QVirt__SnapshotDialog_t {
    QByteArrayData data[10];
    char stringdata0[146];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__SnapshotDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__SnapshotDialog_t qt_meta_stringdata_QVirt__SnapshotDialog = {
    {
QT_MOC_LITERAL(0, 0, 21), // "QVirt::SnapshotDialog"
QT_MOC_LITERAL(1, 22, 18), // "onSnapshotSelected"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 14), // "onTakeSnapshot"
QT_MOC_LITERAL(4, 57, 16), // "onRevertSnapshot"
QT_MOC_LITERAL(5, 74, 16), // "onDeleteSnapshot"
QT_MOC_LITERAL(6, 91, 17), // "onViewSnapshotXML"
QT_MOC_LITERAL(7, 109, 9), // "onRefresh"
QT_MOC_LITERAL(8, 119, 22), // "onContextMenuRequested"
QT_MOC_LITERAL(9, 142, 3) // "pos"

    },
    "QVirt::SnapshotDialog\0onSnapshotSelected\0"
    "\0onTakeSnapshot\0onRevertSnapshot\0"
    "onDeleteSnapshot\0onViewSnapshotXML\0"
    "onRefresh\0onContextMenuRequested\0pos"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__SnapshotDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    0,   54,    2, 0x08 /* Private */,
       8,    1,   55,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,    9,

       0        // eod
};

void QVirt::SnapshotDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SnapshotDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onSnapshotSelected(); break;
        case 1: _t->onTakeSnapshot(); break;
        case 2: _t->onRevertSnapshot(); break;
        case 3: _t->onDeleteSnapshot(); break;
        case 4: _t->onViewSnapshotXML(); break;
        case 5: _t->onRefresh(); break;
        case 6: _t->onContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QVirt::SnapshotDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_QVirt__SnapshotDialog.data,
    qt_meta_data_QVirt__SnapshotDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::SnapshotDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::SnapshotDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__SnapshotDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int QVirt::SnapshotDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
struct qt_meta_stringdata_QVirt__TakeSnapshotDialog_t {
    QByteArrayData data[3];
    char stringdata0[36];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__TakeSnapshotDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__TakeSnapshotDialog_t qt_meta_stringdata_QVirt__TakeSnapshotDialog = {
    {
QT_MOC_LITERAL(0, 0, 25), // "QVirt::TakeSnapshotDialog"
QT_MOC_LITERAL(1, 26, 8), // "validate"
QT_MOC_LITERAL(2, 35, 0) // ""

    },
    "QVirt::TakeSnapshotDialog\0validate\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__TakeSnapshotDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void QVirt::TakeSnapshotDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TakeSnapshotDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->validate(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::TakeSnapshotDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_QVirt__TakeSnapshotDialog.data,
    qt_meta_data_QVirt__TakeSnapshotDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::TakeSnapshotDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::TakeSnapshotDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__TakeSnapshotDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int QVirt::TakeSnapshotDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
