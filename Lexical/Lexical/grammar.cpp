#include "main.cpp"
struct charset{									//语法产生式结点结构体，存储产生式右部
	char c;	
	charset* next;
};
struct sets{									//语法产生式结点结构体，存储产生式
	char head;
	charset* cs;
	bool tag;
	sets* next;
};
struct firstnode{								//First集、Follow集、Select集结点
	char cur;									//结构体，存储单个节点内容
	charset* cs;
	firstnode* next;
};
struct tablenode{								//分析表结点结构体，存储单个转换状态
	char ne;
	char by;
	charset* to;
	tablenode* next;
};
sets* CreateSets(string infile)					//建立语法产生式集合
{
	sets* x=new sets;
	x->cs=NULL;
	x->head=NULL;
	x->next=NULL;
	x->tag=true;
	ifstream fin(infile,std::ios::in);
	string s1=" ";
	charset* temp=NULL;
	sets* temp1=NULL,*temp2=x;
	while(getline(fin,s1))
	{
		char* c=(char*)s1.c_str();
		int len=s1.length();
		temp1=temp2;
		while(temp1->next)
			temp1=temp1->next;
		sets* t=new sets;
		charset* cc=new charset;
		cc->c=c[3];
		cc->next=NULL;
		t->head=c[0];
		t->cs=cc;
		t->next=NULL;
		t->tag=true;
		temp1->next=t;
		temp=t->cs;
		for(int i=4;i<len;i++)
		{
			charset* cc=new charset;
			cc->c=c[i];
			cc->next=NULL;
			temp->next=cc;
			temp=temp->next;
		}
	}
	fin.clear();
	fin.close(); 
	return x;
}
bool exist3(char c,charset* ch)					//过程函数，验证存在函数，应对包含空头的charset集
{
	charset* f=ch->next;
	while(f)
	{
		if(f->c==c)
			return true;
		f=f->next;
	}
	return false;
}
bool exist5(char c,charset* ch)					//过程函数，验证存在函数，应对任意charset集
{
	charset* f=ch;
	while(f)
	{
		if(f->c==c)
			return true;
		f=f->next;
	}
	return false;
}
void add(char c,charset* ch)					//过程函数，普通添加函数，往ch集中添加c
{
		charset* f=ch;
		while(f->next)
			f=f->next;
		charset* t=new charset;
		t->c=c;
		t->next=NULL;
		f->next=t;
}
void addFrist(char ne,char e,firstnode* fi)			//过程函数，添加单个内容至First集结构体
{
	firstnode* f=fi->next;
	while(f)
	{
		if(f->cur==ne)
		{
			if(!exist3(e,f->cs))
				add(e,f->cs);
			break;
		}
		f=f->next;
	}
}

void addFirstSet(char ne,charset* from,firstnode* to)	//过程函数，添加内容集至First集结构体
{														//同getFirst()一样，也可
	charset* f1=from;									//用来构造Follow集
	firstnode* t1=to->next;
	while(t1)
	{
		if(t1->cur==ne)
		{
			while(f1)
			{
				if(!exist3(f1->c,t1->cs))
					add(f1->c,t1->cs);
				f1=f1->next;
			}
		}
		t1=t1->next;
	}
}
void CreateCharset(charset* NotEnd,charset* End,sets* s)	//产生非终结符集、终结符集
{
	charset* ne=NotEnd,*e=End,*temp=NULL,*temp1=NULL;
	sets* temp3=s->next;
	while(temp3)
	{
		if(!exist3(temp3->head,ne))
			add(temp3->head,ne);
		temp3=temp3->next;
	}
	temp3=s->next;
	while(temp3)
	{
		temp=temp3->cs;
		while(temp)
		{
			if(!exist3(temp->c,ne)&&temp->c!=' ')
				add(temp->c,e);
			temp=temp->next;
		}
		temp3=temp3->next;
	}
}
void closeVisit(char ne,sets* a)			//过程函数，关闭左部为ne的产生式的使用状态：tag=false
{
	sets* t=a->next;
	while(t)
	{
		if(t->head==ne)
			t->tag=false;
		t=t->next;
	}
}
void openVisit(char ne,sets* a)				//过程函数，打开左部为ne的产生式的使用状态：tag=true
{
	sets* t=a->next;
	while(t)
	{
		if(t->head==ne)
			t->tag=true;
		t=t->next;
	}
}

void closeVisitCur(char ne,char f,sets* a)	//过程函数，关闭当前产生式的使用状态：tag=false
{
	sets* t=a->next;
	while(t)
	{
		if(t->head==ne&&t->cs->c==f)
			t->tag=false;
		t=t->next;
	}
}
void openVisitCur(char ne,char f,sets* a)	//过程函数，打开当前产生式的使用状态：tag=true
{
	sets* t=a->next;
	while(t)
	{
		if(t->head==ne&&t->cs->c==f)
			t->tag=true;
		t=t->next;
	}
}

void openVisitAll(sets* a)				//过程函数，打开所有产生式的使用状态：tag=true
{
	sets* t=a->next;
	while(t)
	{
		t->tag=true;
		t=t->next;
	}
}
bool judgeEnd(char ne,sets* a)			//过程函数，判断产生式的右部状况是否为空
{
	sets* t=a->next;
	charset* t2=NULL;
	while(t)
	{
		if(t->head==ne&&t->tag)
		{
			t2=t->cs;
			if(t2!=NULL)
			{
				if(t2->c=='$')
				{
					closeVisit(t->head,a);
					return true;
				}
			}
			if(t2==NULL)
				return false;
		}
		t=t->next;
	}
	return false;
}
void dcreaseEnd(sets* a,char ne)		//过程函数，初始化First集时对可达空字符
{										//的非终结符进行处理
	sets* b=a->next;
	charset* ct=NULL,*ct2=NULL;
	while(b)
	{
		ct=b->cs;
		if(ct!=NULL)
		{
			if(ct->c==ne)
			{
				if(ct->next)
				{
					ct->c=ct->next->c;
					charset* x=ct->next;
					ct->next=x->next;
					delete x;
				}
				else
				{
					ct->c='$';
					openVisit(b->head,a);
				}
			}
			else
			{
				while(ct->next!=NULL)
				{
					if(ct->next->c==ne)
					{
						charset* x=ct->next;
						ct->next=x->next;
						delete x;
					}
					if(ct->next==NULL)
						break;
					ct=ct->next;
				}
				if(ct->c==ne)
				{
					charset* x=ct->next;
					ct->next=x->next;
					delete x;
				}
			}
		}
		b=b->next;
	}
}
void InitFirst(firstnode* F,string file)		//First集初始化，书上开始用于检查
{												//是否可达空字符的过程
	sets* a=CreateSets(file);
	int tag=1;
	while(tag)
	{
		tag=0;
		sets* b=a->next;
		while(b)
		{
			if(judgeEnd(b->head,a))
			{
				addFrist(b->head,'$',F);
				dcreaseEnd(a,b->head);
				tag=1;
			}
			b=b->next;
		}
	}
	/*sets* y=a->next;						//查看内容可打开
	charset* ch=NULL;
	printf("\nNot End In Init:%c\n");

	while(y)
	{
		printf("\nNot End:%c\n",y->head);
		ch=y->cs;
		while(ch)
		{
				printf("%c,",ch->c);
			ch=ch->next;
		}
		y=y->next;
	}*/

	/*firstnode* t=F->next;
	charset* te=NULL;
	while(t)
	{
		printf("\nNot End Point: %c\n",t->cur);
		te=t->cs->next;
		while(te)
		{
			printf("%c,",te->c);
			te=te->next;
		}
		t=t->next;
	}*/

}
bool ToNull(firstnode* F,char ne)				//过程函数，检查非终结符是否可达空字符
{
	firstnode* f=F->next;
	charset* te=NULL;
	while(f)
	{
		if(f->cur==ne)
		{
			te=f->cs->next;
			while(te)
			{
				if(te->c=='$')
					return true;
				te=te->next;
			}
		}
		f=f->next;
	}
	return false;
}

charset* CreateFirst(char ne,firstnode* F,sets* s,charset* NotEnd)	//构造First集
{
	charset* first=NULL;
	sets* ts=s->next;
	charset* tc=NULL;
	while(ts)
	{
		if(ts->head==ne)
		{
			tc=ts->cs;
			while(tc)
			{
				if(exist3(tc->c,NotEnd)&&!ToNull(F,tc->c))
				{
					charset* cc=CreateFirst(tc->c,F,s,NotEnd);
					if(!first)
						first=cc;
					else
					{
						charset* te=first;
						while(te->next)
							te=te->next;
						te->next=cc;
					}
					break;
				}
				else if(exist3(tc->c,NotEnd)&&ToNull(F,tc->c))
				{
					charset* cc=CreateFirst(tc->c,F,s,NotEnd);
					charset* cc2=cc;
					charset* cc3=cc;
					while(cc2!=NULL)
					{
						if(cc2->c=='$')
						{
							if(cc2->next!=NULL)
							{
								cc2->c=cc2->next->c;
								charset* x=cc2->next;
								cc2->next=x->next;
								delete x;
							}
							else if(cc2==cc3)
								cc2=NULL;
							else
								cc3->next=NULL;
						}
						cc3=cc2;
						cc2=cc2->next;
					}
					if(!first)
						first=cc;
					else
					{
						charset* te=first;
						while(te->next)
							te=te->next;
						te->next=cc;
					}
				}
				else
				{
					if(!first)
					{
						charset* cc=new charset;
						cc->c=tc->c;
						cc->next=NULL;
						first=cc;
					}
					else
					{
						charset* te=first;
						while(te->next)
							te=te->next;
						charset* cc=new charset;
						cc->c=tc->c;
						cc->next=NULL;
						te->next=cc;
					}
					break;
				}
				tc=tc->next;
			}
		}
		ts=ts->next;
	}
	return first;
}
charset* getFirst(char ne,firstnode* F)				//过程函数，得到对应非终结符的First集，
{													//后来干脆也用来取得Follow集了
	firstnode* ff=F->next;
	while(ff)
	{
		if(ff->cur==ne)
		{
			return ff->cs->next;
		}
		ff=ff->next;
	}
	return NULL;
}

charset* CreateFollow(char ne,firstnode* F,firstnode* Fi,sets* s,charset* NotEnd)		//构造Follow集
{
	charset* follow=NULL;
	sets* ts=s->next;
	charset* tc=NULL,*tc2=NULL;

	if(getFirst(ne,F)!=NULL)
	{
		charset* ch=getFirst(ne,F);
		while(ch)
		{
			if(!exist5(ch->c,follow))
			{
				charset* cx=new charset;
				cx->c=ch->c;
				cx->next=NULL;
				if(follow==NULL)
					follow=cx;
				else
				{
					charset* te=follow;
					while(te->next)
						te=te->next;
					charset* cx=new charset;
					cx->c=ch->c;
					cx->next=NULL;
					te->next=cx;
				}
			}
			ch=ch->next;
		}
	}
	while(ts)
	{
		if(ts->tag)
		{
			tc=ts->cs;
			while(tc)
			{
				if(tc->c==ne)
				{
					tc2=tc->next;
					if(tc2==NULL)
					{
						closeVisitCur(ts->head,ts->cs->c,s);
						charset* cc=CreateFollow(ts->head,F,Fi,s,NotEnd);
						openVisitCur(ts->head,ts->cs->c,s);
						if(!follow)
							follow=cc;
						else
						{
							charset* te=follow;
							while(te->next)
								te=te->next;
							if(cc!=NULL)
							{
								charset* er=cc;
								while(er)
								{
									if(!exist5(er->c,follow))
									{
										charset* ccf=new charset;
										ccf->c=er->c;
										ccf->next=NULL;
										te->next=ccf;
										te=te->next;
									}
									er=er->next;
								}
							}
							else
								te->next=cc;
						}
						break;
					}
					while(tc2)
					{
						if(exist3(tc2->c,NotEnd)&&!ToNull(Fi,tc2->c))
						{
							charset* cc=CreateFirst(tc2->c,F,s,NotEnd);
							if(!follow)
								follow=cc;
							else
							{
								charset* te=follow;
								while(te->next)
									te=te->next;
								if(cc!=NULL)
								{
									charset* er=cc;
									while(er)
									{
										if(!exist5(er->c,follow))
										{
											charset* ccf=new charset;
											ccf->c=er->c;
											ccf->next=NULL;
											te->next=ccf;
											te=te->next;
										}
										er=er->next;
									}
								}
								else
									te->next=cc;
							}
							break;
						}
						else if(exist3(tc2->c,NotEnd)&&ToNull(Fi,tc2->c))
						{
							charset* cc=CreateFirst(tc2->c,F,s,NotEnd);
							charset* cc2=cc;
							charset* cc3=cc;
							while(cc2!=NULL)
							{
								if(cc2->c=='$')
								{
									if(cc2->next!=NULL)
									{
										cc2->c=cc2->next->c;
										charset* x=cc2->next;
										cc2->next=x->next;
										delete x;
									}
									else if(cc2==cc3)
									{
										cc2=NULL;
										cc3=NULL;
									}
									else
										cc3->next=NULL;
								}
								if(cc2!=NULL)
								{
									cc3=cc2;
									cc2=cc2->next;
								}
							}
							if(!follow)
								follow=cc;
							else
							{
								charset* te=follow;
								while(te->next)
									te=te->next;
								if(cc!=NULL)
								{
									charset* er=cc;
									while(er)
									{
										if(!exist5(er->c,follow))
										{
											charset* ccf=new charset;
											ccf->c=er->c;
											ccf->next=NULL;
											te->next=ccf;
											te=te->next;
										}
										er=er->next;
									}
								}
								else
									te->next=cc;
							}
						}
						else
						{
							if(!follow)
							{
								charset* cc=new charset;
								cc->c=tc2->c;
								cc->next=NULL;
								follow=cc;
							}
							else
							{
								charset* te=follow;
								while(te->next)
									te=te->next;
								charset* cc=new charset;
								cc->c=tc2->c;
								cc->next=NULL;
								te->next=cc;
							}
							break;
						}
						tc2=tc2->next;
					}
					if(tc2==NULL)
					{
						closeVisitCur(ts->head,ts->cs->c,s);
						charset* cc=CreateFollow(ts->head,F,Fi,s,NotEnd);
						openVisitCur(ts->head,ts->cs->c,s);
						if(!follow)
							follow=cc;
						else
						{
							charset* te=follow;
							while(te->next)
								te=te->next;
							if(cc!=NULL)
							{
								charset* er=cc;
								while(er)
								{
									if(!exist5(er->c,follow))
									{
										charset* ccf=new charset;
										ccf->c=er->c;
										ccf->next=NULL;
										te->next=ccf;
										te=te->next;
									}
									er=er->next;
								}
							}
							else
								te->next=cc;
						}
						break;
					}
				}
				tc=tc->next;
			}
		}
		ts=ts->next;
	}
	return follow;
}

void InitFiFo(firstnode* FirstSet,firstnode* FollowSet,string filename,sets* x,charset* NotEnd)		//First集、Follow集初始化
{
	charset* nt=NotEnd->next;
	firstnode* ft=FirstSet;
	while(nt)
	{
		firstnode* ff=new firstnode;
		charset* ch=new charset;
		ch->c='@';
		ch->next=NULL;
		ff->cur=nt->c;
		ff->cs=ch;
		ff->next=NULL;
		ft->next=ff;
		ft=ft->next;
		nt=nt->next;
	}
	InitFirst(FirstSet,filename);
	charset* se=NotEnd->next;
	while(se)
	{
		addFirstSet(se->c,CreateFirst(se->c,FirstSet,x,NotEnd),FirstSet);
		se=se->next;
	}

	nt=NotEnd->next;
	ft=FollowSet;
	while(nt)
	{
		firstnode* ff=new firstnode;
		charset* ch=new charset;
		ch->c='@';
		ch->next=NULL;
		ff->cur=nt->c;
		ff->cs=ch;
		ff->next=NULL;
		ft->next=ff;
		ft=ft->next;
		nt=nt->next;
	}
	ft=FollowSet->next;
	charset* ch=new charset;
	ch->c='#';
	ch->next=NULL;
	ft->cs->next=ch;

	se=NotEnd->next;
	while(se)
	{
		openVisitAll(x);
		addFirstSet(se->c,CreateFollow(se->c,FollowSet,FirstSet,x,NotEnd),FollowSet);
		se=se->next;
	}
}

void CreateSelect(firstnode* selectone,sets* xone,firstnode* FirstSet,firstnode* FollowSet,charset* NotEnd)		//构造Select集,单个产生式
{
	charset* selects=selectone->next->cs;
	charset* sone=xone->cs,*sone2=NULL;
	while(sone)
	{
		if(sone->c=='$')
		{
			charset* cc=getFirst(xone->head,FollowSet);
			charset* te=selects;
			while(te->next)
				te=te->next;
			if(cc!=NULL)
			{
				charset* er=cc;
				while(er)
				{
					if(!exist5(er->c,selects))
					{
						charset* ccf=new charset;
						ccf->c=er->c;
						ccf->next=NULL;
						te->next=ccf;
						te=te->next;
					}
					er=er->next;
				}
			}
			else
				te->next=cc;
			break;
		}
		else
		{
			if(!exist3(sone->c,NotEnd))
			{
				charset* te=selects;
				while(te->next)
					te=te->next;
				if(!exist3(te->c,selects))
				{
					charset* cc=new charset;
					cc->c=sone->c;
					cc->next=NULL;
					te->next=cc;
				}
				break;
			}
			sone2=sone;
			while(sone2->next)
			{
				if(exist3(sone2->c,NotEnd)&&!ToNull(FirstSet,sone2->c))
				{
					charset* cc=getFirst(sone2->c,FirstSet);
					charset* te=selects;
					while(te->next)
						te=te->next;
					if(cc!=NULL)
					{
						charset* er=cc;
						while(er)
						{
							if(!exist5(er->c,selects))
							{
								charset* ccf=new charset;
								ccf->c=er->c;
								ccf->next=NULL;
								te->next=ccf;
								te=te->next;
							}
							er=er->next;
						}
					}
					else
						te->next=cc;
					break;
				}
				else if(exist3(sone2->c,NotEnd)&&ToNull(FirstSet,sone2->c))
				{
					charset* cc=getFirst(sone2->c,FirstSet);
					charset* cc2=cc;
					charset* cc3=cc;
					while(cc2!=NULL)
					{
						if(cc2->c=='$')
						{
							if(cc2->next!=NULL)
							{
								cc2->c=cc2->next->c;
								charset* x=cc2->next;
								cc2->next=x->next;
								delete x;
							}
							else if(cc2==cc3)
							{
								cc2=NULL;
								cc3=NULL;
							}
							else
								cc3->next=NULL;
						}
						cc3=cc2;
						cc2=cc2->next;
					}
					charset* te=selects;
					while(te->next)
						te=te->next;
					if(cc!=NULL)
					{
						charset* er=cc;
						while(er)
						{
							if(!exist5(er->c,selects))
							{
								charset* ccf=new charset;
								ccf->c=er->c;
								ccf->next=NULL;
								te->next=ccf;
								te=te->next;
							}
							er=er->next;
						}
					}
					else
						te->next=cc;
				}
				else
				{
					charset* te=selects;
					while(te->next)
						te=te->next;
					if(!exist5(te->c,selects))
					{
						charset* cc=new charset;
						cc->c=sone2->c;
						cc->next=NULL;
						te->next=cc;
					}
					break;
				}
				sone2=sone2->next;
			}
			if(sone2->next==NULL)
			{
				if(exist3(sone2->c,NotEnd)&&!ToNull(FirstSet,sone2->c))
				{
					charset* cc=getFirst(sone2->c,FirstSet);
					charset* te=selects;
					while(te->next)
						te=te->next;
					if(cc!=NULL)
					{
						charset* er=cc;
						while(er)
						{
							if(!exist5(er->c,selects))		//point
							{
								charset* ccf=new charset;
								ccf->c=er->c;
								ccf->next=NULL;
								te->next=ccf;
								te=te->next;
							}
							er=er->next;
						}
					}
					else
						te->next=cc;
					break;
				}
				else if(exist3(sone2->c,NotEnd)&&ToNull(FirstSet,sone2->c))
				{
					charset* cc=getFirst(sone2->c,FirstSet);
					charset* cc2=cc;
					charset* cc3=cc;
					while(cc2!=NULL)
					{
						if(cc2->c=='$')
						{
							if(cc2->next!=NULL)
							{
								cc2->c=cc2->next->c;
								charset* x=cc2->next;
								cc2->next=x->next;
								delete x;
							}
							else if(cc2==cc3)
							{
								cc2=NULL;
								cc3=NULL;
							}
							else
								cc3->next=NULL;
						}
						if(cc2!=NULL)
						{
							cc3=cc2;
							cc2=cc2->next;
						}
					}
					charset* te=selects;
					while(te->next)
						te=te->next;
					if(cc!=NULL)
					{
						charset* er=cc;
						while(er)
						{
							if(!exist5(er->c,selects))		//point
							{
								charset* ccf=new charset;
								ccf->c=er->c;
								ccf->next=NULL;
								te->next=ccf;
								te=te->next;
							}
							er=er->next;
						}
					}
					else
						te->next=cc;
					charset* cc4=getFirst(xone->head,FollowSet);
					if(cc4!=NULL)
					{
						charset* er=cc4;
						while(er)
						{
							if(!exist5(er->c,selects))		//point
							{
								charset* ccf=new charset;
								ccf->c=er->c;
								ccf->next=NULL;
								te->next=ccf;
								te=te->next;
							}
							er=er->next;
						}
					}
					else
						te->next=cc4;

				}
				else
				{
					charset* te=selects;
					while(te->next)
						te=te->next;
					if(!exist5(te->c,selects))		//point
					{
						charset* cc=new charset;
						cc->c=sone2->c;
						cc->next=NULL;
						te->next=cc;
					}
					break;
				}
			}
		}
		sone=sone->next;
	}

}

void InitSelect(firstnode* SelectSet,sets* x,firstnode* FirstSet,firstnode* FollowSet,charset* NotEnd)		//Select集初始化
{
	sets* xt=x->next;
	firstnode* ft=SelectSet;
	while(xt)
	{
		firstnode* ff=new firstnode;
		charset* ch=new charset;
		ch->c='@';
		ch->next=NULL;
		ff->cur=xt->head;
		ff->cs=ch;
		ff->next=NULL;
		ft->next=ff;

		firstnode* selectone=ft;
		sets* xone=xt;
		CreateSelect(selectone,xone,FirstSet,FollowSet,NotEnd);

		ft=ft->next;
		xt=xt->next;
	}
	printf("\n	Select集	:	\n");				//输出状态至控制台，可注释
	ft=SelectSet->next;
	while(ft)
	{
		printf("\nCurrent Select Sets:%c\n",ft->cur);
		charset* ter=ft->cs->next;
		while(ter)
		{
			printf("%c,",ter->c);
			ter=ter->next;
		}
		printf("\n");
		ft=ft->next;
	}
}

void CreateTable(tablenode* Table,firstnode* SelectSet,sets* x)		//建立预测分析表
{
	tablenode* ta=Table;
	sets* xs=x->next;
	firstnode* ses=SelectSet->next;
	charset* css=NULL;
	while(xs)
	{
		css=ses->cs->next;
		while(css)
		{
			tablenode* t=new tablenode;
			t->ne=xs->head;
			t->by=css->c;
			t->to=xs->cs;
			t->next=NULL;
			while(ta->next)
				ta=ta->next;
			ta->next=t;
			css=css->next;
		}
		ses=ses->next;
		xs=xs->next;
	}

	ta=Table->next;
	printf("\n\n Select Analysis Table(预测分析表):  \n");	//输出状态至控制台，可注释
	while(ta)
	{
		printf("\n From: %c ,By: %c ,To:	",ta->ne,ta->by);
		charset* ch=ta->to;
		while(ch)
		{
			printf("%c,",ch->c);
			ch=ch->next;
		}
		printf("\n");
		ta=ta->next;
	}
}
charset* getTrans(char ne,char by,tablenode* Table,charset* NotEnd)	//过程函数，得到分析预测表相应项
{
	if(!exist3(ne,NotEnd))
		return NULL;
	tablenode* ta=Table->next;
	while(ta)
	{
		if(ta->ne==ne&&ta->by==by)
			return ta->to;
		ta=ta->next;
	}
	return NULL;
}
string getTransto(charset* to)				//过程函数，得到入栈字符串
{
	string temp="";
	charset* tt=to;
	while(tt)
	{
		if(tt->c!='$')
		{
			sss.str("");
			sss<<tt->c;
			temp=sss.str()+temp;
			sss.str("");
		}
		tt=tt->next;
	}
	return temp;
}
bool AnalysisString(string str,tablenode* Table,charset* NotEnd)	//字串语法分析
{
	int len=str.length();
	string str2=str;
	int len3=str2.length();
	char* sstr=(char*)str.c_str();
	string storage="#S";
	int len2=storage.length();
	printf("\n分析栈		接收字串	\n");			//输出状态至控制台，可注释
	printf("%s		%s	\n",storage.c_str(),sstr);
	int i=0;
	char* sto=(char*)storage.c_str();
	while(getTrans(sto[len2-1],sstr[0],Table,NotEnd)!=NULL&&len3!=0)
	{
		charset* temp=getTrans(sto[len2-1],sstr[0],Table,NotEnd);
		string temp2=getTransto(temp);
		char* sto3=(char*)storage.c_str();
		sss.str("");
		for(int j=0;j<len2-1;j++)
			sss<<sto3[j];
		storage=sss.str()+temp2;
		len2=storage.length();

		sto3=(char*)storage.c_str();
		char* str3=(char*)str2.c_str();
		while(sto3[len2-1]==str3[0])
		{
			printf("%s		%s	\n",storage.c_str(),str2.c_str());
			sss.str("");
			for(int k=1;k<len3;k++)
				sss<<str3[k];
			str2=sss.str();
			len3=str2.length();

			sss.str("");
			for(int l=0;l<len2-1;l++)
				sss<<sto3[l];
			storage=sss.str();
			len2=storage.length();

			sss.str("");
			if(len3!=1||len2!=1)
				printf("%s		%s	\n",storage.c_str(),str2.c_str());

			sto3=(char*)storage.c_str();
			str3=(char*)str2.c_str();
			sstr=(char*)str2.c_str();

			if(storage==""&&str2=="")
				return true;
		}
		sto=sto3;
		continue;
	}
	return false;
}

void SummaryJudge(string infile,string infile2)		//语法分析（总）
{
	string filename=infile;
	sets* x=CreateSets(filename);
	charset* NotEnd=new charset;					//终结符&非终结符
	NotEnd->c='$';
	NotEnd->next=NULL;
	charset* End=new charset;
	End->c='$';
	End->next=NULL;
	CreateCharset(NotEnd,End,x);					//终结符&非终结符初始化

	firstnode* FirstSet=new firstnode;				//First集定义
	FirstSet->cur='@';
	FirstSet->cs=NULL;
	FirstSet->next=NULL;

	firstnode* FollowSet=new firstnode;				//Follow集定义
	FollowSet->cur='@';
	FollowSet->cs=NULL;
	FollowSet->next=NULL;

	InitFiFo(FirstSet,FollowSet,filename,x,NotEnd);	//First集、Follow集初始化

	charset* se=NotEnd->next;						//输出First、Follow集

	printf("\n	First集	:	\n");					//输出状态至控制台，可注释
	firstnode* fff=FirstSet->next;
	while(fff)
	{
		se=fff->cs->next;
		printf("\nCurrnet First Start:%c\n",fff->cur);
		while(se)
		{
			printf("%c,",se->c);
			se=se->next;
		}
		printf("\n");
		fff=fff->next;
	}
	printf("\n	Follow集	:	\n");					//输出状态至控制台，可注释
	fff=FollowSet->next;
	while(fff)
	{
		se=fff->cs->next;
		printf("\nCurrent Follow Start:%c\n",fff->cur);
		while(se)
		{
			printf("%c,",se->c);
			se=se->next;
		}
		printf("\n");
		fff=fff->next;
	}

	firstnode* SelectSet=new firstnode;				//Select集定义
	SelectSet->cur='@';
	SelectSet->cs=NULL;
	SelectSet->next=NULL;

	InitSelect(SelectSet,x,FirstSet,FollowSet,NotEnd);		//建立Select集

	tablenode* Table=new tablenode;							//定义预测分析表表头
	Table->ne='@';
	Table->by='@';
	Table->to=NULL;
	Table->next=NULL;

	CreateTable(Table,SelectSet,x);							//建立预测分析表

	ifstream fin(infile2,std::ios::in);
	string s1=" ";
	while(getline(fin,s1))
	{
		printf("\n待识别串:	%s\n",s1.c_str());				//输出状态至控制台，可注释
		if(AnalysisString(s1,Table,NotEnd))					//分析字串
			printf("\n--Yes!!!--\n");
		else
			printf("\n--No!!!--\n");
	}
	fin.clear();
	fin.close(); 
}

void InitLexical(string infile,string outfile,string outfile2)		//词法分析器初始化
{
	state** s=new state*[5];
	s[0]=CreateNFA("input_keywords.txt");
	s[1]=CreateNFA("input_identifier.txt");
	s[2]=CreateNFA("input_const.txt");
	s[3]=CreateNFA("input_limiter.txt");
	s[4]=CreateNFA("input_operator.txt");
	result* H=new result,*hr=NULL;
	H->a="";
	H->type=5;
	H->next=NULL;

	getResult(s,H,infile,outfile);

	result* H2=new result,*hr2=NULL;
	H2->a="";
	H2->type=5;
	H2->next=NULL;

	hr=H->next;
	hr2=H2;
	while(hr)
	{
		result* re=new result;
		if(hr->type==0&&(hr->a=="int"||hr->a=="double"))
			re->a="p";
		else if(hr->type==0&&hr->a=="return")
			re->a="s";
		else if(hr->type==0&&hr->a=="else")
			re->a="u";
		else if(hr->type==0&&hr->a=="void")
			re->a="v";
		else if(hr->type==0)
			re->a="t";
		else if(hr->type==1)
			re->a="q";
		else if(hr->type==2)
			re->a="r";
		else
			re->a=hr->a;
		re->next=NULL;
		re->type=hr->type;
		hr2->next=re;
		hr2=hr2->next;
		hr=hr->next;
	}
	printf("\n词法结构1：	\n");		//输出状态至控制台，可注释
	sss.str("");
	hr=H->next;
	while(hr)
	{
		printf("( %s	:	%s )\n",hr->a.c_str(),types[hr->type].c_str());
		hr=hr->next;
	}
	printf("\n转换词法结构2：	\n");	//输出状态至控制台，可注释
	sss.str("");
	hr=H2->next;
	while(hr)
	{
		sss<<hr->a;
		printf("( %s	:	%s )\n",hr->a.c_str(),types[hr->type].c_str());
		hr=hr->next;
	}
	string ssd=sss.str();
	sss.str("");

	ofstream fout(outfile2,std::ios::out);
	fout.open(outfile2,std::ios::out);
	if (fout.is_open())   
	{
		fout <<"";  
		fout.clear();  
		fout.close(); 
	}
	fout.open(outfile2,std::ios::out|std::ios::app);
	if (fout.is_open())   
	{
		fout <<ssd+"#";  
		fout.clear();  
		fout.close(); 
	}
}

int main(int args, char **argv)						//主函数
{
	string infile="input_test.txt";					//词法分析
	string outfile="output.txt";
	string outfile2="input_strings.txt";
	InitLexical(infile,outfile,outfile2);

	string infile2="input_grammar.txt";				//语法分析
	string infile3="input_strings.txt";
	SummaryJudge(infile2,infile3);

	system("pause");
	return 0;
}