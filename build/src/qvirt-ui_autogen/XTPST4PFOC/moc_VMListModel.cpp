/****************************************************************************
** Meta object code from reading C++ file 'VMListModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/models/VMListModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VMListModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QVirt__VMListModel_t {
    QByteArrayData data[10];
    char stringdata0[127];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__VMListModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__VMListModel_t qt_meta_stringdata_QVirt__VMListModel = {
    {
QT_MOC_LITERAL(0, 0, 18), // "QVirt::VMListModel"
QT_MOC_LITERAL(1, 19, 13), // "onDomainAdded"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 7), // "Domain*"
QT_MOC_LITERAL(4, 42, 6), // "domain"
QT_MOC_LITERAL(5, 49, 15), // "onDomainRemoved"
QT_MOC_LITERAL(6, 65, 20), // "onDomainStateChanged"
QT_MOC_LITERAL(7, 86, 13), // "Domain::State"
QT_MOC_LITERAL(8, 100, 5), // "state"
QT_MOC_LITERAL(9, 106, 20) // "onDomainStatsUpdated"

    },
    "QVirt::VMListModel\0onDomainAdded\0\0"
    "Domain*\0domain\0onDomainRemoved\0"
    "onDomainStateChanged\0Domain::State\0"
    "state\0onDomainStatsUpdated"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__VMListModel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x0a /* Public */,
       5,    1,   37,    2, 0x0a /* Public */,
       6,    1,   40,    2, 0x0a /* Public */,
       9,    0,   43,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,

       0        // eod
};

void QVirt::VMListModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VMListModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onDomainAdded((*reinterpret_cast< Domain*(*)>(_a[1]))); break;
        case 1: _t->onDomainRemoved((*reinterpret_cast< Domain*(*)>(_a[1]))); break;
        case 2: _t->onDomainStateChanged((*reinterpret_cast< Domain::State(*)>(_a[1]))); break;
        case 3: _t->onDomainStatsUpdated(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Domain* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Domain* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QVirt::VMListModel::staticMetaObject = { {
    &QAbstractListModel::staticMetaObject,
    qt_meta_stringdata_QVirt__VMListModel.data,
    qt_meta_data_QVirt__VMListModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::VMListModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::VMListModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__VMListModel.stringdata0))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int QVirt::VMListModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
