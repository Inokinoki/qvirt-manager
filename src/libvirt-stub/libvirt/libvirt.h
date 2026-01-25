/**
 * @file libvirt-stub.h
 * @brief Stub definitions for building without libvirt
 *
 * This header provides minimal stub definitions when libvirt is not available.
 * It allows the code to compile for UI testing and basic functionality.
 */

#ifndef LIBVIRT_STUB_H
#define LIBVIRT_STUB_H

#ifdef __cplusplus
extern "C" {
#endif

/* Stub types */
typedef void *virConnect;
typedef void *virDomain;
typedef void *virDomainPtr;
typedef void *virConnectPtr;
typedef void *virNetwork;
typedef void *virStoragePool;
typedef void *virStorageVol;
typedef void *virNodeDevice;
typedef void *virDomainSnapshot;
typedef void *virStream;

/* Error handling */
#define VIR_FROM_NONE 0
#define VIR_FROM_NETWORK 10
#define VIR_FROM_STORAGE 20
#define VIR_FROM_DOMAIN 30

#define VIR_ERR_OK 0
#define VIR_ERR_NO_SUPPORT 50
#define VIR_ERR_INVALID_ARG 51

typedef struct {
    int code;
    int domain;
    const char *message;
    int level1;
    int level2;
} virError;

typedef void *virErrorPtr;

/* Connection flags */
#define VIR_CONNECT_RO 1
#define VIR_CONNECT_NO_ALIASES 2

/* Domain flags */
#define VIR_DOMAIN_NOSTATE 0
#define VIR_DOMAIN_RUNNING 1
#define VIR_DOMAIN_BLOCKED 2
#define VIR_DOMAIN_PAUSED 3
#define VIR_DOMAIN_SHUTDOWN 4
#define VIR_DOMAIN_SHUTOFF 5
#define VIR_DOMAIN_CRASHED 6
#define VIR_DOMAIN_PMSUSPENDED 7

/* Memory flags */
#define VIR_DOMAIN_MEMORY_STAT_ACTIVE 1
#define VIR_DOMAIN_MEM_CONFIG 2
#define VIR_DOMAIN_MEM_CURRENT 3
#define VIR_DOMAIN_MEM_MAXIMUM 4
#define VIR_DOMAIN_MEM_MINIMUM 5

/* Domain XML flags */
#define VIR_DOMAIN_XML_SECURE 1
#define VIR_DOMAIN_XML_INACTIVE 2
#define VIR_DOMAIN_XML_UPDATE_CPU 4
#define VIR_DOMAIN_XML_MIGRATABLE 8

/* Create flags */
#define VIR_DOMAIN_NONE 0
#define VIR_DOMAIN_START_AUTODESTROY 1
#define VIR_DOMAIN_START_PAUSED 2

#define VIR_DOMAIN_AFFECT_LIVE 1
#define VIR_DOMAIN_AFFECT_CONFIG 2
#define VIR_DOMAIN_AFFECT_CURRENT 4
#define VIR_DOMAIN_AFFECT_CONFIG 2

/* Network flags */
#define VIR_NETWORK_NONE 0
#define VIR_NETWORK_XML_INACTIVE 1

/* Storage pool flags */
#define VIR_STORAGE_POOL_INACTIVE 0
#define VIR_STORAGE_POOL_BUILD_NEW 0
#define VIR_STORAGE_POOL_BUILD_REPAIR 1
#define VIR_STORAGE_POOL_BUILD_RESIZE 2
#define VIR_STORAGE_POOL_DELETE_NORMALIZED 1
#define VIR_STORAGE_POOL_WIPE_ALG_ZERO 0

/* Storage volume flags */
#define VIR_STORAGE_VOL_DELETE_NORMALIZED 1
#define VIR_STORAGE_VOL_DELETE_WITH_SNAPSHOTS 2

/* XML flags */
#define VIR_NODE_DEVICE_XML_INACTIVE 1

#define VIR_CONNECT_LIST_NETWORKS 1
#define VIR_CONNECT_LIST_STORAGE_POOLS 1
#define VIR_CONNECT_LIST_DOMAINS 1
#define VIR_CONNECT_LIST_NODE_DEVICES 1

/* Snapshot flags */
#define VIR_DOMAIN_SNAPSHOT_LIST_ROOTS 1
#define VIR_DOMAIN_SNAPSHOT_REVERT_RUNNING 2
#define VIR_DOMAIN_SNAPSHOT_DELETE_CHILDREN 1
#define VIR_DOMAIN_SNAPSHOT_DELETE_METADATA_ONLY 2

/* Key/value pairs */
typedef void *virTypedParameter;
#define VIR_DOMAIN_AFFECT_LIVE 1
#define VIR_DOMAIN_AFFECT_CONFIG 2
#define VIR_TYPED_PARAM_STRING_OKAY (1 << 7)

/* CPU flags */
#define VIR_DOMAIN_VCPU_LIVE 1
#define VIR_DOMAIN_VCPU_CONFIG 2
#define VIR_DOMAIN_VCPU_CURRENT 4

/* Minimal inline stubs for functions that might be used */
static inline void virConnectClose(virConnect *conn) { (void)conn; }
static inline void virDomainFree(virDomain *domain) { (void)domain; }
static inline void virNetworkFree(virNetwork *network) { (void)network; }
static inline void virStoragePoolFree(virStoragePool *pool) { (void)pool; }
static inline void virStorageVolFree(virStorageVol *vol) { (void)vol; }
static inline void virNodeDeviceFree(virNodeDevice *dev) { (void)dev; }
static inline void virDomainSnapshotFree(virDomainSnapshot *snap) { (void)snap; }

#ifdef __cplusplus
}
#endif

#endif /* LIBVIRT_STUB_H */
