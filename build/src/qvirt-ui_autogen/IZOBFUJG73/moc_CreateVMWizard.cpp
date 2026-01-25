/****************************************************************************
** Meta object code from reading C++ file 'CreateVMWizard.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ui/wizards/CreateVMWizard.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CreateVMWizard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QVirt__CreateVMWizard_t {
    QByteArrayData data[5];
    char stringdata0[65];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__CreateVMWizard_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__CreateVMWizard_t qt_meta_stringdata_QVirt__CreateVMWizard = {
    {
QT_MOC_LITERAL(0, 0, 21), // "QVirt::CreateVMWizard"
QT_MOC_LITERAL(1, 22, 18), // "onCurrentIdChanged"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 2), // "id"
QT_MOC_LITERAL(4, 45, 19) // "validateCurrentPage"

    },
    "QVirt::CreateVMWizard\0onCurrentIdChanged\0"
    "\0id\0validateCurrentPage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__CreateVMWizard[] = {

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
       1,    1,   24,    2, 0x08 /* Private */,
       4,    0,   27,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Bool,

       0        // eod
};

void QVirt::CreateVMWizard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CreateVMWizard *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onCurrentIdChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: { bool _r = _t->validateCurrentPage();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QVirt::CreateVMWizard::staticMetaObject = { {
    &QWizard::staticMetaObject,
    qt_meta_stringdata_QVirt__CreateVMWizard.data,
    qt_meta_data_QVirt__CreateVMWizard,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::CreateVMWizard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::CreateVMWizard::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__CreateVMWizard.stringdata0))
        return static_cast<void*>(this);
    return QWizard::qt_metacast(_clname);
}

int QVirt::CreateVMWizard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizard::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_QVirt__NameAndOSPage_t {
    QByteArrayData data[1];
    char stringdata0[21];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__NameAndOSPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__NameAndOSPage_t qt_meta_stringdata_QVirt__NameAndOSPage = {
    {
QT_MOC_LITERAL(0, 0, 20) // "QVirt::NameAndOSPage"

    },
    "QVirt::NameAndOSPage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__NameAndOSPage[] = {

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

void QVirt::NameAndOSPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::NameAndOSPage::staticMetaObject = { {
    &QWizardPage::staticMetaObject,
    qt_meta_stringdata_QVirt__NameAndOSPage.data,
    qt_meta_data_QVirt__NameAndOSPage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::NameAndOSPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::NameAndOSPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__NameAndOSPage.stringdata0))
        return static_cast<void*>(this);
    return QWizardPage::qt_metacast(_clname);
}

int QVirt::NameAndOSPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizardPage::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_QVirt__InstallMediaPage_t {
    QByteArrayData data[11];
    char stringdata0[119];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__InstallMediaPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__InstallMediaPage_t qt_meta_stringdata_QVirt__InstallMediaPage = {
    {
QT_MOC_LITERAL(0, 0, 23), // "QVirt::InstallMediaPage"
QT_MOC_LITERAL(1, 24, 20), // "onInstallTypeChanged"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 11), // "onBrowseISO"
QT_MOC_LITERAL(4, 58, 14), // "onBrowseImport"
QT_MOC_LITERAL(5, 73, 11), // "InstallType"
QT_MOC_LITERAL(6, 85, 8), // "LocalISO"
QT_MOC_LITERAL(7, 94, 3), // "URL"
QT_MOC_LITERAL(8, 98, 6), // "Import"
QT_MOC_LITERAL(9, 105, 3), // "PXE"
QT_MOC_LITERAL(10, 109, 9) // "Container"

    },
    "QVirt::InstallMediaPage\0onInstallTypeChanged\0"
    "\0onBrowseISO\0onBrowseImport\0InstallType\0"
    "LocalISO\0URL\0Import\0PXE\0Container"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__InstallMediaPage[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       1,   32, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x08 /* Private */,
       3,    0,   30,    2, 0x08 /* Private */,
       4,    0,   31,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // enums: name, alias, flags, count, data
       5,    5, 0x2,    5,   37,

 // enum data: key, value
       6, uint(QVirt::InstallMediaPage::InstallType::LocalISO),
       7, uint(QVirt::InstallMediaPage::InstallType::URL),
       8, uint(QVirt::InstallMediaPage::InstallType::Import),
       9, uint(QVirt::InstallMediaPage::InstallType::PXE),
      10, uint(QVirt::InstallMediaPage::InstallType::Container),

       0        // eod
};

void QVirt::InstallMediaPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<InstallMediaPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onInstallTypeChanged(); break;
        case 1: _t->onBrowseISO(); break;
        case 2: _t->onBrowseImport(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::InstallMediaPage::staticMetaObject = { {
    &QWizardPage::staticMetaObject,
    qt_meta_stringdata_QVirt__InstallMediaPage.data,
    qt_meta_data_QVirt__InstallMediaPage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::InstallMediaPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::InstallMediaPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__InstallMediaPage.stringdata0))
        return static_cast<void*>(this);
    return QWizardPage::qt_metacast(_clname);
}

int QVirt::InstallMediaPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizardPage::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_QVirt__CPUMemoryPage_t {
    QByteArrayData data[1];
    char stringdata0[21];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__CPUMemoryPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__CPUMemoryPage_t qt_meta_stringdata_QVirt__CPUMemoryPage = {
    {
QT_MOC_LITERAL(0, 0, 20) // "QVirt::CPUMemoryPage"

    },
    "QVirt::CPUMemoryPage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__CPUMemoryPage[] = {

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

void QVirt::CPUMemoryPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::CPUMemoryPage::staticMetaObject = { {
    &QWizardPage::staticMetaObject,
    qt_meta_stringdata_QVirt__CPUMemoryPage.data,
    qt_meta_data_QVirt__CPUMemoryPage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::CPUMemoryPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::CPUMemoryPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__CPUMemoryPage.stringdata0))
        return static_cast<void*>(this);
    return QWizardPage::qt_metacast(_clname);
}

int QVirt::CPUMemoryPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizardPage::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_QVirt__StoragePage_t {
    QByteArrayData data[9];
    char stringdata0[112];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__StoragePage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__StoragePage_t qt_meta_stringdata_QVirt__StoragePage = {
    {
QT_MOC_LITERAL(0, 0, 18), // "QVirt::StoragePage"
QT_MOC_LITERAL(1, 19, 20), // "onStorageTypeChanged"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 12), // "onBrowseDisk"
QT_MOC_LITERAL(4, 54, 14), // "onCreateVolume"
QT_MOC_LITERAL(5, 69, 11), // "StorageType"
QT_MOC_LITERAL(6, 81, 7), // "NewDisk"
QT_MOC_LITERAL(7, 89, 12), // "ExistingDisk"
QT_MOC_LITERAL(8, 102, 9) // "NoStorage"

    },
    "QVirt::StoragePage\0onStorageTypeChanged\0"
    "\0onBrowseDisk\0onCreateVolume\0StorageType\0"
    "NewDisk\0ExistingDisk\0NoStorage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__StoragePage[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       1,   32, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x08 /* Private */,
       3,    0,   30,    2, 0x08 /* Private */,
       4,    0,   31,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // enums: name, alias, flags, count, data
       5,    5, 0x2,    3,   37,

 // enum data: key, value
       6, uint(QVirt::StoragePage::StorageType::NewDisk),
       7, uint(QVirt::StoragePage::StorageType::ExistingDisk),
       8, uint(QVirt::StoragePage::StorageType::NoStorage),

       0        // eod
};

void QVirt::StoragePage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<StoragePage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onStorageTypeChanged(); break;
        case 1: _t->onBrowseDisk(); break;
        case 2: _t->onCreateVolume(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::StoragePage::staticMetaObject = { {
    &QWizardPage::staticMetaObject,
    qt_meta_stringdata_QVirt__StoragePage.data,
    qt_meta_data_QVirt__StoragePage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::StoragePage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::StoragePage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__StoragePage.stringdata0))
        return static_cast<void*>(this);
    return QWizardPage::qt_metacast(_clname);
}

int QVirt::StoragePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizardPage::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_QVirt__NetworkPage_t {
    QByteArrayData data[4];
    char stringdata0[47];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__NetworkPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__NetworkPage_t qt_meta_stringdata_QVirt__NetworkPage = {
    {
QT_MOC_LITERAL(0, 0, 18), // "QVirt::NetworkPage"
QT_MOC_LITERAL(1, 19, 20), // "onNetworkTypeChanged"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 5) // "index"

    },
    "QVirt::NetworkPage\0onNetworkTypeChanged\0"
    "\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__NetworkPage[] = {

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
       1,    1,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void QVirt::NetworkPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onNetworkTypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QVirt::NetworkPage::staticMetaObject = { {
    &QWizardPage::staticMetaObject,
    qt_meta_stringdata_QVirt__NetworkPage.data,
    qt_meta_data_QVirt__NetworkPage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::NetworkPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::NetworkPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__NetworkPage.stringdata0))
        return static_cast<void*>(this);
    return QWizardPage::qt_metacast(_clname);
}

int QVirt::NetworkPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizardPage::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_QVirt__SummaryPage_t {
    QByteArrayData data[1];
    char stringdata0[19];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__SummaryPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__SummaryPage_t qt_meta_stringdata_QVirt__SummaryPage = {
    {
QT_MOC_LITERAL(0, 0, 18) // "QVirt::SummaryPage"

    },
    "QVirt::SummaryPage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__SummaryPage[] = {

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

void QVirt::SummaryPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::SummaryPage::staticMetaObject = { {
    &QWizardPage::staticMetaObject,
    qt_meta_stringdata_QVirt__SummaryPage.data,
    qt_meta_data_QVirt__SummaryPage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::SummaryPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::SummaryPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__SummaryPage.stringdata0))
        return static_cast<void*>(this);
    return QWizardPage::qt_metacast(_clname);
}

int QVirt::SummaryPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizardPage::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
