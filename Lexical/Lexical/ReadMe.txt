/**************************关于词法&&语法分析器的说明******************************/

一、词法分析器：
	1、源文件：“main.cpp”;
	2、输入文档：“input_keywords.txt”			//关键字产生式，eg(int、if、return)
				“input_identifier.txt”			//标识符产生式，eg(a、er、str2)
				“input_const.txt”				//常量产生式，eg(0、123、23.6)
				“input_limiter.txt”				//界符产生式，eg(;、,、})
				“input_operator.txt”			//操作符产生式，eg(!、+、&)
				“input_test.txt”				//待分析文本
	3、输出文档：“output.txt”					//输出词法分析的二元式结果
				“input_strings.txt”				//输出用于语法分析的串（来源于前者）
	4、限于正规文法分析，所以不能输入太复杂的串，关于符合要求的串，可在相应文档里查看;
	5、一般情况下不要轻易改动五个词法产生式的内容，否则可能在分析时出现不通过的情况;
	6、简单步骤说明：
		（1）、将文本读入的产生式字符串（形如“X->XX、X->X”）存入对应结构体;
		（2）、构造NFA数据结构;
		（3）、构造状态转换函数TransTo();
		（4）、构造识别函数Recognise();
		（5）、构造最终结果（二元式生成）函数getResult().
二、语法分析器：
	1、源文件：“grammar.cpp”;
	2、输入文档：“input_strings.txt”				//词法分析的结果
				“input_grammar.txt”				//语法产生式
	3、不能输入含有左递归和左公因式的产生式，要求输入符合LL(1)文法的产生式.
	4、简单步骤说明：
		（1）、将文本读入的产生式字符串（形如“X->XXXX”）存入对应结构体;
		（2）、建立终结符集链表End、非终结符集链表NotEnd;
		（3）、First集、Follow集、Select集定义及初始化;
		（4）、预测分析表Table定义及初始化;
		（5）、构造字串分析函数AnalysisString().
三、其他说明：
	1、并没有包括所有词法规则，相当于一个类C语言简单识别程序;
	2、改动输入输出要根据词法规则，并且要两个地方同时改（改动语法产生式：
	“input_grammar.txt”、改动待分析串：“input_test.txt”），其他文件会自
	动更新，所以不用管;
	3、其他说明在代码注释中也有注明，具体了解到相应文档查看即可;
	4、空字符 ε 采用'$'表示，直接用 ε 为乱码;
	5、主运行函数在“grammar.cpp”内，词法分析源文件“main.cpp”被包括在内;
	6、若只实现部分功能，到主函数（grammar::main()）里注释相应部分即可;
	7、查看中间结果到“output.txt”、“input_strings.txt”两个文档.
四、测试样例：
	1、输入串(普通样例)：
		int main()
		{
			int a=50;
			double bb=56.7;
			int cc_c=62;
			if(a==bb)
			{
				bb=cc_c;
			}
			else
			{
				int dddd,eeeee;
				dddd=34;
				eeeee=dddd;
			}
			while(!bb)
			{
				cc_c+=bb;
			}
			return 0;
		}
	2、语法产生式：（自己根据式子建了一个，说明：下式中(p、s、t、u、v)、q、r分别对应关键字、标识符、常量）
		S->pE
		S->qI
		S->{S}S
		S->srD
		S->t(L
		S->u{S}S
		S->$
		B->(){S}
		B->=r;S
		B->,q;S
		D->B
		D->;
		E->tB
		E->qB
		F->r;S
		F->q;S
		I->=F
		I->+=q;S
		J->r
		J->q
		K->)S
		K->==J)S
		L->!q)S
		L->qK
	上式可验证结果为通过.

	3、其他分类举了几个例子：
		（1.1）、语法产生式(用于检测变量定义赋值顺序的限定)：
		S->pC
		S->sr;
		S->$
		A->,qA
		A->$
		A->=rB
		B->,q=rB
		B->$
		C->t(){S}
		C->qA;S
		（1.2）、输入字串：
		int main()
		{
			int a,b,c;
			int d,e=1;
			double f,g=7.8,h=6.0;
			double i,j;
			return 0;
		}

		（2.1）、语法产生式(用于检测函数的定义)：
		S->vq(A){S}
		S->pqD
		S->t(B){S}S
		S->q=B;S
		S->$
		A->pqC
		A->$
		B->q
		B->r
		C->,pqC
		C->$
		D->(A){SsB;}
		D->=B;S
		（2.2）、输入字串：
		double function(int a,double b)
		{
			int c=1;
			double d=0;
			c=a;
			while(c)
			{
				d=b;
			}
			return d;
		}
		（3.1）、语法产生式(用于检测运算符及连续运算)：
		S->pE
		S->q=G
		S->t(H
		S->sr;S
		S->$
		A->,pqB
		A->B
		B->=C
		B->$
		C->r
		C->q
		D->OF
		D->$
		E->t(){S}
		E->qA;S
		F->rD
		F->qD
		G->qD;S
		G->rD;S
		G->!C;S
		H->qD){S}S
		H->rD){S}S
		O->+
		O->-
		O->*
		O->/
		O->&
		O->|
		O-><
		O->>
		（3.2）、输入字串：
		int main()
		{
			int a=0;
			int b=1;
			double c=2.3;
			double d=4.5; 
			a=c-d;
			b=d&c;
			a=d*c-b;
			c=1+b-5&a*4;
			d=!c;
			while(a>b)
			{
				b=a-c+d;
			}
			return 0;
		}
