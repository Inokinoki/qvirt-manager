/****************************************************************************
** Meta object code from reading C++ file 'ContextMenu.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/widgets/ContextMenu.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ContextMenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QVirt__VMContextMenu_t {
    QByteArrayData data[13];
    char stringdata0[210];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__VMContextMenu_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__VMContextMenu_t qt_meta_stringdata_QVirt__VMContextMenu = {
    {
QT_MOC_LITERAL(0, 0, 20), // "QVirt::VMContextMenu"
QT_MOC_LITERAL(1, 21, 14), // "startRequested"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 13), // "stopRequested"
QT_MOC_LITERAL(4, 51, 15), // "rebootRequested"
QT_MOC_LITERAL(5, 67, 14), // "pauseRequested"
QT_MOC_LITERAL(6, 82, 15), // "resumeRequested"
QT_MOC_LITERAL(7, 98, 15), // "deleteRequested"
QT_MOC_LITERAL(8, 114, 14), // "cloneRequested"
QT_MOC_LITERAL(9, 129, 16), // "migrateRequested"
QT_MOC_LITERAL(10, 146, 20), // "openConsoleRequested"
QT_MOC_LITERAL(11, 167, 20), // "viewDetailsRequested"
QT_MOC_LITERAL(12, 188, 21) // "takeSnapshotRequested"

    },
    "QVirt::VMContextMenu\0startRequested\0"
    "\0stopRequested\0rebootRequested\0"
    "pauseRequested\0resumeRequested\0"
    "deleteRequested\0cloneRequested\0"
    "migrateRequested\0openConsoleRequested\0"
    "viewDetailsRequested\0takeSnapshotRequested"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__VMContextMenu[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    0,   70,    2, 0x06 /* Public */,
       4,    0,   71,    2, 0x06 /* Public */,
       5,    0,   72,    2, 0x06 /* Public */,
       6,    0,   73,    2, 0x06 /* Public */,
       7,    0,   74,    2, 0x06 /* Public */,
       8,    0,   75,    2, 0x06 /* Public */,
       9,    0,   76,    2, 0x06 /* Public */,
      10,    0,   77,    2, 0x06 /* Public */,
      11,    0,   78,    2, 0x06 /* Public */,
      12,    0,   79,    2, 0x06 /* Public */,

 // signals: parameters
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

       0        // eod
};

void QVirt::VMContextMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VMContextMenu *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startRequested(); break;
        case 1: _t->stopRequested(); break;
        case 2: _t->rebootRequested(); break;
        case 3: _t->pauseRequested(); break;
        case 4: _t->resumeRequested(); break;
        case 5: _t->deleteRequested(); break;
        case 6: _t->cloneRequested(); break;
        case 7: _t->migrateRequested(); break;
        case 8: _t->openConsoleRequested(); break;
        case 9: _t->viewDetailsRequested(); break;
        case 10: _t->takeSnapshotRequested(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VMContextMenu::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VMContextMenu::startRequested)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VMContextMenu::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VMContextMenu::stopRequested)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VMContextMenu::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VMContextMenu::rebootRequested)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VMContextMenu::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VMContextMenu::pauseRequested)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (VMContextMenu::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VMContextMenu::resumeRequested)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (VMContextMenu::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VMContextMenu::deleteRequested)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (VMContextMenu::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VMContextMenu::cloneRequested)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (VMContextMenu::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VMContextMenu::migrateRequested)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (VMContextMenu::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VMContextMenu::openConsoleRequested)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (VMContextMenu::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VMContextMenu::viewDetailsRequested)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (VMContextMenu::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VMContextMenu::takeSnapshotRequested)) {
                *result = 10;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::VMContextMenu::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_QVirt__VMContextMenu.data,
    qt_meta_data_QVirt__VMContextMenu,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::VMContextMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::VMContextMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__VMContextMenu.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QVirt::VMContextMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void QVirt::VMContextMenu::startRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QVirt::VMContextMenu::stopRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QVirt::VMContextMenu::rebootRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void QVirt::VMContextMenu::pauseRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void QVirt::VMContextMenu::resumeRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void QVirt::VMContextMenu::deleteRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void QVirt::VMContextMenu::cloneRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void QVirt::VMContextMenu::migrateRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void QVirt::VMContextMenu::openConsoleRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void QVirt::VMContextMenu::viewDetailsRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void QVirt::VMContextMenu::takeSnapshotRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}
struct qt_meta_stringdata_QVirt__KeyboardShortcuts_t {
    QByteArrayData data[1];
    char stringdata0[25];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__KeyboardShortcuts_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__KeyboardShortcuts_t qt_meta_stringdata_QVirt__KeyboardShortcuts = {
    {
QT_MOC_LITERAL(0, 0, 24) // "QVirt::KeyboardShortcuts"

    },
    "QVirt::KeyboardShortcuts"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__KeyboardShortcuts[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void QVirt::KeyboardShortcuts::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::KeyboardShortcuts::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_QVirt__KeyboardShortcuts.data,
    qt_meta_data_QVirt__KeyboardShortcuts,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::KeyboardShortcuts::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::KeyboardShortcuts::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__KeyboardShortcuts.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QVirt::KeyboardShortcuts::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
