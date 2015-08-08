
#ifndef __ERROR_MANAGER__
#define __ERROR_MANAGER__

#include "./../utils/MsgQueue.h"
#include "./../utils/LogManager.h"

class ErrorManager {
public:
    static ErrorManager *getInstance();
    static void  destroyInstance();

    void set_handle(MsgHandle *handle);
    void notify_error(void *errorMsg);
private:
    ErrorManager();
    ~ErrorManager();

    MsgHandle *_errorHandle;
    
    Logger *_logger;
    static ErrorManager *_instance;
};

#endif

