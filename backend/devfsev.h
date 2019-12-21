#ifndef INCLUDE_FM_FSEV_H
#define INCLUDE_FM_FSEV_H

/*
  TODO: this stuff must be refactorized to not be fsev-specific
  TODO: because it is used from non-devfsev backends
*/
// fsevents is available here:
// http://www.opensource.apple.com/source/xnu/xnu-792/bsd/sys/fsevents.h
//#include <sys/fsevents.h> would have been nice, but it's no longer available, as Apple
// now wraps this with FSEventStream. So instead - rip what we need from the kernel headers..

#if 0
typedef struct kfs_event_a {
	uint16_t type;
	uint16_t refcount;
	pid_t    pid;
} kfs_event_a;

typedef struct kfs_event_arg {
	uint16_t type;
	uint16_t pathlen;
	char data[0];
} kfs_event_arg;
#endif

// Actions for each event type
#define FSE_IGNORE    0
#define FSE_REPORT    1
#define FSE_ASK       2    // Not implemented yet

#define FSEVENTS_CLONE               _IOW('s', 1, fsevent_clone_args)
#define FSEVENTS_DEVICE_FILTER       _IOW('s', 100, fsevent_dev_filter_args)
#define FSEVENTS_WANT_COMPACT_EVENTS _IO('s', 101)
#define FSEVENTS_WANT_EXTENDED_INFO  _IO('s', 102)
#define FSEVENTS_GET_CURRENT_ID      _IOR('s', 103, uint64_t)

#define FSE_TYPE_MASK          0x0fff
#define FSE_FLAG_MASK          0xf000
#define FSE_FLAG_SHIFT         12
#define FSE_GET_FLAGS(type)    (((type) >> 12) & 0x000f)

#define FSE_COMBINED_EVENTS          0x0001
#define FSE_CONTAINS_DROPPED_EVENTS  0x0002

#define FSE_MODE_HLINK         (1 << 31)    // notification is for a hard-link
#define FSE_MODE_LAST_HLINK    (1 << 30)    // link count == 0 on a hard-link delete
#define FSE_REMOTE_DIR_EVENT   (1 << 29)    // this is a remotely generated directory-level granularity event
#define FSE_TRUNCATED_PATH     (1 << 28)    // the path for this item had to be truncated

#define FSE_INVALID             -1
#define FSE_CREATE_FILE          0
#define FSE_DELETE               1
#define FSE_STAT_CHANGED         2
#define FSE_RENAME               3
#define FSE_CONTENT_MODIFIED     4
#define FSE_EXCHANGE             5
#define FSE_FINDER_INFO_CHANGED  6
#define FSE_CREATE_DIR           7
#define FSE_CHOWN                8
#define FSE_XATTR_MODIFIED       9
#define FSE_XATTR_REMOVED        10
#define FSE_MAX_EVENTS           11

/* linux specific */
#define FSE_OPEN -2
#define FSE_UNKNOWN -3

#define FSE_ALL_EVENTS         998

#define FSE_EVENTS_DROPPED     999

// The types of each of the arguments for an event
// Each type is followed by the size and then the
// data.  FSE_ARG_VNODE is just a path string

#define FSE_ARG_NONE     0x0000   // unknown / invalid arg type
#define FSE_ARG_VNODE    0x0001   // next arg is a vnode pointer
#define FSE_ARG_STRING   0x0002   // next arg is length followed by string ptr
#define FSE_ARG_PATH     0x0003   // next arg is a full path
#define FSE_ARG_INT32    0x0004   // next arg is a 32-bit int
#define FSE_ARG_INT64    0x0005   // next arg is a 64-bit int
#define FSE_ARG_RAW      0x0006   // next arg is a length followed by a void ptr
#define FSE_ARG_INO      0x0007   // next arg is the inode number (ino_t)
#define FSE_ARG_UID      0x0008   // next arg is the file's uid (uid_t)
#define FSE_ARG_DEV      0x0009   // next arg is the file's dev_t
#define FSE_ARG_MODE     0x000a   // next arg is the file's mode (as an int32, file type only)
#define FSE_ARG_GID      0x000b   // next arg is the file's gid (gid_t)
#define FSE_ARG_FINFO    0x000c   // next arg is a packed finfo (dev, ino, mode, uid, gid)
#define FSE_ARG_DONE     0xb33f   // no more arguments

#define FSE_MAX_ARGS     12

#if __LP64__
typedef struct fsevent_clone_args {
	int8_t *event_list;
	int32_t num_events;
	int32_t event_queue_depth;
	int32_t *fd;
} fsevent_clone_args;
#else
typedef struct fsevent_clone_args {
	int8_t *event_list;
	int32_t pad1;
	int32_t num_events;
	int32_t event_queue_depth;
	int32_t *fd;
	int32_t pad2;
} fsevent_clone_args;
#endif

#endif
