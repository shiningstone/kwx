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
		�Զ��հ�      StartReceiving
		����Request   Send
		����request   RequestXxxx��KWX-�ⲿͨѶЭ��4.0.xls�еġ���Ϸģ�顱���壩

�ṹ˵��
	����
		��һ�ν������ֽ���ת�ɰ�����Э���ʽ���������Ϣ��Ա�Ļ�����Ϣ
		�ڶ��ν�������������Ϣ��KwxDsMsg��ת�ɰ�������Ϸģ�顱��������лظ�/֪ͨ��DsInstruction��
	����
		��һ�ν�����������Ϸģ�顱�������������RequestXxxxx��ת�ɻ�����Ϣ��KwxUsMsg��
		�ڶ��ν�������������Ϣ������Աת�������ֽ���
	
	KwxMessenger
		���з���KwxUsMsg��RequestXxxxx����
		���е���KwxDsMsg����DsInstruction����diapatch
	DsInstruction
		�����ڶ��ν���������ָ�KWX-�ⲿͨѶЭ��4.0.xls�еġ���Ϸģ�顱���壩���ṩdispatch����֪ͨRoundManager
	RequestXxxxx
		δ����������������KWX-�ⲿͨѶЭ��4.0.xls�еġ���Ϸģ�顱���壩
	CommonMsg��KwxUsMsg��KwxDsMsg��
		�������ν����Ļ�����Ϣ����ԱΪMsgFormats�ж���Ķ���
	MsgFormats
		��Ϣ�Ļ�����Ա��KWX-�ⲿͨѶЭ��4.0.xls�еġ�Э���ʽ�����壩
	DsMsgParser
		���ڽ�������Ϣ��KwxDsMsg��ת�ɾ���ʵ������Ľṹ�壨RequestStructs.h���壩
	RequestStructs
		����ʵ������Ľṹ�壬ΪDsInstruction�ĸ��ӳ�Ա
	MsgIntf
		��Ϣ����ĳ���ӿ�
