/****************************************************************************
** Meta object code from reading C++ file 'VMWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/vmwindow/VMWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VMWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QVirt__VMWindow_t {
    QByteArrayData data[20];
    char stringdata0[286];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__VMWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__VMWindow_t qt_meta_stringdata_QVirt__VMWindow = {
    {
QT_MOC_LITERAL(0, 0, 15), // "QVirt::VMWindow"
QT_MOC_LITERAL(1, 16, 20), // "onDomainStateChanged"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 13), // "Domain::State"
QT_MOC_LITERAL(4, 52, 8), // "newState"
QT_MOC_LITERAL(5, 61, 20), // "onDomainStatsUpdated"
QT_MOC_LITERAL(6, 82, 14), // "onStartClicked"
QT_MOC_LITERAL(7, 97, 13), // "onStopClicked"
QT_MOC_LITERAL(8, 111, 15), // "onRebootClicked"
QT_MOC_LITERAL(9, 127, 14), // "onPauseClicked"
QT_MOC_LITERAL(10, 142, 15), // "onResumeClicked"
QT_MOC_LITERAL(11, 158, 17), // "onForceOffClicked"
QT_MOC_LITERAL(12, 176, 13), // "onSaveClicked"
QT_MOC_LITERAL(13, 190, 21), // "onRestoreSavedClicked"
QT_MOC_LITERAL(14, 212, 9), // "onCloneVM"
QT_MOC_LITERAL(15, 222, 10), // "onDeleteVM"
QT_MOC_LITERAL(16, 233, 11), // "onMigrateVM"
QT_MOC_LITERAL(17, 245, 13), // "onAddHardware"
QT_MOC_LITERAL(18, 259, 9), // "onRefresh"
QT_MOC_LITERAL(19, 269, 16) // "onTakeScreenshot"

    },
    "QVirt::VMWindow\0onDomainStateChanged\0"
    "\0Domain::State\0newState\0onDomainStatsUpdated\0"
    "onStartClicked\0onStopClicked\0"
    "onRebootClicked\0onPauseClicked\0"
    "onResumeClicked\0onForceOffClicked\0"
    "onSaveClicked\0onRestoreSavedClicked\0"
    "onCloneVM\0onDeleteVM\0onMigrateVM\0"
    "onAddHardware\0onRefresh\0onTakeScreenshot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__VMWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x08 /* Private */,
       5,    0,   97,    2, 0x08 /* Private */,
       6,    0,   98,    2, 0x08 /* Private */,
       7,    0,   99,    2, 0x08 /* Private */,
       8,    0,  100,    2, 0x08 /* Private */,
       9,    0,  101,    2, 0x08 /* Private */,
      10,    0,  102,    2, 0x08 /* Private */,
      11,    0,  103,    2, 0x08 /* Private */,
      12,    0,  104,    2, 0x08 /* Private */,
      13,    0,  105,    2, 0x08 /* Private */,
      14,    0,  106,    2, 0x08 /* Private */,
      15,    0,  107,    2, 0x08 /* Private */,
      16,    0,  108,    2, 0x08 /* Private */,
      17,    0,  109,    2, 0x08 /* Private */,
      18,    0,  110,    2, 0x08 /* Private */,
      19,    0,  111,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
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
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QVirt::VMWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VMWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onDomainStateChanged((*reinterpret_cast< Domain::State(*)>(_a[1]))); break;
        case 1: _t->onDomainStatsUpdated(); break;
        case 2: _t->onStartClicked(); break;
        case 3: _t->onStopClicked(); break;
        case 4: _t->onRebootClicked(); break;
        case 5: _t->onPauseClicked(); break;
        case 6: _t->onResumeClicked(); break;
        case 7: _t->onForceOffClicked(); break;
        case 8: _t->onSaveClicked(); break;
        case 9: _t->onRestoreSavedClicked(); break;
        case 10: _t->onCloneVM(); break;
        case 11: _t->onDeleteVM(); break;
        case 12: _t->onMigrateVM(); break;
        case 13: _t->onAddHardware(); break;
        case 14: _t->onRefresh(); break;
        case 15: _t->onTakeScreenshot(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QVirt::VMWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_QVirt__VMWindow.data,
    qt_meta_data_QVirt__VMWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::VMWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::VMWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__VMWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int QVirt::VMWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
