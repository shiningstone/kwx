
/********************************************************************
	Main entrance
*********************************************************************/
extern void testMsgStruct();
extern void testErrorParam();
extern void testRequests();
extern void testKwxMsgUsingSocket();

#include <string.h>
#include "./../../utils/UtilBasic.h"

void testUtfCoding() {
    wchar_t utf16[256] = {L"ÄãaºÃbÂð234ÖÐ¹ú~!"};
    char    utf8[256];
     
    Utf16ToUtf8(utf16, utf8);
    memset(utf16, 0, sizeof(utf16));
    Utf8ToUtf16(utf8, utf16);
}

void startRun() {
    testUtfCoding();

    testMsgStruct();
    testErrorParam();
    testRequests();
    testKwxMsgUsingSocket();
}

