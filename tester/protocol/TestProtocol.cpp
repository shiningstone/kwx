
/********************************************************************
	Main entrance
*********************************************************************/
extern void testMsgStruct();
extern void testErrorParam();
extern void testRequests();
extern void testKwxMsgUsingSocket();

void startRun() {
    testMsgStruct();
    testErrorParam();
    testRequests();
    testKwxMsgUsingSocket();
}

