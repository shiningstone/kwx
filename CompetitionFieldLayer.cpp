#include "CompetitionFieldLayer.h"
#include "EnterForCompetitionLayer.h"


CompetitionFieldLayer::CompetitionFieldLayer(void)
{
}


CompetitionFieldLayer::~CompetitionFieldLayer(void)
{
}

bool CompetitionFieldLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();

	auto buttonNewbieField=Button::create("cainiaochang.png","cainiaochang.png","cainiaochang.png",TextureResType::PLIST);
	buttonNewbieField->setAnchorPoint(Vec2(0.5,0.5));
	buttonNewbieField->setPosition(Vec2(origin.x+visibleSize.width*0.16,origin.y+visibleSize.height*0.655));
	buttonNewbieField->addTouchEventListener(CC_CALLBACK_2(CompetitionFieldLayer::onButtonNewbieField,this));
	this->addChild(buttonNewbieField,1,1);
	
	auto buttonMasterField=Button::create("dashichang.png","dashichang.png","dashichang.png",TextureResType::PLIST);
	buttonMasterField->setAnchorPoint(Vec2(0.5,0.5));
	buttonMasterField->setPosition(Vec2(origin.x+visibleSize.width*0.84,origin.y+visibleSize.height*0.655));
	buttonMasterField->addTouchEventListener(CC_CALLBACK_2(CompetitionFieldLayer::onButtonMasterField,this));
	this->addChild(buttonMasterField,1,2);

	auto buttonEliteField=Button::create("jingyingchang.png","jingyingchang.png","jingyingchang.png",TextureResType::PLIST);
	buttonEliteField->setAnchorPoint(Vec2(0.5,0.5));
	buttonEliteField->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.655));
	buttonEliteField->addTouchEventListener(CC_CALLBACK_2(CompetitionFieldLayer::onButtonEliteField,this));
	this->addChild(buttonEliteField,1,3);
	
	auto button6PersonField=Button::create("6rensai.png","6rensai.png","6rensai.png",TextureResType::PLIST);
	button6PersonField->setAnchorPoint(Vec2(0.5,0.5));
	button6PersonField->setPosition(Vec2(origin.x+visibleSize.width*0.16,origin.y+visibleSize.height*0.225));
	button6PersonField->addTouchEventListener(CC_CALLBACK_2(CompetitionFieldLayer::onButton6PersonField,this));
	this->addChild(button6PersonField,1,4);

	auto button24PersonField=Button::create("24rensai.png","24rensai.png","24rensai.png",TextureResType::PLIST);
	button24PersonField->setAnchorPoint(Vec2(0.5,0.5));
	button24PersonField->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.225));
	button24PersonField->addTouchEventListener(CC_CALLBACK_2(CompetitionFieldLayer::onButton24PersonField,this));
	this->addChild(button24PersonField,1,5);
	
	auto button99PersonField=Button::create("99rensai.png","99rensai.png","99rensai.png",TextureResType::PLIST);
	button99PersonField->setAnchorPoint(Vec2(0.5,0.5));
	button99PersonField->setPosition(Vec2(origin.x+visibleSize.width*0.84,origin.y+visibleSize.height*0.225));
	button99PersonField->addTouchEventListener(CC_CALLBACK_2(CompetitionFieldLayer::onButton99PersonField,this));
	this->addChild(button99PersonField,1,6);

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
			this->addChild(bgOfButton1,0,7);
			this->addChild(bgOfButton2,0,10);
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
			this->addChild(bgOfButton1,0,8);
			this->addChild(bgOfButton2,0,11);
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
			this->addChild(bgOfButton1,0,9);
			this->addChild(bgOfButton2,0,12);
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
	auto Button=(Sprite*)this->getChildByTag(1);
	auto ButtonBg=(Sprite*)this->getChildByTag(7);
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
		Button->runAction(CallFunc::create(this,callfunc_selector(CompetitionFieldLayer::actionNewbieField)));
		scheduleOnce(schedule_selector(CompetitionFieldLayer::entranceToNewbieField),0.5);
		break;
	case Widget::TouchEventType::CANCELED:
		Button->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		break;
	}
}

void CompetitionFieldLayer::onButtonEliteField(Ref* pSender,Widget::TouchEventType type)
{
	auto Button=(Sprite*)this->getChildByTag(3);
	auto ButtonBg=(Sprite*)this->getChildByTag(8);
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
		Button->runAction(CallFunc::create(this,callfunc_selector(CompetitionFieldLayer::actionEliteField)));
		scheduleOnce(schedule_selector(CompetitionFieldLayer::entranceToEliteField),0.5);
		break;
	case Widget::TouchEventType::CANCELED:
		Button->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		break;
	}
}

void CompetitionFieldLayer::onButtonMasterField(Ref* pSender,Widget::TouchEventType type)
{
	auto Button=(Sprite*)this->getChildByTag(2);
	auto ButtonBg=(Sprite*)this->getChildByTag(9);
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
		Button->runAction(CallFunc::create(this,callfunc_selector(CompetitionFieldLayer::actionMasterField)));
		scheduleOnce(schedule_selector(CompetitionFieldLayer::entranceToMasterField),0.5);
		break;
	case Widget::TouchEventType::CANCELED:
		Button->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		break;
	}
}

void CompetitionFieldLayer::onButton99PersonField(Ref* pSender,Widget::TouchEventType type)
{
	auto Button=(Sprite*)this->getChildByTag(6);
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
		Button->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		Button->runAction(CallFunc::create(this,callfunc_selector(CompetitionFieldLayer::action99PersonField)));
		scheduleOnce(schedule_selector(CompetitionFieldLayer::entrance99PersonField),0.5);
		break;
	case Widget::TouchEventType::CANCELED:
		Button->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		break;
	}
}

void CompetitionFieldLayer::onButton24PersonField(Ref* pSender,Widget::TouchEventType type)
{
	auto Button=(Sprite*)this->getChildByTag(5);
	auto ButtonBg=(Sprite*)this->getChildByTag(11);
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
		Button->runAction(CallFunc::create(this,callfunc_selector(CompetitionFieldLayer::action24PersonField)));
		scheduleOnce(schedule_selector(CompetitionFieldLayer::entrance24PersonField),0.5);
		break;
	case Widget::TouchEventType::CANCELED:
		Button->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		break;
	}
}

void CompetitionFieldLayer::onButton6PersonField(Ref* pSender,Widget::TouchEventType type)
{
	auto Button=(Sprite*)this->getChildByTag(4);
	auto ButtonBg=(Sprite*)this->getChildByTag(10);
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
		Button->runAction(CallFunc::create(this,callfunc_selector(CompetitionFieldLayer::action6PersonField)));
		scheduleOnce(schedule_selector(CompetitionFieldLayer::entrance6PersonField),0.5);
		break;
	case Widget::TouchEventType::CANCELED:
		Button->runAction(ScaleTo::create(0.1,1));
		ButtonBg->runAction(ScaleTo::create(0.1,1));
		break;
	}
}

void CompetitionFieldLayer::actionNewbieField()
{
	//cainiaochang dashichang jingyingchang 6rensai 24rensai 99rensai
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();

	auto button=Sprite::createWithSpriteFrameName("cainiaochang.png");
	button->setOpacity(100);
	auto buttonSize=button->getContentSize();
	button->setAnchorPoint(Vec2(0.5,0.5));
	button->setPosition(Vec2(origin.x+visibleSize.width*0.16,origin.y+visibleSize.height*0.655));
	this->addChild(button,0);

	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.3);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	button->runAction(spawn);
}

void CompetitionFieldLayer::entranceToNewbieField(float t)
{
	auto enterForCompete=EnterForCompetitionLayer::create();
	auto runScene=Director::getInstance()->getRunningScene();
	runScene->addChild(enterForCompete,3,1);
}

void CompetitionFieldLayer::actionEliteField()
{
	//cainiaochang dashichang jingyingchang 6rensai 24rensai 99rensai
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();

	auto button=Sprite::createWithSpriteFrameName("jingyingchang.png");
	button->setOpacity(100);
	auto buttonSize=button->getContentSize();
	button->setAnchorPoint(Vec2(0.5,0.5));
	button->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.655));
	this->addChild(button,0);

	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.3);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	button->runAction(spawn);
}

void CompetitionFieldLayer::entranceToEliteField(float t)
{
	auto enterForCompete=EnterForCompetitionLayer::create();
	auto runScene=Director::getInstance()->getRunningScene();
	runScene->addChild(enterForCompete,3,1);
}
void CompetitionFieldLayer::actionMasterField()
{
	//cainiaochang dashichang jingyingchang 6rensai 24rensai 99rensai
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();

	auto button=Sprite::createWithSpriteFrameName("dashichang.png");
	button->setOpacity(100);
	auto buttonSize=button->getContentSize();
	button->setAnchorPoint(Vec2(0.5,0.5));
	button->setPosition(Vec2(origin.x+visibleSize.width*0.84,origin.y+visibleSize.height*0.655));
	this->addChild(button,0);

	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.3);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	button->runAction(spawn);
}

void CompetitionFieldLayer::entranceToMasterField(float t)
{
	auto enterForCompete=EnterForCompetitionLayer::create();
	auto runScene=Director::getInstance()->getRunningScene();
	runScene->addChild(enterForCompete,3,1);
}

void CompetitionFieldLayer::action99PersonField()
{
	//cainiaochang dashichang jingyingchang 6rensai 24rensai 99rensai
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();

	auto button=Sprite::createWithSpriteFrameName("99rensai.png");
	button->setOpacity(100);
	auto buttonSize=button->getContentSize();
	button->setAnchorPoint(Vec2(0.5,0.5));
	button->setPosition(Vec2(origin.x+visibleSize.width*0.84,origin.y+visibleSize.height*0.225));
	this->addChild(button,0);

	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.3);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	button->runAction(spawn);
}

void CompetitionFieldLayer::entrance99PersonField(float t)
{
	auto enterForCompete=EnterForCompetitionLayer::create();
	auto runScene=Director::getInstance()->getRunningScene();
	runScene->addChild(enterForCompete,3,1);
}

void CompetitionFieldLayer::action24PersonField()
{
	//cainiaochang dashichang jingyingchang 6rensai 24rensai 99rensai
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();

	auto button=Sprite::createWithSpriteFrameName("24rensai.png");
	button->setOpacity(100);
	auto buttonSize=button->getContentSize();
	button->setAnchorPoint(Vec2(0.5,0.5));
	button->setPosition(Vec2(origin.x+visibleSize.width*0.5,origin.y+visibleSize.height*0.225));
	this->addChild(button,0);

	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.3);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	button->runAction(spawn);
}

void CompetitionFieldLayer::entrance24PersonField(float t)
{
	auto enterForCompete=EnterForCompetitionLayer::create();
	auto runScene=Director::getInstance()->getRunningScene();
	runScene->addChild(enterForCompete,3,1);
}

void CompetitionFieldLayer::action6PersonField()
{
	//cainiaochang dashichang jingyingchang 6rensai 24rensai 99rensai
	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto origin=Director::getInstance()->getVisibleOrigin();

	auto button=Sprite::createWithSpriteFrameName("6rensai.png");
	button->setOpacity(100);
	auto buttonSize=button->getContentSize();
	button->setAnchorPoint(Vec2(0.5,0.5));
	button->setPosition(Vec2(origin.x+visibleSize.width*0.16,origin.y+visibleSize.height*0.225));
	this->addChild(button,0);

	auto fadeOut=FadeOut::create(0.3);
	auto easeBounce=ScaleTo::create(0.3,1.3);
	auto spawn=Spawn::create(fadeOut,easeBounce,NULL);
	button->runAction(spawn);
}

void CompetitionFieldLayer::entrance6PersonField(float t)
{
	auto enterForCompete=EnterForCompetitionLayer::create();
	auto runScene=Director::getInstance()->getRunningScene();
	runScene->addChild(enterForCompete,3,1);
}
