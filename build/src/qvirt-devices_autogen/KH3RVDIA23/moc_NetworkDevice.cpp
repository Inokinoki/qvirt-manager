/****************************************************************************
** Meta object code from reading C++ file 'NetworkDevice.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "devices/NetworkDevice.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NetworkDevice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QVirt__NetworkDevice_t {
    QByteArrayData data[29];
    char stringdata0[223];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QVirt__NetworkDevice_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QVirt__NetworkDevice_t qt_meta_stringdata_QVirt__NetworkDevice = {
    {
QT_MOC_LITERAL(0, 0, 20), // "QVirt::NetworkDevice"
QT_MOC_LITERAL(1, 21, 11), // "NetworkType"
QT_MOC_LITERAL(2, 33, 4), // "None"
QT_MOC_LITERAL(3, 38, 6), // "Bridge"
QT_MOC_LITERAL(4, 45, 7), // "Network"
QT_MOC_LITERAL(5, 53, 6), // "Direct"
QT_MOC_LITERAL(6, 60, 4), // "User"
QT_MOC_LITERAL(7, 65, 9), // "Vhostuser"
QT_MOC_LITERAL(8, 75, 6), // "Server"
QT_MOC_LITERAL(9, 82, 6), // "Client"
QT_MOC_LITERAL(10, 89, 5), // "MCast"
QT_MOC_LITERAL(11, 95, 3), // "UDP"
QT_MOC_LITERAL(12, 99, 7), // "Hostdev"
QT_MOC_LITERAL(13, 107, 9), // "ModelType"
QT_MOC_LITERAL(14, 117, 6), // "Virtio"
QT_MOC_LITERAL(15, 124, 5), // "E1000"
QT_MOC_LITERAL(16, 130, 7), // "RTL8139"
QT_MOC_LITERAL(17, 138, 7), // "Ne2kPci"
QT_MOC_LITERAL(18, 146, 5), // "Pcnet"
QT_MOC_LITERAL(19, 152, 6), // "i82551"
QT_MOC_LITERAL(20, 159, 7), // "i82557b"
QT_MOC_LITERAL(21, 167, 8), // "i82559er"
QT_MOC_LITERAL(22, 176, 6), // "i82801"
QT_MOC_LITERAL(23, 183, 7), // "VMXNET3"
QT_MOC_LITERAL(24, 191, 5), // "Lance"
QT_MOC_LITERAL(25, 197, 9), // "LinkState"
QT_MOC_LITERAL(26, 207, 7), // "Default"
QT_MOC_LITERAL(27, 215, 2), // "Up"
QT_MOC_LITERAL(28, 218, 4) // "Down"

    },
    "QVirt::NetworkDevice\0NetworkType\0None\0"
    "Bridge\0Network\0Direct\0User\0Vhostuser\0"
    "Server\0Client\0MCast\0UDP\0Hostdev\0"
    "ModelType\0Virtio\0E1000\0RTL8139\0Ne2kPci\0"
    "Pcnet\0i82551\0i82557b\0i82559er\0i82801\0"
    "VMXNET3\0Lance\0LinkState\0Default\0Up\0"
    "Down"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QVirt__NetworkDevice[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       3,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, alias, flags, count, data
       1,    1, 0x2,   11,   29,
      13,   13, 0x2,   12,   51,
      25,   25, 0x2,    3,   75,

 // enum data: key, value
       2, uint(QVirt::NetworkDevice::NetworkType::None),
       3, uint(QVirt::NetworkDevice::NetworkType::Bridge),
       4, uint(QVirt::NetworkDevice::NetworkType::Network),
       5, uint(QVirt::NetworkDevice::NetworkType::Direct),
       6, uint(QVirt::NetworkDevice::NetworkType::User),
       7, uint(QVirt::NetworkDevice::NetworkType::Vhostuser),
       8, uint(QVirt::NetworkDevice::NetworkType::Server),
       9, uint(QVirt::NetworkDevice::NetworkType::Client),
      10, uint(QVirt::NetworkDevice::NetworkType::MCast),
      11, uint(QVirt::NetworkDevice::NetworkType::UDP),
      12, uint(QVirt::NetworkDevice::NetworkType::Hostdev),
       2, uint(QVirt::NetworkDevice::ModelType::None),
      14, uint(QVirt::NetworkDevice::ModelType::Virtio),
      15, uint(QVirt::NetworkDevice::ModelType::E1000),
      16, uint(QVirt::NetworkDevice::ModelType::RTL8139),
      17, uint(QVirt::NetworkDevice::ModelType::Ne2kPci),
      18, uint(QVirt::NetworkDevice::ModelType::Pcnet),
      19, uint(QVirt::NetworkDevice::ModelType::i82551),
      20, uint(QVirt::NetworkDevice::ModelType::i82557b),
      21, uint(QVirt::NetworkDevice::ModelType::i82559er),
      22, uint(QVirt::NetworkDevice::ModelType::i82801),
      23, uint(QVirt::NetworkDevice::ModelType::VMXNET3),
      24, uint(QVirt::NetworkDevice::ModelType::Lance),
      26, uint(QVirt::NetworkDevice::LinkState::Default),
      27, uint(QVirt::NetworkDevice::LinkState::Up),
      28, uint(QVirt::NetworkDevice::LinkState::Down),

       0        // eod
};

void QVirt::NetworkDevice::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QVirt::NetworkDevice::staticMetaObject = { {
    &Device::staticMetaObject,
    qt_meta_stringdata_QVirt__NetworkDevice.data,
    qt_meta_data_QVirt__NetworkDevice,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QVirt::NetworkDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QVirt::NetworkDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QVirt__NetworkDevice.stringdata0))
        return static_cast<void*>(this);
    return Device::qt_metacast(_clname);
}

int QVirt::NetworkDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Device::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
