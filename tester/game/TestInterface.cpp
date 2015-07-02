
#include "./../../utils/UtilPlatform.h"
#include "./../../hall/VersionManager.h"

void test_write_file() {
    vector<char> *buf = new vector<char>();

    for(int i=0;i<10;i++) {
        char c = '0'+i;
        buf->push_back(c);
    }
    
    _write_file("testfile",buf);
}

void test_download_version() {
    VersionManager *vm = VersionManager::getInstance();
    vm->_download();
}

void test_update_version() {
    VersionManager *vm = VersionManager::getInstance();
    vm->_update();
}

void test_interface() {
    test_write_file();
    test_download_version();
    test_update_version();
}

