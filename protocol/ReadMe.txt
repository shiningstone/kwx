**********************************************************************
	protocolģ�����
	���ڣ�2015.04.03
**********************************************************************
	
�ӿ�ʹ��
	KwxMsgBasic.h    
		�������������Ͷ���
	KwxMsgEnv.h
		����������Ϸ�����в���仯����
	KwxMessenger.h   
		�Զ��հ�      StartReceiving���յ�ÿһ��������_HANDLE_DS_PACKAGES
		����Request   Send
		����request   RequestXxxx��KWX-�ⲿͨѶЭ��4.0.xls�еġ���Ϸģ�顱���壩

�ṹ˵��
	��Ϣ����
		���� ��Deserialize��
			��һ�ν������ֽ���ת�ɰ�����Э���ʽ���������Ϣ��Ա�Ļ�����Ϣ
			�ڶ��ν�������������Ϣ��KwxDsMsg��ת�ɰ�������Ϸģ�顱��������лظ�/֪ͨ��DsInstruction��
		���� ��Serialize��
			��һ�ν�����������Ϸģ�顱�������������RequestXxxxx��ת�ɻ�����Ϣ��KwxUsMsg��
			�ڶ��ν�������������Ϣ������Աת�������ֽ���
	
	KwxMessenger
		���з���UsMsg��RequestXxxxx����
		���е���DsMsg����DsInstruction����diapatch
	DsInstruction
		�����ڶ��ν���������ָ�KWX-�ⲿͨѶЭ��4.0.xls�еġ���Ϸģ�顱���壩���ṩdispatch����֪ͨRoundManager
	RequestXxxxx
		δ����������������KWX-�ⲿͨѶЭ��4.0.xls�еġ���Ϸģ�顱���壩
	CommonMsg��UsMsg��DsMsg��
		�������ν����Ļ�����Ϣ����ԱΪMsgFormats�ж���Ķ���
	MsgFormats
		��Ϣ�Ļ�����Ա��KWX-�ⲿͨѶЭ��4.0.xls�еġ�Э���ʽ�����壩
	DsMsgParser
		���ڽ�������Ϣ��DsMsg���еĲ�������ת�ɾ���ʵ������Ľṹ�壨RequestStructs.h���壩
	RequestStructs
		����ʵ������Ľṹ�壬ΪDsInstruction�ĸ��ӳ�Ա
	MsgIntf
		��Ϣ����ĳ���ӿ�
