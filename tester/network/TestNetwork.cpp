#include <thread>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#include <assert.h>
#include "./../../network/CSockets.h"
#include "./../../network/GameSocket.h"
#include "./CTestNetwork.h"

#define DELAY 100

class TestSingleSendAndRecv : public CTestNetwork {
	virtual void ServerActions() {
		SERVER.Start();

		SERVER.Send("to client",strlen("to client"));

		SERVER.Recv(_serverBuff,&_serverBuffLen);
		assert(!strcmp(_serverBuff,"to server"));
		ClearServerBuf();

		SERVER.Stop();
	}

	virtual void ClientActions() {
		CLIENT.Start();

		CLIENT.Send("to server",strlen("to server"));

		CLIENT.Recv(_clientBuff,&_clientBuffLen);
		assert( !strcmp(_clientBuff,"to client") );
		ClearClientBuf();

		CLIENT.Stop();
	}
};

class TestGameSocket : public CTestNetwork {
	virtual void ServerActions() {
		SERVER.Start();

		_serverBuffLen = 10;
		memset(_serverBuff,0,8);
		SERVER.Send("to client",strlen("to client"));
		SERVER.Recv(_serverBuff,&_serverBuffLen);
		Sleep(DELAY);
		ClearServerBuf();

		SERVER.Stop();
	}

	virtual void ClientActions() {
		CGameSocket client;
		client.Create(SERVER_IP, SOCKET_PORT);
		
		client.ForceSend("to server",strlen("to client"));

		_clientBuffLen = 10;
		client.ForceRecv(_clientBuff,_clientBuffLen);
		Sleep(DELAY);
		ClearClientBuf();
	}
};

class TestByteOrder : public CTestNetwork {
	#pragma pack(1)
	typedef struct {
		char a[3];
		int  b;
		char c;
		short d;
	} tStruct;
	#pragma pack()

	virtual void ServerActions() {
		SERVER.Start();

		int a = 0x01020304;
		SERVER.Recv(_serverBuff,&_serverBuffLen);
		assert(!memcmp(_serverBuff,(char *)&a,sizeof(int)));
		assert(_serverBuffLen==4);
		ClearServerBuf();

		tStruct t = {
			{0xa,0xb,0xc},
			0x01020304,
			0x05,
			0x06,
		};
		SERVER.Recv(_serverBuff,&_serverBuffLen);
		assert(!memcmp(_serverBuff,(char *)&t,sizeof(t)));
		assert(_serverBuffLen==10);
		ClearServerBuf();

		SERVER.Stop();
	}

	virtual void ClientActions() {
		CLIENT.Start();

		//test int
		int a = 0x01020304;
		CLIENT.Send((char *)&a,sizeof(int));

		//test struct
		tStruct t = {
			{0xa,0xb,0xc},
			0x01020304,
			0x05,
			0x06,
		};
		CLIENT.Send((char *)&t,sizeof(t));

		CLIENT.Stop();
	}
};

class TestMultipleSendAndRecv : public CTestNetwork {
	virtual void ServerActions() {
		char buf[32] = {0};
		int i        = 0;

		SERVER.Start();

		sprintf_s(buf,"to client %d",i++);
		SERVER.Send(buf,strlen(buf));
		Sleep(DELAY);

		sprintf_s(buf,"to client %d",i++);
		SERVER.Send(buf,strlen(buf));

		SERVER.Recv(_serverBuff,&_serverBuffLen);
		if( (!strcmp(_serverBuff,"to server 0")) || (strstr(_serverBuff,"to server 0")!=0) ) {
			if ( strstr(_serverBuff,"to server 1")!=0 ) {
				SERVER.Stop();
				return;
			} else {
				ClearServerBuf();

				SERVER.Recv(_serverBuff,&_serverBuffLen);
				assert( !strcmp(_serverBuff,"to server 1") );
			}
		} else {
			assert("server : the first package missed\n");
		}

		SERVER.Stop();
	}

	virtual void ClientActions() {
		char buf[32] = {0};
		int i        = 0;

		CLIENT.Start();

		sprintf_s(buf,"to server %d",i++);
		CLIENT.Send(buf,strlen(buf));
		Sleep(DELAY);

		sprintf_s(buf,"to server %d",i++);
		CLIENT.Send(buf,strlen(buf));

		CLIENT.Recv(_clientBuff,&_clientBuffLen);
		if( (!strcmp(_clientBuff,"to client 0")) || (strstr(_clientBuff,"to client 0")!=0) ) {
			if ( strstr(_clientBuff,"to client 1")!=0 ) {
				CLIENT.Stop();
				return;
			} else {
				ClearClientBuf();

				CLIENT.Recv(_clientBuff,&_clientBuffLen);
				assert( !strcmp(_clientBuff,"to client 1") );
			}
		} else {
			assert("CLIENT : the first package missed\n");
		}

		CLIENT.Stop();
	}
};

class TestEmpty : public CTestNetwork {
	virtual void ServerActions() {
	}

	virtual void ClientActions() {
	}
};

void startRun() {
	CTestCase *aCase;

	aCase = new TestSingleSendAndRecv();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

	aCase = new TestByteOrder();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

	aCase = new TestMultipleSendAndRecv();
	aCase->Start();
	aCase->Execute();
	aCase->Stop();

#if 1
	aCase = new TestGameSocket();   //���caseִ���Ժ�socket startʧ��
	aCase->Start();
	aCase->Execute();
	aCase->Stop();
#endif
}

