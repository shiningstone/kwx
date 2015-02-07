#include "GameScene.h"
#include "stdio.h"
using namespace std;

GameScene::GameScene():RaceLayer()
{
}

GameScene::~GameScene()
{
}

bool GameScene::init()
{
	if(!RaceLayer::init())
	{
		return false;
	}
	init_race_sequence();
	return true;
}
void GameScene::distribute_card_event()
{
	DCI scard;
	if(dist_card_no<TOTAL_CARD_NUM)
	{
		scard.card=CARD_KIND(card_seq[dist_card_no++]/4);
		scard.num=dist_card_no;
		_eventDispatcher->dispatchCustomEvent(DISTRIBUTE_DONE_EVENT_TYPE,&scard);
	}
	else
		_eventDispatcher->dispatchCustomEvent(NOONE_WIN_EVENT_TYPE,NULL);
}

void GameScene::race_start_again()
{
	auto _waitstartListener = EventListenerCustom::create(WAIT_START_CALLBACK_EVENT_TYPE, [this](EventCustom * event){

		for(int j=0;j<2;j++)//伪随机数列生成
		{
			for(int i=0;i<TOTAL_CARD_NUM;i++)
			{
				int tmp=card_seq[i];
				int cur=rand()%TOTAL_CARD_NUM;
				card_seq[i]=card_seq[cur];
				card_seq[cur]=tmp;
			}
		}
		set_cards_sequence(card_seq);
		set_aims_sequence(aim);
		set_winner_no(get_last_winner_no());

		dist_card_no=40;
	});

	_eventDispatcher->addEventListenerWithFixedPriority(_waitstartListener, 3);

	auto _calldistributeListener = EventListenerCustom::create(DISTRIBUTE_CALL_EVENT_TYPE, [this](EventCustom * event){

			distribute_card_event();
		});
	_eventDispatcher->addEventListenerWithFixedPriority(_calldistributeListener, 2);

}

void GameScene::init_race_sequence()
{
	memset(aim,0,sizeof(int)*3);

	for(int i=0;i<TOTAL_CARD_NUM;i++)
	{
		card_seq[i]=i;
	}
	set_winner_no(1);

	race_role[0]=new Robot();
	race_role[1]=new Me();
	race_role[2]=new Robot();

	race_action[0]=new RRound();
	race_action[1]=new RRound();
	race_action[2]=new RRound();

	init_role(race_role,race_action);

	race_role[0]->get_parter()->set_role_type(SINGLE_BOARD_ROBOT);
	race_role[1]->get_parter()->set_role_type(SINGLE_BOADR_ME);
	race_role[2]->get_parter()->set_role_type(SINGLE_BOARD_ROBOT);

	create_race();

	race_start_again();
	
}

