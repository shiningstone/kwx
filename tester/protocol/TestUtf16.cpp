#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "./../../utils/UtilBasic.h"

void test_1_byte() {
    char    Utf8Exp[] = "abcdefg";
    wchar_t Utf16Exp[] = L"abcdefg";

    wchar_t utf16[256] = {L"abcdefg"};
    char    utf8[256]  = {0};

    Utf16ToUtf8(utf16,utf8);
    assert(!strcmp(Utf8Exp,utf8));

    Utf8ToUtf16(utf8,utf16);
    assert(!wcscmp(Utf16Exp,utf16));
}

void test_2_bytes() {
	char    Utf8Exp[4]  = {0xc2,0x80,0,0};
	wchar_t Utf16Exp[2] = {0x80,0};

	char    Utf8Buf[4] = {0};
	Utf16ToUtf8(Utf16Exp,Utf8Buf);
	assert(!memcmp(Utf8Buf,Utf8Exp,4));

	wchar_t Utf16Buf[2] = {0};
	Utf8ToUtf16(Utf8Exp,Utf16Buf);
	assert(!memcmp(Utf16Buf,Utf16Exp,4));
}

void test_utf16() {
    test_1_byte();
	test_2_bytes();
}

