/****************************************************************************
** Meta object code from reading C++ file 'ConsolePage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/vmwindow/ConsolePage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ConsolePage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QVirt__ConsolePage_t {
    QByteArrayData data[13];
    char stringdata0[187];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__ConsolePage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__ConsolePage_t qt_meta_stringdata_QVirt__ConsolePage = {
    {
QT_MOC_LITERAL(0, 0, 18), // "QVirt::ConsolePage"
QT_MOC_LITERAL(1, 19, 16), // "consoleConnected"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 19), // "consoleDisconnected"
QT_MOC_LITERAL(4, 57, 12), // "consoleError"
QT_MOC_LITERAL(5, 70, 7), // "message"
QT_MOC_LITERAL(6, 78, 16), // "onConnectClicked"
QT_MOC_LITERAL(7, 95, 19), // "onDisconnectClicked"
QT_MOC_LITERAL(8, 115, 19), // "onFullscreenToggled"
QT_MOC_LITERAL(9, 135, 7), // "checked"
QT_MOC_LITERAL(10, 143, 9), // "onSendKey"
QT_MOC_LITERAL(11, 153, 12), // "onScreenshot"
QT_MOC_LITERAL(12, 166, 20) // "onResizeGuestChanged"

    },
    "QVirt::ConsolePage\0consoleConnected\0"
    "\0consoleDisconnected\0consoleError\0"
    "message\0onConnectClicked\0onDisconnectClicked\0"
    "onFullscreenToggled\0checked\0onSendKey\0"
    "onScreenshot\0onResizeGuestChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__ConsolePage[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    0,   60,    2, 0x06 /* Public */,
       4,    1,   61,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   64,    2, 0x08 /* Private */,
       7,    0,   65,    2, 0x08 /* Private */,
       8,    1,   66,    2, 0x08 /* Private */,
      10,    0,   69,    2, 0x08 /* Private */,
      11,    0,   70,    2, 0x08 /* Private */,
      12,    1,   71,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    9,

       0        // eod
};

void QVirt::ConsolePage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ConsolePage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->consoleConnected(); break;
        case 1: _t->consoleDisconnected(); break;
        case 2: _t->consoleError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->onConnectClicked(); break;
        case 4: _t->onDisconnectClicked(); break;
        case 5: _t->onFullscreenToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->onSendKey(); break;
        case 7: _t->onScreenshot(); break;
        case 8: _t->onResizeGuestChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ConsolePage::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConsolePage::consoleConnected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ConsolePage::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConsolePage::consoleDisconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ConsolePage::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConsolePage::consoleError)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QVirt::ConsolePage::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_QVirt__ConsolePage.data,
    qt_meta_data_QVirt__ConsolePage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::ConsolePage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::ConsolePage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__ConsolePage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int QVirt::ConsolePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void QVirt::ConsolePage::consoleConnected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QVirt::ConsolePage::consoleDisconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QVirt::ConsolePage::consoleError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
