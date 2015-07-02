
#ifndef __UTIL_PLATFORM__
#define __UTIL_PLATFORM__

#include "UtilBasic.h"

typedef struct _DeviceInfo_t {
    INT8U mac[128];    
    INT8U imsi[128];    
    INT8U resolution[128];    
    INT8U protoType[128];    
    INT8U osVer[128];
}DeviceInfo_t;

extern void _get_device_info(DeviceInfo_t &device);
extern void _write_file(const char *filename,std::vector<char>* buf);
extern void _update_version();

#endif

