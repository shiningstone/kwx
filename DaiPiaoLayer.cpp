#include "DaiPiaoLayer.h"


DaiPiaoLayer::DaiPiaoLayer(void)
{
}


DaiPiaoLayer::~DaiPiaoLayer(void)
{
}

bool DaiPiaoLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();

	auto buttonKaWuXing=Button::create("bingyise.png","bingyise.png","bingyise.png",TextureResType::PLIST);
	auto kaWuXingSize=buttonKaWuXing->getContentSize();
	buttonKaWuXing->setAnchorPoint(Vec2(0.5,0.5));
	buttonKaWuXing->setPosition(Vec2(origin.x+visibleSize.width*0.026+kaWuXingSize.width/2,origin.y+visibleSize.height*0.43+kaWuXingSize.height/2));
	buttonKaWuXing->addTouchEventListener(CC_CALLBACK_2(DaiPiaoLayer::onButtonKaWuXing,this));
	this->addChild(buttonKaWuXing,1,11);

	auto buttonTiaoYiSe=Button::create("dasanyuan.png","dasanyuan.png","dasanyuan.png",TextureResType::PLIST);
	auto sizeOfButtonTiaoYiSe=buttonTiaoYiSe->getContentSize();
	buttonTiaoYiSe->setAnchorPoint(Vec2(0.5,0.5));
	buttonTiaoYiSe->setPosition(Vec2(origin.x+visibleSize.width*0.326+sizeOfButtonTiaoYiSe.width/2,origin.y+visibleSize.height*0.43+sizeOfButtonTiaoYiSe.height/2));
	buttonTiaoYiSe->addTouchEventListener(CC_CALLBACK_2(DaiPiaoLayer::onButtonTiaoYiSe,this));
	this->addChild(buttonTiaoYiSe,1,33);

	auto buttonGangShangHua=Button::create("mingsigui.png","mingsigui.png","mingsigui.png",TextureResType::PLIST);
	auto sizeOfButtonGangShangHua=buttonGangShangHua->getContentSize();
	buttonGangShangHua->setAnchorPoint(Vec2(0.5,0.5));
	buttonGangShangHua->setPosition(Vec2(origin.x+visibleSize.width*0.626+sizeOfButtonGangShangHua.width/2,origin.y+visibleSize.height*0.43+sizeOfButtonGangShangHua.height/2));
	buttonGangShangHua->addTouchEventListener(CC_CALLBACK_2(DaiPiaoLayer::onButtonGangShangHua,this));
	this->addChild(buttonGangShangHua,1,54);
	
	auto buttonAnSiGui=Button::create("gangshanggang.png","gangshanggang.png","gangshanggang.png",TextureResType::PLIST);
	auto sizeOfButtonAnSiGui=buttonAnSiGui->getContentSize();
	buttonAnSiGui->setAnchorPoint(Vec2(0.5,0.5));
	buttonAnSiGui->setPosition(Vec2(origin.x+visibleSize.width*0.026+sizeOfButtonAnSiGui.width/2,origin.y+sizeOfButtonAnSiGui.height/2));
	buttonAnSiGui->addTouchEventListener(CC_CALLBACK_2(DaiPiaoLayer::onButtonAnSiGui,this));
	this->addChild(buttonAnSiGui,1,22);

	auto buttonHaiDiLao=Button::create("xiaosanyuan.png","xiaosanyuan.png","xiaosanyuan.png",TextureResType::PLIST);
	auto sizeOfButtonHaiDiLao=buttonHaiDiLao->getContentSize();
	buttonHaiDiLao->setAnchorPoint(Vec2(0.5,0.5));
	buttonHaiDiLao->setPosition(Vec2(origin.x+visibleSize.width*0.326+sizeOfButtonHaiDiLao.width/2,origin.y+sizeOfButtonHaiDiLao.height/2));
	buttonHaiDiLao->addTouchEventListener(CC_CALLBACK_2(DaiPiaoLayer::onButtonHaiDilao,this));
	this->addChild(buttonHaiDiLao,1,44);

	auto buttonPengPengHu=Button::create("longqidui.png","longqidui.png","longqidui.png",TextureResType::PLIST);
	auto sizeOfButtonPengPengHu=buttonPengPengHu->getContentSize();
	buttonPengPengHu->setAnchorPoint(Vec2(0.5,0.5));
	buttonPengPengHu->setPosition(Vec2(origin.x+visibleSize.width*0.626+sizeOfButtonPengPengHu.width/2,origin.y+sizeOfButtonPengPengHu.height/2));
	buttonPengPengHu->addTouchEventListener(CC_CALLBACK_2(DaiPiaoLayer::onButtonPengPengHu,this));
	this->addChild(buttonPengPengHu,1,65);

	for(int i=0;i<3;i++)
	{
		auto widthOfButton=buttonKaWuXing->getContentSize().width;
		auto heightOfButton=buttonKaWuXing->getContentSize().height;

		auto bgOfButton1=Sprite::createWithSpriteFrameName("yinying.png");
		auto bgOfButton1Size=bgOfButton1->getContentSize();
		bgOfButton1->setAnchorPoint(Vec2(0.5,0.5));
		bgOfButton1->setPosition(Vec2(origin.x+visibleSize.width*(0.06+0.3*i)+bgOfButton1Size.width/2,origin.y+visibleSize.height*0.39+bgOfButton1Size.height/2));

		auto bgOfButton2=Sprite::createWithSpriteFrameName("yinying.png");
		auto bgOfButton2Size=bgOfButton2->getContentSize();
		bgOfButton2->setAnchorPoint(Vec2(0.5,0.5));
		bgOfButton2->setPosition(Vec2(origin.x+visibleSize.width*(0.06+0.3*i)+bgOfButton2Size.width/2,origin.y+visibleSize.height*(-0.04)+bgOfButton2Size.height/2));

		auto labelImage1=Sprite::createWithSpriteFrameName("dizhu.png");
		labelImage1->setAnchorPoint(Vec2(0.5,0.5));
		labelImage1->setPosition(Vec2(120,85));

		auto labelImage2=Sprite::createWithSpriteFrameName("dizhu.png");
		labelImage2->setAnchorPoint(Vec2(0.5,0.5));
		labelImage2->setPosition(Vec2(120,85));

		auto labelImage3=Sprite::createWithSpriteFrameName("zuishaoxiedai.png");
		labelImage3->setAnchorPoint(Vec2(0.5,0.5));
		labelImage3->setPosition(Vec2(100,45));

		auto labelImage4=Sprite::createWithSpriteFrameName("zuishaoxiedai.png");
		labelImage4->setAnchorPoint(Vec2(0.5,0.5));
		labelImage4->setPosition(Vec2(100,45));

		auto labelImage5=Sprite::createWithSpriteFrameName("renzaiwan.png");
		labelImage5->setAnchorPoint(Vec2(0.5,0.5));
		labelImage5->setPosition(Vec2(140,270));

		auto labelImage6=Sprite::createWithSpriteFrameName("renzaiwan.png");
		labelImage6->setAnchorPoint(Vec2(0.5,0.5));
		labelImage6->setPosition(Vec2(140,270));

		auto labelImage26=Sprite::createWithSpriteFrameName("piao.png");
		labelImage26->setAnchorPoint(Vec2(0.5,0.5));
		labelImage26->setPosition(Vec2(100,230));

		auto labelImage27=Sprite::createWithSpriteFrameName("piao.png");
		labelImage27->setAnchorPoint(Vec2(0.5,0.5));
		labelImage27->setPosition(Vec2(100,230));

		auto labelImage14=Sprite::createWithSpriteFrameName("jinbi2.png");
		labelImage14->setAnchorPoint(Vec2(0.5,0.5));
		labelImage14->setPosition(Vec2(230,45));

		auto labelImage15=Sprite::createWithSpriteFrameName("jinbi2.png");
		labelImage15->setAnchorPoint(Vec2(0.5,0.5));
		labelImage15->setPosition(Vec2(230,45));

		auto labelImage8=Sprite::createWithSpriteFrameName("buxianfengding.png");
		labelImage8->setAnchorPoint(Vec2(0.5,0.5));
		labelImage8->setPosition(Vec2(215,85));

		auto labelImage25=Sprite::createWithSpriteFrameName("buxianfengding.png");
		labelImage25->setAnchorPoint(Vec2(0.5,0.5));
		labelImage25->setPosition(Vec2(215,85));

		if(i==0)
		{
			this->addChild(bgOfButton1,0,12);
			this->addChild(bgOfButton2,0,23);
			buttonKaWuXing->addChild(labelImage1,2);
			buttonAnSiGui->addChild(labelImage2,2);
			buttonKaWuXing->addChild(labelImage3,2);
			buttonAnSiGui->addChild(labelImage4,2);
			buttonKaWuXing->addChild(labelImage5,2);
			buttonAnSiGui->addChild(labelImage6,2);
			buttonKaWuXing->addChild(labelImage14,2);
			buttonAnSiGui->addChild(labelImage15,2);
			buttonKaWuXing->addChild(labelImage8,2);
			buttonAnSiGui->addChild(labelImage25,2);
			buttonKaWuXing->addChild(labelImage26,2);
			buttonAnSiGui->addChild(labelImage27,2);
		}
		else if(i==1)
		{
			this->addChild(bgOfButton1,0,34);
			this->addChild(bgOfButton2,0,45);
			buttonTiaoYiSe->addChild(labelImage1,2);
			buttonHaiDiLao->addChild(labelImage2,2);
			buttonTiaoYiSe->addChild(labelImage3,2);
			buttonHaiDiLao->addChild(labelImage4,2);
			buttonTiaoYiSe->addChild(labelImage5,2);
			buttonHaiDiLao->addChild(labelImage6,2);
			buttonTiaoYiSe->addChild(labelImage14,2);
			buttonHaiDiLao->addChild(labelImage15,2);
			buttonTiaoYiSe->addChild(labelImage8,2);
			buttonHaiDiLao->addChild(labelImage25,2);
			buttonTiaoYiSe->addChild(labelImage26,2);
			buttonHaiDiLao->addChild(labelImage27,2);
		}
		else if(i==2)
		{
			this->addChild(bgOfButton1,0,55);
			this->addChild(bgOfButton2,0,66);
			buttonGangShangHua->addChild(labelImage1,2);
			buttonPengPengHu->addChild(labelImage2,2);
			buttonGangShangHua->addChild(labelImage3,2);
			buttonPengPengHu->addChild(labelImage4,2);
			buttonGangShangHua->addChild(labelImage5,2);
			buttonPengPengHu->addChild(labelImage6,2);
			buttonGangShangHua->addChild(labelImage14,2);
			buttonPengPengHu->addChild(labelImage15,2);
			buttonGangShangHua->addChild(labelImage8,2);
			buttonPengPengHu->addChild(labelImage25,2);
			buttonGangShangHua->addChild(labelImage26,2);
			buttonPengPengHu->addChild(labelImage27,2);
		}

		char numAnte1[10]={0};
		char numAnte2[10]={0};
		if(i==0)
		{
			sprintf(numAnte1,"%d",100);
			sprintf(numAnte2,"%d",1000);
			auto labelImage9=LabelAtlas::create(numAnte1, "fonts/resultbox_add_lscore.png", 18, 26, '0');
			labelImage9->setAnchorPoint(Vec2(0.5,0.5));
			labelImage9->setPosition(Vec2(60,83));
			buttonKaWuXing->addChild(labelImage9,2);

			auto labelImage10=LabelAtlas::create(numAnte2, "fonts/resultbox_add_lscore.png", 18, 26, '0');
			labelImage10->setAnchorPoint(Vec2(0.5,0.5));
			labelImage10->setPosition(Vec2(55,83));
			buttonAnSiGui->addChild(labelImage10,2);

			auto labelImage16=LabelAtlas::create("1","fonts/goumai_number3.png",17,27,'0');
			labelImage16->setAnchorPoint(Vec2(0.5,0.5));
			labelImage16->setPosition(Vec2(170,45));
			buttonKaWuXing->addChild(labelImage16,2);

			auto labelImage20=Sprite::createWithSpriteFrameName("qian2.png");
			labelImage20->setAnchorPoint(Vec2(0.5,0.5));
			labelImage20->setPosition(Vec2(193,45));
			buttonKaWuXing->addChild(labelImage20,2);
			
			auto labelImage17=LabelAtlas::create("5","fonts/goumai_number3.png",17,27,'0');
			labelImage17->setAnchorPoint(Vec2(0.5,0.5));
			labelImage17->setPosition(Vec2(165,45));
			buttonAnSiGui->addChild(labelImage17,2);

			auto labelImage18=Sprite::createWithSpriteFrameName("wan1.png");
			labelImage18->setAnchorPoint(Vec2(0.5,0.5));
			labelImage18->setPosition(Vec2(193,45));
			buttonAnSiGui->addChild(labelImage18,2);
		}
		else if(i==1)
		{
			sprintf(numAnte1,"%d",200);
			sprintf(numAnte2,"%d",5000);
			auto labelImage9=LabelAtlas::create(numAnte1, "fonts/resultbox_add_lscore.png", 18, 26, '0');
			labelImage9->setAnchorPoint(Vec2(0.5,0.5));
			labelImage9->setPosition(Vec2(60,83));
			buttonTiaoYiSe->addChild(labelImage9,2);

			auto labelImage10=LabelAtlas::create(numAnte2, "fonts/resultbox_add_lscore.png", 18, 26, '0');
			labelImage10->setAnchorPoint(Vec2(0.5,0.5));
			labelImage10->setPosition(Vec2(55,83));
			buttonHaiDiLao->addChild(labelImage10,2);

			auto labelImage22=Sprite::createWithSpriteFrameName("qian2.png");
			labelImage22->setAnchorPoint(Vec2(0.5,0.5));
			labelImage22->setPosition(Vec2(193,45));
			buttonTiaoYiSe->addChild(labelImage22,2,43);

			auto labelImage16=LabelAtlas::create("5","fonts/goumai_number3.png",17,27,'0');
			labelImage16->setAnchorPoint(Vec2(0.5,0.5));
			labelImage16->setScaleX(0.75);
			labelImage16->setPosition(Vec2(170,45));
			buttonTiaoYiSe->addChild(labelImage16,2);

			auto labelImage17=LabelAtlas::create("20","fonts/goumai_number3.png",17,27,'0');
			labelImage17->setAnchorPoint(Vec2(0.5,0.5));
			labelImage17->setScaleX(0.75);
			labelImage17->setPosition(Vec2(170,45));
			buttonHaiDiLao->addChild(labelImage17,2,52);

			auto labelImage18=Sprite::createWithSpriteFrameName("wan1.png");
			labelImage18->setAnchorPoint(Vec2(0.5,0.5));
			labelImage18->setPosition(Vec2(193,45));
			buttonHaiDiLao->addChild(labelImage18,2,53);
		}
		else if(i==2)
		{
			sprintf(numAnte1,"%d",500);
			sprintf(numAnte2,"%d",1);

			auto labelImage9=LabelAtlas::create(numAnte1, "fonts/resultbox_add_lscore.png", 18, 26, '0');
			labelImage9->setAnchorPoint(Vec2(0.5,0.5));
			labelImage9->setPosition(Vec2(60,83));
			buttonGangShangHua->addChild(labelImage9,2);

			auto labelImage10=LabelAtlas::create(numAnte2, "fonts/resultbox_add_lscore.png", 18, 26, '0');
			auto SizeOflabelImage10=labelImage10->getContentSize();
			labelImage10->setAnchorPoint(Vec2(0.5,0.5));
			labelImage10->setPosition(Vec2(50,83));
			buttonPengPengHu->addChild(labelImage10,2);

			auto labelImage11=Sprite::createWithSpriteFrameName("wan.png");
			auto SizeOflabelImage11=labelImage11->getContentSize();
			labelImage11->setAnchorPoint(Vec2(0.5,0.5));
			labelImage11->setPosition(Vec2(75,83));
			buttonPengPengHu->addChild(labelImage11,2);

			auto labelImage16=LabelAtlas::create("1","fonts/goumai_number3.png",17,27,'0');
			labelImage16->setAnchorPoint(Vec2(0.5,0.5));
			labelImage16->setScaleX(0.75);
			labelImage16->setPosition(Vec2(170,45));
			buttonGangShangHua->addChild(labelImage16,2);

			auto labelImage18=Sprite::createWithSpriteFrameName("wan1.png");
			labelImage18->setAnchorPoint(Vec2(0.5,0.5));
			labelImage18->setPosition(Vec2(193,45));
			buttonGangShangHua->addChild(labelImage18,2,64);

			auto labelImage17=LabelAtlas::create("50","fonts/goumai_number3.png",17,27,'0');
			labelImage17->setAnchorPoint(Vec2(0.5,0.5));
			labelImage17->setScaleX(0.75);
			labelImage17->setPosition(Vec2(170,45));
			buttonPengPengHu->addChild(labelImage17,2);

			auto labelImage19=Sprite::createWithSpriteFrameName("wan1.png");
			labelImage19->setAnchorPoint(Vec2(0.5,0.5));
			labelImage19->setPosition(Vec2(193,45));
			buttonPengPengHu->addChild(labelImage19,2);
		}
	}

	return true;
}

void DaiPiaoLayer::onButtonKaWuXing(Ref* pSender,Widget::TouchEventType type)
{
	auto Button=(Sprite*)this->getChildByTag(11);
	auto ButtonBg=(Sprite*)this->getChildByTag(12);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		Button->runAction(ScaleTo::create(0.1,1.1));
		ButtonBg->runAction(ScaleTo::create(0.1,1.1));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		Button->runAction(Sequence::create(ScaleTo::create(0.1,1),CallFunc::create(this,callfunc_selector(DaiPiaoLayer::actionKaWuXing)),NULL));
		scheduleOnce(schedule_selector(DaiPiaoLayer::entranceToKaWuXing),0.5);
		break;
	case Widget::TouchEventType::CANCELED:
		Button->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		break;
	}
}

void DaiPiaoLayer::onButtonTiaoYiSe(Ref* pSender,Widget::TouchEventType type)
{
	auto Button=(Sprite*)this->getChildByTag(33);
	auto ButtonBg=(Sprite*)this->getChildByTag(34);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		Button->runAction(ScaleTo::create(0.1,1.1));
		ButtonBg->runAction(ScaleTo::create(0.1,1.1));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		Button->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		Button->runAction(CallFunc::create(this,callfunc_selector(DaiPiaoLayer::actionTiaoYiSe)));
		scheduleOnce(schedule_selector(DaiPiaoLayer::entranceToKaWuXing),0.5);
		break;
	case Widget::TouchEventType::CANCELED:
		Button->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		break;
	}
}
void DaiPiaoLayer::onButtonGangShangHua(Ref* pSender,Widget::TouchEventType type)
{
	auto Button=(Sprite*)this->getChildByTag(54);
	auto ButtonBg=(Sprite*)this->getChildByTag(55);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		Button->runAction(ScaleTo::create(0.1,1.1));
		ButtonBg->runAction(ScaleTo::create(0.1,1.1));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		Button->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		Button->runAction(CallFunc::create(this,callfunc_selector(DaiPiaoLayer::actionGangShangHua)));
		scheduleOnce(schedule_selector(DaiPiaoLayer::entranceGangShangHua),0.5);
		break;
	case Widget::TouchEventType::CANCELED:
		Button->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		break;
	}
}
void DaiPiaoLayer::onButtonAnSiGui(Ref* pSender,Widget::TouchEventType type)
{
	auto Button=(Sprite*)this->getChildByTag(22);
	auto ButtonBg=(Sprite*)this->getChildByTag(23);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		Button->runAction(ScaleTo::create(0.1,1.1));
		ButtonBg->runAction(ScaleTo::create(0.1,1.1));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		Button->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		Button->runAction(CallFunc::create(this,callfunc_selector(DaiPiaoLayer::actionAnSiGui)));
		scheduleOnce(schedule_selector(DaiPiaoLayer::entranceToTiaoYiSe),0.5);
		break;
	case Widget::TouchEventType::CANCELED:
		Button->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		break;
	}
}
void DaiPiaoLayer::onButtonHaiDilao(Ref* pSender,Widget::TouchEventType type)
{
	auto Button=(Sprite*)this->getChildByTag(44);
	auto ButtonBg=(Sprite*)this->getChildByTag(45);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		Button->runAction(ScaleTo::create(0.1,1.1));
		ButtonBg->runAction(ScaleTo::create(0.1,1.1));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		Button->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		Button->runAction(CallFunc::create(this,callfunc_selector(DaiPiaoLayer::actionHaiDiLao)));
		scheduleOnce(schedule_selector(DaiPiaoLayer::entranceHaiDiLao),0.5);
		break;
	case Widget::TouchEventType::CANCELED:
		Button->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		break;
	}
}
void DaiPiaoLayer::onButtonPengPengHu(Ref* pSender,Widget::TouchEventType type)
{
	auto Button=(Sprite*)this->getChildByTag(65);
	auto ButtonBg=(Sprite*)this->getChildByTag(66);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		Button->runAction(ScaleTo::create(0.1,1.1));
		ButtonBg->runAction(ScaleTo::create(0.1,1.1));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		Button->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		Button->runAction(CallFunc::create(this,callfunc_selector(DaiPiaoLayer::actionPengPengHu)));
		scheduleOnce(schedule_selector(DaiPiaoLayer::entrancePengPengHu),0.5);
		break;
	case Widget::TouchEventType::CANCELED:
		Button->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		break;
	}
}

void DaiPiaoLayer::actionKaWuXing()
{
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();

	auto buttonKaWuXing=Sprite::createWithSpriteFrameName("bingyise.png");
	buttonKaWuXing->setOpacity(100);
	auto kaWuXingSize=buttonKaWuXing->getContentSize();
	buttonKaWuXing->setAnchorPoint(Vec2(0.5,0.5));
	buttonKaWuXing->setPosition(Vec2(origin.x+visibleSize.width*0.026+kaWuXingSize.width/2,origin.y+visibleSize.height*0.43+kaWuXingSize.height/2));
	this->addChild(buttonKaWuXing,0);

	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.3);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	buttonKaWuXing->runAction(spawn);
}

void DaiPiaoLayer::entranceToKaWuXing(float t)
{
	//进入游戏场卡五星
}

void DaiPiaoLayer::actionAnSiGui()
{
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();

	auto button=Sprite::createWithSpriteFrameName("gangshanggang.png");
	button->setOpacity(100);
	auto buttonSize=button->getContentSize();
	button->setAnchorPoint(Vec2(0.5,0.5));
	button->setPosition(Vec2(origin.x+visibleSize.width*0.026+buttonSize.width/2,origin.y+buttonSize.height/2));
	this->addChild(button,0);

	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.3);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	button->runAction(spawn);
}

void DaiPiaoLayer::entranceToAnSiGui(float t)
{
	//进入游戏场暗四归
}

void DaiPiaoLayer::actionTiaoYiSe()
{
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();

	auto button=Sprite::createWithSpriteFrameName("dasanyuan.png");
	button->setOpacity(100);
	auto buttonSize=button->getContentSize();
	button->setAnchorPoint(Vec2(0.5,0.5));
	button->setPosition(Vec2(origin.x+visibleSize.width*0.326+buttonSize.width/2,origin.y+visibleSize.height*0.43+buttonSize.height/2));
	this->addChild(button,0);

	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.3);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	button->runAction(spawn);
}

void DaiPiaoLayer::entranceToTiaoYiSe(float t)
{
	//进入游戏条一色
}

void DaiPiaoLayer::actionHaiDiLao()
{
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();

	auto button=Sprite::createWithSpriteFrameName("xiaosanyuan.png");
	button->setOpacity(100);
	auto buttonSize=button->getContentSize();
	button->setAnchorPoint(Vec2(0.5,0.5));
	button->setPosition(Vec2(origin.x+visibleSize.width*0.326+buttonSize.width/2,origin.y+buttonSize.height/2));
	this->addChild(button,0);

	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.3);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	button->runAction(spawn);
}

void DaiPiaoLayer::entranceHaiDiLao(float t)
{
	//进入游戏海底捞
}

void DaiPiaoLayer::actionGangShangHua()
{
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();

	auto button=Sprite::createWithSpriteFrameName("mingsigui.png");
	button->setOpacity(100);
	auto buttonSize=button->getContentSize();
	button->setAnchorPoint(Vec2(0.5,0.5));
	button->setPosition(Vec2(origin.x+visibleSize.width*0.626+buttonSize.width/2,origin.y+visibleSize.height*0.43+buttonSize.height/2));
	this->addChild(button,0);

	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.3);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	button->runAction(spawn);
}
void DaiPiaoLayer::entranceGangShangHua(float t)
{
	//进入游戏杠上花
}

void DaiPiaoLayer::actionPengPengHu()
{
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();

	auto button=Sprite::createWithSpriteFrameName("longqidui.png");
	button->setOpacity(100);
	auto buttonSize=button->getContentSize();
	button->setAnchorPoint(Vec2(0.5,0.5));
	button->setPosition(Vec2(origin.x+visibleSize.width*0.626+buttonSize.width/2,origin.y+buttonSize.height/2));
	this->addChild(button,0);

	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.3);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	button->runAction(spawn);
}

void DaiPiaoLayer::entrancePengPengHu(float t)
{
	//进入游戏碰碰胡
}