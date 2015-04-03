
#include "./../../protocol/MsgFormats.h"
#include "./../../protocol/KwxMsg.h"

/********************************************************************
	TestCases
*********************************************************************/
#include <assert.h>
#include "./CTestProtocol.h"

class TestNonKwxMsg : public CTestCase {
public:
    virtual int Execute() {

        INT8U msgInNetwork[DnHeader::DN_HEADER_LEN] = {
            0,                     //reserved
        };
        KwxDsMsg *aMsg=KwxDsMsg::getInstance();

		int len = aMsg->Deserialize(msgInNetwork);
        assert(len==KWX_INVALID_PCHC);

        return 0;
    }
};

void testDsDeserializeErrorParam() {
    CTestCase *aCase = new TestNonKwxMsg();
    aCase->Execute();
}

void testErrorParam() {
    testDsDeserializeErrorParam();
}
