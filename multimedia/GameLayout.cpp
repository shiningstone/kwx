
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


