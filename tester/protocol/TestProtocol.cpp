
/********************************************************************
	Main entrance
*********************************************************************/
extern void testMsgStruct();
extern void testErrorParam();
extern void testRequests();
extern void testKwxMsgUsingSocket();

extern void test_utf16();

void startRun() {
    test_utf16();

    testMsgStruct();
    testErrorParam();
    testRequests();
    testKwxMsgUsingSocket();
}

