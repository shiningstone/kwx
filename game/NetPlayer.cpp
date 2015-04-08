
#include "NetPlayer.h"

#include "./../utils/LogManager.h"

USING_NS_CC;

NetPlayer::NetPlayer() {
    _logger = LOGGER_REGISTER("NetPlayer");
}

NetPlayer::NetPlayer(int id) {//this is for default settings ( robot ) 
    _logger = LOGGER_REGISTER("NetPlayer_%d");
}

NetPlayer::~NetPlayer() {
    LOGGER_DEREGISTER(_logger);
}

int NetPlayer::Robot_check_pickup_card(CARD_KIND kind,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
{
	int j,k;
	for(j=0;j<len1;j++)
		if(list1[j]==kind)
			break;
	for(k=0;k<len2;k++)
		if(list2[k]==kind)
			break;
	if(j==len1&&k==len2)
		return 0;
	
	return -1;
}

int NetPlayer::river_reserved_card(HAH *hash_table,int card)
{
	int num=0;
	for(int i=0;i<hash_table->river_len;i++)
		if(hash_table->card_in_river[i]==card)
		{
			num++;			
		}
	if(hash_table->reserved_card_num<10&&num==0)
		num=3;
	return (4-num);
}

int NetPlayer::Robot_check_card_stable(HAH *card_array,CARD_KIND card)
{
	if(card_array->list[card].same_times==1||card_array->list[card].same_times==2)
	{
		if(card%9==0)
		{
			if(card_array->list[card+1].same_times==card_array->list[card].same_times
			&&card_array->list[card+2].same_times==card_array->list[card].same_times)
				return 0;
		}
		else if(card%9==8)
		{
			if(card_array->list[card-1].same_times==card_array->list[card].same_times&&
			card_array->list[card-2].same_times==card_array->list[card].same_times)
				return 0;
		}
		else if(card_array->list[card+1].same_times==card_array->list[card].same_times
			&&card_array->list[card+2].same_times==card_array->list[card].same_times||
			card_array->list[card-1].same_times==card_array->list[card].same_times&&
			card_array->list[card-2].same_times==card_array->list[card].same_times||
			card_array->list[card-1].same_times==card_array->list[card].same_times&&
			card_array->list[card+1].same_times==card_array->list[card].same_times)
				return 0;
		else
		{
			if(card_array->list[card+1].same_times==card_array->list[card].same_times
			&&card_array->list[card+2].same_times!=card_array->list[card].same_times
			||card_array->list[card].same_times==card_array->list[card+2].same_times
			&&card_array->list[card+1].same_times!=card_array->list[card].same_times
			||card_array->list[card+1].same_times==card_array->list[card].same_times
			&&card_array->list[card-1].same_times!=card_array->list[card].same_times)
				return 0;
		}
	}
	else if(card_array->list[card].same_times==3||card_array->list[card].same_times==4)
		return 0;
	return -1;
}

int NetPlayer::Robot_picup_single_for_samecolor(int color,HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
{
	int chose_place=-1;
	int offset=color*9;
	int index1=card_array->river_len-1;
	int index2=index1-1;
	CARD_KIND kind1=card_array->card_in_river[index1];
	CARD_KIND kind2=card_array->card_in_river[index2];
	HA card1=card_array->list[kind1];
	HA card2=card_array->list[kind2];

	if(color!=2)
	{
		if(len1==0&&len2==0&&card_array->reserved_card_num>38)
		{
			for(int i=offset;i<offset+9;i++)
				if(Robot_check_card_stable(card_array,(CARD_KIND)i)!=0)
				{
					if(card_array->list[i].same_times!=0)
					{
						chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
						break;
					}
				}
		}
		if(len1!=0||len2!=0)
		{
			if(Robot_check_pickup_card((CARD_KIND)(kind1),list1,list2,len1,len2)==0||Robot_check_pickup_card((CARD_KIND)(kind2),list1,list2,len1,len2)==0)
			{
				if(kind2/9==color&&card1.same_times!=0&&Robot_check_card_stable(card_array,kind1)!=0)
				{
					chose_place=card_array->list[kind1].place[card1.same_times-1];
				}
				else if(kind2/9==color&&card2.same_times!=0&&Robot_check_card_stable(card_array,kind2)!=0)
				{
					chose_place=card_array->list[kind2].place[card2.same_times-1];
				}
			}
		}
		if(chose_place==-1)
		{
			for(int i=18;i<21;i++)
				if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0&&card_array->list[i].same_times==1)
				{
					chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
					break;
				}
		}
		if(chose_place==-1)
		{
			if(Robot_check_pickup_card((CARD_KIND)(offset),list1,list2,len1,len2)==0||Robot_check_pickup_card((CARD_KIND)(offset+8),list1,list2,len1,len2)==0)
			{
				if(card_array->list[offset].same_times==1&&card_array->list[offset+1].same_times==0)
					chose_place=card_array->list[offset].place[card_array->list[offset].same_times-1];
				else if(card_array->list[offset+8].same_times==1&&card_array->list[offset+7].same_times==0)
					chose_place=card_array->list[offset+8].place[card_array->list[offset+8].same_times-1];
			}
			else
			{	
				for(int i=offset+1;i<8+offset;i++)
				{
					if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0)
						if(card_array->list[i].same_times==1&&card_array->list[i-1].same_times==0
							&&card_array->list[i+1].same_times==0)
						{
							chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
							break;
						}			
				}
			}
			if(chose_place==-1)
			{
				if(Robot_check_pickup_card((CARD_KIND)(offset),list1,list2,len1,len2)==0||Robot_check_pickup_card((CARD_KIND)(offset+8),list1,list2,len1,len2)==0)
				{
					if(card_array->list[offset].same_times==1&&Robot_check_card_stable(card_array,(CARD_KIND)offset)!=0)
						chose_place=card_array->list[offset].place[card_array->list[offset].same_times-1];
					else if(card_array->list[offset+8].same_times==1&&Robot_check_card_stable(card_array,(CARD_KIND)(offset+8))!=0)
						chose_place=card_array->list[offset+8].place[card_array->list[offset+8].same_times-1];
				}
			}
			if(chose_place==-1)
			{
				for(int i=offset+1;i<8+offset;i++)
				{
					if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0)
						if(card_array->list[i].same_times==1&&Robot_check_card_stable(card_array,(CARD_KIND)i)!=0)
						{
								chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
								break;
						}
				}
			}
			if(chose_place==-1)
			{
				for(int i=offset;i<offset+9;i++)
					if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
						&&card_array->list[i].same_times!=0&&Robot_check_card_stable(card_array,(CARD_KIND)i)!=0)
						{
							chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
							break;
						}
			}
			if(chose_place==-1)
			{
				for(int i=18;i<21;i++)
					if(card_array->list[i].same_times!=0&&Robot_check_card_stable(card_array,(CARD_KIND)i)!=0&&
						Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0)
					{
						chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
						break;
					}
			}
			if(chose_place==-1)
			{
				for(int i=offset;i<offset+9;i++)
					if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
						&&card_array->list[i].same_times!=0)
						{
							chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
							break;
						}
			}
			if(chose_place==-1)
			{
				for(int i=18;i<21;i++)
					if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
						&&card_array->list[i].same_times!=0)
					{
						chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
						break;
					}
			}
			if(chose_place==-1)
			{
				offset=(1-color)*9;
				if(Robot_check_pickup_card((CARD_KIND)(offset),list1,list2,len1,len2)==0
					&&card_array->list[offset].same_times==1&&card_array->list[1+offset].same_times==0)
						chose_place=card_array->list[offset].place[card_array->list[offset].same_times-1];
				else if(Robot_check_pickup_card((CARD_KIND)(offset+8),list1,list2,len1,len2)==0
					&&card_array->list[offset+8].same_times==1&&card_array->list[offset+7].same_times==0)
						chose_place=card_array->list[offset+8].place[card_array->list[offset+8].same_times-1];
				else
				{	
					for(int i=offset+1;i<8+offset;i++)
					{
						if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0)
							if(card_array->list[i].same_times==1&&card_array->list[i-1].same_times==0
								&&card_array->list[i+1].same_times==0)
							{
									chose_place=card_array->list[i].place[card_array->list[i].same_times-1];;
									break;
							}			
					}
				}
				if(chose_place==-1)
				{
						if(Robot_check_pickup_card((CARD_KIND)(offset),list1,list2,len1,len2)==0&&
							card_array->list[offset].same_times==1)
								chose_place=card_array->list[offset].place[card_array->list[offset].same_times-1];
						else if(Robot_check_pickup_card((CARD_KIND)(offset+8),list1,list2,len1,len2)==0&&
							card_array->list[offset+8].same_times==1)
								chose_place=card_array->list[offset+8].place[card_array->list[offset+8].same_times-1];
				}
				if(chose_place==-1)
				{
					for(int j=offset;j<offset+9;j++)
						if(Robot_check_pickup_card(CARD_KIND(j),list1,list2,len1,len2)==0&&
							card_array->list[j].same_times!=0&&Robot_check_card_stable(card_array,CARD_KIND(j))!=0)
						{	
							chose_place=card_array->list[j].place[card_array->list[j].same_times-1];
							break;
						}
				}
				if(chose_place==-1)
				{
					for(int j=offset+1;j<offset+8;j++)
						if(Robot_check_pickup_card(CARD_KIND(j),list1,list2,len1,len2)==0&&
							(card_array->list[j].same_times==1&&card_array->list[j-1].same_times==0)||
							(card_array->list[j].same_times==1&&card_array->list[j+1].same_times==0))
						{	
							chose_place=card_array->list[j].place[card_array->list[j].same_times-1];
							break;
						}
				}
				if(chose_place==-1)
				{
					for(int j=offset;j<offset+9;j++)
						if(Robot_check_pickup_card(CARD_KIND(j),list1,list2,len1,len2)==0&&
							card_array->list[j].same_times!=3&&card_array->list[j].same_times!=4)
						{	
							chose_place=card_array->list[j].place[card_array->list[j].same_times-1];
							break;
						}
				}
			}
		}
	}
	return chose_place;
}

int NetPlayer::Robot_pickup_for_sevencouples(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
{	
	int chose_place=-1;

	int index1=card_array->river_len-1;
	int index2=index1-1;
	CARD_KIND kind1=card_array->card_in_river[index1];
	CARD_KIND kind2=card_array->card_in_river[index2];
	HA card1=card_array->list[kind1];
	HA card2=card_array->list[kind2];

	if(len1!=0||len2!=0)
	{
		if(Robot_check_pickup_card((CARD_KIND)(kind1),list1,list2,len1,len2)==0&&
			card1.same_times!=2&&card1.same_times!=4)
		{
			chose_place=card_array->list[kind1].place[card1.same_times-1];
		}
		else if(Robot_check_card_stable(card_array,kind2)!=0&&
			card2.same_times!=2&&card2.same_times!=4)
		{
			chose_place=card_array->list[kind2].place[card2.same_times-1];
		}
	}
	if(chose_place==-1)
	{
		for(int i=18;i<21;i++)
			if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0&&card_array->list[i].same_times==1)
			{
				chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
				break;
			}
	}
	if(chose_place==-1)
	{
		for(int i=0;i<TOTAL_CARD_KIND;i++)
			if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
				&&card_array->list[i].same_times==3)
				{
					chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
					break;
				}
	}
	if(chose_place==-1)
	{
		for(int i=0;i<TOTAL_CARD_KIND;i++)
			if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0&&
				card_array->list[i].same_times==1&&river_reserved_card(card_array,i)==1)
				{
					chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
					break;
				}
	}
	if(chose_place==-1)
	{
		for(int i=0;i<TOTAL_CARD_KIND;i++)
			if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
				&&card_array->list[i].same_times==1&&river_reserved_card(card_array,i)==2)
				{
					chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
					break;
				}
	}
	if(chose_place==-1)
	{
		for(int i=0;i<TOTAL_CARD_KIND;i++)
			if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0&&
				card_array->list[i].same_times==1&&river_reserved_card(card_array,i)==3)
				{
					chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
					break;
				}
	}
	if(chose_place==-1)
	{
		for(int i=0;i<TOTAL_CARD_KIND;i++)
			if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
				&&card_array->list[i].same_times==1&&river_reserved_card(card_array,i)==4)
				{
					chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
					break;
				}
	}
	return chose_place;
}

int NetPlayer::Robot_pickup_for_fourpeng(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
{
	int chose_place=-1;
	
	int index1=card_array->river_len-1;
	int index2=index1-1;
	CARD_KIND kind1=card_array->card_in_river[index1];
	CARD_KIND kind2=card_array->card_in_river[index2];
	HA card1=card_array->list[kind1];
	HA card2=card_array->list[kind2];

	if(len1!=0||len2!=0)
	{
		if(Robot_check_pickup_card((CARD_KIND)(kind1),list1,list2,len1,len2)==0
			&&card1.same_times==1&&Robot_check_card_stable(card_array,kind1)!=0)
		{
			chose_place=card_array->list[kind1].place[card1.same_times-1];
		}
		else if(card2.same_times==1&&Robot_check_card_stable(card_array,kind2)!=0
			&&Robot_check_pickup_card((CARD_KIND)(kind2),list1,list2,len1,len2)==0)
		{
			chose_place=card_array->list[kind2].place[card2.same_times-1];
		}
	}
	if(chose_place==-1)
	{
		for(int i=18;i<21;i++)
			if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0&&card_array->list[i].same_times==1)
			{
				chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
				break;
			}
	}
	if(chose_place==-1)
	{
		for(int i=0;i<TOTAL_CARD_KIND;i++)
			{
				if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
					&&card_array->list[i].same_times==1&&river_reserved_card(card_array,i)==1)
				{
					chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
					break;
				}
			}
		if(chose_place==-1)
		{
			for(int i=0;i<TOTAL_CARD_KIND;i++)
			{
				if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
					&&card_array->list[i].same_times==2&&river_reserved_card(card_array,i)==2)
				{
					chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
					break;
				}
			}
		}
		if(chose_place==-1)
		{
			for(int i=0;i<TOTAL_CARD_KIND;i++)
			{
				if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
					&&card_array->list[i].same_times==1&&river_reserved_card(card_array,i)==2)
				{
					chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
					break;
				}
			}
		}
		if(chose_place==-1)
		{
			for(int i=0;i<TOTAL_CARD_KIND;i++)
			{
				if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
					&&card_array->list[i].same_times==2&&river_reserved_card(card_array,i)==3)
				{
					chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
					break;
				}
			}	
		}
		if(chose_place==-1)
		{
			for(int i=0;i<TOTAL_CARD_KIND;i++)
			{
				if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0&&
					card_array->list[i].same_times==1&&river_reserved_card(card_array,i)==3)
				{
					chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
					break;
				}
			}
		}
		if(chose_place==-1)
		{
			for(int i=0;i<TOTAL_CARD_KIND;i++)
			{
				if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0&&
					card_array->list[i].same_times==1&&river_reserved_card(card_array,i)==4)
				{
					chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
					break;
				}
			}
		}
		if(chose_place==-1)
		{
			for(int i=0;i<TOTAL_CARD_KIND;i++)
			{
				if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
					&&card_array->list[i].same_times==2&&river_reserved_card(card_array,i)==4)
				{
					chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
					break;
				}
			}
		}
	}
	return chose_place;
}

int NetPlayer::Robot_pickup_for_lowwin(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
{
	int chose_place=-1;
	int index1=card_array->river_len-1;
	int index2=index1-1;
	CARD_KIND kind1=card_array->card_in_river[index1];
	CARD_KIND kind2=card_array->card_in_river[index2];
	HA card1=card_array->list[kind1];
	HA card2=card_array->list[kind2];

	if(len1!=0||len2!=0)
	{
		if(Robot_check_pickup_card((CARD_KIND)(kind1),list1,list2,len1,len2)==0
			&&card1.same_times==1&&Robot_check_card_stable(card_array,kind1)!=0)
		{
			chose_place=card_array->list[kind1].place[card1.same_times-1];
		}
		else if(card2.same_times==1&&Robot_check_card_stable(card_array,kind2)!=0
			&&Robot_check_pickup_card((CARD_KIND)(kind2),list1,list2,len1,len2)==0)
		{
			chose_place=card_array->list[kind2].place[card2.same_times-1];
		}
	}
	if(chose_place==-1)
	{
		for(int i=18;i<21;i++)
			if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0&&card_array->list[i].same_times==1)
			{
				chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
				break;
			}
	}
	if(chose_place==-1)
	{		
		for(int i=0;i<TOTAL_CARD_KIND;i++)
		{
				if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0&&
					card_array->list[i].same_times==1&&river_reserved_card(card_array,i)!=4&&
					Robot_check_card_stable(card_array,CARD_KIND(i))!=0)
				{
					chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
					break;
				}
		}
	}
	if(chose_place==-1)
	{
		for(int i=0;i<TOTAL_CARD_KIND;i++)
		{
				if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
					&&card_array->list[i].same_times==1&&river_reserved_card(card_array,i)==4&&
					Robot_check_card_stable(card_array,card_kind(i))!=0)
				{
					chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
					break;
				}
		}
	}
	if(chose_place==-1)
	{
		for(int i=0;i<TOTAL_CARD_KIND;i++)
		{
				if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
					&&card_array->list[i].same_times==2&&river_reserved_card(card_array,i)!=4&&
					Robot_check_card_stable(card_array,CARD_KIND(i))!=0)
				{
					chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
					break;
				}
		}
	}
	if(chose_place==-1)
	{
		for(int i=0;i<TOTAL_CARD_KIND;i++)
		{
				if(Robot_check_pickup_card((CARD_KIND)(i),list1,list2,len1,len2)==0
					&&card_array->list[i].same_times==2&&river_reserved_card(card_array,i)==4&&
					Robot_check_card_stable(card_array,CARD_KIND(i))!=0)
				{
					chose_place=card_array->list[i].place[card_array->list[i].same_times-1];
					break;
				}
		}
	}
	return chose_place;
}
int NetPlayer::Robot_pickup_single(HAH *card_array,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
{
	int chose_place=-1;
	
	switch(card_array->target)
	{
		case SAME_TIAO_TARGET:
			chose_place=Robot_picup_single_for_samecolor(1,card_array,list1,list2,len1,len2);
			break;
		case SAME_TONG_TARGET:
			chose_place=Robot_picup_single_for_samecolor(0,card_array,list1,list2,len1,len2);
			break;
		case SEVEN_COUPLES_TARGET:
			chose_place=Robot_pickup_for_sevencouples(card_array,list1,list2,len1,len2);
			break;
		case FOUR_PENG_TARGET:
			chose_place=Robot_pickup_for_fourpeng(card_array,list1,list2,len1,len2);
			break;
		default:
			chose_place=Robot_pickup_for_lowwin(card_array,list1,list2,len1,len2);
			break;		
	}
	return chose_place;
}

void NetPlayer::set_robot_hu_target(ROBOT_TARGET par_target)
{
	g_target=par_target;
}

ROBOT_TARGET NetPlayer::get_robot_hu_target()
{
	return g_target;
}

int NetPlayer::chose_card(HAH *pres,int reseved,CARD_KIND list1[],CARD_KIND list2[],int len1,int len2)
{
	//unsigned char ting_flag;
	CARD_ARRAY *list=_act->get_card_list();
	
	int show_place=-1;
	int hu_num=-1;
	MRES *res=new MRES;
	
	for(int i=0;i<MAX_HANDIN_NUM;i++)
		memset(res->hu_cards[i],ck_NOT_DEFINED,sizeof(CARD_KIND)*9);
	if( _act->get_ming_check_result(res) )
	{
		int s_k;
		int s_num;
		int ming_index=_act->get_ming_indexes();
		CARD_KIND s_kind;
		for(int k=0;k<MAX_HANDIN_NUM;k++)
		{
			if(ming_index&(1<<k))
			{
				s_kind=_act->get_card_list()->data[k].kind;
				if(Robot_check_pickup_card(s_kind,list1,list2,len1,len2)!=0)
				{
					res->hu_cards_num[k]=0;
					continue;
				}
				s_num=0;
				for(int j=0;j<9;j++)
					if(res->hu_cards[k][j]>=0)
					{
						s_num+=river_reserved_card(pres,res->hu_cards[k][j]);
						s_num -= pres->list[res->hu_cards[k][j]].same_times;
					}
				res->hu_cards_num[k]=s_num;
			}
			else
				res->hu_cards_num[k]=0;
		}
		hu_num=res->hu_cards_num[0];
		int l_place=-1;
		for(s_k=0;s_k<MAX_HANDIN_NUM;s_k++)
			if(ming_index&(1<<s_k))
			{
				s_kind=_act->get_card_list()->data[s_k].kind;
				if(res->hu_cards_num[s_k]>=hu_num&&Robot_check_pickup_card(s_kind,list1,list2,len1,len2)==0)
					l_place=s_k;
			}
		show_place=l_place;
		pres->hu_nums=res->hu_cards_num[l_place];
	}
	if(show_place==-1)
	{
		show_place=Robot_pickup_single(pres,list1,list2,len1,len2);
	}
	if(res)
		delete res;

	return show_place;
}

