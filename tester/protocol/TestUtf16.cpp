#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "./../../utils/UtilBasic.h"

void test_1_byte() {
    char    Utf8Exp[] = "abcdefg";
    wchar_t Utf16Exp[] = L"abcdefg";

    wchar_t utf16[256] = {L"abcdefg"};
    char    utf8[256]  = {0};

    int len = 0;

    len = Utf16ToUtf8(utf16,utf8);
    assert(!strcmp(Utf8Exp,utf8));
    assert(len==14);

    len = Utf8ToUtf16(utf8,utf16);
    assert(!wcscmp(Utf16Exp,utf16));
    assert(len==7);
}

void test_2_bytes() {
	char    Utf8Exp[4]  = {0xc2,0x80,0,0};
	wchar_t Utf16Exp[2] = {0x80,0};
    int len = 0;

	char    Utf8Buf[4] = {0};
	len = Utf16ToUtf8(Utf16Exp,Utf8Buf);
	assert(!memcmp(Utf8Buf,Utf8Exp,4));
    assert(len==2);

	wchar_t Utf16Buf[2] = {0};
	len = Utf8ToUtf16(Utf8Exp,Utf16Buf);
	assert(!memcmp(Utf16Buf,Utf16Exp,4));
    assert(len==2);
}

void test_3_bytes() {
	char    Utf8Exp[4]  = {0xe0,0xa0,0x80,0};
	wchar_t Utf16Exp[2] = {0x800,0};
    int len = 0;

	char    Utf8Buf[4] = {0};
	len = Utf16ToUtf8(Utf16Exp,Utf8Buf);
	assert(!memcmp(Utf8Buf,Utf8Exp,4));
    assert(len==2);

	wchar_t Utf16Buf[2] = {0};
	len = Utf8ToUtf16(Utf8Exp,Utf16Buf);
	assert(!memcmp(Utf16Buf,Utf16Exp,4));
    assert(len==3);
}

void test_utf16_name() {
	char Utf16Buf[] = {0xfe,0xff,0x73,0x8b,0x5c,0x0f,0x4e,0x8c,0,0};

	char    Utf8Buf[4] = {0};
	Utf16ToUtf8((Utf16 *)Utf16Buf,Utf8Buf);
}

#include "./../../protocol/MsgFormats.h"
#include "./../../protocol/CommonMsg.h"
#include "./../../protocol/DsInstruction.h"
#include "./../../protocol/KwxMsgLogin.h"
void test_split() {
    wchar_t utf16Buf[] = L"name1%@name2%@name3";
    INT8U strings[3][128] = {{0}};

    EnterRoomResponse::_LoadStrings(strings,(INT8U *)utf16Buf,wcslen(utf16Buf));

    assert(!strcmp((char *)strings[1],"name1"));
    assert(!strcmp((char *)strings[2],"name2"));
    assert(!strcmp((char *)strings[0],"name3"));
}

void test_utf16() {
    test_1_byte();
	test_2_bytes();
    test_3_bytes();

	test_utf16_name();

    test_split();
}

