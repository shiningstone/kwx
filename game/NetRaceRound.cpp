
#include "./../utils/UtilBasic.h"

#include "Ai.h"
#include "NetRaceRound.h"
#include "CardCollection.h"

USING_NS_CC;

NetRRound::NetRRound(CardInHand *cardInHand,HuFan_t &fan,HuTarget_t aim)
:_fan(fan),_AIM(aim) {
    _cardInHand = cardInHand;
	_fan=0;

    _logger = LOGGER_REGISTER("RaceRound");
}

NetRRound::~NetRRound() {
    LOGGER_DEREGISTER(_logger);
}

long NetRRound::calcScore(Card_t kind,bool isNewDistributed,bool isLastOne,unsigned char last_action_WithGold,unsigned int continue_gang_times,bool isGangHua) {
    _cardInHand->update_statistics(kind);
    _fan = _cardInHand->statHuFanMask;
    
    if(isLastOne) {
		_fan |= RH_HAIDILAO;
    }

	if(isNewDistributed) {
		_fan |= RH_ZIMO;
	}

	if((continue_gang_times!=0)
        &&(last_action_WithGold==a_MING_GANG
            ||last_action_WithGold==a_AN_GANG
            ||last_action_WithGold==a_QIANG_GANG)) {
		if(isNewDistributed&&last_action_WithGold==a_QIANG_GANG)
			_fan |= RH_QIANGGANG;
		else if(isNewDistributed&&isGangHua)
			_fan |= RH_GANGHUA;
		else if(!isNewDistributed)
			_fan |= RH_GANGPAO;
	}

	if(_AIM!=RA_NOTASK)
		taskCheck(_fan);
    
	return Ai::getInstance()->sum_up_score(_fan);
}

void NetRRound::taskCheck(unsigned int flag) {
	if( !(flag&_AIM) ) {
		_aimDone=0;
    }
}

unsigned char NetRRound::init(int card_array[],int len)
{
	_score=0;

    _cardInHand->clear();

	for(int i=0;i<13;i++) {
        CardNode_t node;

		node.kind    = (Card_t)(card_array[i]);
		node.status  = sFREE;
		node.canPlay = true;
        
		_cardInHand->insert_card(node,1);
	}

    if(len<14) {/*NON-ZHUANG*/
        return a_JUMP;
    } else {
        return hand_in((CARD_KIND)(card_array[13]),0,0,false,a_JUMP,0,false);
    }
}

unsigned char NetRRound::ActiontodoCheckAgain() {
    LOGGER_WRITE("%s",__FUNCTION__);

	unsigned char res = 0x0;

    if(_cardInHand->has_shou_gang()) {/*BUG HERE??? always shougang*/
        res |= aSHOU_GANG;
    }

	if( _cardInHand->collect_ming_info() ) {
		res |= aMING;
    }

	return res;
}

unsigned char NetRRound::hand_in(CARD_KIND newCard,unsigned char isNewDistributed,unsigned char tingStatus,bool isLastOne,unsigned char last_action_WithGold,unsigned int continue_gang_times,bool isGangHua) {
	unsigned char res = 0x0;

    _cardInHand->push_back((Card_t)newCard);

	if(!_cardInHand->IsMing) {
        if( _cardInHand->has_shou_gang() && isNewDistributed && !isLastOne ) {
            res |= a_SHOU_GANG;
            break;
		}

        int freeNum = 0;
		for(int i=0;i<_cardInHand->size()-1;i++) {
			if(_cardInHand->get_kind(i)==newCard) {
				if(_cardInHand->get_status(i)==sPENG) {
					if(isNewDistributed&&!isLastOne) {
						res |= a_MING_GANG;
                        break;
					}
				} else if(_cardInHand->get_status(i)==sFREE) {
					freeNum++;
				}
			}
		}

        if(freeNum==3) {
            if(isNewDistributed) {
                if(!isLastOne) {
                    res |= (a_MING_GANG | a_AN_GANG);
                }
            } else {
                if(!isLastOne) {
                    res |= (a_MING_GANG | a_PENG);
                } else {
                    res |= a_PENG;
                }
            }
        } else if(freeNum==2&&!isNewDistributed) {
        	res |= a_PENG;
        }
	} else {
		for(int i=0;i<_cardInHand->FreeStart;i++) {
			if(_cardInHand->get_status(i)==sMING_KOU && _cardInHand->get_kind(i)==newCard) {
				if(!isLastOne) {
					if(isNewDistributed)
						res |= (a_MING_GANG | a_AN_GANG);
					else
						res |= a_MING_GANG;
				}
                
				break;
			}
        }
	}

	if(_cardInHand->can_hu((Card_t)newCard)) {
		_score = calcScore((Card_t)newCard,isNewDistributed,isLastOne,last_action_WithGold,continue_gang_times,isGangHua);

		if(isNewDistributed || (tingStatus==1||_score!=1)) {/* only ming can hu dianpao ??? */
			res|=a_HU;
        }
	}

    /* BUG ??? ming can implemented after some action take place, such as PENG/GANG */
    /*     !!! maybe done by ActiontodoCheckAgain                                   */
	if(isNewDistributed) {
		if(!_cardInHand->IsMing && !isLastOne) {
			if(_cardInHand->collect_ming_info())
				res |= a_MING;
		}
	}

    LOGGER_WRITE("NETWORK : %x %s action %d: newCard %d,isNewDistributed %d,tingStatus %d,isLastOne %d",
        (int)this,__FUNCTION__,res,newCard,isNewDistributed,tingStatus,isLastOne);
	return res;
}

CARD_KIND NetRRound::hand_out(unsigned int place)
{
	if( _cardInHand->get_status(place) != sFREE ){
		return ck_NOT_DEFINED;
	}

	CARD_KIND l_kind = (CARD_KIND)_cardInHand->get_kind(place);
    _cardInHand->delete_card(place,1);

    return l_kind;
}


ACT_RES NetRRound::others_action(bool isNewDistributed,ARRAY_ACTION act,Card_t kind) {
    LOGGER_WRITE("%x %s : %d (isNewDistributed=%d)",this,__FUNCTION__,act,isNewDistributed);

	InsertPlaceForMG=0;

    CardNode_t node;
    node.kind    = kind;
    node.canPlay = false;

	if(act==a_PENG) {
        node.status=sPENG;
        
        _cardInHand->delete_card(_cardInHand->FreeStart,3);
        _cardInHand->insert_card(node,3);
        _cardInHand->FreeStart += 3;
	} else if(act==a_MING_GANG) {
        node.status=sMING_GANG;

        _cardInHand->delete_card(_cardInHand->FreeStart,4);
        _cardInHand->insert_card(node,4);
		_cardInHand->FreeStart += 4;
	} else if(act==a_AN_GANG) {
        node.kind=CARD_UNKNOWN;
        node.status=sAN_GANG;

        _cardInHand->delete_card(_cardInHand->FreeStart,4);
        _cardInHand->insert_card(node,4);
		_cardInHand->FreeStart += 4;
	} else if(act==a_JUMP) {
		if(isNewDistributed) {
			node.status  = sFREE;
            node.canPlay = false;
            
			_cardInHand->pop_back();
            _cardInHand->insert_card(node,1);
		}
	}

	return ar_DONE;
}

ACT_RES NetRRound::action(bool isCardFromOther,ARRAY_ACTION act)
{
    LOGGER_WRITE("%x %s : %d (isCardFromOther=%d)",this,__FUNCTION__,act,isCardFromOther);

	InsertPlaceForMG=-1;

	CARD temp_data;
	temp_data.can_play=cps_YES;
	temp_data.kind=(CARD_KIND)_cardInHand->back()->kind;

    CardNode_t node;
    node.kind    = (Card_t)_cardInHand->back()->kind;
    node.canPlay = false;

	if(act==a_PENG) {
        node.status=sPENG;

        for(int i=_cardInHand->FreeStart;i<_cardInHand->size();i++) {
			if(_cardInHand->get_kind(i)==node.kind){
				_cardInHand->delete_card(i,2);
				break;
			}
        }

        _cardInHand->pop_back();
        _cardInHand->insert_card(node,3);
        _cardInHand->FreeStart += 3;
	}
	else if(act==a_KOU) {
        node.status=sMING_KOU;

		for(int i=_cardInHand->size()-1;i>=_cardInHand->FreeStart;i--)
			if(_cardInHand->get_status(i)==sMING_KOU) {
				node.kind=_cardInHand->get_kind(i);
				_cardInHand->delete_card(i,1);
				_cardInHand->insert_card(node,1);
                _cardInHand->FreeStart+=1;
				i++;
			}
	}
	else if(act==a_KOU_CANCEL) {
		node.status  = sFREE;
		node.canPlay = true;
        
		for(int i=_cardInHand->FreeStart-1;i>=0;i--) {
			if(_cardInHand->get_status(i)==sMING_KOU) {
				node.kind = _cardInHand->get_kind(i);
                
				_cardInHand->delete_card(i,1);
				_cardInHand->insert_card(node,1);
				_cardInHand->FreeStart-=1;
			}
        }
	}
	else if(act==a_MING_GANG) {
		node.status = sMING_GANG;

		bool ifFirst=true;
		for(int i=_cardInHand->size()-1;i>=0;i--) {
			if(_cardInHand->get_status(i)==sPENG&&_cardInHand->get_kind(i)==node.kind) {
				if(ifFirst)
					InsertPlaceForMG=i-2;
                
				ifFirst=false;

                _cardInHand->FreeStart--;
			} else if(_cardInHand->get_status(i)==sMING_KOU&& _cardInHand->get_kind(i)==node.kind) {
				_cardInHand->FreeStart--;
            }

			if(_cardInHand->get_kind(i)==node.kind)
				_cardInHand->delete_card(i,1);
		}
        _cardInHand->insert_card(node,4);
		_cardInHand->FreeStart += 4;
	}
	else if(act==a_AN_GANG) {
		_cardInHand->perform(aAN_GANG);
	}
	else if(act==a_SHOU_GANG) {
		int card_num;
		int i;
		for(i=_cardInHand->FreeStart;i<_cardInHand->size();i++)
		{
			card_num=1;
			for(int j=i+1;j<_cardInHand->size();j++)
				if(_cardInHand->get_kind(i)==_cardInHand->get_kind(j))
					card_num++;
			if(card_num==4) {
				node.kind = _cardInHand->get_kind(i);
				break;
			}
		}
        
		for(int k=_cardInHand->size();k>=_cardInHand->FreeStart;k--)
		{
			if(_cardInHand->get_kind(k)==_cardInHand->get_kind(i))
				_cardInHand->delete_card(k,1);
		}

		node.status  = sAN_GANG;
		node.canPlay = false;
        _cardInHand->insert_card(node,4);
		_cardInHand->FreeStart += 4;

        /*WHY*/
		node.kind    = _cardInHand->back()->kind;
		node.status  = sFREE;
		node.canPlay = true;
        
		_cardInHand->pop_back();
        _cardInHand->insert_card(node,1);
	}
	else if(act==a_MING)
	{
		for (int i=_cardInHand->FreeStart;i<_cardInHand->size();i++) {
			if(_cardInHand->can_handout(i)) {
				_cardInHand->at(i)->canPlay=true;
			} else {
				_cardInHand->at(i)->canPlay=false;
			}
		}
	}
	else if(act==a_HU)
	{
		node.status  = sFREE;
		node.canPlay = false;
        
		if(isCardFromOther) {
			_cardInHand->insert_card(node,1);
        } else {
			_cardInHand->pop_back();
            _cardInHand->insert_card(node,1);
		}
	
	}
	else if(act==a_JUMP)
	{
		if(!isCardFromOther) {
			node.status=sFREE;
            
			_cardInHand->pop_back();
            _cardInHand->insert_card(node,1);
		}
	}

	return ar_DONE;
}

HuTarget_t NetRRound::get_aim() const {
	return _aimDone;
}

HuTarget_t NetRRound::get_hu_flag() const {
	return _fan;
}

long NetRRound::get_score() const {
	return _score;
}

