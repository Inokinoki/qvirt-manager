/****************************************************************************
** Meta object code from reading C++ file 'AddHardwareDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/dialogs/AddHardwareDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AddHardwareDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QVirt__AddHardwareDialog_t {
    QByteArrayData data[7];
    char stringdata0[91];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__AddHardwareDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__AddHardwareDialog_t qt_meta_stringdata_QVirt__AddHardwareDialog = {
    {
QT_MOC_LITERAL(0, 0, 24), // "QVirt::AddHardwareDialog"
QT_MOC_LITERAL(1, 25, 19), // "onDeviceTypeChanged"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(4, 63, 7), // "current"
QT_MOC_LITERAL(5, 71, 8), // "previous"
QT_MOC_LITERAL(6, 80, 10) // "onAccepted"

    },
    "QVirt::AddHardwareDialog\0onDeviceTypeChanged\0"
    "\0QListWidgetItem*\0current\0previous\0"
    "onAccepted"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__AddHardwareDialog[] = {

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
       1,    2,   24,    2, 0x08 /* Private */,
       6,    0,   29,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    5,
    QMetaType::Void,

       0        // eod
};

void QVirt::AddHardwareDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AddHardwareDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onDeviceTypeChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        case 1: _t->onAccepted(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QVirt::AddHardwareDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_QVirt__AddHardwareDialog.data,
    qt_meta_data_QVirt__AddHardwareDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::AddHardwareDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::AddHardwareDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__AddHardwareDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int QVirt::AddHardwareDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
struct qt_meta_stringdata_QVirt__DeviceConfigPage_t {
    QByteArrayData data[1];
    char stringdata0[24];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__DeviceConfigPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__DeviceConfigPage_t qt_meta_stringdata_QVirt__DeviceConfigPage = {
    {
QT_MOC_LITERAL(0, 0, 23) // "QVirt::DeviceConfigPage"

    },
    "QVirt::DeviceConfigPage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__DeviceConfigPage[] = {

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

void QVirt::DeviceConfigPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::DeviceConfigPage::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_QVirt__DeviceConfigPage.data,
    qt_meta_data_QVirt__DeviceConfigPage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::DeviceConfigPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::DeviceConfigPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__DeviceConfigPage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int QVirt::DeviceConfigPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_QVirt__StorageDevicePage_t {
    QByteArrayData data[1];
    char stringdata0[25];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__StorageDevicePage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__StorageDevicePage_t qt_meta_stringdata_QVirt__StorageDevicePage = {
    {
QT_MOC_LITERAL(0, 0, 24) // "QVirt::StorageDevicePage"

    },
    "QVirt::StorageDevicePage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__StorageDevicePage[] = {

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

void QVirt::StorageDevicePage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::StorageDevicePage::staticMetaObject = { {
    &DeviceConfigPage::staticMetaObject,
    qt_meta_stringdata_QVirt__StorageDevicePage.data,
    qt_meta_data_QVirt__StorageDevicePage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::StorageDevicePage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::StorageDevicePage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__StorageDevicePage.stringdata0))
        return static_cast<void*>(this);
    return DeviceConfigPage::qt_metacast(_clname);
}

int QVirt::StorageDevicePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DeviceConfigPage::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_QVirt__NetworkDevicePage_t {
    QByteArrayData data[1];
    char stringdata0[25];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__NetworkDevicePage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__NetworkDevicePage_t qt_meta_stringdata_QVirt__NetworkDevicePage = {
    {
QT_MOC_LITERAL(0, 0, 24) // "QVirt::NetworkDevicePage"

    },
    "QVirt::NetworkDevicePage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__NetworkDevicePage[] = {

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

void QVirt::NetworkDevicePage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::NetworkDevicePage::staticMetaObject = { {
    &DeviceConfigPage::staticMetaObject,
    qt_meta_stringdata_QVirt__NetworkDevicePage.data,
    qt_meta_data_QVirt__NetworkDevicePage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::NetworkDevicePage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::NetworkDevicePage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__NetworkDevicePage.stringdata0))
        return static_cast<void*>(this);
    return DeviceConfigPage::qt_metacast(_clname);
}

int QVirt::NetworkDevicePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DeviceConfigPage::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_QVirt__ControllerDevicePage_t {
    QByteArrayData data[1];
    char stringdata0[28];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__ControllerDevicePage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__ControllerDevicePage_t qt_meta_stringdata_QVirt__ControllerDevicePage = {
    {
QT_MOC_LITERAL(0, 0, 27) // "QVirt::ControllerDevicePage"

    },
    "QVirt::ControllerDevicePage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__ControllerDevicePage[] = {

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

void QVirt::ControllerDevicePage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::ControllerDevicePage::staticMetaObject = { {
    &DeviceConfigPage::staticMetaObject,
    qt_meta_stringdata_QVirt__ControllerDevicePage.data,
    qt_meta_data_QVirt__ControllerDevicePage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::ControllerDevicePage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::ControllerDevicePage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__ControllerDevicePage.stringdata0))
        return static_cast<void*>(this);
    return DeviceConfigPage::qt_metacast(_clname);
}

int QVirt::ControllerDevicePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DeviceConfigPage::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_QVirt__InputDevicePage_t {
    QByteArrayData data[1];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__InputDevicePage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__InputDevicePage_t qt_meta_stringdata_QVirt__InputDevicePage = {
    {
QT_MOC_LITERAL(0, 0, 22) // "QVirt::InputDevicePage"

    },
    "QVirt::InputDevicePage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__InputDevicePage[] = {

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

void QVirt::InputDevicePage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::InputDevicePage::staticMetaObject = { {
    &DeviceConfigPage::staticMetaObject,
    qt_meta_stringdata_QVirt__InputDevicePage.data,
    qt_meta_data_QVirt__InputDevicePage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::InputDevicePage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::InputDevicePage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__InputDevicePage.stringdata0))
        return static_cast<void*>(this);
    return DeviceConfigPage::qt_metacast(_clname);
}

int QVirt::InputDevicePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DeviceConfigPage::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_QVirt__GraphicsDevicePage_t {
    QByteArrayData data[1];
    char stringdata0[26];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__GraphicsDevicePage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__GraphicsDevicePage_t qt_meta_stringdata_QVirt__GraphicsDevicePage = {
    {
QT_MOC_LITERAL(0, 0, 25) // "QVirt::GraphicsDevicePage"

    },
    "QVirt::GraphicsDevicePage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__GraphicsDevicePage[] = {

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

void QVirt::GraphicsDevicePage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::GraphicsDevicePage::staticMetaObject = { {
    &DeviceConfigPage::staticMetaObject,
    qt_meta_stringdata_QVirt__GraphicsDevicePage.data,
    qt_meta_data_QVirt__GraphicsDevicePage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::GraphicsDevicePage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::GraphicsDevicePage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__GraphicsDevicePage.stringdata0))
        return static_cast<void*>(this);
    return DeviceConfigPage::qt_metacast(_clname);
}

int QVirt::GraphicsDevicePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DeviceConfigPage::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_QVirt__VideoDevicePage_t {
    QByteArrayData data[1];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__VideoDevicePage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__VideoDevicePage_t qt_meta_stringdata_QVirt__VideoDevicePage = {
    {
QT_MOC_LITERAL(0, 0, 22) // "QVirt::VideoDevicePage"

    },
    "QVirt::VideoDevicePage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__VideoDevicePage[] = {

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

void QVirt::VideoDevicePage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::VideoDevicePage::staticMetaObject = { {
    &DeviceConfigPage::staticMetaObject,
    qt_meta_stringdata_QVirt__VideoDevicePage.data,
    qt_meta_data_QVirt__VideoDevicePage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::VideoDevicePage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::VideoDevicePage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__VideoDevicePage.stringdata0))
        return static_cast<void*>(this);
    return DeviceConfigPage::qt_metacast(_clname);
}

int QVirt::VideoDevicePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DeviceConfigPage::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_QVirt__SoundDevicePage_t {
    QByteArrayData data[1];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__SoundDevicePage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__SoundDevicePage_t qt_meta_stringdata_QVirt__SoundDevicePage = {
    {
QT_MOC_LITERAL(0, 0, 22) // "QVirt::SoundDevicePage"

    },
    "QVirt::SoundDevicePage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__SoundDevicePage[] = {

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

void QVirt::SoundDevicePage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::SoundDevicePage::staticMetaObject = { {
    &DeviceConfigPage::staticMetaObject,
    qt_meta_stringdata_QVirt__SoundDevicePage.data,
    qt_meta_data_QVirt__SoundDevicePage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::SoundDevicePage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::SoundDevicePage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__SoundDevicePage.stringdata0))
        return static_cast<void*>(this);
    return DeviceConfigPage::qt_metacast(_clname);
}

int QVirt::SoundDevicePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DeviceConfigPage::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
