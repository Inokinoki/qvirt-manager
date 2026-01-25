/****************************************************************************
** Meta object code from reading C++ file 'SnapshotsPage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/vmwindow/SnapshotsPage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SnapshotsPage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QVirt__SnapshotsPage_t {
    QByteArrayData data[12];
    char stringdata0[163];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__SnapshotsPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__SnapshotsPage_t qt_meta_stringdata_QVirt__SnapshotsPage = {
    {
QT_MOC_LITERAL(0, 0, 20), // "QVirt::SnapshotsPage"
QT_MOC_LITERAL(1, 21, 18), // "onSnapshotSelected"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 11), // "QModelIndex"
QT_MOC_LITERAL(4, 53, 5), // "index"
QT_MOC_LITERAL(5, 59, 14), // "onTakeSnapshot"
QT_MOC_LITERAL(6, 74, 16), // "onRevertSnapshot"
QT_MOC_LITERAL(7, 91, 16), // "onDeleteSnapshot"
QT_MOC_LITERAL(8, 108, 17), // "onViewSnapshotXML"
QT_MOC_LITERAL(9, 126, 9), // "onRefresh"
QT_MOC_LITERAL(10, 136, 22), // "onContextMenuRequested"
QT_MOC_LITERAL(11, 159, 3) // "pos"

    },
    "QVirt::SnapshotsPage\0onSnapshotSelected\0"
    "\0QModelIndex\0index\0onTakeSnapshot\0"
    "onRevertSnapshot\0onDeleteSnapshot\0"
    "onViewSnapshotXML\0onRefresh\0"
    "onContextMenuRequested\0pos"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__SnapshotsPage[] = {

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
       1,    1,   49,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    0,   54,    2, 0x08 /* Private */,
       8,    0,   55,    2, 0x08 /* Private */,
       9,    0,   56,    2, 0x08 /* Private */,
      10,    1,   57,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,   11,

       0        // eod
};

void QVirt::SnapshotsPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SnapshotsPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onSnapshotSelected((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
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

QT_INIT_METAOBJECT const QMetaObject QVirt::SnapshotsPage::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_QVirt__SnapshotsPage.data,
    qt_meta_data_QVirt__SnapshotsPage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::SnapshotsPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::SnapshotsPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__SnapshotsPage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int QVirt::SnapshotsPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_QVirt__SnapshotListModel_t {
    QByteArrayData data[1];
    char stringdata0[25];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__SnapshotListModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__SnapshotListModel_t qt_meta_stringdata_QVirt__SnapshotListModel = {
    {
QT_MOC_LITERAL(0, 0, 24) // "QVirt::SnapshotListModel"

    },
    "QVirt::SnapshotListModel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__SnapshotListModel[] = {

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

void QVirt::SnapshotListModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::SnapshotListModel::staticMetaObject = { {
    &QAbstractTableModel::staticMetaObject,
    qt_meta_stringdata_QVirt__SnapshotListModel.data,
    qt_meta_data_QVirt__SnapshotListModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::SnapshotListModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::SnapshotListModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__SnapshotListModel.stringdata0))
        return static_cast<void*>(this);
    return QAbstractTableModel::qt_metacast(_clname);
}

int QVirt::SnapshotListModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
