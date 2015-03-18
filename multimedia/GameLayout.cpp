
#include "GameLayout.h"

GameLayout::GameLayout(cocos2d::Point origin, cocos2d::Size size)
:ORIGIN(origin),SIZE(size){

}

/*************************************
    player
*************************************/
Vec2 GameLayout::AnchorOfHeadBkg(int dir) {
    switch(dir) {
        case 0:   return Vec2(0.0f,1.0f);
        case 1:   return Vec2(0,0);
        case 2:   return Vec2(1,1);
    }
}

void GameLayout::SetPlayerBkg(PlayerDir_t dir,Sprite *bkg) {
    _playerBkg[dir] = bkg;

    switch(dir) {
        case LEFT:
            _playerPosi[dir].basePoint.x = 29 + _playerBkg[dir]->getTextureRect().size.width*1.5 + ORIGIN.x;
            _playerPosi[dir].basePoint.y = ORIGIN.y + SIZE.height-30;
            _playerPosi[dir].riverPoint.x = ORIGIN.x + SIZE.width*0.33;
            _playerPosi[dir].riverPoint.y = ORIGIN.y + SIZE.height*0.64;
            _playerPosi[dir].zhuangPoint.x = 29 + _playerBkg[dir]->getTextureRect().size.width/2;
            _playerPosi[dir].zhuangPoint.y = 536 - _playerBkg[dir]->getTextureRect().size.height;
            break;
        case MIDDLE:
            _playerPosi[dir].basePoint.x = SIZE.width*0.06 + ORIGIN.x;
            _playerPosi[dir].basePoint.y = ORIGIN.y;
            _playerPosi[dir].riverPoint.x = ORIGIN.x + SIZE.width*0.412;
            _playerPosi[dir].riverPoint.y = ORIGIN.y + SIZE.height*0.38;
            _playerPosi[dir].zhuangPoint.x = 49 + _playerBkg[dir]->getTextureRect().size.width;
            _playerPosi[dir].zhuangPoint.y = 129 + _playerBkg[1]->getTextureRect().size.height/2;
            break;
        case RIGHT:
            _playerPosi[dir].basePoint.x = 1190-_playerBkg[dir]->getTextureRect().size.width*2 + ORIGIN.x + SIZE.width*0.019 + 10;
            _playerPosi[dir].basePoint.y = 220;
            _playerPosi[dir].riverPoint.x = ORIGIN.x + SIZE.width*0.67;
            _playerPosi[dir].riverPoint.y = ORIGIN.y + SIZE.height*0.43;
            _playerPosi[dir].zhuangPoint.x = 1190 - _playerBkg[dir]->getTextureRect().size.width/2;
            _playerPosi[dir].zhuangPoint.y = 536 - _playerBkg[dir]->getTextureRect().size.height;
            break;
    }
}

Vec2 GameLayout::PositionOfMicIcon() {
    return Vec2(49, 129 + _playerBkg[MIDDLE]->getTextureRect().size.height);
}

Vec2 GameLayout::PositionOfMapai(int direction) {
    switch(direction) {
        case 0:
            return Vec2(ORIGIN.x + SIZE.width*419/1218,ORIGIN.y + SIZE.height*390/716);
            break;
        case 1:
            return Vec2(ORIGIN.x + SIZE.width*650/1218,ORIGIN.y + SIZE.height*498/716);
            break;
        case 2:
            return Vec2(ORIGIN.x + SIZE.width*799/1218,ORIGIN.y + SIZE.height*326/716);
            break;
        case 3:
            return Vec2(ORIGIN.x + SIZE.width*568/1218,ORIGIN.y + SIZE.height*218/716);
            break;
    }
}

Vec2 GameLayout::PositionOfClock() {
    return Vec2(ORIGIN.x+SIZE.width*0.5,ORIGIN.y+SIZE.height*0.65);
}

Vec2 GameLayout::PositionOfClockIndicator(PlayerDir_t dir) {
    Vec2 clock = PositionOfClock();
    
    switch(dir) {
        case LEFT:
            return Vec2(clock.x-60,clock.y);
        case MIDDLE:
            return Vec2(clock.x,clock.y-60);
        case RIGHT:
            return Vec2(clock.x+60,clock.y);
    }
}

Vec2 GameLayout::AnchorOfZhuang(PlayerDir_t dir) {
    switch(dir) {
        case LEFT:
            return Vec2(0.5,1);
        case MIDDLE:
            return Vec2(0,0.5);
        case RIGHT:
            return Vec2(0.5,1);
    }
}

/*************************************
    gold count
*************************************/
Vec2 GameLayout::AnchorOfSign(int dir) {
    switch(dir) {
        case 0:   return Vec2(0.5,0.5);
        case 1:   return Vec2(0.5,0.5);
        case 2:   return Vec2(1,0.5);
    }
}

Vec2 GameLayout::PositionOfSign(int dir,int xOffset) {
    switch(dir) {
        case 0:   
            return Vec2(ORIGIN.x + SIZE.width*271/1218,
                        ORIGIN.y + SIZE.height*406/716);
        case 1:   
            return Vec2(ORIGIN.x + SIZE.width*559/1218,
                        ORIGIN.y + SIZE.height*154/716);
        case 2:   
            return Vec2(ORIGIN.x + SIZE.width*996/1218 - xOffset,
                        ORIGIN.y + SIZE.height*406/716);
    }
}

Vec2 GameLayout::DestOfSign(int dir,int xOffset) {
    switch(dir) {
        case 0:   
            return Vec2(ORIGIN.x + SIZE.width*271/1218,
                        ORIGIN.y + SIZE.height*480/716);
        case 1:   
            return Vec2(ORIGIN.x + SIZE.width*559/1218,
                        ORIGIN.y + SIZE.height*228/716);
        case 2:   
            return Vec2(ORIGIN.x + SIZE.width*996/1218 - xOffset,
                        ORIGIN.y + SIZE.height*480/716);
    }
}

Vec2 GameLayout::AnchorOfNumber(int dir) {
    switch(dir) {
        case 0:   return Vec2(0,0.5);
        case 1:   return Vec2(0,0.5);
        case 2:   return Vec2(1,0.5);
    }
}

Vec2 GameLayout::PositionOfNumber(int dir) {
    switch(dir) {
        case 0:   
            return Vec2(ORIGIN.x + SIZE.width*291/1218,
                        ORIGIN.y + SIZE.height*406/716);
        case 1:   
            return Vec2(ORIGIN.x + SIZE.width*578/1218,
                        ORIGIN.y + SIZE.height*154/716);
        case 2:   
            return Vec2(ORIGIN.x + SIZE.width*996/1218,
                        ORIGIN.y + SIZE.height*406/716);
    }
}

Vec2 GameLayout::DestOfNumber(int dir) {
    switch(dir) {
        case 0:   
            return Vec2(ORIGIN.x + SIZE.width*291/1218,
                        ORIGIN.y + SIZE.height*480/716);
        case 1:   
            return Vec2(ORIGIN.x + SIZE.width*578/1218,
                        ORIGIN.y + SIZE.height*228/716);
        case 2:   
            return Vec2(ORIGIN.x + SIZE.width*996/1218,
                        ORIGIN.y + SIZE.height*480/716);
    }
}

Vec2 GameLayout::AnchorOfGold(int dir) {
    switch(dir) {
        case 0:   return Vec2(0.5,0.5);
        case 1:   return Vec2(0.5,0.5);
        case 2:   return Vec2(0.5,0.5);
    }
}

Vec2 GameLayout::PositionOfGold(int dir,int xOffset) {
    switch(dir) {
        case 0:   
            return Vec2(ORIGIN.x + SIZE.width*221/1218,
                        ORIGIN.y + SIZE.height*406/716);
        case 1:   
            return Vec2(ORIGIN.x + SIZE.width*509/1218,
                        ORIGIN.y + SIZE.height*154/716);
        case 2:   
            return Vec2(ORIGIN.x + SIZE.width*936/1218 - xOffset,
                        ORIGIN.y + SIZE.height*406/716);
                
    }
}

Vec2 GameLayout::DestOfGold(int dir,int xOffset) {
    switch(dir) {
        case 0:   
            return Vec2(ORIGIN.x + SIZE.width*221/1218,
                        ORIGIN.y + SIZE.height*480/716);
        case 1:   
            return Vec2(ORIGIN.x + SIZE.width*509/1218,
                        ORIGIN.y + SIZE.height*228/716);
        case 2:   
            return Vec2(ORIGIN.x + SIZE.width*936/1218 - xOffset,
                        ORIGIN.y + SIZE.height*480/716);
    }
}

/*************************************
    out card
*************************************/
int  GameLayout::RotateAngleOfOutcard(int dir) {
    switch(dir) {
        case 0:
            return 90;
        case 1:
            return 0;
        case 2:
            return 270;
        default:
            LOGGER_WRITE("%s Error ! invalid player");
    }
}

Vec2 GameLayout::AnchorOfOutcard(int dir) {
    switch(dir) {
        case 0:
            return Vec2(0,0.5);
        case 2:
            return Vec2(1,0.5);
        default:
            LOGGER_WRITE("%s Error ! invalid player");
    }
}

Vec2 GameLayout::PositionOfOutcard(int dir) {
    switch(dir) {
        case 0:
            return Vec2(ORIGIN.x + SIZE.width*0.18, ORIGIN.y + SIZE.height*0.6);
        case 2:
            return Vec2(ORIGIN.x + SIZE.width*0.82, ORIGIN.y + SIZE.height*0.6);
        default:
            LOGGER_WRITE("%s Error ! invalid player");
    }
}

/*************************************
    action
*************************************/
Vec2 GameLayout::Center() {
    return Vec2(ORIGIN.x+SIZE.width/2, ORIGIN.y+SIZE.height/2);
}


Vec2 GameLayout::PositionOfMingSign() {
    return Vec2(ORIGIN.x+SIZE.width/2, ORIGIN.y+SIZE.height/2+50);
}

Vec2 GameLayout::PositionOfMingSignForMe(float zhuangWidth) {
    return Vec2(ORIGIN.x+SIZE.width*255/1218+zhuangWidth, ORIGIN.y+SIZE.height*168/716);
}

Vec2 GameLayout::PositionOfActSign(int dir) {
    switch(dir) {
        case 0:
            return Vec2(ORIGIN.x+SIZE.width*0.206,ORIGIN.y+SIZE.height*0.6);
        case 1:
            return Vec2(ORIGIN.x+SIZE.width/2,ORIGIN.y+SIZE.height/2);
        case 2:
            return Vec2(ORIGIN.x+SIZE.width*0.79,ORIGIN.y+SIZE.height*0.6);
        default:
            LOGGER_WRITE("%s Error ! invalid player");
    }
}

Vec2 GameLayout::OrigPositionOfGangCard(int i,Size cardSize) {
    switch(i+1) {
        case 1: return Vec2(
            ORIGIN.x + SIZE.width*0.5 + cardSize.width*0.98,
            ORIGIN.y + SIZE.height*0.255);
        case 2: return Vec2(
            ORIGIN.x + SIZE.width*0.5 - cardSize.width*1.96,
            ORIGIN.y + SIZE.height*0.255);
        case 3: return Vec2(
            ORIGIN.x + SIZE.width*0.5 - cardSize.width*0.98,
            ORIGIN.y + SIZE.height*0.255);
        case 4: return Vec2(
            ORIGIN.x + SIZE.width*0.5,
            ORIGIN.y + SIZE.height*0.255);
    }
}

Vec2 GameLayout::MiddlePositionOfGangCard(int i,Size cardSize) {
    switch(i+1) {
        case 1: return Vec2(
                ORIGIN.x + SIZE.width*0.5  +  cardSize.width*0.98  +  150,
                ORIGIN.y + SIZE.height*0.255);
        case 2: return Vec2(
			    ORIGIN.x + SIZE.width*0.5 - cardSize.width*1.96 - 150,
			    ORIGIN.y + SIZE.height*0.255);
        case 3: return Vec2(
			    ORIGIN.x + SIZE.width*0.5 - cardSize.width*0.98 - 150,
			    ORIGIN.y + SIZE.height*0.255);
        case 4: return Vec2(
			    ORIGIN.x + SIZE.width*0.5 - 150,
			    ORIGIN.y + SIZE.height*0.255);
    }
}

Vec2 GameLayout::DestPositionOfGangCard(int i,Size cardSize) {
    switch(i+1) {
        case 1: return Vec2(
                ORIGIN.x + SIZE.width*0.5  +  cardSize.width*0.98,
                ORIGIN.y + SIZE.height*0.255);
        case 2: return Vec2(
    			ORIGIN.x + SIZE.width*0.5 - cardSize.width*1.96,
    			ORIGIN.y + SIZE.height*0.255);
        case 3: return Vec2(
			    ORIGIN.x + SIZE.width*0.5 - cardSize.width*0.98,
			    ORIGIN.y + SIZE.height*0.255);
        case 4: return Vec2(
			    ORIGIN.x + SIZE.width*0.5,
			    ORIGIN.y + SIZE.height*0.255);
    }
}

Vec2 GameLayout::DestPositionOfGangCardInHand(int i,Size gangCard) {
    switch(i+1) {
        case 1: return Vec2(
            ORIGIN.x+SIZE.width*0.5 - gangCard.width*1.96,
            ORIGIN.y+SIZE.height*0.26);
        case 2: return Vec2(
            ORIGIN.x+SIZE.width*0.5 - gangCard.width*0.98,
            ORIGIN.y+SIZE.height*0.26);
        case 3: return Vec2(
            ORIGIN.x+SIZE.width*0.5,
            ORIGIN.y+SIZE.height*0.26);
        case 4: return Vec2(
            ORIGIN.x+SIZE.width*0.5 + gangCard.width*0.98,
            ORIGIN.y+SIZE.height*0.255);
    }
}

Vec2 GameLayout::PositionOfMingGangCard(int i,Size cardSize) {
    switch(i+1) {
        case 1: return Vec2(
            ORIGIN.x + SIZE.width*0.5 + cardSize.width*0.98,
            ORIGIN.y + SIZE.height*0.26);
        case 2: return Vec2(
            ORIGIN.x + SIZE.width*0.5 - cardSize.width*1.96,
            ORIGIN.y + SIZE.height*0.26);
        case 3: return Vec2(
            ORIGIN.x + SIZE.width*0.5 - cardSize.width*0.98,
            ORIGIN.y + SIZE.height*0.26);
        case 4: return Vec2(
            ORIGIN.x + SIZE.width*0.5,
            ORIGIN.y + SIZE.height*0.26);
    }
}

Vec2 GameLayout::MiddlePositionOfGangCardInHand(int i,Vec2 origPos,Size freeCard) {
    switch(i+1) {
        case 1: return Vec2(origPos.x,origPos.y);
        case 2: return Vec2(origPos.x - freeCard.width*0.4, origPos.y);
        case 3: return Vec2(origPos.x - freeCard.width*0.8, origPos.y);
        case 4: return Vec2(origPos.x,origPos.y);
    }
}

Vec2 GameLayout::PositionOfTingSignBar(PlayerDir_t dir) {
    switch(dir) {
        case LEFT:
            return Vec2(ORIGIN.x+SIZE.width*266/1218, ORIGIN.y+SIZE.height*0.65);//0.69 0.23
        case RIGHT:
            return Vec2(ORIGIN.x+SIZE.width*952/1218, ORIGIN.y+SIZE.height*0.62);//0.77 0.66
    }
}

/*************************************
    card in hand
*************************************/
Vec2 GameLayout::AnchorOfNormalCard(PlayerDir_t dir) {
    switch(dir) {
        case LEFT:
            return Vec2(0.3f,1.0f);
        case MIDDLE:
            return Vec2(0,0);
        case RIGHT:
            return Vec2(0.3f,0);
    }
}

Vec2 GameLayout::PositionOfNormalCard(PlayerDir_t dir,int x,int y) {
    switch(dir) {
        case LEFT:
            return Vec2(x-10,y-8);
        case MIDDLE:
            return Vec2(x,y);
        case RIGHT:
            return Vec2(x-10,y-8);
    }
}

Vec2 GameLayout::AnchorOfFreeCard(PlayerDir_t dir) {
    switch(dir) {
        case LEFT:
            return Vec2(0.0f,1.0f);
        case RIGHT:
            return Vec2(0.0f,0);
    }
}

int  GameLayout::RotateAngleOfCard(PlayerDir_t dir) {
    switch(dir) {
        case LEFT:
            return 90;
        case MIDDLE:
            return 0;
        case RIGHT:
            return -90;
    }
}

float GameLayout::YOfNextCard(PlayerDir_t dir,float originY,Size cardSize) {
    switch(dir) {
        case LEFT:
            return originY - cardSize.height*0.65;
        case RIGHT:
            return originY + cardSize.height*0.65;
    }
}

/*************************************
    card in river
*************************************/
int GameLayout::_cardsInLine(int line) {
    switch(line) {
        case 0:   return RIVER_CARD_IN_LINE_1;
        case 1:   return RIVER_CARD_IN_LINE_2;
        case 2:   return 10;
    }
}

int GameLayout::_lineIdx(int i) {
    if(i<RIVER_CARD_IN_LINE_1) {
        return i;
    } else if (i<RIVER_CARD_IN_LINE_1+RIVER_CARD_IN_LINE_2) {
        return i-RIVER_CARD_IN_LINE_1;
    } else {
        return i-RIVER_CARD_IN_LINE_1+RIVER_CARD_IN_LINE_2;
    }
}

int GameLayout::_lineNo(int i) {
    if(i<RIVER_CARD_IN_LINE_1) {
        return 0;
    } else if (i<RIVER_CARD_IN_LINE_1+RIVER_CARD_IN_LINE_2) {
        return 1;
    } else {
        return 2;
    }
}

int GameLayout::_viewIdx(int i) {

    return _lineIdx(i)-_lineNo(i);
}

Vec2 GameLayout::OrigPositionOfRiverCard(PlayerDir_t dir,int idx) {
    int lineNo  = _lineNo(idx);
    
    switch(dir) {
        case LEFT:
            return Vec2(_playerPosi[dir].riverPoint.x-48*lineNo,
                        _playerPosi[dir].riverPoint.y-30*(_viewIdx(idx)-1));
        case MIDDLE:
            return Vec2(_playerPosi[dir].riverPoint.x+((_viewIdx(idx))*36)+20,
                        _playerPosi[dir].riverPoint.y-41*lineNo+5);
        case RIGHT:
            return Vec2(_playerPosi[dir].riverPoint.x+48*lineNo,
                        _playerPosi[dir].riverPoint.y+30*(_viewIdx(idx)+1));
    }
}

Vec2 GameLayout::Middle1PositionOfRiverCard(PlayerDir_t dir,int idx) {
    int lineNo  = _lineNo(idx);
    
    switch(dir) {
        case LEFT:
            return Vec2(_playerPosi[dir].riverPoint.x-48*lineNo,
                        _playerPosi[dir].riverPoint.y-30*(_viewIdx(idx)-1)+10);
        case MIDDLE:
            return Vec2(_playerPosi[dir].riverPoint.x+((_viewIdx(idx))*36)+20,
                        _playerPosi[dir].riverPoint.y-41*lineNo+15);
        case RIGHT:
            return Vec2(_playerPosi[dir].riverPoint.x+48*lineNo,
                        _playerPosi[dir].riverPoint.y+30*(_viewIdx(idx)+1)+10);
    }
}

Vec2 GameLayout::Middle2PositionOfRiverCard(PlayerDir_t dir,int idx) {
    int lineNo  = _lineNo(idx);
    
    switch(dir) {
        case LEFT:
            return Vec2(_playerPosi[dir].riverPoint.x-48*lineNo,
                        _playerPosi[dir].riverPoint.y-30*(_viewIdx(idx)-1));
        case MIDDLE:
            return Vec2(_playerPosi[dir].riverPoint.x+((_viewIdx(idx))*36)+20,
                        _playerPosi[dir].riverPoint.y-41*lineNo+5);
        case RIGHT:
            return Vec2(_playerPosi[dir].riverPoint.x+48*lineNo,
                        _playerPosi[dir].riverPoint.y+30*(_viewIdx(idx)+1));
    }
}

Vec2 GameLayout::DestPositionOfRiverCard(PlayerDir_t dir,int idx) {
    int lineNo  = _lineNo(idx);
    
    switch(dir) {
        case LEFT:
            return Vec2(_playerPosi[dir].riverPoint.x-48*lineNo,
                        _playerPosi[dir].riverPoint.y-30*(_viewIdx(idx)));
        case MIDDLE:
            return Vec2(_playerPosi[dir].riverPoint.x+(_viewIdx(idx))*36,
                        _playerPosi[dir].riverPoint.y-41*lineNo);
        case RIGHT:
            return Vec2(_playerPosi[dir].riverPoint.x+48*lineNo,
                        _playerPosi[dir].riverPoint.y+30*(_viewIdx(idx)));
    }
}

void GameLayout::GetBizerPoints(ccBezierConfig &cfg, int riverIdx, const Vec2 &location) {
    Vec2 ctrlPoint1Offset = Vec2(0,0);
    Vec2 ctrlPoint2Offset = Vec2(0,0);

    int lineNo  = _lineNo(riverIdx);
    
	if(lineNo==0) {
		if(location.y<=SIZE.height*0.4) {
			if(location.x<SIZE.width*0.4) {
			    ctrlPoint1Offset = Vec2(30,30);
                ctrlPoint2Offset = Vec2(100,-100);
			} else if(location.x>=SIZE.width*0.6) {
			    ctrlPoint1Offset = Vec2(-30,-30);
                ctrlPoint2Offset = Vec2(100,-100);
			}
		} else if(location.y>=SIZE.height*0.4) {
			if(location.x<SIZE.width*0.4) {
			    ctrlPoint1Offset = Vec2(100,-100);
                ctrlPoint2Offset = Vec2(100,100+82*lineNo);
			} else if(location.x>=SIZE.width*0.6) {
			    ctrlPoint1Offset = Vec2(-100,-100);
                ctrlPoint2Offset = Vec2(100,100+82*lineNo);
			}
		}

        cfg.endPosition = Vec2(
            _playerPosi[1].riverPoint.x+((_viewIdx(riverIdx))*36),
            _playerPosi[1].riverPoint.y);
        cfg.controlPoint_1 = Vec2(location.x+ctrlPoint1Offset.x,location.y+ctrlPoint1Offset.y);
        cfg.controlPoint_2 = Vec2(
            _playerPosi[1].riverPoint.x+((_viewIdx(riverIdx))*36)+ctrlPoint2Offset.x,
            _playerPosi[1].riverPoint.y+ctrlPoint2Offset.y);
	}
	else if(lineNo==1)
	{
		if(location.y<=SIZE.height*0.4)
		{
			if(location.x<SIZE.width*0.4) {
			    ctrlPoint1Offset = Vec2(30,30);
                ctrlPoint2Offset = Vec2(-100,-100);
			} else if(location.x>=SIZE.width*0.6) {
			    ctrlPoint1Offset = Vec2(-30,-30);
                ctrlPoint2Offset = Vec2(100,-100);
			}
		}
		else if(location.y>=SIZE.height*0.4)
		{
			if(location.x<SIZE.width*0.4) {
			    ctrlPoint1Offset = Vec2(30,-30);
                ctrlPoint2Offset = Vec2(-100,100+82*lineNo);
			} else if(location.x>=SIZE.width*0.6) {
			    ctrlPoint1Offset = Vec2(-30,-30);
                ctrlPoint2Offset = Vec2(100,100+82*lineNo);
			}
		}
        
        cfg.endPosition = Vec2(
            _playerPosi[1].riverPoint.x+((_viewIdx(riverIdx))*36),
            _playerPosi[1].riverPoint.y-41*lineNo);
        cfg.controlPoint_1 = Vec2(location.x+ctrlPoint1Offset.x,location.y+ctrlPoint1Offset.y);
        cfg.controlPoint_2 = Vec2(
            _playerPosi[1].riverPoint.x+((_viewIdx(riverIdx))*36)+ctrlPoint2Offset.x,
            _playerPosi[1].riverPoint.y-41*lineNo+ctrlPoint2Offset.y);
	}
	else
	{
		if(location.y<=SIZE.height*0.4)
		{
			if(location.x<SIZE.width*0.4) {
			    ctrlPoint1Offset = Vec2(30,30);
                ctrlPoint2Offset = Vec2(-100,-100);
			} else if(location.x>=SIZE.width*0.6) {
			    ctrlPoint1Offset = Vec2(-30,-30);
                ctrlPoint2Offset = Vec2(100,-100);
			}
		}
		else if(location.y>=SIZE.height*0.4)
		{
			if(location.x<SIZE.width*0.4) {
			    ctrlPoint1Offset = Vec2(30,-30);
                ctrlPoint2Offset = Vec2(-100,100+82*lineNo);
			} else if(location.x>=SIZE.width*0.6) {
			    ctrlPoint1Offset = Vec2(-30,-30);
                ctrlPoint2Offset = Vec2(100,100+82*lineNo);
			}
		}
        
        cfg.endPosition = Vec2(
            _playerPosi[1].riverPoint.x+_viewIdx(riverIdx)*36,
            _playerPosi[1].riverPoint.y-41*lineNo);
        cfg.controlPoint_1 = Vec2(location.x+ctrlPoint1Offset.x,location.y+ctrlPoint1Offset.y);
        cfg.controlPoint_2 = Vec2(
            _playerPosi[1].riverPoint.x+((_viewIdx(riverIdx)+1)*36)+ctrlPoint2Offset.x,
            _playerPosi[1].riverPoint.y-41*lineNo+ctrlPoint2Offset.y);
	}
}

/*************************************
    singleton
*************************************/
GameLayout *GameLayout::_instance = NULL;

GameLayout *GameLayout::getInstance(cocos2d::Point origin, cocos2d::Size size) {
    if (_instance==NULL) {
        _instance = new GameLayout(origin,size);
    }

    return _instance;
}

void GameLayout::destroyInstance() {
    delete _instance;
    _instance = NULL;
}


