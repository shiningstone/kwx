#include "BuDaiPiaoLayer.h"
#include "game/RaceLayer.h"


BuDaiPiaoLayer::BuDaiPiaoLayer(void)
{
}


BuDaiPiaoLayer::~BuDaiPiaoLayer(void)
{
}

TargetedAction* BuDaiPiaoLayer::TouchShadeAction(Vec2 curPos,std::string actionName)
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
void BuDaiPiaoLayer::enterRoomStandAlone()
{
	auto scene = Scene::create();
	//SpriteFrameCache::getInstance()->removeSpriteFrames();
	//TextureCache::sharedTextureCache()->removeAllTextures();

#ifndef NETWORK_GAME_DEBUG
	RaceLayer *layer = RaceLayer::create();
	scene->addChild(layer);
	layer->CreateRace(LOCAL_GAME);
#else
	RaceLayer *layer = RaceLayer::create();
	scene->addChild(layer);
	layer->CreateRace(NETWORK_GAME);
#endif

	Director::getInstance()->replaceScene(scene);
}
bool BuDaiPiaoLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}
	visibleSize=Director::getInstance()->getVisibleSize();
	origin=Director::getInstance()->getVisibleOrigin();

	auto buttonKaWuXing=Button::create("kawuxingBtn.png","kawuxingBtn.png","kawuxingBtn.png",TextureResType::PLIST);
	buttonKaWuXing->setScale(0.8);
	auto kaWuXingSize=buttonKaWuXing->getContentSize();
	buttonKaWuXing->setAnchorPoint(Vec2(0.5,0.5));
	buttonKaWuXing->setPosition(Vec2(origin.x+visibleSize.width*0.026+kaWuXingSize.width/2,origin.y+visibleSize.height*0.43+kaWuXingSize.height/2+30));
	buttonKaWuXing->addTouchEventListener(CC_CALLBACK_2(BuDaiPiaoLayer::onButtonKaWuXing,this));
	this->addChild(buttonKaWuXing,1,KAWUXING_BUTTON);

	auto buttonTiaoYiSe=Button::create("tiaoyise.png","tiaoyise.png","tiaoyise.png",TextureResType::PLIST);
	buttonTiaoYiSe->setScale(0.8);
	auto sizeOfButtonTiaoYiSe=buttonTiaoYiSe->getContentSize();
	buttonTiaoYiSe->setAnchorPoint(Vec2(0.5,0.5));
	buttonTiaoYiSe->setPosition(Vec2(origin.x+visibleSize.width*0.326+sizeOfButtonTiaoYiSe.width/2,origin.y+visibleSize.height*0.43+sizeOfButtonTiaoYiSe.height/2+30));
	buttonTiaoYiSe->addTouchEventListener(CC_CALLBACK_2(BuDaiPiaoLayer::onButtonTiaoYiSe,this));
	this->addChild(buttonTiaoYiSe,1,TIAOYISE_BUTTON);

	auto buttonGangShangHua=Button::create("gangshanghua.png","gangshanghua.png","gangshanghua.png",TextureResType::PLIST);
	buttonGangShangHua->setScale(0.8);
	auto sizeOfButtonGangShangHua=buttonGangShangHua->getContentSize();
	buttonGangShangHua->setAnchorPoint(Vec2(0.5,0.5));
	buttonGangShangHua->setPosition(Vec2(origin.x+visibleSize.width*0.626+sizeOfButtonGangShangHua.width/2,origin.y+visibleSize.height*0.43+sizeOfButtonGangShangHua.height/2+30));
	buttonGangShangHua->addTouchEventListener(CC_CALLBACK_2(BuDaiPiaoLayer::onButtonGangShangHua,this));
	this->addChild(buttonGangShangHua,1,GANGSHANGHUA_BT);
	
	auto buttonAnSiGui=Button::create("ansigui.png","ansigui.png","ansigui.png",TextureResType::PLIST);
	buttonAnSiGui->setScale(0.8);
	auto sizeOfButtonAnSiGui=buttonAnSiGui->getContentSize();
	buttonAnSiGui->setAnchorPoint(Vec2(0.5,0.5));
	buttonAnSiGui->setPosition(Vec2(origin.x+visibleSize.width*0.026+sizeOfButtonAnSiGui.width/2,origin.y+sizeOfButtonAnSiGui.height/2+60));
	buttonAnSiGui->addTouchEventListener(CC_CALLBACK_2(BuDaiPiaoLayer::onButtonAnSiGui,this));
	this->addChild(buttonAnSiGui,1,ANSIGUI_BUTTON);

	auto buttonHaiDiLao=Button::create("haidilao.png","haidilao.png","haidilao.png",TextureResType::PLIST);
	buttonHaiDiLao->setScale(0.8);
	auto sizeOfButtonHaiDiLao=buttonHaiDiLao->getContentSize();
	buttonHaiDiLao->setAnchorPoint(Vec2(0.5,0.5));
	buttonHaiDiLao->setPosition(Vec2(origin.x+visibleSize.width*0.326+sizeOfButtonHaiDiLao.width/2,origin.y+sizeOfButtonHaiDiLao.height/2+60));
	buttonHaiDiLao->addTouchEventListener(CC_CALLBACK_2(BuDaiPiaoLayer::onButtonHaiDilao,this));
	this->addChild(buttonHaiDiLao,1,HAIDILAO_BUTTON);

	auto buttonPengPengHu=Button::create("pengpenghu.png","pengpenghu.png","pengpenghu.png",TextureResType::PLIST);
	buttonPengPengHu->setScale(0.8);
	auto sizeOfButtonPengPengHu=buttonPengPengHu->getContentSize();
	buttonPengPengHu->setAnchorPoint(Vec2(0.5,0.5));
	buttonPengPengHu->setPosition(Vec2(origin.x+visibleSize.width*0.626+sizeOfButtonPengPengHu.width/2,origin.y+sizeOfButtonPengPengHu.height/2+60));
	buttonPengPengHu->addTouchEventListener(CC_CALLBACK_2(BuDaiPiaoLayer::onButtonPengPengHu,this));
	this->addChild(buttonPengPengHu,1,PENGPENG_BUTTON);

	for(int i=0;i<3;i++)
	{
		auto widthOfButton=buttonKaWuXing->getContentSize().width;
		auto heightOfButton=buttonKaWuXing->getContentSize().height;

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

		auto labelImage7=Sprite::createWithSpriteFrameName("bei.png");
		labelImage7->setAnchorPoint(Vec2(0.5,0.5));
		labelImage7->setPosition(Vec2(230,85));

		int j=8*(i+1);
		char num[10]={0};
		sprintf(num,"%d",j);
		auto labelImage12=LabelAtlas::create(num, "fonts/resultbox_add_lscore.png", 18, 26, '0');
		labelImage12->setAnchorPoint(Vec2(0.5,0.5));
		labelImage12->setPosition(Vec2(170,83));

		auto labelImage14=Sprite::createWithSpriteFrameName("jinbi2.png");
		labelImage14->setAnchorPoint(Vec2(0.5,0.5));
		labelImage14->setPosition(Vec2(230,45));

		auto labelImage15=Sprite::createWithSpriteFrameName("jinbi2.png");
		labelImage15->setAnchorPoint(Vec2(0.5,0.5));
		labelImage15->setPosition(Vec2(230,45));

		if(i==0)
		{
			this->addChild(bgOfButton1,0,KAWUXING_SHADE);
			this->addChild(bgOfButton2,0,ANSIGUI_SHADE);
			buttonKaWuXing->addChild(labelImage1,2);
			buttonAnSiGui->addChild(labelImage2,2);
			buttonKaWuXing->addChild(labelImage3,2);
			buttonAnSiGui->addChild(labelImage4,2);
			buttonKaWuXing->addChild(labelImage5,2);
			buttonAnSiGui->addChild(labelImage6,2);
			buttonKaWuXing->addChild(labelImage7,2,16);
			buttonKaWuXing->addChild(labelImage12,2,17);
			buttonKaWuXing->addChild(labelImage14,2,18);
			buttonAnSiGui->addChild(labelImage15,2,27);
		}
		else if(i==1)
		{
			this->addChild(bgOfButton1,0,TIAOYISE_SHADE);
			this->addChild(bgOfButton2,0,HAIDILAO_SHADE);
			buttonTiaoYiSe->addChild(labelImage1,2);
			buttonHaiDiLao->addChild(labelImage2,2);
			buttonTiaoYiSe->addChild(labelImage3,2);
			buttonHaiDiLao->addChild(labelImage4,2);
			buttonTiaoYiSe->addChild(labelImage5,2);
			buttonHaiDiLao->addChild(labelImage6,2);
			buttonTiaoYiSe->addChild(labelImage7,2);
			buttonTiaoYiSe->addChild(labelImage12,2);
			buttonTiaoYiSe->addChild(labelImage14,2);
			buttonHaiDiLao->addChild(labelImage15,2);
		}
		else if(i==2)
		{
			this->addChild(bgOfButton1,0,GANGHUA_SHADE);
			this->addChild(bgOfButton2,0,PENGPENG_SHADE);
			buttonGangShangHua->addChild(labelImage1,2);
			buttonPengPengHu->addChild(labelImage2,2,67);
			buttonGangShangHua->addChild(labelImage3,2);
			buttonPengPengHu->addChild(labelImage4,2,68);
			buttonGangShangHua->addChild(labelImage5,2);
			buttonPengPengHu->addChild(labelImage6,2,69);
			buttonGangShangHua->addChild(labelImage7,2);
			buttonGangShangHua->addChild(labelImage12,2);
			buttonGangShangHua->addChild(labelImage14,2);
			buttonPengPengHu->addChild(labelImage15,2,70);
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

			auto labelImage13=LabelAtlas::create("64", "fonts/resultbox_add_lscore.png", 18, 26, '0');
			labelImage13->setAnchorPoint(Vec2(0.5,0.5));
			labelImage13->setPosition(Vec2(170,83));
			buttonAnSiGui->addChild(labelImage13,2);

			auto labelImage8=Sprite::createWithSpriteFrameName("bei.png");
			labelImage8->setAnchorPoint(Vec2(0.5,0.5));
			labelImage8->setPosition(Vec2(230,85));
			buttonAnSiGui->addChild(labelImage8,2);

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

			auto labelImage8=Sprite::createWithSpriteFrameName("buxianfengding.png");
			labelImage8->setAnchorPoint(Vec2(0.5,0.5));
			labelImage8->setPosition(Vec2(215,85));
			buttonHaiDiLao->addChild(labelImage8,2);

			auto labelImage22=Sprite::createWithSpriteFrameName("qian2.png");
			labelImage22->setAnchorPoint(Vec2(0.5,0.5));
			labelImage22->setPosition(Vec2(193,45));
			buttonTiaoYiSe->addChild(labelImage22,2);

			auto labelImage16=LabelAtlas::create("5","fonts/goumai_number3.png",17,27,'0');
			labelImage16->setAnchorPoint(Vec2(0.5,0.5));
			labelImage16->setScaleX(0.75);
			labelImage16->setPosition(Vec2(170,45));
			buttonTiaoYiSe->addChild(labelImage16,2);

			auto labelImage17=LabelAtlas::create("20","fonts/goumai_number3.png",17,27,'0');
			labelImage17->setAnchorPoint(Vec2(0.5,0.5));
			labelImage17->setScaleX(0.75);
			labelImage17->setPosition(Vec2(170,45));
			buttonHaiDiLao->addChild(labelImage17,2);

			auto labelImage18=Sprite::createWithSpriteFrameName("wan1.png");
			labelImage18->setAnchorPoint(Vec2(0.5,0.5));
			labelImage18->setPosition(Vec2(193,45));
			buttonHaiDiLao->addChild(labelImage18,2);
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

			auto labelImage8=Sprite::createWithSpriteFrameName("buxianfengding.png");
			labelImage8->setAnchorPoint(Vec2(0.5,0.5));
			labelImage8->setPosition(Vec2(215,85));
			buttonPengPengHu->addChild(labelImage8,2);

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

	auto quickStartButton=Button::create("QuickGame.png","QuickGame2.png","QuickGame2.png",UI_TEX_TYPE_PLIST);
	quickStartButton->setAnchorPoint(Vec2(0.5,0));
	quickStartButton->setPosition(Vec2(origin.x+visibleSize.width*0.48,origin.y/*-visibleSize.height*0.15*/));
	quickStartButton->setScale(0.8);
	this->addChild(quickStartButton);
	
	return true;
}

void BuDaiPiaoLayer::onButtonKaWuXing(Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto ButtonBg=(Sprite*)this->getChildByTag(KAWUXING_SHADE);
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
			auto shadeTarget=TouchShadeAction(curButton->getPosition(),ACTION_PNG_KWX);
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

void BuDaiPiaoLayer::onButtonTiaoYiSe(Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto ButtonBg=(Sprite*)this->getChildByTag(TIAOYISE_SHADE);
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
			auto shadeTarget=TouchShadeAction(curButton->getPosition(),ACTION_PNG_TYS);
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
void BuDaiPiaoLayer::onButtonGangShangHua(Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto ButtonBg=(Sprite*)this->getChildByTag(GANGHUA_SHADE);
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
			auto shadeTarget=TouchShadeAction(curButton->getPosition(),ACTION_PNG_GSH);
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
void BuDaiPiaoLayer::onButtonAnSiGui(Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto ButtonBg=(Sprite*)this->getChildByTag(ANSIGUI_SHADE);
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
			auto shadeTarget=TouchShadeAction(curButton->getPosition(),ACTION_PNG_ASG);
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
void BuDaiPiaoLayer::onButtonHaiDilao(Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto ButtonBg=(Sprite*)this->getChildByTag(HAIDILAO_SHADE);
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
			auto shadeTarget=TouchShadeAction(curButton->getPosition(),ACTION_PNG_HDL);
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
void BuDaiPiaoLayer::onButtonPengPengHu(Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto ButtonBg=(Sprite*)this->getChildByTag(PENGPENG_SHADE);
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
			auto shadeTarget=TouchShadeAction(curButton->getPosition(),ACTION_PNG_PPH);
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

void BuDaiPiaoLayer::entranceToKaWuXing()
{
	//卡五星
}

void BuDaiPiaoLayer::entranceToAnSiGui()
{
	//进入游戏场暗四归
}

void BuDaiPiaoLayer::entranceToTiaoYiSe()
{
	//进入游戏条一色
}

void BuDaiPiaoLayer::entranceHaiDiLao()
{
	//进入游戏海底捞
}

void BuDaiPiaoLayer::entranceGangShangHua()
{
	//进入游戏杠上花
}

void BuDaiPiaoLayer::entrancePengPengHu()
{
	//进入游戏碰碰胡
}