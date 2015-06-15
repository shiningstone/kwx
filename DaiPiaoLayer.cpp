#include "DaiPiaoLayer.h"
#include "game/RaceLayer.h"

DaiPiaoLayer::DaiPiaoLayer(void)
{
}

DaiPiaoLayer::~DaiPiaoLayer(void)
{
}

TargetedAction* DaiPiaoLayer::TouchShadeAction(Vec2 curPos,std::string actionName)
{
	auto curActionButton=Sprite::createWithSpriteFrameName(actionName);
	curActionButton->setScale(0.8);
	curActionButton->setOpacity(100);
	auto curSize=curActionButton->getContentSize();
	curActionButton->setAnchorPoint(Vec2(0.5,0.5));
	curActionButton->setPosition(curPos);
	this->addChild(curActionButton,0);

	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.1);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	auto shadeTarget=TargetedAction::create(curActionButton,spawn);

	return shadeTarget;
}
void DaiPiaoLayer::enterRoomStandAlone()
{
	auto scene = Scene::create();
	SpriteFrameCache::getInstance()->removeSpriteFrames();
	TextureCache::sharedTextureCache()->removeAllTextures();
	RaceLayer *layer = RaceLayer::create();
	scene->addChild(layer);
#ifndef NETWORK_GAME_DEBUG
	layer->CreateRace(LOCAL_GAME);
#else
	layer->CreateRace(NETWORK_GAME);
#endif

	Director::getInstance()->replaceScene(scene);
}
bool DaiPiaoLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	visibleSize=Director::getInstance()->getVisibleSize();
	origin=Director::getInstance()->getVisibleOrigin();

	auto buttonBingYiSe=Button::create("bingyise.png","bingyise.png","bingyise.png",TextureResType::PLIST);
	buttonBingYiSe->setScale(0.8);
	auto BingYiSeSize=buttonBingYiSe->getContentSize();
	buttonBingYiSe->setAnchorPoint(Vec2(0.5,0.5));
	buttonBingYiSe->setPosition(Vec2(origin.x+visibleSize.width*0.026+BingYiSeSize.width/2,origin.y+visibleSize.height*0.43+BingYiSeSize.height/2+30));
	buttonBingYiSe->addTouchEventListener(CC_CALLBACK_2(DaiPiaoLayer::onbuttonBingYiSe,this));
	this->addChild(buttonBingYiSe,1,BUTTON_BINGYISE);

	auto buttonDaSanYuan=Button::create("dasanyuan.png","dasanyuan.png","dasanyuan.png",TextureResType::PLIST);
	buttonDaSanYuan->setScale(0.8);
	auto sizeOfbuttonDaSanYuan=buttonDaSanYuan->getContentSize();
	buttonDaSanYuan->setAnchorPoint(Vec2(0.5,0.5));
	buttonDaSanYuan->setPosition(Vec2(origin.x+visibleSize.width*0.326+sizeOfbuttonDaSanYuan.width/2,origin.y+visibleSize.height*0.43+sizeOfbuttonDaSanYuan.height/2+30));
	buttonDaSanYuan->addTouchEventListener(CC_CALLBACK_2(DaiPiaoLayer::onbuttonDaSanYuan,this));
	this->addChild(buttonDaSanYuan,1,BUTTON_DASANYUAN);

	auto buttonMingSiGui=Button::create("mingsigui.png","mingsigui.png","mingsigui.png",TextureResType::PLIST);
	buttonMingSiGui->setScale(0.8);
	auto sizeOfbuttonMingSiGui=buttonMingSiGui->getContentSize();
	buttonMingSiGui->setAnchorPoint(Vec2(0.5,0.5));
	buttonMingSiGui->setPosition(Vec2(origin.x+visibleSize.width*0.626+sizeOfbuttonMingSiGui.width/2,origin.y+visibleSize.height*0.43+sizeOfbuttonMingSiGui.height/2+30));
	buttonMingSiGui->addTouchEventListener(CC_CALLBACK_2(DaiPiaoLayer::onbuttonMingSiGui,this));
	this->addChild(buttonMingSiGui,1,BUTTON_MINGSIGUI);
	
	auto buttonGangGang=Button::create("gangshanggang.png","gangshanggang.png","gangshanggang.png",TextureResType::PLIST);
	buttonGangGang->setScale(0.8);
	auto sizeOfButtonGangGang=buttonGangGang->getContentSize();
	buttonGangGang->setAnchorPoint(Vec2(0.5,0.5));
	buttonGangGang->setPosition(Vec2(origin.x+visibleSize.width*0.026+sizeOfButtonGangGang.width/2,origin.y+sizeOfButtonGangGang.height/2+60));
	buttonGangGang->addTouchEventListener(CC_CALLBACK_2(DaiPiaoLayer::onButtonGangGang,this));
	this->addChild(buttonGangGang,1,BUTTON_GANGGANG);

	auto buttonXSY=Button::create("xiaosanyuan.png","xiaosanyuan.png","xiaosanyuan.png",TextureResType::PLIST);
	buttonXSY->setScale(0.8);
	auto sizeOfButtonXSY=buttonXSY->getContentSize();
	buttonXSY->setAnchorPoint(Vec2(0.5,0.5));
	buttonXSY->setPosition(Vec2(origin.x+visibleSize.width*0.326+sizeOfButtonXSY.width/2,origin.y+sizeOfButtonXSY.height/2+60));
	buttonXSY->addTouchEventListener(CC_CALLBACK_2(DaiPiaoLayer::onButtonXSY,this));
	this->addChild(buttonXSY,1,BUTTON_XIAOSANYUAN);

	auto buttonLQD=Button::create("longqidui.png","longqidui.png","longqidui.png",TextureResType::PLIST);
	buttonLQD->setScale(0.8);
	auto sizeOfButtonLQD=buttonLQD->getContentSize();
	buttonLQD->setAnchorPoint(Vec2(0.5,0.5));
	buttonLQD->setPosition(Vec2(origin.x+visibleSize.width*0.626+sizeOfButtonLQD.width/2,origin.y+sizeOfButtonLQD.height/2+60));
	buttonLQD->addTouchEventListener(CC_CALLBACK_2(DaiPiaoLayer::onButtonLQD,this));
	this->addChild(buttonLQD,1,BUTTON_LONGQIDUI);

	for(int i=0;i<3;i++)
	{
		auto widthOfButton=buttonBingYiSe->getContentSize().width;
		auto heightOfButton=buttonBingYiSe->getContentSize().height;

		auto bgOfButton1=Sprite::createWithSpriteFrameName("yinying.png");
		bgOfButton1->setScale(0.8);
		auto bgOfButton1Size=bgOfButton1->getContentSize();
		bgOfButton1->setAnchorPoint(Vec2(0.5,0.5));
		bgOfButton1->setPosition(Vec2(origin.x+visibleSize.width*(0.06+0.3*i)+bgOfButton1Size.width/2-3,origin.y+visibleSize.height*0.39+bgOfButton1Size.height/2+33));

		auto bgOfButton2=Sprite::createWithSpriteFrameName("yinying.png");
		bgOfButton2->setScale(0.8);
		auto bgOfButton2Size=bgOfButton2->getContentSize();
		bgOfButton2->setAnchorPoint(Vec2(0.5,0.5));
		bgOfButton2->setPosition(Vec2(origin.x+visibleSize.width*(0.06+0.3*i)+bgOfButton2Size.width/2,origin.y+visibleSize.height*(-0.04)+bgOfButton2Size.height/2+60));

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
			this->addChild(bgOfButton1,0,SHADE_BINGYISE);
			this->addChild(bgOfButton2,0,SHADE_GANGGANG);
			buttonBingYiSe->addChild(labelImage1,2);
			buttonGangGang->addChild(labelImage2,2);
			buttonBingYiSe->addChild(labelImage3,2);
			buttonGangGang->addChild(labelImage4,2);
			buttonBingYiSe->addChild(labelImage5,2);
			buttonGangGang->addChild(labelImage6,2);
			buttonBingYiSe->addChild(labelImage14,2);
			buttonGangGang->addChild(labelImage15,2);
			buttonBingYiSe->addChild(labelImage8,2);
			buttonGangGang->addChild(labelImage25,2);
			buttonBingYiSe->addChild(labelImage26,2);
			buttonGangGang->addChild(labelImage27,2);
		}
		else if(i==1)
		{
			this->addChild(bgOfButton1,0,SHADE_DASANYUAN);
			this->addChild(bgOfButton2,0,SHADE_XIAOSANYUAN);
			buttonDaSanYuan->addChild(labelImage1,2);
			buttonXSY->addChild(labelImage2,2);
			buttonDaSanYuan->addChild(labelImage3,2);
			buttonXSY->addChild(labelImage4,2);
			buttonDaSanYuan->addChild(labelImage5,2);
			buttonXSY->addChild(labelImage6,2);
			buttonDaSanYuan->addChild(labelImage14,2);
			buttonXSY->addChild(labelImage15,2);
			buttonDaSanYuan->addChild(labelImage8,2);
			buttonXSY->addChild(labelImage25,2);
			buttonDaSanYuan->addChild(labelImage26,2);
			buttonXSY->addChild(labelImage27,2);
		}
		else if(i==2)
		{
			this->addChild(bgOfButton1,0,SHADE_MINGSIGUI);
			this->addChild(bgOfButton2,0,SHADE_LONGQIDUI);
			buttonMingSiGui->addChild(labelImage1,2);
			buttonLQD->addChild(labelImage2,2);
			buttonMingSiGui->addChild(labelImage3,2);
			buttonLQD->addChild(labelImage4,2);
			buttonMingSiGui->addChild(labelImage5,2);
			buttonLQD->addChild(labelImage6,2);
			buttonMingSiGui->addChild(labelImage14,2);
			buttonLQD->addChild(labelImage15,2);
			buttonMingSiGui->addChild(labelImage8,2);
			buttonLQD->addChild(labelImage25,2);
			buttonMingSiGui->addChild(labelImage26,2);
			buttonLQD->addChild(labelImage27,2);
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
			buttonBingYiSe->addChild(labelImage9,2);

			auto labelImage10=LabelAtlas::create(numAnte2, "fonts/resultbox_add_lscore.png", 18, 26, '0');
			labelImage10->setAnchorPoint(Vec2(0.5,0.5));
			labelImage10->setPosition(Vec2(55,83));
			buttonGangGang->addChild(labelImage10,2);

			auto labelImage16=LabelAtlas::create("1","fonts/goumai_number3.png",17,27,'0');
			labelImage16->setAnchorPoint(Vec2(0.5,0.5));
			labelImage16->setPosition(Vec2(170,45));
			buttonBingYiSe->addChild(labelImage16,2);

			auto labelImage20=Sprite::createWithSpriteFrameName("qian2.png");
			labelImage20->setAnchorPoint(Vec2(0.5,0.5));
			labelImage20->setPosition(Vec2(193,45));
			buttonBingYiSe->addChild(labelImage20,2);
			
			auto labelImage17=LabelAtlas::create("5","fonts/goumai_number3.png",17,27,'0');
			labelImage17->setAnchorPoint(Vec2(0.5,0.5));
			labelImage17->setPosition(Vec2(165,45));
			buttonGangGang->addChild(labelImage17,2);

			auto labelImage18=Sprite::createWithSpriteFrameName("wan1.png");
			labelImage18->setAnchorPoint(Vec2(0.5,0.5));
			labelImage18->setPosition(Vec2(193,45));
			buttonGangGang->addChild(labelImage18,2);
		}
		else if(i==1)
		{
			sprintf(numAnte1,"%d",200);
			sprintf(numAnte2,"%d",5000);
			auto labelImage9=LabelAtlas::create(numAnte1, "fonts/resultbox_add_lscore.png", 18, 26, '0');
			labelImage9->setAnchorPoint(Vec2(0.5,0.5));
			labelImage9->setPosition(Vec2(60,83));
			buttonDaSanYuan->addChild(labelImage9,2);

			auto labelImage10=LabelAtlas::create(numAnte2, "fonts/resultbox_add_lscore.png", 18, 26, '0');
			labelImage10->setAnchorPoint(Vec2(0.5,0.5));
			labelImage10->setPosition(Vec2(55,83));
			buttonXSY->addChild(labelImage10,2);

			auto labelImage22=Sprite::createWithSpriteFrameName("qian2.png");
			labelImage22->setAnchorPoint(Vec2(0.5,0.5));
			labelImage22->setPosition(Vec2(193,45));
			buttonDaSanYuan->addChild(labelImage22,2,43);

			auto labelImage16=LabelAtlas::create("5","fonts/goumai_number3.png",17,27,'0');
			labelImage16->setAnchorPoint(Vec2(0.5,0.5));
			labelImage16->setScaleX(0.75);
			labelImage16->setPosition(Vec2(170,45));
			buttonDaSanYuan->addChild(labelImage16,2);

			auto labelImage17=LabelAtlas::create("20","fonts/goumai_number3.png",17,27,'0');
			labelImage17->setAnchorPoint(Vec2(0.5,0.5));
			labelImage17->setScaleX(0.75);
			labelImage17->setPosition(Vec2(170,45));
			buttonXSY->addChild(labelImage17,2,52);

			auto labelImage18=Sprite::createWithSpriteFrameName("wan1.png");
			labelImage18->setAnchorPoint(Vec2(0.5,0.5));
			labelImage18->setPosition(Vec2(193,45));
			buttonXSY->addChild(labelImage18,2,53);
		}
		else if(i==2)
		{
			sprintf(numAnte1,"%d",500);
			sprintf(numAnte2,"%d",1);

			auto labelImage9=LabelAtlas::create(numAnte1, "fonts/resultbox_add_lscore.png", 18, 26, '0');
			labelImage9->setAnchorPoint(Vec2(0.5,0.5));
			labelImage9->setPosition(Vec2(60,83));
			buttonMingSiGui->addChild(labelImage9,2);

			auto labelImage10=LabelAtlas::create(numAnte2, "fonts/resultbox_add_lscore.png", 18, 26, '0');
			auto SizeOflabelImage10=labelImage10->getContentSize();
			labelImage10->setAnchorPoint(Vec2(0.5,0.5));
			labelImage10->setPosition(Vec2(50,83));
			buttonLQD->addChild(labelImage10,2);

			auto labelImage11=Sprite::createWithSpriteFrameName("wan.png");
			auto SizeOflabelImage11=labelImage11->getContentSize();
			labelImage11->setAnchorPoint(Vec2(0.5,0.5));
			labelImage11->setPosition(Vec2(75,83));
			buttonLQD->addChild(labelImage11,2);

			auto labelImage16=LabelAtlas::create("1","fonts/goumai_number3.png",17,27,'0');
			labelImage16->setAnchorPoint(Vec2(0.5,0.5));
			labelImage16->setScaleX(0.75);
			labelImage16->setPosition(Vec2(170,45));
			buttonMingSiGui->addChild(labelImage16,2);

			auto labelImage18=Sprite::createWithSpriteFrameName("wan1.png");
			labelImage18->setAnchorPoint(Vec2(0.5,0.5));
			labelImage18->setPosition(Vec2(193,45));
			buttonMingSiGui->addChild(labelImage18,2,64);

			auto labelImage17=LabelAtlas::create("50","fonts/goumai_number3.png",17,27,'0');
			labelImage17->setAnchorPoint(Vec2(0.5,0.5));
			labelImage17->setScaleX(0.75);
			labelImage17->setPosition(Vec2(170,45));
			buttonLQD->addChild(labelImage17,2);

			auto labelImage19=Sprite::createWithSpriteFrameName("wan1.png");
			labelImage19->setAnchorPoint(Vec2(0.5,0.5));
			labelImage19->setPosition(Vec2(193,45));
			buttonLQD->addChild(labelImage19,2);
		}
	}

	auto quickStartButton=Button::create("QuickGame.png","QuickGame2.png","QuickGame2.png",UI_TEX_TYPE_PLIST);
	quickStartButton->setScale(0.8);
	quickStartButton->setAnchorPoint(Vec2(0.5,0));
	quickStartButton->setPosition(Vec2(origin.x+visibleSize.width*0.48,origin.y/*-visibleSize.height*0.15*/));
	this->addChild(quickStartButton);

	return true;
}

void DaiPiaoLayer::onbuttonBingYiSe(Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto ButtonBg=(Sprite*)this->getChildByTag(SHADE_BINGYISE);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		curButton->runAction(ScaleTo::create(0.1,0.9));
		ButtonBg->runAction(ScaleTo::create(0.1,0.9));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			curButton->setTouchEnabled(false);
			auto ButtonBgTar=TargetedAction::create(ButtonBg,ScaleTo::create(0.1,0.8));
			auto shadeTarget=TouchShadeAction(curButton->getPosition(),ACTION_PNG_BYS);
			auto seqButtonAction=Sequence::create(ScaleTo::create(0.1,0.8),shadeTarget,NULL);
			auto curButtonTar=TargetedAction::create(curButton,seqButtonAction);
			auto EffectForThis=Spawn::create(ButtonBgTar,curButtonTar,NULL);
			auto entranceFun=CallFunc::create([=](){
				enterRoomStandAlone();
			});
			this->runAction(Sequence::create(EffectForThis,entranceFun,NULL));
		}
		break;
	case Widget::TouchEventType::CANCELED:
		curButton->runAction(ScaleTo::create(0.1,0.8));
		ButtonBg->runAction(ScaleTo::create(0.1,0.8));
		break;
	}
}

void DaiPiaoLayer::onbuttonDaSanYuan(Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto ButtonBg=(Sprite*)this->getChildByTag(SHADE_DASANYUAN);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		curButton->runAction(ScaleTo::create(0.1,0.9));
		ButtonBg->runAction(ScaleTo::create(0.1,0.9));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			curButton->setTouchEnabled(false);
			auto ButtonBgTar=TargetedAction::create(ButtonBg,ScaleTo::create(0.1,0.8));
			auto shadeTarget=TouchShadeAction(curButton->getPosition(),ACTION_PNG_DSY);
			auto seqButtonAction=Sequence::create(ScaleTo::create(0.1,0.8),shadeTarget,NULL);
			auto curButtonTar=TargetedAction::create(curButton,seqButtonAction);
			auto EffectForThis=Spawn::create(ButtonBgTar,curButtonTar,NULL);
			auto entranceFun=CallFunc::create([=](){
				enterRoomStandAlone();
			});
			this->runAction(Sequence::create(EffectForThis,entranceFun,NULL));
		}
		break;
	case Widget::TouchEventType::CANCELED:
		curButton->runAction(ScaleTo::create(0.1,0.8));
		ButtonBg->runAction(ScaleTo::create(0.1,0.8));
		break;
	}
}
void DaiPiaoLayer::onbuttonMingSiGui(Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto ButtonBg=(Sprite*)this->getChildByTag(SHADE_MINGSIGUI);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		curButton->runAction(ScaleTo::create(0.1,0.9));
		ButtonBg->runAction(ScaleTo::create(0.1,0.9));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			curButton->setTouchEnabled(false);
			auto ButtonBgTar=TargetedAction::create(ButtonBg,ScaleTo::create(0.1,0.8));
			auto shadeTarget=TouchShadeAction(curButton->getPosition(),ACTION_PNG_MSG);
			auto seqButtonAction=Sequence::create(ScaleTo::create(0.1,0.8),shadeTarget,NULL);
			auto curButtonTar=TargetedAction::create(curButton,seqButtonAction);
			auto EffectForThis=Spawn::create(ButtonBgTar,curButtonTar,NULL);
			auto entranceFun=CallFunc::create([=](){
				enterRoomStandAlone();
			});
			this->runAction(Sequence::create(EffectForThis,entranceFun,NULL));
		}
		break;
	case Widget::TouchEventType::CANCELED:
		curButton->runAction(ScaleTo::create(0.1,0.8));
		ButtonBg->runAction(ScaleTo::create(0.1,0.8));
		break;
	}
}
void DaiPiaoLayer::onButtonGangGang(Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto ButtonBg=(Sprite*)this->getChildByTag(SHADE_GANGGANG);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		curButton->runAction(ScaleTo::create(0.1,0.9));
		ButtonBg->runAction(ScaleTo::create(0.1,0.9));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			curButton->setTouchEnabled(false);
			auto ButtonBgTar=TargetedAction::create(ButtonBg,ScaleTo::create(0.1,0.8));
			auto shadeTarget=TouchShadeAction(curButton->getPosition(),ACTION_PNG_GSG);
			auto seqButtonAction=Sequence::create(ScaleTo::create(0.1,0.8),shadeTarget,NULL);
			auto curButtonTar=TargetedAction::create(curButton,seqButtonAction);
			auto EffectForThis=Spawn::create(ButtonBgTar,curButtonTar,NULL);
			auto entranceFun=CallFunc::create([=](){
				enterRoomStandAlone();
			});
			this->runAction(Sequence::create(EffectForThis,entranceFun,NULL));
		}
		break;
	case Widget::TouchEventType::CANCELED:
		curButton->runAction(ScaleTo::create(0.1,0.8));
		ButtonBg->runAction(ScaleTo::create(0.1,0.8));
		break;
	}
}
void DaiPiaoLayer::onButtonXSY(Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto ButtonBg=(Sprite*)this->getChildByTag(SHADE_XIAOSANYUAN);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		curButton->runAction(ScaleTo::create(0.1,0.9));
		ButtonBg->runAction(ScaleTo::create(0.1,0.9));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			curButton->setTouchEnabled(false);
			auto ButtonBgTar=TargetedAction::create(ButtonBg,ScaleTo::create(0.1,0.8));
			auto shadeTarget=TouchShadeAction(curButton->getPosition(),ACTION_PNG_XSY);
			auto seqButtonAction=Sequence::create(ScaleTo::create(0.1,0.8),shadeTarget,NULL);
			auto curButtonTar=TargetedAction::create(curButton,seqButtonAction);
			auto EffectForThis=Spawn::create(ButtonBgTar,curButtonTar,NULL);
			auto entranceFun=CallFunc::create([=](){
				enterRoomStandAlone();
			});
			this->runAction(Sequence::create(EffectForThis,entranceFun,NULL));
		}
		break;
	case Widget::TouchEventType::CANCELED:
		curButton->runAction(ScaleTo::create(0.1,0.8));
		ButtonBg->runAction(ScaleTo::create(0.1,0.8));
		break;
	}
}
void DaiPiaoLayer::onButtonLQD(Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto ButtonBg=(Sprite*)this->getChildByTag(SHADE_LONGQIDUI);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		curButton->runAction(ScaleTo::create(0.1,0.9));
		ButtonBg->runAction(ScaleTo::create(0.1,0.9));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			curButton->setTouchEnabled(false);
			auto ButtonBgTar=TargetedAction::create(ButtonBg,ScaleTo::create(0.1,0.8));
			auto shadeTarget=TouchShadeAction(curButton->getPosition(),ACTION_PNG_LQD);
			auto seqButtonAction=Sequence::create(ScaleTo::create(0.1,0.8),shadeTarget,NULL);
			auto curButtonTar=TargetedAction::create(curButton,seqButtonAction);
			auto EffectForThis=Spawn::create(ButtonBgTar,curButtonTar,NULL);
			auto entranceFun=CallFunc::create([=](){
				enterRoomStandAlone();
			});
			this->runAction(Sequence::create(EffectForThis,entranceFun,NULL));
		}
		break;
	case Widget::TouchEventType::CANCELED:
		curButton->runAction(ScaleTo::create(0.1,0.8));
		ButtonBg->runAction(ScaleTo::create(0.1,0.8));
		break;
	}
}

void DaiPiaoLayer::entranceToBingYiSe()
{
	//进入游戏场卡五星
}

void DaiPiaoLayer::entranceToGangGang()
{
	//进入游戏场暗四归
}

void DaiPiaoLayer::entranceDaSanYuan()
{
	//进入游戏条一色
}

void DaiPiaoLayer::entranceXSY()
{
	//进入游戏海底捞
}

void DaiPiaoLayer::entranceMingSiGui()
{
	//进入游戏杠上花
}

void DaiPiaoLayer::entranceLQD()
{
	//进入游戏碰碰胡
}