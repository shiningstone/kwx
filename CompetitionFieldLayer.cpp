#include "CompetitionFieldLayer.h"
#include "EnterForCompetitionLayer.h"


CompetitionFieldLayer::CompetitionFieldLayer(void)
{
}


CompetitionFieldLayer::~CompetitionFieldLayer(void)
{
}
void CompetitionFieldLayer::entranceToEachRoom()
{
	auto runScene=Director::getInstance()->getRunningScene();
	auto enterForCompete=new EnterForCompetitionLayer(runScene);
	runScene->addChild(enterForCompete,3,BAO_MING_COMPETE_LAYER);
}
TargetedAction* CompetitionFieldLayer::TouchShadeAction(Vec2 curPos,std::string actionName)
{
	auto curActionButton=Sprite::createWithSpriteFrameName(actionName);
	curActionButton->setOpacity(100);
	auto curSize=curActionButton->getContentSize();
	curActionButton->setAnchorPoint(Vec2(0.5,0.5));
	curActionButton->setPosition(curPos);
	this->addChild(curActionButton,0);

	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.3);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	auto shadeTarget=TargetedAction::create(curActionButton,spawn);

	return shadeTarget;
}
bool CompetitionFieldLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}
	visibleSize=Director::getInstance()->getVisibleSize();
	origin=Director::getInstance()->getVisibleOrigin();

	auto buttonNewbieField=Button::create("cainiaochang.png","cainiaochang.png","cainiaochang.png",TextureResType::PLIST);
	buttonNewbieField->setAnchorPoint(Vec2(0.5,0.5));
	buttonNewbieField->setPosition(Vec2(origin.x+visibleSize.width*0.16,origin.y+visibleSize.height*0.655));
	buttonNewbieField->addTouchEventListener(CC_CALLBACK_2(CompetitionFieldLayer::onButtonNewbieField,this));
	this->addChild(buttonNewbieField,1,BUTTON_NEWBIEFIELD);

	auto buttonMasterField=Button::create("dashichang.png","dashichang.png","dashichang.png",TextureResType::PLIST);
	buttonMasterField->setAnchorPoint(Vec2(0.5,0.5));
	buttonMasterField->setPosition(Vec2(origin.x+visibleSize.width*0.84,origin.y+visibleSize.height*0.655));
	buttonMasterField->addTouchEventListener(CC_CALLBACK_2(CompetitionFieldLayer::onButtonMasterField,this));
	this->addChild(buttonMasterField,1,BUTTON_MASTERFIELD);

	auto buttonEliteField=Button::create("jingyingchang.png","jingyingchang.png","jingyingchang.png",TextureResType::PLIST);
	buttonEliteField->setAnchorPoint(Vec2(0.5,0.5));
	buttonEliteField->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.655));
	buttonEliteField->addTouchEventListener(CC_CALLBACK_2(CompetitionFieldLayer::onButtonEliteField,this));
	this->addChild(buttonEliteField,1,BUTTON_ELITEFIELD);

	auto button6PersonField=Button::create("6rensai.png","6rensai.png","6rensai.png",TextureResType::PLIST);
	button6PersonField->setAnchorPoint(Vec2(0.5,0.5));
	button6PersonField->setPosition(Vec2(origin.x+visibleSize.width*0.16,origin.y+visibleSize.height*0.225));
	button6PersonField->addTouchEventListener(CC_CALLBACK_2(CompetitionFieldLayer::onButton6PersonField,this));
	this->addChild(button6PersonField,1,BUTTON_6PERSONFIELD);

	auto button24PersonField=Button::create("24rensai.png","24rensai.png","24rensai.png",TextureResType::PLIST);
	button24PersonField->setAnchorPoint(Vec2(0.5,0.5));
	button24PersonField->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.225));
	button24PersonField->addTouchEventListener(CC_CALLBACK_2(CompetitionFieldLayer::onButton24PersonField,this));
	this->addChild(button24PersonField,1,BUTTON_24PERSONFIELD);

	auto button99PersonField=Button::create("99rensai.png","99rensai.png","99rensai.png",TextureResType::PLIST);
	button99PersonField->setAnchorPoint(Vec2(0.5,0.5));
	button99PersonField->setPosition(Vec2(origin.x+visibleSize.width*0.84,origin.y+visibleSize.height*0.225));
	button99PersonField->addTouchEventListener(CC_CALLBACK_2(CompetitionFieldLayer::onButton99PersonField,this));
	this->addChild(button99PersonField,1,BUTTON_99PERSONFIELD);

	for(int i=0;i<3;i++)
	{
		auto bgOfButton1=Sprite::createWithSpriteFrameName("yinying.png");
		bgOfButton1->setAnchorPoint(Vec2(0.5,0.5));
		bgOfButton1->setPosition(Vec2(origin.x+visibleSize.width*(0.2+0.335*i),origin.y+visibleSize.height*0.6));

		auto bgOfButton2=Sprite::createWithSpriteFrameName("yinying.png");
		bgOfButton2->setAnchorPoint(Vec2(0.5,0.5));
		bgOfButton2->setPosition(Vec2(origin.x+visibleSize.width*(0.2+0.34*i),origin.y+visibleSize.height*(0.17)));

		auto labelImage1=Sprite::createWithSpriteFrameName("baomingfei.png");
		labelImage1->setAnchorPoint(Vec2(0.5,0.5));
		labelImage1->setPosition(Vec2(70,75));

		auto labelImage2=Sprite::createWithSpriteFrameName("baomingfei.png");
		labelImage2->setAnchorPoint(Vec2(0.5,0.5));
		labelImage2->setPosition(Vec2(70,75));

		auto labelImage3=Sprite::createWithSpriteFrameName("fuwufei.png");
		labelImage3->setAnchorPoint(Vec2(0.5,0.5));
		labelImage3->setPosition(Vec2(90,45));

		auto labelImage4=Sprite::createWithSpriteFrameName("fuwufei.png");
		labelImage4->setAnchorPoint(Vec2(0.5,0.5));
		labelImage4->setPosition(Vec2(90,45));

		auto labelImage5=Sprite::createWithSpriteFrameName("renzaiwan.png");
		labelImage5->setAnchorPoint(Vec2(0.5,0.5));
		labelImage5->setPosition(Vec2(130,265));

		auto labelImage6=Sprite::createWithSpriteFrameName("renzaiwan.png");
		labelImage6->setAnchorPoint(Vec2(0.5,0.5));
		labelImage6->setPosition(Vec2(130,265));

		auto labelImage7=Sprite::createWithSpriteFrameName("jinbi56.png");
		labelImage7->setAnchorPoint(Vec2(0.5,0.5));
		labelImage7->setPosition(Vec2(220,75));

		auto labelImage8=Sprite::createWithSpriteFrameName("jinbi56.png");
		labelImage8->setAnchorPoint(Vec2(0.5,0.5));
		labelImage8->setPosition(Vec2(220,75));

		auto labelImage9=Sprite::createWithSpriteFrameName("jinbi2.png");
		labelImage9->setAnchorPoint(Vec2(0.5,0.5));
		labelImage9->setPosition(Vec2(210,45));

		auto labelImage10=Sprite::createWithSpriteFrameName("jinbi2.png");
		labelImage10->setAnchorPoint(Vec2(0.5,0.5));
		labelImage10->setPosition(Vec2(210,45));

		if(i==0)
		{
			this->addChild(bgOfButton1,0,SHADE_NEWBIEFIELD);
			this->addChild(bgOfButton2,0,SHADE_6PERSONFIELD);
			buttonNewbieField->addChild(labelImage1);
			button6PersonField->addChild(labelImage2);
			buttonNewbieField->addChild(labelImage3);
			button6PersonField->addChild(labelImage4);
			buttonNewbieField->addChild(labelImage5);
			button6PersonField->addChild(labelImage6);
			buttonNewbieField->addChild(labelImage7);
			button6PersonField->addChild(labelImage8);
			buttonNewbieField->addChild(labelImage9);
			button6PersonField->addChild(labelImage10);
		}
		else if(i==1)
		{
			this->addChild(bgOfButton1,0,SHADE_MASTERFIELD);
			this->addChild(bgOfButton2,0,SHADE_24PERSONFIELD);
			buttonEliteField->addChild(labelImage1);
			button24PersonField->addChild(labelImage2);
			buttonEliteField->addChild(labelImage3);
			button24PersonField->addChild(labelImage4);
			buttonEliteField->addChild(labelImage5);
			button24PersonField->addChild(labelImage6);
			buttonEliteField->addChild(labelImage7);
			button24PersonField->addChild(labelImage8);
			buttonEliteField->addChild(labelImage9);
			button24PersonField->addChild(labelImage10);
		}
		else if(i==2)
		{
			this->addChild(bgOfButton1,0,SHADE_ELITEFIELD);
			this->addChild(bgOfButton2,0,SHADE_99PERSONFIELD);
			buttonMasterField->addChild(labelImage1);
			button99PersonField->addChild(labelImage2);
			buttonMasterField->addChild(labelImage3);
			button99PersonField->addChild(labelImage4);
			buttonMasterField->addChild(labelImage5);
			button99PersonField->addChild(labelImage6);
			buttonMasterField->addChild(labelImage7);
			button99PersonField->addChild(labelImage8);
			buttonMasterField->addChild(labelImage9);
			button99PersonField->addChild(labelImage10);
		}

		if(i==0)
		{
			//buttonNewbieField button6PersonField
			auto labelImage11=LabelAtlas::create("5000", "fonts/resultbox_add_lscore.png", 18, 26, '0');
			labelImage11->setAnchorPoint(Vec2(1,0.5));
			labelImage11->setScaleX(0.9);
			labelImage11->setPosition(Vec2(190,75));
			buttonNewbieField->addChild(labelImage11);

			auto labelImage12=LabelAtlas::create("1000", "fonts/resultbox_add_lscore.png", 18, 26, '0');
			labelImage12->setAnchorPoint(Vec2(1,0.5));
			labelImage12->setScaleX(0.9);
			labelImage12->setPosition(Vec2(190,75));
			button6PersonField->addChild(labelImage12);

			auto labelImage13=LabelAtlas::create("500","fonts/goumai_number3.png",17,27,'0');
			labelImage13->setAnchorPoint(Vec2(1,0.5));
			labelImage13->setScaleX(0.75);
			labelImage13->setPosition(Vec2(180,43));
			buttonNewbieField->addChild(labelImage13);

			auto labelImage14=LabelAtlas::create("100","fonts/goumai_number3.png",17,27,'0');
			labelImage14->setAnchorPoint(Vec2(1,0.5));
			labelImage14->setScaleX(0.75);
			labelImage14->setPosition(Vec2(180,43));
			button6PersonField->addChild(labelImage14);
		}
		else if(i==1)
		{
			//buttonEliteField button24PersonField
			auto labelImage11=LabelAtlas::create("3", "fonts/resultbox_add_lscore.png", 18, 26, '0');
			labelImage11->setAnchorPoint(Vec2(1,0.5));
			labelImage11->setScaleX(0.9);
			labelImage11->setPosition(Vec2(150,75));
			buttonEliteField->addChild(labelImage11,2);

			auto labelImage15=Sprite::createWithSpriteFrameName("wan.png");
			labelImage15->setAnchorPoint(Vec2(1,0.5));
			labelImage15->setPosition(Vec2(190,75));
			buttonEliteField->addChild(labelImage15,2);

			auto labelImage12=LabelAtlas::create("3000", "fonts/resultbox_add_lscore.png", 18, 26, '0');
			labelImage12->setAnchorPoint(Vec2(1,0.5));
			labelImage12->setScaleX(0.9);
			labelImage12->setPosition(Vec2(190,75));
			button24PersonField->addChild(labelImage12,2);

			auto labelImage13=LabelAtlas::create("3000","fonts/goumai_number3.png",17,27,'0');
			labelImage13->setAnchorPoint(Vec2(1,0.5));
			labelImage13->setScaleX(0.75);
			labelImage13->setPosition(Vec2(180,43));
			buttonEliteField->addChild(labelImage13,2);

			auto labelImage14=LabelAtlas::create("300","fonts/goumai_number3.png",17,27,'0');
			labelImage14->setAnchorPoint(Vec2(1,0.5));
			labelImage14->setScaleX(0.75);
			labelImage14->setPosition(Vec2(180,43));
			button24PersonField->addChild(labelImage14,2);
		}
		else
			if(i==2)
			{
				//buttonMasterField button99PersonField
				auto labelImage11=LabelAtlas::create("10", "fonts/resultbox_add_lscore.png", 18, 26, '0');
				labelImage11->setAnchorPoint(Vec2(1,0.5));
				labelImage11->setScaleX(0.9);
				labelImage11->setPosition(Vec2(150,75));
				buttonMasterField->addChild(labelImage11,2);

				auto labelImage16=Sprite::createWithSpriteFrameName("wan.png");
				labelImage16->setAnchorPoint(Vec2(1,0.5));
				labelImage16->setPosition(Vec2(190,75));
				buttonMasterField->addChild(labelImage16,2);

				auto labelImage12=LabelAtlas::create("1", "fonts/resultbox_add_lscore.png", 18, 26, '0');
				labelImage12->setAnchorPoint(Vec2(1,0.5));
				labelImage12->setScaleX(0.9);
				labelImage12->setPosition(Vec2(150,75));
				button99PersonField->addChild(labelImage12,2);

				auto labelImage17=Sprite::createWithSpriteFrameName("wan.png");
				labelImage17->setAnchorPoint(Vec2(1,0.5));
				labelImage17->setPosition(Vec2(190,75));
				button99PersonField->addChild(labelImage17,2);

				auto labelImage13=LabelAtlas::create("1","fonts/goumai_number3.png",17,27,'0');
				labelImage13->setAnchorPoint(Vec2(1,0.5));
				labelImage13->setScaleX(0.75);
				labelImage13->setPosition(Vec2(150,43));
				buttonMasterField->addChild(labelImage13,2);

				auto labelImage15=Sprite::createWithSpriteFrameName("wan1.png");
				labelImage15->setAnchorPoint(Vec2(1,0.5));
				labelImage15->setPosition(Vec2(180,43));
				buttonMasterField->addChild(labelImage15,2);

				auto labelImage14=LabelAtlas::create("1000","fonts/goumai_number3.png",17,27,'0');
				labelImage14->setAnchorPoint(Vec2(1,0.5));
				labelImage14->setScaleX(0.75);
				labelImage14->setPosition(Vec2(180,43));
				button99PersonField->addChild(labelImage14,2);
			}
	}

	return true;
}

void CompetitionFieldLayer::onButtonNewbieField(Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto ButtonBg=(Sprite*)this->getChildByTag(SHADE_NEWBIEFIELD);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		curButton->runAction(ScaleTo::create(0.1,1.1));
		ButtonBg->runAction(ScaleTo::create(0.1,1.1));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			curButton->setTouchEnabled(false);
			auto ButtonBgTar=TargetedAction::create(ButtonBg,ScaleTo::create(0.1,1));
			auto shadeTarget=TouchShadeAction(curButton->getPosition(),ACTION_PNG_CLS);
			auto seqButtonAction=Sequence::create(ScaleTo::create(0.1,1),shadeTarget,NULL);
			auto curButtonTar=TargetedAction::create(curButton,seqButtonAction);
			auto EffectForThis=Spawn::create(ButtonBgTar,curButtonTar,NULL);
			auto entranceFun=CallFunc::create([=](){
				entranceToEachRoom();
			});
			this->runAction(Sequence::create(EffectForThis,entranceFun,NULL));
		}
		break;
	case Widget::TouchEventType::CANCELED:
		curButton->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		break;
	}
}

void CompetitionFieldLayer::onButtonEliteField(Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto ButtonBg=(Sprite*)this->getChildByTag(SHADE_ELITEFIELD);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		curButton->runAction(ScaleTo::create(0.1,1.1));
		ButtonBg->runAction(ScaleTo::create(0.1,1.1));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			curButton->setTouchEnabled(false);
			auto ButtonBgTar=TargetedAction::create(ButtonBg,ScaleTo::create(0.1,1));
			auto shadeTarget=TouchShadeAction(curButton->getPosition(),ACTION_PNG_JYS);
			auto seqButtonAction=Sequence::create(ScaleTo::create(0.1,1),shadeTarget,NULL);
			auto curButtonTar=TargetedAction::create(curButton,seqButtonAction);
			auto EffectForThis=Spawn::create(ButtonBgTar,curButtonTar,NULL);
			auto entranceFun=CallFunc::create([=](){
				entranceToEachRoom();
			});
			this->runAction(Sequence::create(EffectForThis,entranceFun,NULL));
		}
		break;
	case Widget::TouchEventType::CANCELED:
		curButton->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		break;
	}
}

void CompetitionFieldLayer::onButtonMasterField(Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto ButtonBg=(Sprite*)this->getChildByTag(SHADE_MASTERFIELD);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		curButton->runAction(ScaleTo::create(0.1,1.1));
		ButtonBg->runAction(ScaleTo::create(0.1,1.1));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			curButton->setTouchEnabled(false);
			auto ButtonBgTar=TargetedAction::create(ButtonBg,ScaleTo::create(0.1,1));
			auto shadeTarget=TouchShadeAction(curButton->getPosition(),ACTION_PNG_DSS);
			auto seqButtonAction=Sequence::create(ScaleTo::create(0.1,1),shadeTarget,NULL);
			auto curButtonTar=TargetedAction::create(curButton,seqButtonAction);
			auto EffectForThis=Spawn::create(ButtonBgTar,curButtonTar,NULL);
			auto entranceFun=CallFunc::create([=](){
				entranceToEachRoom();
			});
			this->runAction(Sequence::create(EffectForThis,entranceFun,NULL));
		}
		break;
	case Widget::TouchEventType::CANCELED:
		curButton->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		break;
	}
}

void CompetitionFieldLayer::onButton99PersonField(Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto ButtonBg=(Sprite*)this->getChildByTag(SHADE_99PERSONFIELD);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		curButton->runAction(ScaleTo::create(0.1,1.1));
		ButtonBg->runAction(ScaleTo::create(0.1,1.1));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			curButton->setTouchEnabled(false);
			auto ButtonBgTar=TargetedAction::create(ButtonBg,ScaleTo::create(0.1,1));
			auto shadeTarget=TouchShadeAction(curButton->getPosition(),ACTION_PNG_99R);
			auto seqButtonAction=Sequence::create(ScaleTo::create(0.1,1),shadeTarget,NULL);
			auto curButtonTar=TargetedAction::create(curButton,seqButtonAction);
			auto EffectForThis=Spawn::create(ButtonBgTar,curButtonTar,NULL);
			auto entranceFun=CallFunc::create([=](){
				entranceToEachRoom();
			});
			this->runAction(Sequence::create(EffectForThis,entranceFun,NULL));
		}
		break;
	case Widget::TouchEventType::CANCELED:
		curButton->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		break;
	}
}

void CompetitionFieldLayer::onButton24PersonField(Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto ButtonBg=(Sprite*)this->getChildByTag(SHADE_24PERSONFIELD);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		curButton->runAction(ScaleTo::create(0.1,1.1));
		ButtonBg->runAction(ScaleTo::create(0.1,1.1));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			curButton->setTouchEnabled(false);
			auto ButtonBgTar=TargetedAction::create(ButtonBg,ScaleTo::create(0.1,1));
			auto shadeTarget=TouchShadeAction(curButton->getPosition(),ACTION_PNG_24R);
			auto seqButtonAction=Sequence::create(ScaleTo::create(0.1,1),shadeTarget,NULL);
			auto curButtonTar=TargetedAction::create(curButton,seqButtonAction);
			auto EffectForThis=Spawn::create(ButtonBgTar,curButtonTar,NULL);
			auto entranceFun=CallFunc::create([=](){
				entranceToEachRoom();
			});
			this->runAction(Sequence::create(EffectForThis,entranceFun,NULL));
		}
		break;
	case Widget::TouchEventType::CANCELED:
		curButton->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		break;
	}
}

void CompetitionFieldLayer::onButton6PersonField(Ref* pSender,Widget::TouchEventType type)
{
	auto curButton=(Button*)pSender;
	auto ButtonBg=(Sprite*)this->getChildByTag(SHADE_6PERSONFIELD);
	switch(type)
	{
	case Widget::TouchEventType::BEGAN:
		curButton->runAction(ScaleTo::create(0.1,1.1));
		ButtonBg->runAction(ScaleTo::create(0.1,1.1));
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
		{
			curButton->setTouchEnabled(false);
			auto ButtonBgTar=TargetedAction::create(ButtonBg,ScaleTo::create(0.1,1));
			auto shadeTarget=TouchShadeAction(curButton->getPosition(),ACTION_PNG_6RS);
			auto seqButtonAction=Sequence::create(ScaleTo::create(0.1,1),shadeTarget,NULL);
			auto curButtonTar=TargetedAction::create(curButton,seqButtonAction);
			auto EffectForThis=Spawn::create(ButtonBgTar,curButtonTar,NULL);
			auto entranceFun=CallFunc::create([=](){
				entranceToEachRoom();
			});
			this->runAction(Sequence::create(EffectForThis,entranceFun,NULL));
		}
		break;
	case Widget::TouchEventType::CANCELED:
		curButton->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		break;
	}
}
