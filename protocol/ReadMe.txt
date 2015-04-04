**********************************************************************
	protocol模块介绍
	日期：2015.04.03
**********************************************************************
	
接口使用
	KwxMsgBasic.h    
		基本常量、类型定义
	KwxMsgEnv.h
		用于设置游戏过程中不需变化的量
	KwxMessenger.h   
		自动收包      StartReceiving，收到每一个包调用_HANDLE_DS_PACKAGES
		发送Request   Send
		设置request   RequestXxxx（KWX-外部通讯协议4.0.xls中的“游戏模块”定义）

结构说明
	消息解析
		下行 （Deserialize）
			第一次解析：字节流转成包含“协议格式”定义的消息成员的基本消息
			第二次解析：将基本消息（KwxDsMsg）转成包含“游戏模块”定义的下行回复/通知（DsInstruction）
		上行 （Serialize）
			第一次解析：将“游戏模块”定义的上行请求（RequestXxxxx）转成基本消息（KwxUsMsg）
			第二次解析：将基本消息各个成员转成有序字节流
	
	KwxMessenger
		上行发送UsMsg（RequestXxxxx）；
		下行调用DsMsg构造DsInstruction，并diapatch
	DsInstruction
		经过第二次解析的下行指令（KWX-外部通讯协议4.0.xls中的“游戏模块”定义），提供dispatch方法通知RoundManager
	RequestXxxxx
		未经解析的上行请求（KWX-外部通讯协议4.0.xls中的“游戏模块”定义）
	CommonMsg（UsMsg、DsMsg）
		经过初次解析的基本消息，成员为MsgFormats中定义的对象
	MsgFormats
		消息的基本成员（KWX-外部通讯协议4.0.xls中的“协议格式”定义）
	DsMsgParser
		用于将基本消息（DsMsg）中的部分内容转成具有实际意义的结构体（RequestStructs.h定义）
	RequestStructs
		具有实际意义的结构体，为DsInstruction的复杂成员
	MsgIntf
		消息构造的抽象接口
