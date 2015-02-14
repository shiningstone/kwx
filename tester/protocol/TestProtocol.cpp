
#include "./../../protocol/MsgFormats.h"

/********************************************************************
	TestCases
*********************************************************************/
#include <assert.h>
#include "./CTestProtocol.h"

class TestUpHeader : public CTestCase {
public:
    virtual int Execute() {
		//解包测试
        const INT8U msgInNetwork[UpHeader::UP_HEADER_LEN] = {
            'K','W','X',           //KWX
            0x10,                  //protocol version
            0x01,0x02,0x03,0x04,   //user id
            0x05,                  //language id
            0x06,                  //client platform
            0x07,                  //client build number
            0x08,0x09,             //customer id
            0x0a,0x0b,             //product id
            0x0c,0x0d,             //request code
            0x0e,0x0f,             //package size
            0,                     //reserved
        };
        UpHeader aHeader;

		aHeader.Deserialize(msgInNetwork);
		assert(aHeader._protocol==0x10);
		assert(aHeader._userId==0x01020304);
		assert(aHeader._customerId==0x0809);
		assert(aHeader._productId==0x0a0b);
		assert(aHeader._requestCode==0x0c0d);
        assert(aHeader._size==0x0e0f);
		
		//组包测试
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        UpHeader bHeader;
		bHeader._protocol=16;
		bHeader._userId=0x01020304;
		bHeader._language=0x05;
		bHeader._platform=0x06;
		bHeader._buildNo=0x07;
		bHeader._customerId=0x0809;
		bHeader._productId=0x0a0b;
		bHeader._requestCode=0x0c0d;
        bHeader._size=0x0e0f;

        len = bHeader.Serialize(buf);

        assert(len==30);
        assert(!memcmp(msgInNetwork,buf,len));

		return OK;
    }
};

class TestDnHeader : public CTestCase {
public:
    virtual int Execute() {
		//解包测试
        const INT8U msgInNetwork[DnHeader::DN_HEADER_LEN] = {
            'K','W','X',           //KWX
            0x01,0x02,             //request code
            0x03,                  //level
            0x04,0x05,             //package size
            0,                     //reserved
        };
        DnHeader aHeader;

		aHeader.Deserialize(msgInNetwork);
		assert(aHeader._requestCode==0x0102);
		assert(aHeader._level==0x03);
		assert(aHeader._size==0x0405);
		
		//组包测试
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        DnHeader bHeader;
		bHeader._requestCode=0x0102;
        bHeader._level=0x03;
        bHeader._size=0x0405;

        len = bHeader.Serialize(buf);

        assert(len==20);
        assert(!memcmp(msgInNetwork,buf,len));

		return OK;
    }
};

class TestItemOnePureId : public CTestCase {
public:
    virtual int Execute() {
        const INT8U msgInNetwork[] = {
            0,
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        //解包测试
        Item aItem;

		len = aItem.Deserialize(msgInNetwork);
        assert(len==1);
		assert(aItem._id==0);

		//组包测试
        Item bItem;
		bItem._id=0;

        len = bItem.Serialize(buf);

        assert(len==1);
        assert(!memcmp(msgInNetwork,buf,len));

		return OK;
    }
};

class TestItemOneIdPlusInt : public CTestCase {
public:
    virtual int Execute() {
        const INT8U msgInNetwork[] = {
            50,51
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        //解包测试
        Item aItem;

		len = aItem.Deserialize(msgInNetwork);
        assert(len==2);
		assert(aItem._id==50);
        assert(aItem._value=51);

		//组包测试
        Item bItem;
		bItem._id=50;
		bItem._value=51;

        len = bItem.Serialize(buf);

        assert(len==2);
        assert(!memcmp(msgInNetwork,buf,len));

		return OK;
    }
};

class TestItemOneIdPlusBuf : public CTestCase {
public:
    virtual int Execute() {
        const INT8U msgInNetwork[] = {
            128,1,0
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        //解包测试
        Item aItem;

		len = aItem.Deserialize(msgInNetwork);
        assert(len==3);
		assert(aItem._id==128);
        assert(aItem._bufLen==1);
        assert(!memcmp(aItem._buf,msgInNetwork+2,aItem._bufLen));

		//组包测试
        Item bItem;
		bItem._id=128;
        bItem._bufLen=1;
        memset(bItem._buf, 0, bItem._bufLen);

        len = bItem.Serialize(buf);

        assert(len==3);
        assert(!memcmp(msgInNetwork,buf,len));

		return OK;
    }
};

class TestItemOneIdPlusLongBuf : public CTestCase {
public:
    virtual int Execute() {
        const INT8U msgInNetwork[] = {
            128,2,0,1
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        //解包测试
        Item aItem;

		len = aItem.Deserialize(msgInNetwork);
        assert(len==4);
		assert(aItem._id==128);
        assert(aItem._bufLen==2);
        assert(!memcmp(aItem._buf,msgInNetwork+2,aItem._bufLen));

		//组包测试
        Item bItem;
		bItem._id=128;
        bItem._bufLen=2;
        bItem._buf[0]=0;
        bItem._buf[1]=1;

        len = bItem.Serialize(buf);

        assert(len==4);
        assert(!memcmp(msgInNetwork,buf,len));

		return OK;
    }
};

class TestMsgBody : public CTestCase {
public:
    virtual int Execute() {
        const INT8U msgInNetwork[] = {
            1,0,
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        //解包测试
        MsgBody aBody;

		len = aBody.Deserialize(msgInNetwork);
        assert(len==2);
        assert(aBody._itemNum==1);
		assert(aBody._items[0]->_id==0);

		//组包测试
        MsgBody bBody;
        bBody._itemNum=1;
        Item *aItem = new Item();
		aItem->_id=0;
        bBody._items[0] = aItem;

        len = bBody.Serialize(buf);

        assert(len==2);
        assert(!memcmp(msgInNetwork,buf,len));

		return OK;
    }
};

class TestItemTwoPureId : public CTestCase {
public:
    virtual int Execute() {
        const INT8U msgInNetwork[] = {
            2,0,1
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        //解包测试
        MsgBody aBody;

		len = aBody.Deserialize(msgInNetwork);
        assert(len==3);
        assert(aBody._itemNum==2);
		    assert(aBody._items[0]->_id==0);
		    assert(aBody._items[1]->_id==1);

		//组包测试
        MsgBody bBody;
        bBody._itemNum=2;

            Item *aItem = new Item();
		    aItem->_id=0;
            bBody._items[0] = aItem;

            Item *bItem = new Item();
		    bItem->_id=1;
            bBody._items[1] = bItem;

        len = bBody.Serialize(buf);

        assert(len==3);
        assert(!memcmp(msgInNetwork,buf,len));

		return OK;
    }
};

class TestItemOnePureIdTwoWithValue : public CTestCase {
public:
    virtual int Execute() {
        const INT8U msgInNetwork[] = {
            2,
            0,
            50,2
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        //解包测试
        MsgBody aBody;

		len = aBody.Deserialize(msgInNetwork);
        assert(len==4);
        assert(aBody._itemNum==2);
		    assert(aBody._items[0]->_id==0);
		    assert(aBody._items[1]->_id==50);
		    assert(aBody._items[1]->_value==2);

		//组包测试
        MsgBody bBody;
        bBody._itemNum=2;

            Item *aItem = new Item();
		    aItem->_id=0;
            bBody._items[0] = aItem;

            Item *bItem = new Item();
		    bItem->_id=50;
		    bItem->_value=2;
            bBody._items[1] = bItem;

        len = bBody.Serialize(buf);

        assert(len==4);
        assert(!memcmp(msgInNetwork,buf,len));

		return OK;
    }
};

class TestItemOneWithBufTwoWithValueThreePureId : public CTestCase {
public:
    virtual int Execute() {
        const INT8U msgInNetwork[] = {
            3,
            128,4,0,1,2,3,
            50,4,
            0
        };
        INT8U buf[MSG_MAX_LEN] = {0};
        int   len = 0;

        //解包测试
        MsgBody aBody;

		len = aBody.Deserialize(msgInNetwork);
        assert(len==sizeof(msgInNetwork));
        assert(aBody._itemNum==3);
		    assert(aBody._items[0]->_id==128);
		    assert(aBody._items[0]->_bufLen==4);
		    assert( !memcmp(aBody._items[0]->_buf,msgInNetwork+3,4) );
		    
            assert(aBody._items[1]->_id==50);
		    assert(aBody._items[1]->_value==4);

            assert(aBody._items[2]->_id==0);
		//组包测试
        MsgBody bBody;
        bBody._itemNum=3;

            bBody._items[0] = new Item();
		    bBody._items[0]->_id=128;
            bBody._items[0]->_bufLen=4;
            bBody._items[0]->_buf[0]=0;
            bBody._items[0]->_buf[1]=1;
            bBody._items[0]->_buf[2]=2;
            bBody._items[0]->_buf[3]=3;

            bBody._items[1] = new Item();
		    bBody._items[1]->_id=50;
		    bBody._items[1]->_value=4;

            bBody._items[2] = new Item();
		    bBody._items[2]->_id=0;

        len = bBody.Serialize(buf);

        assert(len==sizeof(msgInNetwork));
        assert( !memcmp(msgInNetwork,buf,len) );

		return OK;
    }
};
/********************************************************************
	Main entrance
*********************************************************************/
void startRun() {
    CTestCase *aCase = new TestUpHeader();
    aCase->Start();
    aCase->Execute();
    aCase->Stop();

    aCase = new TestDnHeader();
    aCase->Start();
    aCase->Execute();
    aCase->Stop();

    aCase = new TestItemOnePureId();
    aCase->Start();
    aCase->Execute();
    aCase->Stop();

    aCase = new TestItemOneIdPlusInt();
    aCase->Start();
    aCase->Execute();
    aCase->Stop();

    aCase = new TestItemOneIdPlusBuf();
    aCase->Start();
    aCase->Execute();
    aCase->Stop();

    aCase = new TestItemOneIdPlusLongBuf();
    aCase->Start();
    aCase->Execute();
    aCase->Stop();

    aCase = new TestMsgBody();
    aCase->Start();
    aCase->Execute();
    aCase->Stop();

    aCase = new TestItemTwoPureId();
    aCase->Start();
    aCase->Execute();
    aCase->Stop();

    aCase = new TestItemOnePureIdTwoWithValue();
    aCase->Start();
    aCase->Execute();
    aCase->Stop();

    aCase = new TestItemOneWithBufTwoWithValueThreePureId();
    aCase->Start();
    aCase->Execute();
    aCase->Stop();
}

