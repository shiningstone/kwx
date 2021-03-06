#include "NetRaceRound.h"
USING_NS_CC;

NetRRound::NetRRound()
{
	out_card_list=NULL;
	card_list=NULL;
	hu_len=0;
	kind_hu=0;

    _logger = LOGGER_REGISTER("NetRaceRound");
}

NetRRound::~NetRRound()
{
	if(card_list)
		delete card_list;
	if(out_card_list)
		delete out_card_list;

    LOGGER_DEREGISTER(_logger);
}

int NetRRound::card_delete(unsigned int from,unsigned int len)
{
	int i;

	for(i=0;i<card_list->len-from-len;i++)
	{
		memcpy(&(card_list->data[i+from]),&(card_list->data[i+from+len]),sizeof(CARD));
	}
	for(i=0;i<len;i++)
	{
		card_list->data[card_list->len-1-i].kind=ck_NOT_DEFINED;
		card_list->data[card_list->len-1-i].status=c_NOT_DEFINDED;
		card_list->data[card_list->len-1-i].can_play=cps_NOT_DEFINDED;
	}
	card_list->len -= len;
	return 0;
}

void NetRRound::card_insert(CARD data,int times)
{
	int i;
	if(data.status!=c_FREE)
	{
		int insertPlace;
		if(InsertPlaceForMG!=-1)
		{
			insertPlace=InsertPlaceForMG;
		}
		else
		{
			for(i=card_list->atcvie_place;i>=0;i--)
			{
				if(i==0)
				{
					insertPlace=i;
					break;
				}
				else
				{
					if(data.status==c_MING_KOU)
					{
						if((card_list->data[i-1].status==c_MING_KOU&&data.kind>=card_list->data[i-1].kind)||card_list->data[i-1].status!=c_MING_KOU)
						{
							insertPlace=i;
							break;
						}
					}
					else if(card_list->data[i-1].status!=c_MING_KOU)
					{
						insertPlace=i;
						break;
					}
				}
			}
		}
		for(i=card_list->len-1;i>=insertPlace;i--)
		{
			memcpy(&(card_list->data[i+times]),&(card_list->data[i]),sizeof(CARD));
		}

		for(i=insertPlace;i<(insertPlace+times);i++)
		{
			memcpy(&(card_list->data[i]),&data,sizeof(CARD));
		}
	}
	else
	{
		int insert_place=0;
		if( card_list->len==0 )
		{
			for(int j=0;j<times;j++)
				memcpy(&card_list->data[j],&data,sizeof(CARD));
		}
		else
		{
			for(i=card_list->atcvie_place;i<card_list->len;i++)
				if( card_list->data[i].kind>=data.kind )
				{
					insert_place=i;
					break;
				}
			if(i==card_list->len)
			{
				for(int j=0;j<times;j++)
					memcpy(&(card_list->data[i+j]),&data,sizeof(CARD));
			}
			else
			{
				for(i=card_list->len-1;i>=insert_place;i--)
				{
					memcpy(&(card_list->data[i+times]),&(card_list->data[i]),sizeof(CARD));
				}
				for(int j=0;j<times;j++)
					memcpy(&(card_list->data[insert_place+j]),&data,sizeof(CARD));
			}
		}
	}
	card_list->len += times;
}

long NetRRound::cal_score(CARD_KIND kind,unsigned char who_give,bool is_last_one,unsigned char last_action_WithGold,unsigned int continue_gang_times,bool isGangHua)
{
	int i;
	unsigned char color;
	unsigned char l_four_flag=0;
	unsigned char color_flag=1;
	unsigned char si_peng_flag=0;
	unsigned char free_num=0;
	unsigned char couple_num=0;
	unsigned char zhong_num=0;
	unsigned char fa_num=0;
	unsigned char bai_num=0;
	unsigned char four_num=0;
	unsigned char last_card_same_num=0;
	unsigned char si_gui_flag = 0;
	unsigned int  hu_flag = 0;
	long score = 1;
	color=kind/9;

	for(i=0;i<card_list->len+1;i++)
	{
		int l_cards=1;
		if(	card_list->data[i].kind/9!=color )
		{
			color_flag = 0;
		}
		if(card_list->data[i].status==c_FREE )
		{
			free_num++;
		}
		if( card_list->data[i].kind==kind )
		{
			if(card_list->data[i].status==c_PENG)
				si_gui_flag=1;
			last_card_same_num++;
		}
		for(int k=i+1;k<card_list->len+1;k++)
			if(card_list->data[i].kind==card_list->data[k].kind &&
			 card_list->data[i].status==c_FREE &&
			 card_list->data[i].status == card_list->data[k].status)
				l_cards++;
		if(l_cards==2||l_cards==4)
			couple_num++;
		l_four_flag=1;
		for(int k=i+1;k<card_list->len+1;k++)
			if(card_list->data[i].kind==card_list->data[k].kind &&
			card_list->data[k].status==c_FREE)
				l_four_flag++;
		if(l_four_flag==4)
			four_num++;

		if(card_list->data[i].kind==ck_ZHONG)
		{
			zhong_num++;
		}
		else if(card_list->data[i].kind==ck_FA)
		{
			fa_num++;
		}
		else if(card_list->data[i].kind==ck_BAI)
		{
			bai_num++;
		}
	}
	if(free_num!=2)
	{
		int len=card_list->len+1-card_list->atcvie_place;
		int l_num;
		int l_couple=0;
		for(int k=card_list->atcvie_place;k<card_list->len+1;k++)
		{
			l_num=1;
			for(int t=k+1;t<card_list->len+1;t++)
				if(card_list->data[k].kind==card_list->data[t].kind)
					l_num++;
			if(l_num==3)
				len -= 3;
			l_num=0;
			for(int t=card_list->atcvie_place;t<card_list->len+1;t++)
				if(card_list->data[k].kind==card_list->data[t].kind)
					l_num++;
			if(l_num==2)
				l_couple++;
		}
		if(len==2&&l_couple==2)
			si_peng_flag=1;
	}
	else
	{
		score *=4;
		hu_flag|=RH_SHOUYIZHUA;//手抓一
	}
	if(is_last_one)
	{
		score *= 2;
		hu_flag|=RH_HAIDILAO;//海底捞
	}
	if(rr_ting_flag==1)
	{
		score *= 2;
		hu_flag|=RH_MING;//明牌
	}
	if(who_give==0)
	{
		score *= 2;
		hu_flag|=RH_ZIMO;//自摸
	}
	if(color_flag)
	{
		score *= 4;
		hu_flag |=RH_QINYISE;//清一色
	}
	if(si_peng_flag==1)
	{
		score *= 2;
		hu_flag |=RH_SIPENG;//碰碰胡
	}
	if((continue_gang_times!=0)&&(last_action_WithGold==a_MING_GANG||last_action_WithGold==a_AN_GANG||last_action_WithGold==a_QIANG_GANG))//杠胡
	{
		score *= 2;
		if(who_give==0&&last_action_WithGold==a_QIANG_GANG)
			hu_flag |= RH_QIANGGANG;
		else if(who_give==0&&isGangHua)
			hu_flag |= RH_GANGHUA;
		else if(who_give!=0)
			hu_flag |= RH_GANGPAO;
	}
	if(zhong_num>=3&&fa_num>=3&&bai_num>=3)//大三元
	{
		score *= 8;
		hu_flag |= RH_DASANYUAN;
	}
	else if( (zhong_num>=3&&fa_num>=3&&bai_num==2) ||
			 (zhong_num==2&&fa_num>=3&&bai_num>=3) ||
			 (zhong_num>=3&&fa_num==2&&bai_num>=3) )
	{
		score *= 4;
		hu_flag |= RH_XIAOSANYUAN;//小三元
	}
	if(last_card_same_num==4)
	{
		if(si_gui_flag==1)
		{
			score *= 2;
			hu_flag |= RH_MINGSIGUI;//明四归
		}
		else
		{
			score *= 4;
			hu_flag |= RH_ANSIGUI;//暗四归
		}
	}
	if(couple_num==7&&card_list->len==HAND_IN_CARD_NUM)
	{
		score *= 4;
		int j;
		if(zhong_num==2&&fa_num==2&&bai_num==2)
		{
			score *= 8;
			hu_flag |= RH_SANYUANQIDUI;//三元七对
		}
		if(four_num==1)
		{
			score *= 2;
			hu_flag |= RH_HAOHUAQIDUI;//豪华七对
		}
		else if(four_num==2)
		{
			score *= 8;
			hu_flag |= RH_CHAOHAOHUAQIDUI;//超豪华七对
		}
		else if(four_num==3)
		{
			score *= 32;
			hu_flag |= RH_CHAOCHAOHAOHUAQIDUI;//超超豪华七对
		}
		else
			hu_flag |= RH_QIDUI;//七对
	}
	else if(kind==ck_WU_TIAO||kind==ck_WU_TONG)
	{
		int k;
		CARD_KIND temp[MAX_HANDIN_NUM];
		int its_si=card_list->len;
		int its_liu=card_list->len;
		int its_five=card_list->len;
		for(k=card_list->atcvie_place;k<card_list->len;k++)
			if(card_list->data[k].kind==kind-1)
				its_si=k;
			else if(card_list->data[k].kind==kind+1)
				its_liu=k;
		if(its_si!=card_list->len&&its_liu!=card_list->len)
		{
			int active=card_list->atcvie_place;
			int active_len=card_list->len-card_list->atcvie_place;

			array_sort2(&(card_list->data[active]),its_si-active,its_liu-active,active_len,ck_NOT_DEFINED,ck_NOT_DEFINED,temp);

			if(cards_stable(temp,active_len-2)==1)
			{
				score *= 4;
				hu_flag |= RH_KAWUXIN;
			}
		}
	}
	kind_hu=hu_flag;
	if(rr_aim!=RA_NOTASK)
		task_check(hu_flag);
	return score;
}

int NetRRound::cal_times(CARD_KIND kind,CARD_KIND data[],int len)
{
	int i;
	unsigned char color;
	unsigned char color_flag=1;
	unsigned char si_peng_flag=0;
	unsigned char couple_num=0;
	unsigned char zhong_num=0;
	unsigned char fa_num=0;
	unsigned char bai_num=0;
	unsigned char four_num=0;
	unsigned char last_card_same_num=0;
	unsigned char si_gui_flag = 0;
	unsigned int  hu_flag = 0;
	long score = 1;
	color=kind/9;

	for(i=0;i<len;i++)
	{
		int l_cards=1;
		if(	data[i]/9 != color )
		{
			color_flag = 0;
		}
		if( data[i]==kind )
		{
			last_card_same_num++;
		}
		for(int k=i+1;k<len;k++)
			if(data[i]==data[k] )
				l_cards++;
		if(l_cards==2||l_cards==4)
			couple_num++;
		if(l_cards==4)
			four_num++;

		if(data[i]==ck_ZHONG)
		{
			zhong_num++;
		}
		else if(data[i]==ck_FA)
		{
			fa_num++;
		}
		else if(data[i]==ck_BAI)
		{
			bai_num++;
		}
	}
	if(len!=2)
	{
		int l_num;
		int l_couple=0;
		int l_len=len;
		for(int k=0;k<l_len;k++)
		{
			l_num=1;
			for(int t=k+1;t<l_len;t++)
				if(data[k]==data[t])
					l_num++;
			if(l_num==3)
				l_len -= 3;
			l_num=0;
			for(int t=0;t<l_len;t++)
				if(data[k]==data[t])
					l_num++;
			if(l_num==2)
				l_couple++;
		}
		if(l_len==2&&l_couple==2)
			si_peng_flag=1;
	}
	else
		score *= 4;
	if(color_flag)
		score *= 4;
	if(si_peng_flag==1)
		score *= 2;
	if(zhong_num==3&&fa_num==3&&bai_num==3)
		score *= 8;
	else if((zhong_num==3&&fa_num==3&&bai_num==2) ||
			(zhong_num==2&&fa_num==3&&bai_num==3) ||
			(zhong_num==3&&fa_num==2&&bai_num==3))
		score *= 4;
	if(last_card_same_num==4)
		score *= 4;
	if( couple_num==7 && len==HAND_IN_CARD_NUM )
	{
		score *= 4;
		int j;
		if(zhong_num==2&&fa_num==2&&bai_num==2)
			score *= 8;
		if(four_num==1)
			score *= 2;
		else if(four_num==2)
			score *= 8;
		else if(four_num==3)
			score *= 32;
	}
	else if(kind==ck_WU_TIAO||kind==ck_WU_TONG)
	{
		int k;
		CARD_KIND temp[MAX_HANDIN_NUM];
		int its_si=len;
		int its_liu=len;
		int its_five=len;
		CARD_KIND temp_kind;
		for(k=0;k<len;k++)
			if(data[k]==kind-1)
				its_si=k;
			else if(data[k]==kind+1)
				its_liu=k;
			else if(data[k]==kind)
				its_five=k;
		if(its_si!=len&&its_liu!=len&&its_five!=len)
		{
			for(k=0;k<len;k++)
				if(k==its_si||k==its_liu||k==its_five)
					temp[k]=ck_NOT_DEFINED;
				else
					temp[k]=data[k];
			for(k=0;k<len;k++)
			{
				temp_kind=temp[k];
				for(int j=k+1;j<len;j++)
				{
					if(temp[j]<temp_kind)
					{
						temp[k]=temp[j];
						temp[j]=temp_kind;
						temp_kind=temp[k];
					}
				}
			}
			if(cards_stable(temp,len-3)==1)
				score *= 4;
		}
	}
	return score;
}

int NetRRound::pattern_match(CARD_KIND data[],int len)
{
	int i=0;
	int zhong_num,fa_num,bai_num;
	zhong_num=0;
	fa_num=0;
	bai_num=0;
	for(int j=0;j<len;j++)
	{
		if(data[j]==ck_ZHONG)
			zhong_num++;
		if(data[j]==ck_FA)
			fa_num++;
		if(data[j]==ck_BAI)
			bai_num++;
	}
	if(zhong_num==1 ||fa_num==1 || bai_num == 1 )
		return 0;
	else if(zhong_num==fa_num&&zhong_num==bai_num&&zhong_num==2&&len!=12)
		return 0;
	while(i<len)
	{
		if(len-i>=3)
		{
			if( data[i+2]==data[i] && data[i+1]==data[i] ||
			(data[i+2]==data[i]+2 && data[i+1]==data[i]+1)&&
			(data[i+2]/9==data[i]/9&&data[i+2]/9==data[i+1]/9) )
				i += 3;
			else if(len-i>=6)
			{
				if( (data[i+1]/9==data[i]/9 && data[i+2]/9==data[i]/9 &&
					data[i+3]/9==data[i]/9 && data[i+4]/9==data[i]/9 && data[i+5]/9==data[i]/9)
				&&( ( data[i+1]==data[i]+1 && data[i+2]==data[i]+1 &&
				data[i+3]==data[i]+2 && data[i+4]==data[i]+2 && data[i+5]==data[i]+3 ) ||
					( data[i+1]==data[i]+1 && data[i+2]==data[i]+1 &&
				data[i+3]==data[i]+1 && data[i+4]==data[i]+1 && data[i+5]==data[i]+2) ||
					( data[i+1]==data[i] && data[i+2]==data[i]+1 &&
				data[i+3]==data[i]+1 && data[i+4]==data[i]+2 && data[i+5]==data[i]+2) ) )
					i += 6;
				else if(len-i>=9)
				{
					if( (data[i]/9==data[i+1]/9&&data[i]/9==data[i+2]/9&&data[i]/9==data[i+3]/9&&
					data[i]/9==data[i+4]/9&&data[i]/9==data[i+5]/9&&data[i]/9==data[i+7]/9 && data[i]/9==data[i+8]/9)
						&&( data[i+1]==data[i]+1 && data[i+2]==data[i]+1 && data[i+3]==data[i]+1 &&
					data[i+4]==data[i]+2 && data[i+5]==data[i]+2 && data[i+6]==data[i]+2  &&
					data[i+7]==data[i]+3 && data[i+8]==data[i]+3 || data[i+1]==data[i] &&
					data[i+2]==data[i]+1 && data[i+3]==data[i]+1 && data[i+4]==data[i]+1 &&
					data[i+5]==data[i]+2 && data[i+6]==data[i]+2 && data[i+7]==data[i]+2 && data[i+8]==data[i]+3) )
						i += 9;
					else if(len-i==12)
					{
						if(  (data[i]/9==data[i+1]/9&&data[i]/9==data[i+2]/9&&data[i]/9==data[i+3]/9&&
						data[i]/9==data[i+4]/9&&data[i]/9==data[i+5]/9&&data[i]/9==data[i+7]/9 && 
						data[i]/9==data[i+8]/9&&data[i]/9==data[i+9]/9&&data[i]/9==data[i+10]/9&&data[i]/9==data[i+11]/9)
							&&(data[i+1]==data[i] && data[i+2]==data[i]+1 && data[i+3]==data[i]+1 &&
						data[i+4]==data[i]+1 && data[i+5]==data[i]+1 && data[i+6]==data[i]+2  &&
						data[i+7]==data[i]+2 && data[i+8]==data[i]+2 && data[i+9]==data[i]+2  &&
						data[i+10]==data[i]+3 && data[i+11]==data[i]+3) || (data[i+1]==data[i] &&
						data[i+2]==data[i+3] && data[i+4]==data[i+5] && data[i+6]==data[i+7] &&
						data[i+8]==data[i+9] && data[i+10]==data[i+11]) )
							i +=12;
						else
							break;
					}
					else
						break;
				}
				else
					break;
			}
			else
				break;
		}
		else
			break;
	}
	if(i==len)
		return 1;
	return 0;
}

int NetRRound::cards_stable(CARD_KIND clist[],int len)
{
	CARD_KIND temp_list2[MAX_HANDIN_NUM-2];
	int i=0;
	if( len==2 && clist[0]==clist[1] )
		return 1;
	while(i<len-1)
	{
		if(	clist[i]==clist[i+1] )
		{
			if(i!=0)
				memcpy(temp_list2,clist,i*sizeof(CARD_KIND));
			if(i<len-2)
				memcpy(&temp_list2[i],&clist[i+2],(len-i-2)*sizeof(CARD_KIND));
			if(pattern_match(temp_list2,len-2)==1)
				return 1;
			i += 2;
		}
		else
			i++;
	}
	return 0;
}

void NetRRound::task_check(unsigned int flag)
{
	if( !(flag&rr_aim) )
		rr_aim=0;
}

int NetRRound::hu_check(CARD_KIND data_kind)
{
	CARD_KIND temp_list[MAX_HANDIN_NUM];
	CARD_KIND temp_kind=data_kind;
	int res=0;

	int index=card_list->atcvie_place;
	int i;
	for(i=index;i<card_list->len;i++)
	{
		if(temp_kind<=card_list->data[i].kind)
			break;
	}
	if(i==index)
	{
		temp_list[0]=temp_kind;
		for(int j=0;j<card_list->len-index;j++)
			temp_list[j+1]=card_list->data[j+index].kind;
	}
	else if(i==card_list->len)
	{
		for(int j=0;j<card_list->len-index;j++)
			temp_list[j]=card_list->data[index+j].kind;
		temp_list[card_list->len-index]=temp_kind;
	}
	else
	{
		for(int j=index;j<i;j++)
			temp_list[j-index]=card_list->data[j].kind;
		temp_list[i-index]=temp_kind;
		for(int j=i-index;j<card_list->len-index;j++)
			temp_list[j+1]=card_list->data[j+index].kind;
	}

	res=cards_stable(temp_list,card_list->len-card_list->atcvie_place+1);

	return res;
}

void NetRRound::array_sort(CARD clist[],int index,int len,CARD_KIND kind,CARD_KIND rlist[])
{
	int i,j;
	CARD_KIND temp_kind;

	for(i=0;i<len;i++)
		if(	i != index )
			rlist[i]=clist[i].kind;
		else
			rlist[index]=kind;
	for(i=0;i<len;i++)
	{
		temp_kind=rlist[i];
		for(j=i+1;j<len;j++)
		{
			if(rlist[j]<temp_kind)
			{
				rlist[i]=rlist[j];
				rlist[j]=temp_kind;
				temp_kind=rlist[i];
			}
		}
	}
}

void NetRRound::array_sort2(CARD clist[],int index1,int index2,int len,CARD_KIND kind1,CARD_KIND kind2,CARD_KIND rlist[])
{
	int i,j;
	CARD_KIND temp_kind;

	for(i=0;i<len;i++)
		if(i==index1)
			rlist[index1]=kind1;
		else if(i==index2)
			rlist[index2]=kind2;
		else
			rlist[i]=clist[i].kind;
	for(i=0;i<len;i++)
	{
		temp_kind=rlist[i];
		for(j=i+1;j<len;j++)
		{
			if(rlist[j]<temp_kind)
			{
				rlist[i]=rlist[j];
				rlist[j]=temp_kind;
				temp_kind=rlist[i];
			}
		}
	}
}
unsigned int NetRRound::ting_check(int index,CARD_KIND cur_card,int kind,CARD_KIND rlist[])
{
	int active=card_list->atcvie_place;
	int len=card_list->len-card_list->atcvie_place;

	array_sort(&(card_list->data[active]),index-active,len,CARD_KIND(kind),rlist);
	if(cards_stable(rlist,len)==1)
		return 1;

	return 0;
}

int NetRRound::judge_kou_cards(CARD_KIND card,int no,CARD_KIND RototHandOutIndex)
{
	CARD temp_list[MAX_HANDIN_NUM];
	CARD_KIND temp_list2[MAX_HANDIN_NUM];
	for(int m=0;m<MAX_HANDIN_NUM;m++)
		temp_list[m].kind=ck_NOT_DEFINED;
	memset(temp_list2,ck_NOT_DEFINED,MAX_HANDIN_NUM*sizeof(CARD_KIND));

	int j=0;
	int same_no=0;
	for(int i=card_list->atcvie_place;i<card_list->len;i++)
	{
		if(card_list->data[i].status==c_MING_KOU)
			continue;
		else if(card_list->data[i].kind==card&&same_no<3)
		{
			same_no++;
			continue;
		}
		else
			temp_list[j++].kind=card_list->data[i].kind;
	}
	if(no==1)
	{
		for(int n=0;n<j;n++)
			for(int k=ck_YI_TIAO;k<=ck_BAI;k++)
			{
				array_sort(temp_list,n,j,CARD_KIND(k),temp_list2);
				if(cards_stable(temp_list2,j)==1)
					return 1;
			}
	}
	else if(no!=1)
	{
		int index;
		for(int a=0;a<j;a++)
		{
			if(temp_list[a].kind==RototHandOutIndex)
			{
				index=a;
				break;
			}
		}
		for(int k=ck_YI_TIAO;k<=ck_BAI;k++)
		{
			array_sort(temp_list,index,j,CARD_KIND(k),temp_list2);
			if(cards_stable(temp_list2,j)==1)
				return 1;
		}
	}
	return 0;
}
//void NetRRound::kou_check(CARD_KIND river_list[],int river_len,CARD_KIND list[],int *len)
//{
//	int same_num=0;
//	if(list==NULL)
//		return;
//	for(int i=card_list->atcvie_place;i<card_list->len;i++)
//	{
//		same_num=0;
//		for(int j=card_list->atcvie_place;j<card_list->len;j++)
//			if(card_list->data[j].kind==card_list->data[i].kind)
//				same_num++;
//		if(same_num==3)
//		{
//			int m;
//			for(m=0;m<river_len;m++)
//				if(river_list[m]==card_list->data[i].kind)
//					break;
//			if(m==river_len)
//			{
//				int l;
//				for(l=0;l<*len;l++)
//					if(list[l]==card_list->data[i].kind)
//						break;
//				if(l==*len)
//				{
//					if(judge_kou_cards(card_list->data[i].kind)==1,no,int RototHandOutIndex)
//						list[*len++]=card_list->data[i].kind;
//				}
//			}
//		}
//	}
//}
void NetRRound::get_hu_cards(CARD_KIND curArray[MAX_HANDIN_NUM][9])
{
	for(int a=0;a<MAX_HANDIN_NUM;a++)
		for(int b=0;b<9;b++)
			curArray[a][b]=hu_cards[a][b];
}
void NetRRound::get_huTiemsForEveryOne(int curArray[MAX_HANDIN_NUM][9])
{
	for(int a=0;a<MAX_HANDIN_NUM;a++)
		for(int b=0;b<9;b++)
			curArray[a][b]=huTiemsForEveryOne[a][b]*2;
}
void NetRRound::get_hu_NumForEveryCard(int curArray[MAX_HANDIN_NUM])
{
	for(int a=0;a<MAX_HANDIN_NUM;a++)
			//curArray[a]=hu_NumForEveryCard[a];
			curArray[a]=hu_cards_num[a];
}
void NetRRound::get_hu_residueForEvery(int curArray[MAX_HANDIN_NUM][9])
{
	int temp=0;
	for(int a=0;a<MAX_HANDIN_NUM;a++)
	{
		if(hu_cards[a][0]==-1)
			continue;
		for(int b=0;b<9;b++)
		{
			if(hu_cards[a][b]==-1)
				break;
			int num=0;
			for(int c=0;c<card_list->len;c++)
			{
				if(card_list->data[c].status==c_FREE||card_list->data[c].status==c_AN_GANG||card_list->data[c].status==c_MING_KOU)
					if(hu_cards[a][b]==card_list->data[c].kind)
						num++;
			}
			temp=hu_residueForEvery[a][b]-num;
			curArray[a][b]=temp;
		}
	}
}
unsigned int NetRRound::ming_check()
{
	unsigned int res =0;
	CARD_KIND cur_card/*,last_card*/;
	int active_len=card_list->len-card_list->atcvie_place;
	int index=card_list->atcvie_place;
	CARD_KIND temp_list[MAX_HANDIN_NUM];
	CARD_KIND last_card=ck_NOT_DEFINED;
	memset(temp_list,ck_NOT_DEFINED,MAX_HANDIN_NUM*sizeof(CARD_KIND));
	//memset(hu_NumForEveryCard,0,sizeof(int)*MAX_HANDIN_NUM);
	int m=0;
	hu_places_num=0;
	memset(hu_cards_num,0,sizeof(int)*MAX_HANDIN_NUM);
	memset(hu_cards,0xff,sizeof(CARD_KIND)*MAX_HANDIN_NUM*9);
	memset(huTiemsForEveryOne,0,sizeof(int)*MAX_HANDIN_NUM*9);
	for(int i=index;i<card_list->len;i++)
	{
		m=0;
		if(card_list->data[i].status==c_MING_KOU)
			continue;
		cur_card=card_list->data[i].kind;
		if(last_card==cur_card)
		{
			res |= ( ( res&( 1<<(i-1) ) )<<1 );
			hu_cards_num[i]=hu_cards_num[i-1];
			memcpy(hu_cards[i],hu_cards[i-1],sizeof(CARD_KIND)*9);
			memcpy(huTiemsForEveryOne[i],huTiemsForEveryOne[i-1],sizeof(int)*9);
			continue;
		}
		for(int k=ck_YI_TIAO;k<=ck_BAI;k++)
		{
			if(ting_check(i,cur_card,k,temp_list)==1)
			{
				res |= (1<<i);
				hu_cards[i][m]=CARD_KIND(k);
				hu_cards_num[i]++;
				int s_core=cal_times(CARD_KIND(k),temp_list,card_list->len-card_list->atcvie_place);
				for(int s_l=0;s_l<card_list->atcvie_place;s_l++)
					if( card_list->data[s_l].kind==CARD_KIND(k)&&card_list->data[s_l].status==c_PENG)
					{
						s_core *= 2;
						break;
					}
				huTiemsForEveryOne[i][m++]=s_core;
			}
		}
		last_card=cur_card;
	}
	for(int i=0;i<MAX_HANDIN_NUM;i++)
		if(res&(1<<i))
			hu_places_num++;
	hu_places=res;
	return res;
}

unsigned char NetRRound::init(int card_array[],int len,int aim)
{
	int i;

	rr_aim = aim;
	rr_ting_flag = 0;
	total_score = 0;
	card_score=0;
	hu_len=0;
	hu_places_num=0;
	memset(hu_cards_num,0,sizeof(int)*MAX_HANDIN_NUM);
	memset(hu_cards,0xff,sizeof(CARD_KIND)*MAX_HANDIN_NUM*9);
	if(out_card_list)
		delete out_card_list;
	if(card_list)
		delete card_list;
	out_card_list=new outCardList;
	card_list = new CARD_ARRAY;
	if(!card_list||!out_card_list)
		return a_TIMEOUT;
	card_list->len=0;
	card_list->atcvie_place =0;
	memset(card_list->data,0xff,MAX_HANDIN_NUM*sizeof(CARD));//手牌初始化

	CARD temp;
	unsigned char ac=a_JUMP;
	for(i=0;i<len;i++)
	{
		temp.kind=(CARD_KIND)(card_array[i]/4);
		temp.status=c_FREE;
		temp.can_play=cps_YES;
		if(i==13)
		{
			ac=hand_in((CARD_KIND)(card_array[i]/4),0,0,false,a_JUMP,0,false);
			card_delete(13,1);
			card_insert(temp,1);
		}
		else
			card_insert(temp,1);
	}
	return ac;
}

unsigned char NetRRound::ActiontodoCheckAgain()
{
    LOGGER_WRITE("%s",__FUNCTION__);

	unsigned char res = 0x0;
	int card_num;
	for(int i=card_list->atcvie_place;i<card_list->len;i++)
	{
		card_num=1;
		for(int k=i+1;k<card_list->len;k++)
		{
			if(card_list->data[k].kind==card_list->data[i].kind)
				card_num++;
		}
		if(card_num==4)
		{
			res |= a_SHOU_GANG;
			break;
		}
	}
	if( ( archive_ming_indexes=ming_check() )!=0 )
		res |= a_MING;
	return res;
}

unsigned char NetRRound::hand_in(CARD_KIND kind,unsigned char who_give,unsigned char tingStatus,bool is_last_one,unsigned char last_action_WithGold,unsigned int continue_gang_times,bool isGangHua)
{
	int num = 0;
	unsigned char res = 0x0;
	card_list->data[card_list->len].kind=kind;
	card_list->data[card_list->len].status=c_FREE;
	card_list->data[card_list->len].can_play=cps_YES;

	if(rr_ting_flag==0)
	{
		int card_num;
		for(int i=card_list->atcvie_place;i<card_list->len;i++)
		{
			card_num=1;
			for(int k=i+1;k<card_list->len;k++)
			{
				if(card_list->data[k].kind==card_list->data[i].kind)
					card_num++;
			}
			if(card_num==4 &&who_give==0&&!is_last_one)
			{
				res |= a_SHOU_GANG;
				break;
			}
		}
		for(int i=0;i<card_list->len;i++)
		{
			if(card_list->data[i].kind==kind)
			{
				if(card_list->data[i].status==c_PENG)
				{
					if(who_give==0&&!is_last_one)
					{
						res |= a_MING_GANG;
					}
					break;
				}
				else if(card_list->data[i].status==c_FREE)
				{
					num++;
				}
			}
		}
		if(num==3)
		{
			if(who_give==0&&!is_last_one)
				res |= (a_MING_GANG | a_AN_GANG);
			else if(!is_last_one)
				res |= (a_MING_GANG | a_PENG);
			else if(is_last_one)
				res |= a_PENG;
		}
		else if(num==2&&who_give!=0)
		{
				res |= a_PENG;
		}
	}
	else
	{

		for(int i=0;i<card_list->atcvie_place;i++)
			if(card_list->data[i].status==c_MING_KOU && card_list->data[i].kind==kind)
			{
				if(!is_last_one)
				{
					if(who_give==0)
						res |= (a_MING_GANG | a_AN_GANG);
					else
						res |= a_MING_GANG;
				}
				break;
			}
	}

	if(hu_check(kind)==1)
	{
		card_score=cal_score(kind,who_give,is_last_one,last_action_WithGold,continue_gang_times,isGangHua);
		if(who_give==0||(who_give==1&&(tingStatus==1||card_score!=1)))
			res|=a_HU;
	}

	if(who_give==0)
	{
		card_list->len++;
		if(rr_ting_flag==0 && !is_last_one)
		{
			if(( archive_ming_indexes=ming_check())!=0 )
				res |= a_MING;
		}
	}

    LOGGER_WRITE("NETWORK : %x %s action %d: kind %d,who_give %d,tingStatus %d,is_last_one %d",
        (int)this,__FUNCTION__,res,kind,who_give,tingStatus,is_last_one);
	return res;
}
/*if ting place=card_list->len*/
CARD_KIND NetRRound::hand_out(unsigned int place)
{

	CARD_KIND l_kind;
	CARD tail_card;
	tail_card.kind = card_list->data[card_list->len-1].kind;
	tail_card.status = card_list->data[card_list->len-1].status;
	tail_card.can_play=card_list->data[card_list->len-1].can_play;
	if( card_list->data[place].status != c_FREE )
	{
		return ck_NOT_DEFINED;
	}
	l_kind=card_list->data[place].kind;
    LOGGER_WRITE("NETWORK : %x %s : %d",this,__FUNCTION__,l_kind);

	out_card_list->insertItem(card_list->data[place]);

	if( place==card_list->len-1 )
	{
		card_delete(place,1);
		return l_kind;
	}

	card_delete(place,1);

	if(rr_ting_flag==0)
	{
		card_insert(tail_card,1);
		card_list->len--;
	}

	if(role_type==INTERNET_ME)
	{
		;
	}
		return l_kind;
}

void NetRRound::action1()
{
	for (int i=0;i<=card_list->len-1;i++)
	{
		card_list->data[i].can_play=cps_NO;
	}
}

void NetRRound::MingCancel()
{
	for(int a=0;a<card_list->len;a++)
	{
		if(a<card_list->atcvie_place)
			card_list->data[a].can_play=cps_NO;
		else
			card_list->data[a].can_play=cps_YES;
	}
}
ACT_RES NetRRound::action(unsigned char who_give,ARRAY_ACTION act)
{
    LOGGER_WRITE("%x %s : %d (who_give=%d)",this,__FUNCTION__,act,who_give);

	CARD temp_data;
	InsertPlaceForMG=-1;

	temp_data.can_play=cps_YES;
	if(who_give==0)
		temp_data.kind=card_list->data[card_list->len-1].kind;
	else
		temp_data.kind=card_list->data[card_list->len].kind;

	if(act==a_PENG)
	{
		temp_data.status=c_PENG;
		temp_data.can_play=cps_NO;
		int i=card_list->atcvie_place;
		for(;i<card_list->len;i++)
			if(card_list->data[i].kind==temp_data.kind)
			{
				card_delete(i,2);
				break;
			}
		card_insert(temp_data,3);
		card_list->atcvie_place += 3;
	}
	else if(act==a_KOU)
	{
		temp_data.status=c_MING_KOU;
		temp_data.can_play=cps_NO;
		int i=card_list->len-1;
		for(;i>=card_list->atcvie_place;i--)
			if(card_list->data[i].status==c_MING_KOU)
			{
				temp_data.kind=card_list->data[i].kind;
				card_delete(i,1);
				card_insert(temp_data,1);
				i++;
				card_list->atcvie_place+=1;
			}
	}
	else if(act==a_KOU_CANCEL)
	{
		temp_data.status=c_FREE;
		temp_data.can_play=cps_YES;
		int i=card_list->atcvie_place-1;
		for(;i>=0;i--)
			if(card_list->data[i].status==c_MING_KOU)
			{
				temp_data.kind=card_list->data[i].kind;
				card_delete(i,1);
				card_list->atcvie_place-=1;
				card_insert(temp_data,1);
			}
	}
	else if(act==a_MING_GANG)
	{
		bool ifFirst=true;
		temp_data.status=c_MING_GANG;
		temp_data.can_play=cps_NO;
		for(int i=card_list->len-1;i>=0;i--)
		{
			if(card_list->data[i].status==c_PENG&& card_list->data[i].kind==temp_data.kind)
			{
				if(ifFirst)
					InsertPlaceForMG=i-2;
				ifFirst=false;
				card_list->atcvie_place--;
			}
			else if(card_list->data[i].status==c_MING_KOU&& card_list->data[i].kind==temp_data.kind)
				card_list->atcvie_place--;
			if(card_list->data[i].kind==temp_data.kind)
				card_delete(i,1);
		}
		card_insert(temp_data,4);
		card_list->atcvie_place += 4;
	}
	else if(act==a_AN_GANG)
	{
		int m=card_list->len-1;
		for(;m>=0;m--)
		{
			if(card_list->data[m].kind==temp_data.kind)
			{				 
				if(card_list->data[m].status==c_MING_KOU)
					card_list->atcvie_place--;
				card_delete(m,1);
			}
		}
		temp_data.status=c_AN_GANG;
		temp_data.can_play=cps_NO;
		card_insert(temp_data,4);
		//if(rr_ting_flag!=0)
		//	card_list->atcvie_place++;
		//else
			card_list->atcvie_place+=4;		
	}
	else if(act==a_SHOU_GANG)
	{
		int card_num;
		int i;
		for(i=card_list->atcvie_place;i<card_list->len-1;i++)
		{
			card_num=1;
			for(int j=i+1;j<card_list->len-1;j++)
				if(card_list->data[i].kind==card_list->data[j].kind)
					card_num++;
			if(card_num==4)
			{
				temp_data.kind=card_list->data[i].kind;
				break;
			}
		}
		for(int k=card_list->len-1;k>=card_list->atcvie_place;k--)
		{
			if(card_list->data[k].kind==card_list->data[i].kind)
				card_delete(k,1);
		}
		//temp_data.kind=card_list->data[i].kind;
		temp_data.status=c_AN_GANG;
		temp_data.can_play=cps_NO;
		card_insert(temp_data,4);
		card_list->atcvie_place += 4;

		temp_data.status=c_FREE;
		temp_data.can_play=cps_YES;
		temp_data.kind=card_list->data[card_list->len-1].kind;
		card_delete(card_list->len-1,1);
		card_insert(temp_data,1);
	}
	else if(act==a_MING)
	{
		unsigned int active_place_indexes=get_ming_indexes();
		for (int i=card_list->atcvie_place;i<card_list->len;i++)
		{
			int m=0;
			m=active_place_indexes&(1<<i);
			if(m>0)
			{
				card_list->data[i].can_play=cps_YES;
			}
			else
			{
				card_list->data[i].can_play=cps_NO;
			}
			
		}
	}
	else if(act==a_HU)
	{
		temp_data.status=c_FREE;
		temp_data.can_play=cps_NO;
		if(who_give!=0)
			card_insert(temp_data,1);
		else
		{
			card_delete(card_list->len-1,1);
			card_insert(temp_data,1);
		}
	
	}
	else if(act==a_JUMP)
	{
		if(who_give==0)
		{
			temp_data.status=c_FREE;
			card_delete(card_list->len-1,1);
			card_insert(temp_data,1);
		}
	}

	if(role_type==INTERNET_ME)
	{
		;
	}

	return ar_DONE;
}

unsigned int NetRRound::get_aim()
{
	return rr_aim;
}

unsigned int NetRRound::get_ming_indexes()
{
	return archive_ming_indexes;
}

int *NetRRound::get_ming_reserved_cards_num(outCardList *river_list)
{
	int i;	
	memset(hu_residueForEvery,0,sizeof(int)*MAX_HANDIN_NUM*9);
	CARD card;
	int m=0;
	for(i=0;i<MAX_HANDIN_NUM;i++)
		if(archive_ming_indexes&(1<<i))
		{
			m=0;
			for(int j=0;j<9;j++)
				if(hu_cards[i][j]!=-1)
				{
					int k=1;
					int default_reserved_cards=4;
					while(river_list->getCard(card,k++)==true)
					{
						if(card.kind==hu_cards[i][j])
							default_reserved_cards--;
					}
					hu_residueForEvery[i][m++]=default_reserved_cards;
					hu_reserved_num[i] += default_reserved_cards;
				}
		}
	return hu_reserved_num;
}

void NetRRound::set_ming_indexes(unsigned int indexesFlag)
{
	archive_ming_indexes=indexesFlag;
}

void NetRRound::set_ting_status(unsigned char flag)
{
	rr_ting_flag=flag;
	hu_len=0;
	for(int i=0;i<21;i++)
		if(hu_check(CARD_KIND(i))==1)
			hucards[hu_len++]=CARD_KIND(i);
}

void NetRRound::get_hu_cards(CARD_KIND c_list[],int *len)
{
	*len=hu_len;
	for(int k=0;k<hu_len;k++)
		c_list[k]=hucards[k];
}

unsigned char NetRRound::get_ting_status()
{
	return rr_ting_flag;
}

long NetRRound::get_card_score()
{
	return card_score;
}

void NetRRound::update_score(long score)
{
	total_score += score;
}
CARD_ARRAY* NetRRound::get_card_list()
{
	return card_list;
}

outCardList* NetRRound::getOutCardList()
{
	return out_card_list;
}

bool NetRRound::get_Hu_Flag(unsigned int *hu_kind)
{
	*hu_kind=kind_hu;
	return true;
}

bool NetRRound::get_ming_check_result(MRES *res)
{
	//unsigned int result;
	if(hu_places_num!=0)
	{
		res->hu_places_num=hu_places_num;
		res->hu_places=hu_places;
		for(int i=0;i<MAX_HANDIN_NUM;i++)
			res->hu_cards_num[i]=hu_cards_num[i];
		for(int i=0;i<MAX_HANDIN_NUM;i++)
			for(int j=0;j<hu_cards_num[i];j++)
				res->hu_cards[i][j]=hu_cards[i][j];

		return true;
	}
	else
		return false;
}
void NetRRound::set_role_type(RT type)
{
	role_type=type;
}
RT NetRRound::get_role_type()
{
	return role_type;
}
