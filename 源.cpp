//魔方及降群法还原算法的实现
//刘洪飞
//参考https://www.stefan-pochmann.info/spocc/other_stuff/tools/solver_thistlethwaite/solver_thistlethwaite_cpp.txt

#include "stdio.h"
#include "conio.h"
#include "math.h"
#include "stdlib.h"
#include "time.h" 
#include <cstdlib>
#include <iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include"Windows.h"
#include "graphics.h"
#include "easyx.h"
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <stack>
using namespace std;
#define ORANGE 0x00A5FF
#define MAX 10000
char turn[MAX] = {'1'};

queue<char>solution;//解决方案


//class FileException 
void outfile();
void infile();

void attachment(int , int );
void coloring(int , int );



typedef vector<int> cubestate;


int col[6][9] = {0};//颜色序列,最后一位为中心颜色


int applicableMoves[] = {  /*0,262143, 259263, 74943, 74898*/0,10,10,10,10 };//每一阶段的上限



int affectedCubies[][8] =
{

	{ 0, 1, 2, 3, 0, 1, 2, 3 },   // U

	{ 4, 7, 6, 5, 4, 5, 6, 7 },   // D

	{ 0, 9, 4, 8, 0, 3, 5, 4 },   // F

	{ 2, 10, 6, 11, 2, 1, 7, 6 }, // B

	{ 3, 11, 7, 9, 3, 2, 6, 5 },  // L

	{ 1, 8, 5, 10, 1, 0, 4, 7 },  // R

};

//对每一个块编码，前四位棱块，后四位角块

int phase;//表示阶段，具体可查阅降群法

//在phase=2时，控制FB面只能旋转180°即降群到<U,D,F2,B2,L,R>

//在phase=3时，控制FBLR面只能旋转180°即降群到<U,D,F2,B2,L2,R2>

//在phase=4时，控制UDFBLR面只能旋转180°即降群到<U2,D2,F2,B2,L2,R2>


class Spaces//魔方的一个面
{
public:

	int color;
	int dimension;
	int** matrix;

	Spaces(int b = 0, int c = 0)
	{
		color = c;
		dimension = b;
		matrix = new int*[dimension];
			
		for ( int i = 0; i < dimension; i++)
		{
			matrix[i] = new int[dimension];
			for (int j = 0; j < dimension; j++)
			{
				matrix[i][j] = color;
			}
		}
	}

	void print()
	{
		//cleardevice();
	for (int i = 0; i < dimension; i++)
		{
			for (int j = 0; j < dimension; j++)
				cout << matrix[i][j] << ' ';
			cout << endl;
		}

	}


	~Spaces()
	{
		for (int i = 0; i < dimension; i++)
		{
			if (matrix[i])
			{
				delete  matrix[i];
				matrix[i] = 0;
			}
		}
		if (matrix)
		{
			delete[]matrix; matrix = 0;
		}
	
	}
};

class Cube//魔方类
{
public:
	int dimension;
	Spaces  **Space;

	Cube(int b)
	{
		dimension = b;
		Space = new Spaces*[6];

		for (int i = 0; i < 6; i++)
		{
			Space[i] = new Spaces(dimension, i);
		}
	}

/*	Spaces*& operator[](int i)
	{
		return Space[i];
	}
*/
	virtual void F()=0;
	virtual void F2()=0;
	virtual void F3()=0;
	virtual void B()=0;
	virtual void B2()=0;
	virtual void B3()=0;
	virtual void U()=0;
	virtual void U2()=0;
	virtual void U3()=0;
	virtual void R3()=0;
	virtual void R2()=0;
	virtual void R()=0;
	virtual void D3()=0;
	virtual void D2()=0;
	virtual void D()=0;
	virtual void L()=0;
	virtual void L2()=0;
	virtual void L3()=0;

	virtual	void clockwise(int i) = 0;
	virtual	void anticlockwise(int i) = 0;
	virtual void Turnanticlockwise() = 0;
	virtual void Turnclockwise() = 0;
	virtual void Left() = 0;
	virtual void Right() = 0;
	virtual	void Down() = 0;
	virtual void Up() = 0;

	virtual void disorganize() = 0;
	virtual void CubePrint() = 0;

	

	void fill(int i, int j, int k)
	{
		switch (Space[i]->matrix[j][k])

		{
		case 0:setfillcolor(RED); break;
		case 1:setfillcolor(WHITE); break;
		case 2:setfillcolor(ORANGE); break;
		case 3:setfillcolor(YELLOW); break;
		case 4:setfillcolor(GREEN);  break;
		case 5:setfillcolor(BLUE); break;


		default:break;

		}
	}


	virtual~Cube() 
	{
		for (int i = 0; i < 6; i++)
		{
			if (Space[i]) 
			{ 
				delete  Space[i];
				Space[i] = 0;
			}
		}
		if (Space) 
		{ 
			delete[]Space; Space = 0; 
		}
	}

};

class Cube3/**/ :public Cube//三维魔方
{
public:


	Cube3(int b = 3) :Cube(b)
	{

	}

	void F();
	void F2();
	void F3();
	void B();
	void B2();
	void B3();
	void U();
	void U2();
	void U3();
	void R3();
	void R2();
	void R();
	void D3();
	void D2();
	void D();
	void L();
	void L2();
	void L3();

	void clockwise(int i);
	void anticlockwise(int i);

	void Turnanticlockwise();
	void Turnclockwise();
	void Left();
	void Right();
	void Down();
	void Up();

	void disorganize();
	void CubePrint();
	void operation(int i);
	void printcube();


	void getcol(int*col, int i);


};

void Cube3::clockwise(int i)//平面顺时针

{
	int a, b;
	a = Space[i]->matrix[0][0];
	b = Space[i]->matrix[1][0];
	Space[i]->matrix[0][0] = Space[i]->matrix[2][0];
	Space[i]->matrix[2][0] = Space[i]->matrix[2][2];
	Space[i]->matrix[2][2] = Space[i]->matrix[0][2];
	Space[i]->matrix[0][2] = a;
	Space[i]->matrix[1][0] = Space[i]->matrix[2][1];
	Space[i]->matrix[2][1] = Space[i]->matrix[1][2];
	Space[i]->matrix[1][2] = Space[i]->matrix[0][1];
	Space[i]->matrix[0][1] = b;
}

void Cube3::anticlockwise(int i)//平面逆时针

{
	clockwise(i);
	clockwise(i);
	clockwise(i);
}

void Cube3::F()//前面右转
{	
	Right();
	R();
    Left();

}

void Cube3::F2()//前面180
{
	F();
	F();

}

void Cube3::F3()//前面左转
{
	F();
	F();
	F();
}

void Cube3::B()//后面左转
{
	Left();
	R();
	Right();
}

void Cube3::B2()//后面180
{
	B();
	B();

}

void Cube3::B3()//后面左转
{
	B();
	B();
	B();
}

void Cube3::D3()//下面一层左转
{
	Turnanticlockwise();
	Turnanticlockwise();
	U3();
	Turnanticlockwise();
	Turnanticlockwise();

}

void Cube3::D2()//下面一层180
{
	D();
	D();

}

void Cube3::D()//下面一层右转
{
	Turnanticlockwise();
	Turnanticlockwise();
	U();
	Turnanticlockwise();
	Turnanticlockwise();
}

void Cube3::L()//最左边下转

{
	Turnanticlockwise();
	Turnanticlockwise();
	R();
	Turnanticlockwise();
	Turnanticlockwise();
}

void Cube3::L2()//最左边180

{
	L();
	L();
	
}

void Cube3::L3()//最右边上转
{
	Turnanticlockwise();
	Turnanticlockwise();
	R3();
	Turnanticlockwise();
	Turnanticlockwise();
}

void Cube3::R()//正面最右边转上

{
	int i;
	int a, b, c;
	a = Space[0]->matrix[0][2];
	b = Space[0]->matrix[1][2];
	c = Space[0]->matrix[2][2];
	for (i = 0; i < 3; i++)
		Space[0]->matrix[i][2] = Space[5]->matrix[i][2];
	for (i = 0; i < 3; i++)
		Space[5]->matrix[i][2] = Space[2]->matrix[2 - i][0];
	for (i = 0; i < 3; i++)
		Space[2]->matrix[i][0] = Space[4]->matrix[2 - i][2];
	Space[4]->matrix[0][2] = a;
	Space[4]->matrix[1][2] = b;
	Space[4]->matrix[2][2] = c;
	clockwise(1);

}

void Cube3::R2()//正面最右边转下

{
	R();
	R();

}
void Cube3::R3()//正面最右边转下

{
	R();
	R();
	R();
}

void Cube3::U()//正面第一层左转

{

	int a, b, c;
	a = Space[0]->matrix[0][0];
	b = Space[0]->matrix[0][1];
	c = Space[0]->matrix[0][2];
	int i;
	for (i = 0; i < 3; i++)
		Space[0]->matrix[0][i] = Space[1]->matrix[0][i];
	for (i = 0; i < 3; i++)
		Space[1]->matrix[0][i] = Space[2]->matrix[0][i];
	for (i = 0; i < 3; i++)
		Space[2]->matrix[0][i] = Space[3]->matrix[0][i];

	Space[3]->matrix[0][0] = a;
	Space[3]->matrix[0][1] = b;
	Space[3]->matrix[0][2] = c;
	clockwise(4);

}

void Cube3::U2()//正面第一层右转

{
	U();
	U();

}
void Cube3::U3()//正面第一层右转

{
	U();
	U();
	U();
}

void Cube3::Up()//整体向上翻
{
	int a, b, c;
	for (int y = 0; y < 3; y++)
	{

		a = Space[0]->matrix[0][y];
		b = Space[0]->matrix[1][y];
		c = Space[0]->matrix[2][y];
		int i;
		for ( i = 0; i < 3; i++)
			Space[0]->matrix[i][y] = Space[5]->matrix[i][y];
		for (i = 0; i < 3; i++)
			Space[5]->matrix[i][y] = Space[2]->matrix[2 - i][2 - y];
		for (i = 0; i < 3; i++)
			Space[2]->matrix[i][2 - y] = Space[4]->matrix[2 - i][y];

		Space[4]->matrix[0][y] = a;
		Space[4]->matrix[1][y] = b;
		Space[4]->matrix[2][y] = c;
	}

	clockwise(1);
	anticlockwise(3);
}

void Cube3::Down()//整体向下翻
{
	Up();
	Up();
	Up();
}

void Cube3::Left()//整体向左翻
{
	int a, b, c;
	for (int x = 0; x < 3; x++)

	{
		a = Space[0]->matrix[x][0];
		b = Space[0]->matrix[x][1];
		c = Space[0]->matrix[x][2];
		int i;
		for (i = 0; i < 3; i++)
			Space[0]->matrix[x][i] = Space[1]->matrix[x][i];
		for (i = 0; i < 3; i++)
			Space[1]->matrix[x][i] = Space[2]->matrix[x][i];
		for (i = 0; i < 3; i++)
			Space[2]->matrix[x][i] = Space[3]->matrix[x][i];

		Space[3]->matrix[x][0] = a;
		Space[3]->matrix[x][1] = b;
		Space[3]->matrix[x][2] = c;

	}

	clockwise(4);
	anticlockwise(5);
}

void Cube3::Right()//整体向右翻
{
	Left();
	Left();
	Left();
}

void Cube3::Turnanticlockwise()//整体逆时针
{

	Right();
	Down();
	Left();


}

void Cube3::Turnclockwise()//整体顺时针
{
	Turnanticlockwise();
	Turnanticlockwise();
	Turnanticlockwise();

}


void Cube3::disorganize()//打乱魔方

{
	srand(static_cast<unsigned int>(time(0)));
	int a;


	for (int p = 0; p < 100; p++)
	{
		a = rand() % 10;
		switch (a)

		{
		case 0:Up(); break;
		case 1:Down(); break;
		case 2:Right(); break;
		case 3:Left(); break;
		case 4:U(); break;
		case 5:D(); break;
		case 6:R(); break;
		case 7:F(); break;
		case 8:L(); break;
		case 9:B(); break;

		}

	}


}


void infile()//存储
{
	ifstream infile;
	infile.open("data1.txt");
	if (!infile)
	{
		cout << "Cannot open the file" << endl;
		exit(1);
	}
	char *a = &turn[0];
	for (int ig = 0; ig < 10000; ig++)
	{
		infile >> *a;
		a++;
	}

	infile.close();
}

void outfile()//载入
{

	ofstream outfile;
	outfile.open("data1.txt");
	/*
	try
	*/
	if (!outfile)
	{
		cout << "Cannot creat the file" << endl;
		exit(1);
	}
	char *a = &turn[0];
	for (int ig = 0; ig < 1000000; ig++)
	{
		outfile << *a << endl;
		a++;
	}

	outfile.close();
	Sleep(1200);

}

/**/
void Cube3::CubePrint()//打印魔方

{
	for (int s = 0; s < 6; s++)

	{

		switch (s)

		{

		case 0:cout << " 正面:" << endl; break;

		case 1:cout << " 右面:" << endl; break;

		case 2:cout << " 后面:" << endl; break;

		case 3:cout << " 左面:" << endl; break;

		case 4:cout << " 上面:" << endl; break;

		case 5:cout << " 下面:" << endl; break;

		default:break;

		}
		Space[s][0].print();

	}

	cout << endl;

	cout << "---------------------------------------" << endl;

	Sleep(100);

}

cubestate applyMove(int move, cubestate state)//对状态进行操作

{

	int turns = move % 3 + 1;// 旋转90度的次数

	int face = move / 3;//旋转哪一个面



	while (turns--)
	{

		cubestate oldState = state;



		for (int i = 0; i < 8; i++)
		{
			/*在旋转过程中分别对8个棱块和8个角块的方向进行赋值*/
			int isCorner = i > 3;
			//判断是否是角块

			int target = affectedCubies[face][i] + isCorner * 12;
			//计算目标值

			int killer = affectedCubies[face][(i & 3) == 3 ? i - 3 : i + 1] + isCorner * 12;
			//将面按顺序的下一个棱块或者角块的值取出来

			int orientationDelta = (i < 4) ? (face > 1 && face < 4) : (face < 2) ? 0 : 2 - (i & 1);
			//顺时针旋转后方向改变量

			state[target] = oldState[killer];

			//用后一个替换前一个，完成顺时针旋转

			state[target + 20] = oldState[killer + 20] + orientationDelta;
			//记录旋转后方向的值


			if (!turns)

				state[target + 20] %= 2 + isCorner;

			/*防止方向值超过270*/
		}

	}

	return state;

}



int inverse(int move)/*用于返回move的逆动作*/

{

	return move + 2 - 2 * (move % 3);

}



/*取出输入状态的方向的值id*/
cubestate id(cubestate state)

{


	//第一步：棱块取向


	if (phase < 2)

		return cubestate(state.begin() + 20, state.begin() + 32);
	//返回输入状态的棱块的取向，共12位，0表示方向正确，1表示方向错误（即翻转了180°）

		//第二步：角块方向，中间层棱块

	if (phase < 3)
	{

		cubestate result(state.begin() + 31, state.begin() + 40);
		//取角块的方向值
		for (int e = 0; e < 12; e++)

			result[0] |= (state[e] / 8) << e;
		// result[0]用于中间层棱块的位置（用二进制表示）
		return result;
		//返回角块的方向和棱块的位置（result[0]）
	}



	//第三步：M层S层的棱块，对应角块呈现正四面体型

	if (phase < 4)
	{

		cubestate result(3);

		for (int e = 0; e < 12; e++)

			result[0] |= ((state[e] > 7) ? 2 : (state[e] & 1)) << (2 * e);
		//result[0]用24位存12个棱块位置正确，
		for (int c = 0; c < 8; c++)

			result[1] |= ((state[c + 12] - 12) & 5) << (3 * c);
		//result[1]用24位存放8个角块的位置
		for (int i = 12; i < 20; i++)

			for (int j = i + 1; j < 20; j++)

				result[2] ^= state[i] > state[j];
		//result[2]=0表示角块方向正确，result[2]=1表示角块方向错误
		return result;

	}

	//剩余状态

	return state;

}



void Cube3:: getcol(int*col,int i)
{
	col[0] = Space[i]->matrix[0][0];
	col[1] = Space[i]->matrix[0][1];
	col[2] = Space[i]->matrix[0][2];
	col[3] = Space[i]->matrix[1][0];
	col[4] = Space[i]->matrix[1][2];
	col[5] = Space[i]->matrix[2][0];
	col[6] = Space[i]->matrix[2][1];
	col[7] = Space[i]->matrix[2][2];
	col[8] = Space[i]->matrix[1][1];

}

void solve(Cube3* a)
{
	setfillcolor(WHITE);
	solidrectangle(600, 600, 900, 200);
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	RECT r = { 600, 600, 900, 200 };
	drawtext(_T("请等待"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	char goal1[12][2];
	char goal2[8][3];
	//目标序列
	char cubie1[12][2];
	char cubie2[8][3];
	//实际序列

	//定义实际魔方棱块和角块的位置

	for (int i = 0; i < 6; i++)
	{
		a->getcol(col[i],i);
	}


	cubie1[0][0] = (char)col[4][6];
	cubie1[1][0] = (char)col[4][4];
	cubie1[2][0] = (char)col[4][1];
	cubie1[3][0] = (char)col[4][3];
	cubie1[4][0] = (char)col[5][1];
	cubie1[5][0] = (char)col[5][4];
	cubie1[6][0] = (char)col[5][6];
	cubie1[7][0] = (char)col[5][3];
	cubie1[8][0] = (char)col[0][4];
	cubie1[9][0] = (char)col[0][3];
	cubie1[10][0] = (char)col[2][3];
	cubie1[11][0] = (char)col[2][4];

	cubie1[0][1] = (char)col[0][1];
	cubie1[1][1] = (char)col[1][1];
	cubie1[2][1] = (char)col[2][1];
	cubie1[3][1] = (char)col[3][1];
	cubie1[4][1] = (char)col[0][6];
	cubie1[5][1] = (char)col[1][6];
	cubie1[6][1] = (char)col[2][6];
	cubie1[7][1] = (char)col[3][6];
	cubie1[8][1] = (char)col[1][3];
	cubie1[9][1] = (char)col[3][4];
	cubie1[10][1] = (char)col[1][4];
	cubie1[11][1] = (char)col[3][3];

	cubie2[0][0] = (char)col[4][7];
	cubie2[0][1] = (char)col[0][2];
	cubie2[0][2] = (char)col[1][0];

	cubie2[1][0] = (char)col[4][2];
	cubie2[1][1] = (char)col[1][2];
	cubie2[1][2] = (char)col[2][0];

	cubie2[2][0] = (char)col[4][0];
	cubie2[2][1] = (char)col[2][2];
	cubie2[2][2] = (char)col[3][0];

	cubie2[3][0] = (char)col[4][5];
	cubie2[3][1] = (char)col[3][2];
	cubie2[3][2] = (char)col[0][0];

	cubie2[4][0] = (char)col[5][2];
	cubie2[4][1] = (char)col[1][5];
	cubie2[4][2] = (char)col[0][7];

	cubie2[5][0] = (char)col[5][0];
	cubie2[5][1] = (char)col[0][5];
	cubie2[5][2] = (char)col[3][7];

	cubie2[6][0] = (char)col[5][5];
	cubie2[6][1] = (char)col[3][5];
	cubie2[6][2] = (char)col[2][7];

	cubie2[7][0] = (char)col[5][7];
	cubie2[7][1] = (char)col[2][5];
	cubie2[7][2] = (char)col[1][7];
	//定义目标魔方棱块和角块的位置


	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			col[i][j]=col[i][8];
		}
	}

	goal1[0][0] = (char)col[4][6];
	goal1[1][0] = (char)col[4][4];
	goal1[2][0] = (char)col[4][1];
	goal1[3][0] = (char)col[4][3];
	goal1[4][0] = (char)col[5][1];
	goal1[5][0] = (char)col[5][4];
	goal1[6][0] = (char)col[5][6];
	goal1[7][0] = (char)col[5][3];
	goal1[8][0] = (char)col[0][4];
	goal1[9][0] = (char)col[0][3];
	goal1[10][0] = (char)col[2][3];
	goal1[11][0] = (char)col[2][4];

	goal1[0][1] = (char)col[0][1];
	goal1[1][1] = (char)col[1][1];
	goal1[2][1] = (char)col[2][1];
	goal1[3][1] = (char)col[3][1];
	goal1[4][1] = (char)col[0][6];
	goal1[5][1] = (char)col[1][6];
	goal1[6][1] = (char)col[2][6];
	goal1[7][1] = (char)col[3][6];
	goal1[8][1] = (char)col[1][3];
	goal1[9][1] = (char)col[3][4];
	goal1[10][1] = (char)col[1][4];
	goal1[11][1] = (char)col[3][3];

	goal2[0][0] = (char)col[4][7];
	goal2[0][1] = (char)col[0][2];
	goal2[0][2] = (char)col[1][0];

	goal2[1][0] = (char)col[4][2];
	goal2[1][1] = (char)col[1][2];
	goal2[1][2] = (char)col[2][0];

	goal2[2][0] = (char)col[4][0];
	goal2[2][1] = (char)col[2][2];
	goal2[2][2] = (char)col[3][0];

	goal2[3][0] = (char)col[4][5];
	goal2[3][1] = (char)col[3][2];
	goal2[3][2] = (char)col[0][0];

	goal2[4][0] = (char)col[5][2];
	goal2[4][1] = (char)col[1][5];
	goal2[4][2] = (char)col[0][7];

	goal2[5][0] = (char)col[5][0];
	goal2[5][1] = (char)col[0][5];
	goal2[5][2] = (char)col[3][7];

	goal2[6][0] = (char)col[5][5];
	goal2[6][1] = (char)col[3][5];
	goal2[6][2] = (char)col[2][7];

	goal2[7][0] = (char)col[5][7];
	goal2[7][1] = (char)col[2][5];
	goal2[7][2] = (char)col[1][7];






	cubestate currentState(40), goalState(40);
	/**/
	/* 将当前魔方状态输入到数表currentState里，
由字符串顺序转化到数字顺序，字符串顺序包涵了位置和方向，
数字顺序用一位表示位置，一位表示方向。
规则为：0-11/12-19存按goal里编号棱块/角块的数字位置，
20-39存棱块和角块的方向
棱块如需翻转，则20-31对应位置为1；
角块如需旋转，则顺时针旋转90°记为1，顺时针旋转180°记为2*/
/*
*/
	for (int i = 0; i < 20; i++) 
	{

		goalState[i] = i; //初始化目标魔方各个棱块和角块的位置


	}

	for (int i = 0; i < 12; i++)
	{
		//和目标魔方块的位置比较，块需要顺时针旋转90°或者棱块翻转180°
		while ((currentState[i] = find(goal1, goal1 + 12, cubie1[i]) - goal1) == 20) 
		{
			
			char temp= cubie1[i][1];
			cubie1[i][1] = cubie1[i][0] ;
			cubie1[i][0] = cubie1[i][1];

			currentState[i + 20]++;
		

		}

	}

	for (int i = 0; i < 8; i++)
	{
		//和目标魔方块的位置比较，块需要顺时针旋转90°或者棱块翻转180°
		while ((currentState[i+12] = find(goal2, goal2 + 8, cubie2[i]) - goal2) == 20)
		{
			char temp = cubie2[i][0];
			cubie2[i][0] = cubie2[i][1];
			cubie2[i][1] = cubie2[i][2];
			cubie2[i][2] = temp;

			currentState[i +32]++;
			//记录到方向，回到正确位置需要顺时针旋转180°为2，顺时针旋转90°为1

		}

	}


	while (++phase < 5)
	{



		//计算当前和目标状态的方向取值id，如果相等则跳过
		cubestate currentId = id(currentState), goalId = id(goalState);

		if (currentId == goalId)

			continue;

		//初始化广度优先队列

		queue<cubestate> q; //定义一个队列q，用于存放不同状态

		q.push(currentState); //push入队，当前状态表入队

		q.push(goalState);//push入队，目标状态表入队



		//初始化BFS算法的图表 map通过平衡二叉树对节点进行存储

		map<cubestate, cubestate> predecessor;
		//旋转前后的状态表存进predecessor，旋转后的表前面出现过则不存（即状态等价不存）

		map<cubestate, int> direction, lastMove;
		//direction：存放不同状态的方向，该状态由输入魔方旋转得到，则关键字为1；有目标魔方旋转得到，关键字为2
		//lastMove：将旋转后的方向值存入并记录当时的move值(即旋转的方式）

		direction[currentId] = 1;

		direction[goalId] = 2;



		// BFS

		while (1)
		{



			//从队列获取状态，计算它的ID并得到它的方向
			cubestate oldState = q.front();

			q.pop();

			cubestate oldId = id(oldState);

			int& oldDir = direction[oldId];



			//将所有适用的动作(每个面旋转90.180.270)应用到它并记录新的状态
			for (int move = 0; move < 18; move++)
			{

				if (applicableMoves[phase] & (1 << move))
				{


					//--- Apply the move.（//旋转）
					cubestate newState = applyMove(move, oldState);
					//旋转后的状态

					cubestate newId = id(newState);
					//旋转后的状态各个棱块和角块的方向
					int& newDir = direction[newId];
					//拥有新方向的状态是否出现过，是，则返回关键字给newDir；否，则以关键字为0存入direction




					//判断是否能和关键字为2的状态联系起来，如果能，则找到解法，否，则继续搜索

					if (newDir && newDir != oldDir)
					{



						//使oldId表示向前搜索状态，newId表示向后搜索状态

						if (oldDir > 1)
						{

							swap(newId, oldId);

							move = inverse(move);

						}



						//重现联系这两个状态的步骤move

						cubestate algorithm(1, move);
						//用于存放步骤
						while (oldId != currentId)
						{
							//在predecessor表里查找oldId==currentId，并记录需要的步骤到algorithm（算法）

							//即输入魔方按照algorithm的步骤旋转就可到达目标魔方旋转后的状态（即联系direction的关键字1和2）
							algorithm.insert(algorithm.begin(), lastMove[oldId]);

							oldId = predecessor[oldId];

						}

						while (newId != goalId)
						{
							//还原到目标魔方状态需要转动的步骤
							algorithm.push_back(inverse(lastMove[newId]));

							newId = predecessor[newId];

						}

						//存取解决方案

						for (int i = 0; i < (int)algorithm.size(); i++)
						{



							solution.push("UDFBLR"[algorithm[i] / 3]);
							solution.push(algorithm[i] % 3 + 1);

							//存取需要旋转的面和角度，1.2.3表示顺时针旋转90.180.270
							currentState = applyMove(algorithm[i], currentState);
							//旋转后的值赋给currentState（当前值）
						}


						goto nextPhase;
						//进入下一步
					}



					//如果以前从未见过此状态(id)，访问它

					if (!newDir)
					{

						q.push(newState);

						newDir = oldDir;

						lastMove[newId] = move;

						predecessor[newId] = oldId;

					}

				}

			}

		}

	nextPhase:;

	}
	


char* sol=new char[solution.size() + 1];

int num = 0;

while (!solution.empty())
{
	sol[num]=solution.back();
	solution.pop();
	num++;
}
sol[num] = NULL;

setfillcolor(WHITE);
solidrectangle(600, 600, 900, 200);

RECT m = { 600, 600, 900, 200 };
drawtext(sol , &m, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

delete[]sol;
/**/
}


void Cube3::operation(int i)//游戏操作
{
	switch (turn[i])

	{
	case'W':Up(); break;
	case'S':Down(); break;
	case'D':Right(); break;
	case'A':Left(); break;
	case'Q':Turnclockwise(); break;
	case'E':Turnanticlockwise(); break;
	case'w':R(); break;
	case'h':R2(); break;
	case's':R3(); break;
	case'a':U(); break;
	case't':U2(); break;
	case'd':U3(); break;
	case'g':B(); break;
	case'j':B2(); break;
	case'k':B3(); break;
	case'l':F(); break;
	case'z':F2(); break;
	case'x':F3(); break;
	case'q':D3(); break;
	case'y':D2(); break;
	case'e':D(); break;
	case'r':L3(); break;
	case'i':L2(); break;
	case'f':L(); break;
	case'p':disorganize(); break;
	case'm':outfile(); break;
	case'n':solve(this); break;
		//case'o':CubePrint(); break;
	case'0':exit(1); break;
	default:break;

	}
}

void attachment(int left, int up)//连线
{
	rectangle(left, up, left + 30, up - 30);
}

void coloring(int left, int up)//上色
{
	solidrectangle(left, up, left + 30, up - 30);
}



void Cube3:: printcube()//绘制魔方图形
{
	setlinecolor(BLACK);
	for (int i = 100; i <= 300; i = i + 100)
	{
		for (int j = 325; j <= 525; j = j + 100)
		{
			fill(0, (j - 325) / 100, (i - 100) / 100);

			solidrectangle(i, j, i + 100, j - 100);

			rectangle(i, j, i + 100, j - 100);
		}
	}
	//正边涂色

	fill(4, 0, 0);
	POINT up11[] = { {200, 50}, {300, 50} , {270, 100}, {170, 100} };
	solidpolygon(up11, 4);
	fill(4, 0, 1);
	POINT up12[] = { {300, 50}, {400, 50} , {370, 100}, {270, 100} };
	solidpolygon(up12, 4);
	fill(4, 0, 2);
	POINT up13[] = { {400, 50}, {500, 50} , {470, 100}, {370, 100} };
	solidpolygon(up13, 4);
	fill(4, 1, 0);
	POINT up21[] = { {170, 100}, {270, 100} , {240, 150}, {140, 150} };
	solidpolygon(up21, 4);
	fill(4, 1, 1);
	POINT up22[] = { {270, 100}, {370, 100} , {340, 150}, {240, 150} };
	solidpolygon(up22, 4);
	fill(4, 1, 2);
	POINT up23[] = { {370, 100}, {470, 100} , {440, 150}, {340, 150} };
	solidpolygon(up23, 4);
	fill(4, 2, 0);
	POINT up31[] = { {140, 150}, {240, 150} , {200, 225}, {100, 225} };
	solidpolygon(up31, 4);
	fill(4, 2, 1);
	POINT up32[] = { {240, 150}, {340, 150} , {300, 225}, {200, 225} };
	solidpolygon(up32, 4);
	fill(4, 2, 2);
	POINT up33[] = { {340, 150}, {440, 150} , {400, 225}, {300, 225} };
	solidpolygon(up33, 4);
	fill(1, 0, 2);
	
	polygon(up11, 4);

	polygon(up12, 4);

	polygon(up13, 4);

	polygon(up21, 4);

	polygon(up22, 4);

	polygon(up23, 4);

	polygon(up31, 4);

	polygon(up32, 4);

	polygon(up33, 4);
	//上面涂色


	POINT right13[] = { {500, 50} ,{500, 150},  {470, 200}, {470, 100} };
	solidpolygon(right13, 4);
	fill(1, 1, 2);
	POINT right23[] = { {500, 250} ,{500, 150},  {470, 200}, {470, 300} };
	solidpolygon(right23, 4);
	fill(1, 2, 2);
	POINT right33[] = { {500, 250} ,{500, 350},  {470, 400}, {470, 300} };
	solidpolygon(right33, 4);
	fill(1, 0, 1);
	POINT right12[] = { {440, 150} ,{440, 250},  {470, 200}, {470, 100} };
	solidpolygon(right12, 4);
	fill(1, 1, 1);
	POINT right22[] = { {440, 350} ,{440, 250},  {470, 200}, {470, 300} };
	solidpolygon(right22, 4);
	fill(1, 2, 1);
	POINT right32[] = { {440, 350} ,{440, 450},  {470, 400}, {470, 300} };
	solidpolygon(right32, 4);
	fill(1, 0, 0);
	POINT right11[] = { {440, 150} ,{440, 250}, {400, 325},  {400, 225} };
	solidpolygon(right11, 4);
	fill(1, 1, 0);
	POINT right21[] = { {440, 350} ,{440, 250}, {400, 325},  {400, 425} };
	solidpolygon(right21, 4);
	fill(1, 2, 0);
	POINT right31[] = { {440, 350} ,{440, 450},  {400, 525}, {400, 425} };
	solidpolygon(right31, 4);

	polygon(right13, 4);

	polygon(right23, 4);

	polygon(right33, 4);

	polygon(right12, 4);

	polygon(right22, 4);

	polygon(right32, 4);

	polygon(right11, 4);

	polygon(right21, 4);

	polygon(right31, 4);

	//右边涂色


	for (int i = 530; i <= 590; i = i + 30)//左边涂色
	{
		for (int j = 60; j <= 120; j = j + 30)
		{
			fill(3, (j - 60) / 30, (i - 530) / 30);

			coloring(i, j);

			attachment(i, j);
		}
	}
	
	for (int k = 650; k <= 710; k = k + 30)//后面涂色
	{
		for (int l = 60; l <= 120; l = l + 30)
		{
			fill(2, (l - 60) / 30, (k - 650) / 30);

			coloring(k, l);

			attachment(k, l);
		}
	}

	for (int m = 770; m <= 830; m = m + 30)//下面涂色
	{
		for (int n = 60; n <= 120; n = n + 30)
		{
			fill(5, (n - 60) / 30, (m - 770) / 30);

			coloring(m, n);

			attachment(m, n);
		}
	}//

}

template<class T>
void game(T a, bool begin)//载入游戏
{
	if (begin)
	{
		a.printcube();
		//a.CubePrint();

		int i ;

		i = 0;

		while (turn[i] != '0')
		{
			turn[i] = _getch();

			a.operation(i);
			a.printcube();
			//a.CubePrint();
			i++;

		}

	}
	else 
	{
		infile();
		int i = 0;
		while (turn[i] != '1')
		{
			a.operation(i);
			i++;
		}
		a.printcube();
		//a.CubePrint();

		for (i = 0; i < MAX; i++)
			turn[i] = '1';

		i = 0;
		while (turn[i] != '0')
		{
			turn[i] = _getch();

			a.operation(i);

			
			a.printcube();
			//a.CubePrint();

			i++;

		}

	}
}

int main()

{
	
	bool begin;
	cout << "1......重新开始" << endl;
	cout << "0......继续" << endl;
	
	cout << "下面显示操作规则" << endl;
	cout << "右侧为左面，后面，下面" << endl;
	cout << "整体的操作：" << endl;
	    cout << "'W':Up();"<<endl;
		cout << "'S':Down();"<<endl;
		cout<<"'D':Right();"<<endl;
		cout<<"'A':Left();"<<endl;
		cout<<"'Q':Turnclockwise();"<<endl;
		cout<<"'E':Turnanticlockwise();"<<endl;
	cout << "统一标准下简写（2表示180度）：" << endl;
		cout<<"'w':R();"<<endl;
		cout<<"'h':R2();"<<endl;
		cout<<"'s':R3();"<<endl;
		cout<<"'a':U();"<<endl;
		cout<<"'t':U2();"<<endl;
		cout<<"'d':U3();"<<endl;
		cout<<"'g':B();"<<endl;
		cout<<"'j':B2();"<<endl;
		cout<<"'k':B3();"<<endl;
		cout<<"'l':F();"<<endl;
		cout<<"'z':F2();"<<endl;
		cout<<"'x':F3();"<<endl;
		cout<<"'q':D3();"<<endl;
		cout<<"'y':D2();"<<endl;
		cout<<"'e':D();"<<endl;
		cout<<"'r':L3();"<<endl;
		cout<<"'i':L2();"<<endl;
		cout<<"'f':L();"<<endl;
	cout << "特殊操作：" << endl;
		cout<<"'p':disorganize();（打乱）"<<endl;
		cout<<"'m':outfile();（存储）"<<endl;
		cout<<"'n':solve()（解法，显示在右侧）;"<<endl;

	cin >> begin;
	initgraph(900, 600);
	Cube3 a(3);

	game(a, begin);

	system("pause");

	return 0;

}

