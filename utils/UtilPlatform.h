
#ifndef __UTIL_PLATFORM__
#define __UTIL_PLATFORM__

#include "UtilBasic.h"

extern void _get_device_info(DeviceInfo_t &device);
extern void _write_file(const char *filename,std::vector<char>* buf);
extern void _update_version(const char *apk);
extern int  _get_file_size(const char *filename);
extern long _get_system_time();

extern void _ali_pay(const char *product,const char *desc,const char *price);
extern bool _ali_is_account_exist();
extern string _ali_get_sdk_version();

#endif

