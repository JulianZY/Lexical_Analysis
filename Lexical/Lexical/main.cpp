#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;
stringstream sss;									//字符流通道
string types[]={"keywords","identifier","const","limiter","operator"};	//类别数组
struct state;										//声明
struct node{										//NFA内部转换结点
	char by;
	state* to;
	node* next;
};
struct state{										//NFA状态
	char cur;
	node* head;
	state* next;
};
struct result{										//保存结果二元式
	string a;
	int type;
	result* next;
};
bool exist(state* s,char src)						//验证存在函数 for state not end
{
	state* t=s;
	while(t)
	{
		if(t->cur==src)
			return true;
		else
			t=t->next;
	}
	return false;
}
bool exist1(state* x,char s)					   //验证存在函数2 for by end
{
	state* temp=x;
	node* temp1=NULL;
	while(temp)
	{
		temp1=x->head;
		while(temp1)
		{
			if(temp1->by==s)
				return true;
			else
				temp1=temp1->next;
		}
		temp=temp->next;
	}
	return false;
}
state* CreateNFA(string filename)					//构造NFA
{
	state* end=new state;
	end->next=NULL;
	end->head=NULL;
	end->cur='Z';
	state* start=new state;
	start->next=NULL;
	start->head=NULL;
	start->cur='S';
	state *temp=NULL,*temp2=NULL,*temp3=NULL;
	node *temp4=NULL; 

	ifstream fin(filename,std::ios::in);
	char line[1000]={0};
	string s1=" ";
	while(fin.getline(line, sizeof(line)))
	{
		stringstream word(line);
		word >> s1;
		char* t=(char*)s1.c_str();
		int len=s1.length();
		if(exist(start,t[0]))
		{
			if(len==5)
			{
				temp=start;
				while(temp)
				{
					if(temp->cur==t[0])
					{
						if(exist(start,t[4]))
						{
							temp2=start;
							while(temp2)
							{
								if(temp2->cur==t[4])
								{
									node* y=new node;
									y->by=t[3];
									y->next=NULL;
									y->to=temp2;
									temp4=temp->head;
									if(temp4==NULL)
										temp->head=y;
									else
									{
										while(temp4->next)
											temp4=temp4->next;
										temp4->next=y;
									}
									break;
								}
								else
									temp2=temp2->next;
							}
						}
						else
						{
							state* x=new state;
							x->cur=t[4];
							x->head=NULL;
							x->next=NULL;
							temp3=start;
							while(temp3->next)
								temp3=temp3->next;
							temp3->next=x;
							node* y=new node;
							y->by=t[3];
							y->next=NULL;
							y->to=x;
							temp4=temp->head;
							if(temp4==NULL)
								temp->head=y;
							else
							{
								while(temp4->next)
									temp4=temp4->next;
								temp4->next=y;
							}
						}
						break;
					}
					else
						temp=temp->next;
				}
			}
			else
			{
				temp=start;
				while(temp)
				{
					if(temp->cur==t[0])
					{
						node* y=new node;
						y->by=t[3];
						y->next=NULL;
						y->to=end;
						temp4=temp->head;
						if(temp4==NULL)
							temp->head=y;
						else
						{
							while(temp4->next)
								temp4=temp4->next;
							temp4->next=y;
						}
						break;
					}
					else
						temp=temp->next;

				}
			}
		}
		else
		{
			state* m=new state;
			m->cur=t[0];
			m->head=NULL;
			m->next=NULL;
			temp=start;
			while(temp->next)
				temp=temp->next;
			temp->next=m;
			temp=temp->next;
			if(len==5)
			{
				if(exist(start,t[4]))
				{
					temp2=start;
					while(temp2)
					{
						if(temp2->cur==t[4])
						{
							node* y=new node;
							y->by=t[3];
							y->next=NULL;
							y->to=temp2;
							temp4=temp->head;
							if(temp4==NULL)
								temp->head=y;
							else
							{
								while(temp4->next)
									temp4=temp4->next;
								temp4->next=y;
							}
							break;
						}
						else
							temp2=temp2->next;
					}
				}
				else
				{
					state* x=new state;
					x->cur=t[4];
					x->head=NULL;
					x->next=NULL;
					temp3=start;
					while(temp3->next)
						temp3=temp3->next;
					temp3->next=x;
					node* y=new node;
					y->by=t[3];
					y->next=NULL;
					y->to=x;
					temp4=temp->head;
					if(temp4==NULL)
						temp->head=y;
					else
					{
						while(temp4->next)
							temp4=temp4->next;
						temp4->next=y;
					}

				}
			}
			else
			{
				node* y=new node;
				y->by=t[3];
				y->next=NULL;
				y->to=end;
				temp4=temp->head;
				if(temp4==NULL)
					temp->head=y;
				else
				{
					while(temp4->next)
						temp4=temp4->next;
					temp4->next=y;
				}
			}
		}
	}
	fin.clear();
	fin.close(); 
	return start;
}
state* TransTo1(char input,state* current)				//状态转换函数（旧）
{
	state* temp=current;
	node* temp2=current->head;
	while(temp2)
	{
		if(temp2->by==input)
		{
			temp=temp2->to;
			return temp;
		}
		else
			temp2=temp2->next;
	}
	return NULL;
}

state* TransTo2(char input,char next,state* current)				//状态转换函数 for 1,2,3,4
{
	state* temp=current;
	node* temp2=current->head;
	node* temp3=NULL;
	int tag=0;
	while(temp2)
	{
		if(temp2->by==next)
		{
			tag=1;
			break;
		}
		else
			temp2=temp2->next;
	}
	temp2=current->head;
	while(temp2)
	{
		if(temp2->by==input)
		{
			temp3=temp2->next;
			while(temp3)
			{
				if(temp3->by==input)
					break;
				else
					temp3=temp3->next;
			}
			break;
		}
		else
			temp2=temp2->next;
	}
	if(temp2&&temp3)
	{
		if(temp2->to->cur=='Z')
		{
			if(tag==1)
				temp=temp3->to;
			else
				temp=temp2->to;
		}
		else
		{
			if(tag==1)
				temp=temp2->to;
			else
				temp=temp3->to;
		}
		return temp;
	}
	else if(temp2!=NULL)
	{
		temp=temp2->to;
		return temp;
	}
	else
		return NULL;
}

state* TransTo3(char input,char next,state* current,state** s)				//状态转换函数,for 0
{
	state* temp=current,*temp3=NULL;
	node* temp1=current->head,*temp4=NULL;
	while(temp1)
	{
		if(temp1->by==input)
		{
			temp3=temp1->to;
			if(temp3->cur!='Z')
			{
				temp4=temp3->head;
				while(temp4)
				{
					if(temp4->by==next)
						return temp3;
					else
						temp4=temp4->next;
				}
				return NULL;
			}
			else
			{
				if(exist1(s[3],next)||exist1(s[4],next)||exist1(s[2],next)||next==' '||next==NULL)
					return temp3;
				else
					return NULL;
			}
		}
		else
			temp1=temp1->next;
	}
	return NULL;
}

int Recognise(state** s,int from,int to,char* src,result* r)		//串识别
{
	int f=from;
	if(f>=to)
		return -1;
	result* res=r;
	while(res->next)
		res=res->next;
	if(f<to)
	{	
		state* temp=NULL,*temp3=NULL;
		node* temp2=NULL;
		for(int i=0;i<5;i++)
		{
			temp=s[i];
			if(TransTo2(src[f],src[f+1],temp)==NULL)
				continue;
			else
			{
				temp3=temp;
				if(i==0)
				{
					while(temp3)
					{
						temp3=TransTo3(src[f],src[f+1],temp3,s);
						if(temp3!=NULL)
						{
							sss<<src[f];
							if(temp3->cur=='Z')
								break;
						}
						++f;
					}
					if(temp3!=NULL&&temp3->cur=='Z')
					{
						string re=sss.str();
						sss.str("");
						result* R=new result;
						R->a=re;
						R->type=0;
						R->next=NULL;
						res->next=R;
						return f+1;
					}
					else 
					{
						sss.str("");
						f=from;
						continue;
					}
				}
				else
				{
					if(i==1)
					{
						if(exist1(s[2],src[f]))
							continue;
					}
					while(temp3)
					{
						temp3=TransTo2(src[f],src[f+1],temp3);
						if(temp3!=NULL)
							sss<<src[f];
						++f;
					}
					string re=sss.str();
					sss.str("");
					result* R=new result;
					R->a=re;
					R->type=i;
					R->next=NULL;
					res->next=R;
					return f-1;
				}
			}
		}
		return f+1;
	}
}
void RecogLine(state** s,result* r,string line)					//行识别
{
	int len=line.length(),tag;
	char* sr=(char*)line.c_str();
	tag=Recognise(s,0,len,sr,r);
	while(tag!=-1)
	{
		tag=Recognise(s,tag,len,sr,r);
	}
}
void getResult(state** s,result* r,string infile,string outfile)		//最终结果
{
	ifstream fin(infile,std::ios::in);
	ofstream fout(outfile,std::ios::out);
	string s1=" ";
	while(getline(fin,s1))
		RecogLine(s,r,s1);
	fin.clear();
	fin.close(); 

	result* hr=r->next;
	fout.open(outfile,std::ios::out);
	if (fout.is_open())   
	{
		fout <<"";  
		fout.clear();  
		fout.close(); 
	}
	while(hr)
	{
		fout.open(outfile,std::ios::out|std::ios::app);
		if (fout.is_open())   
		{  
			string te="( "+hr->a+" : "+types[hr->type]+" )\n";
			fout << te;  
			fout.clear();  
			fout.close(); 
		}  
		hr=hr->next;
	}
}
/*int main(int args, char **argv)
{
	int i,j,tag=0,count=0;
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
	string infile="input_test.txt";
	string outfile="output.txt";
	getResult(s,H,infile,outfile);
	hr=H->next;
	while(hr)
	{
		printf("( %s : %s )\n",hr->a.c_str(),types[hr->type].c_str());
		hr=hr->next;
	}
	system("pause");
	for(i=0;i<5;i++)
		delete s[i];
	delete s;
	delete H;
	return 0;
}*/