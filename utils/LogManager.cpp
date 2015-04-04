
#include "LogManager.h"

#ifdef __DISABLE_LOGGER__

//BLANK INTENTIONALLY

#else
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

//////////////////////////////////////////////////
//              Logger
//////////////////////////////////////////////////
Logger::Logger(const char *name) {
    memset( _owner, 0, MODULE_NAME_LEN );
    memcpy( _owner, name, strlen(name) );
    _isEnabled = true;
}

void Logger::Set(bool value) {
    _isEnabled = value;
}

#ifdef __UNIT_TEST__
#define log(x) printf(x)
#else
/*************************************
    use cocos2d log function
*************************************/
#include "cocos2d.h"
USING_NS_CC;
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;
#endif

/******************************************************
    WINDOWS����Ҫ����C/C++�궨�壺 _CRT_SECURE_NO_WARNINGS
    ��֪��Ϊʲô�ڴ������á�#define����������
******************************************************/
bool Logger::IsEnabled() {
    return _isEnabled;
}

int Logger::Write(const char * format, ...) {
    if ( _isEnabled ) {
        char buf[256] = {0};

        sprintf(buf, "[%s]: ", _owner);

        va_list args;
        va_start(args, format);
        vsprintf( buf+strlen(_owner)+strlen("[]: "), format, args );
        va_end(args);

        log(buf);
    }

    return RET_OK;
}

#include <ctype.h>
int Logger::WriteArray(const char *pkg,int len) {
    char buf[1024] = {0};
    int usedBytes  = 0;

	sprintf(buf+usedBytes, "\t");
    usedBytes += 1;

	for(int i=0; i<len; i++) {
		sprintf(buf+usedBytes, "%02x ",(unsigned char)pkg[i]);
        usedBytes += 3;

		if( (i+1)%16==0 ) {
			sprintf(buf+usedBytes, "\n\t");
            usedBytes += 2;
		}
	}
	
    sprintf(buf+usedBytes, "\n");

    log(buf);

    return RET_OK;
}

//////////////////////////////////////////////////
//              LogManager
//////////////////////////////////////////////////
LogManager::LogManager() {
    for ( int i=0; i<MAX_MODULES; i++ ) {
        _employees[i] = NULL;
    }
}

Logger *LogManager::Recruit(const char *moduleName) {
    Logger * logger = NULL;

    if ( (logger=Find(moduleName))==NULL ) {
        logger = new Logger(moduleName);
        Register(logger);
    }

    return logger;
}

void LogManager::Dismiss(Logger *target) {
    Logger *logger = NULL;
    
    char *moduleName = target->_owner;
    if ( (logger=Find(moduleName))!=NULL ) {
        Deregister(logger);
        delete logger;
    }
}

Logger *LogManager::Find(const char *moduleName) {
    for ( int i=0; i<MAX_MODULES; i++ ) {
        if ( _employees[i] && !strcmp( _employees[i]->_owner, moduleName ) ) {
            return _employees[i];
        }
    }

    return NULL;
}

int LogManager::Register(Logger *logger) {
    for ( int i=0; i<MAX_MODULES; i++ ) {
        if ( _employees[i]==NULL ) {
            _employees[i] = logger;
            return RET_OK;
        }
    }

    return RET_ERROR;
}

int LogManager::Deregister(Logger *logger) {
    for ( int i=0; i<MAX_MODULES; i++ ) {
        if ( _employees[i]==logger ) {
            _employees[i] = NULL;
        }
    }

    return RET_OK;
}


/*************************************
    singleton
*************************************/
LogManager* LogManager::_instance = NULL;
Logger *LogManager::_employees[] = {0};

LogManager *LogManager::GetInstance() {
    if (_instance==NULL) {
        _instance = new LogManager();
    }

    return _instance;
}

void LogManager::DestroyInstance() {
    for ( int i=0; i<MAX_MODULES; i++ ) {
        if ( _employees[i]!=NULL ) {
            Dismiss(_employees[i]);
            _employees[i] = NULL;
        }
    }

    delete _instance;
    _instance = NULL;
}

#endif/*function control macro*/