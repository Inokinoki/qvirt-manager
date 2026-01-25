/****************************************************************************
** Meta object code from reading C++ file 'Domain.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "libvirt/Domain.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Domain.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QVirt__Domain_t {
    QByteArrayData data[15];
    char stringdata0[178];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__Domain_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__Domain_t qt_meta_stringdata_QVirt__Domain = {
    {
QT_MOC_LITERAL(0, 0, 13), // "QVirt::Domain"
QT_MOC_LITERAL(1, 14, 12), // "stateChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 5), // "State"
QT_MOC_LITERAL(4, 34, 8), // "newState"
QT_MOC_LITERAL(5, 43, 13), // "configChanged"
QT_MOC_LITERAL(6, 57, 12), // "statsUpdated"
QT_MOC_LITERAL(7, 70, 12), // "StateNoState"
QT_MOC_LITERAL(8, 83, 12), // "StateRunning"
QT_MOC_LITERAL(9, 96, 12), // "StateBlocked"
QT_MOC_LITERAL(10, 109, 11), // "StatePaused"
QT_MOC_LITERAL(11, 121, 13), // "StateShutdown"
QT_MOC_LITERAL(12, 135, 12), // "StateShutOff"
QT_MOC_LITERAL(13, 148, 12), // "StateCrashed"
QT_MOC_LITERAL(14, 161, 16) // "StatePMSuspended"

    },
    "QVirt::Domain\0stateChanged\0\0State\0"
    "newState\0configChanged\0statsUpdated\0"
    "StateNoState\0StateRunning\0StateBlocked\0"
    "StatePaused\0StateShutdown\0StateShutOff\0"
    "StateCrashed\0StatePMSuspended"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__Domain[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       1,   34, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       5,    0,   32,    2, 0x06 /* Public */,
       6,    0,   33,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,

 // enums: name, alias, flags, count, data
       3,    3, 0x0,    8,   39,

 // enum data: key, value
       7, uint(QVirt::Domain::StateNoState),
       8, uint(QVirt::Domain::StateRunning),
       9, uint(QVirt::Domain::StateBlocked),
      10, uint(QVirt::Domain::StatePaused),
      11, uint(QVirt::Domain::StateShutdown),
      12, uint(QVirt::Domain::StateShutOff),
      13, uint(QVirt::Domain::StateCrashed),
      14, uint(QVirt::Domain::StatePMSuspended),

       0        // eod
};

void QVirt::Domain::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Domain *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stateChanged((*reinterpret_cast< State(*)>(_a[1]))); break;
        case 1: _t->configChanged(); break;
        case 2: _t->statsUpdated(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Domain::*)(State );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Domain::stateChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Domain::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Domain::configChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Domain::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Domain::statsUpdated)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QVirt::Domain::staticMetaObject = { {
    &BaseObject::staticMetaObject,
    qt_meta_stringdata_QVirt__Domain.data,
    qt_meta_data_QVirt__Domain,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::Domain::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::Domain::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__Domain.stringdata0))
        return static_cast<void*>(this);
    return BaseObject::qt_metacast(_clname);
}

int QVirt::Domain::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void QVirt::Domain::stateChanged(State _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QVirt::Domain::configChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QVirt::Domain::statsUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
