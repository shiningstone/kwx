
#ifndef __LOG_MANAGER__
#define __LOG_MANAGER__

#define MAX_MODULES         10
#define MODULE_NAME_LEN     32

#define RET_OK              0
#define RET_ERROR           1

class Logger {
public :
    Logger(const char *name);
    
    int  Write(const char * format, ...);
    int  WritePackage(char *buf,int len);
    void Set(bool enable);
    bool IsEnabled();

    char _owner[MODULE_NAME_LEN];
    bool _isEnabled;
};

class LogManager {
public :
    static LogManager *GetInstance();
    static void DestroyInstance();
    
    Logger *Recruit(const char *moduleName);
    static void Dismiss(Logger *target);

private :
    LogManager();

    static LogManager *_instance;
    static Logger     *_employees[MAX_MODULES];

    static Logger *Find(const char *moduleName);
    static int Register(Logger *l);
    static int Deregister(Logger *l);
};

#endif
