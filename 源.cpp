//ħ������Ⱥ����ԭ�㷨��ʵ��
//�����
//�ο�https://www.stefan-pochmann.info/spocc/other_stuff/tools/solver_thistlethwaite/solver_thistlethwaite_cpp.txt

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

queue<char>solution;//�������


//class FileException 
void outfile();
void infile();

void attachment(int , int );
void coloring(int , int );



typedef vector<int> cubestate;


int col[6][9] = {0};//��ɫ����,���һλΪ������ɫ


int applicableMoves[] = {  /*0,262143, 259263, 74943, 74898*/0,10,10,10,10 };//ÿһ�׶ε�����



int affectedCubies[][8] =
{

	{ 0, 1, 2, 3, 0, 1, 2, 3 },   // U

	{ 4, 7, 6, 5, 4, 5, 6, 7 },   // D

	{ 0, 9, 4, 8, 0, 3, 5, 4 },   // F

	{ 2, 10, 6, 11, 2, 1, 7, 6 }, // B

	{ 3, 11, 7, 9, 3, 2, 6, 5 },  // L

	{ 1, 8, 5, 10, 1, 0, 4, 7 },  // R

};

//��ÿһ������룬ǰ��λ��飬����λ�ǿ�

int phase;//��ʾ�׶Σ�����ɲ��Ľ�Ⱥ��

//��phase=2ʱ������FB��ֻ����ת180�㼴��Ⱥ��<U,D,F2,B2,L,R>

//��phase=3ʱ������FBLR��ֻ����ת180�㼴��Ⱥ��<U,D,F2,B2,L2,R2>

//��phase=4ʱ������UDFBLR��ֻ����ת180�㼴��Ⱥ��<U2,D2,F2,B2,L2,R2>


class Spaces//ħ����һ����
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

class Cube//ħ����
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

class Cube3/**/ :public Cube//��άħ��
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

void Cube3::clockwise(int i)//ƽ��˳ʱ��

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

void Cube3::anticlockwise(int i)//ƽ����ʱ��

{
	clockwise(i);
	clockwise(i);
	clockwise(i);
}

void Cube3::F()//ǰ����ת
{	
	Right();
	R();
    Left();

}

void Cube3::F2()//ǰ��180
{
	F();
	F();

}

void Cube3::F3()//ǰ����ת
{
	F();
	F();
	F();
}

void Cube3::B()//������ת
{
	Left();
	R();
	Right();
}

void Cube3::B2()//����180
{
	B();
	B();

}

void Cube3::B3()//������ת
{
	B();
	B();
	B();
}

void Cube3::D3()//����һ����ת
{
	Turnanticlockwise();
	Turnanticlockwise();
	U3();
	Turnanticlockwise();
	Turnanticlockwise();

}

void Cube3::D2()//����һ��180
{
	D();
	D();

}

void Cube3::D()//����һ����ת
{
	Turnanticlockwise();
	Turnanticlockwise();
	U();
	Turnanticlockwise();
	Turnanticlockwise();
}

void Cube3::L()//�������ת

{
	Turnanticlockwise();
	Turnanticlockwise();
	R();
	Turnanticlockwise();
	Turnanticlockwise();
}

void Cube3::L2()//�����180

{
	L();
	L();
	
}

void Cube3::L3()//���ұ���ת
{
	Turnanticlockwise();
	Turnanticlockwise();
	R3();
	Turnanticlockwise();
	Turnanticlockwise();
}

void Cube3::R()//�������ұ�ת��

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

void Cube3::R2()//�������ұ�ת��

{
	R();
	R();

}
void Cube3::R3()//�������ұ�ת��

{
	R();
	R();
	R();
}

void Cube3::U()//�����һ����ת

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

void Cube3::U2()//�����һ����ת

{
	U();
	U();

}
void Cube3::U3()//�����һ����ת

{
	U();
	U();
	U();
}

void Cube3::Up()//�������Ϸ�
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

void Cube3::Down()//�������·�
{
	Up();
	Up();
	Up();
}

void Cube3::Left()//��������
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

void Cube3::Right()//�������ҷ�
{
	Left();
	Left();
	Left();
}

void Cube3::Turnanticlockwise()//������ʱ��
{

	Right();
	Down();
	Left();


}

void Cube3::Turnclockwise()//����˳ʱ��
{
	Turnanticlockwise();
	Turnanticlockwise();
	Turnanticlockwise();

}


void Cube3::disorganize()//����ħ��

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


void infile()//�洢
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

void outfile()//����
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
void Cube3::CubePrint()//��ӡħ��

{
	for (int s = 0; s < 6; s++)

	{

		switch (s)

		{

		case 0:cout << " ����:" << endl; break;

		case 1:cout << " ����:" << endl; break;

		case 2:cout << " ����:" << endl; break;

		case 3:cout << " ����:" << endl; break;

		case 4:cout << " ����:" << endl; break;

		case 5:cout << " ����:" << endl; break;

		default:break;

		}
		Space[s][0].print();

	}

	cout << endl;

	cout << "---------------------------------------" << endl;

	Sleep(100);

}

cubestate applyMove(int move, cubestate state)//��״̬���в���

{

	int turns = move % 3 + 1;// ��ת90�ȵĴ���

	int face = move / 3;//��ת��һ����



	while (turns--)
	{

		cubestate oldState = state;



		for (int i = 0; i < 8; i++)
		{
			/*����ת�����зֱ��8������8���ǿ�ķ�����и�ֵ*/
			int isCorner = i > 3;
			//�ж��Ƿ��ǽǿ�

			int target = affectedCubies[face][i] + isCorner * 12;
			//����Ŀ��ֵ

			int killer = affectedCubies[face][(i & 3) == 3 ? i - 3 : i + 1] + isCorner * 12;
			//���水˳�����һ�������߽ǿ��ֵȡ����

			int orientationDelta = (i < 4) ? (face > 1 && face < 4) : (face < 2) ? 0 : 2 - (i & 1);
			//˳ʱ����ת����ı���

			state[target] = oldState[killer];

			//�ú�һ���滻ǰһ�������˳ʱ����ת

			state[target + 20] = oldState[killer + 20] + orientationDelta;
			//��¼��ת�����ֵ


			if (!turns)

				state[target + 20] %= 2 + isCorner;

			/*��ֹ����ֵ����270*/
		}

	}

	return state;

}



int inverse(int move)/*���ڷ���move���涯��*/

{

	return move + 2 - 2 * (move % 3);

}



/*ȡ������״̬�ķ����ֵid*/
cubestate id(cubestate state)

{


	//��һ�������ȡ��


	if (phase < 2)

		return cubestate(state.begin() + 20, state.begin() + 32);
	//��������״̬������ȡ�򣬹�12λ��0��ʾ������ȷ��1��ʾ������󣨼���ת��180�㣩

		//�ڶ������ǿ鷽���м�����

	if (phase < 3)
	{

		cubestate result(state.begin() + 31, state.begin() + 40);
		//ȡ�ǿ�ķ���ֵ
		for (int e = 0; e < 12; e++)

			result[0] |= (state[e] / 8) << e;
		// result[0]�����м������λ�ã��ö����Ʊ�ʾ��
		return result;
		//���ؽǿ�ķ��������λ�ã�result[0]��
	}



	//��������M��S�����飬��Ӧ�ǿ��������������

	if (phase < 4)
	{

		cubestate result(3);

		for (int e = 0; e < 12; e++)

			result[0] |= ((state[e] > 7) ? 2 : (state[e] & 1)) << (2 * e);
		//result[0]��24λ��12�����λ����ȷ��
		for (int c = 0; c < 8; c++)

			result[1] |= ((state[c + 12] - 12) & 5) << (3 * c);
		//result[1]��24λ���8���ǿ��λ��
		for (int i = 12; i < 20; i++)

			for (int j = i + 1; j < 20; j++)

				result[2] ^= state[i] > state[j];
		//result[2]=0��ʾ�ǿ鷽����ȷ��result[2]=1��ʾ�ǿ鷽�����
		return result;

	}

	//ʣ��״̬

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
	drawtext(_T("��ȴ�"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	char goal1[12][2];
	char goal2[8][3];
	//Ŀ������
	char cubie1[12][2];
	char cubie2[8][3];
	//ʵ������

	//����ʵ��ħ�����ͽǿ��λ��

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
	//����Ŀ��ħ�����ͽǿ��λ��


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
	/* ����ǰħ��״̬���뵽����currentState�
���ַ���˳��ת��������˳���ַ���˳�������λ�úͷ���
����˳����һλ��ʾλ�ã�һλ��ʾ����
����Ϊ��0-11/12-19�水goal�������/�ǿ������λ�ã�
20-39�����ͽǿ�ķ���
������跭ת����20-31��Ӧλ��Ϊ1��
�ǿ�������ת����˳ʱ����ת90���Ϊ1��˳ʱ����ת180���Ϊ2*/
/*
*/
	for (int i = 0; i < 20; i++) 
	{

		goalState[i] = i; //��ʼ��Ŀ��ħ���������ͽǿ��λ��


	}

	for (int i = 0; i < 12; i++)
	{
		//��Ŀ��ħ�����λ�ñȽϣ�����Ҫ˳ʱ����ת90�������鷭ת180��
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
		//��Ŀ��ħ�����λ�ñȽϣ�����Ҫ˳ʱ����ת90�������鷭ת180��
		while ((currentState[i+12] = find(goal2, goal2 + 8, cubie2[i]) - goal2) == 20)
		{
			char temp = cubie2[i][0];
			cubie2[i][0] = cubie2[i][1];
			cubie2[i][1] = cubie2[i][2];
			cubie2[i][2] = temp;

			currentState[i +32]++;
			//��¼�����򣬻ص���ȷλ����Ҫ˳ʱ����ת180��Ϊ2��˳ʱ����ת90��Ϊ1

		}

	}


	while (++phase < 5)
	{



		//���㵱ǰ��Ŀ��״̬�ķ���ȡֵid��������������
		cubestate currentId = id(currentState), goalId = id(goalState);

		if (currentId == goalId)

			continue;

		//��ʼ��������ȶ���

		queue<cubestate> q; //����һ������q�����ڴ�Ų�ͬ״̬

		q.push(currentState); //push��ӣ���ǰ״̬�����

		q.push(goalState);//push��ӣ�Ŀ��״̬�����



		//��ʼ��BFS�㷨��ͼ�� mapͨ��ƽ��������Խڵ���д洢

		map<cubestate, cubestate> predecessor;
		//��תǰ���״̬����predecessor����ת��ı�ǰ����ֹ��򲻴棨��״̬�ȼ۲��棩

		map<cubestate, int> direction, lastMove;
		//direction����Ų�ͬ״̬�ķ��򣬸�״̬������ħ����ת�õ�����ؼ���Ϊ1����Ŀ��ħ����ת�õ����ؼ���Ϊ2
		//lastMove������ת��ķ���ֵ���벢��¼��ʱ��moveֵ(����ת�ķ�ʽ��

		direction[currentId] = 1;

		direction[goalId] = 2;



		// BFS

		while (1)
		{



			//�Ӷ��л�ȡ״̬����������ID���õ����ķ���
			cubestate oldState = q.front();

			q.pop();

			cubestate oldId = id(oldState);

			int& oldDir = direction[oldId];



			//���������õĶ���(ÿ������ת90.180.270)Ӧ�õ�������¼�µ�״̬
			for (int move = 0; move < 18; move++)
			{

				if (applicableMoves[phase] & (1 << move))
				{


					//--- Apply the move.��//��ת��
					cubestate newState = applyMove(move, oldState);
					//��ת���״̬

					cubestate newId = id(newState);
					//��ת���״̬�������ͽǿ�ķ���
					int& newDir = direction[newId];
					//ӵ���·����״̬�Ƿ���ֹ����ǣ��򷵻عؼ��ָ�newDir�������Թؼ���Ϊ0����direction




					//�ж��Ƿ��ܺ͹ؼ���Ϊ2��״̬��ϵ����������ܣ����ҵ��ⷨ�������������

					if (newDir && newDir != oldDir)
					{



						//ʹoldId��ʾ��ǰ����״̬��newId��ʾ�������״̬

						if (oldDir > 1)
						{

							swap(newId, oldId);

							move = inverse(move);

						}



						//������ϵ������״̬�Ĳ���move

						cubestate algorithm(1, move);
						//���ڴ�Ų���
						while (oldId != currentId)
						{
							//��predecessor�������oldId==currentId������¼��Ҫ�Ĳ��赽algorithm���㷨��

							//������ħ������algorithm�Ĳ�����ת�Ϳɵ���Ŀ��ħ����ת���״̬������ϵdirection�Ĺؼ���1��2��
							algorithm.insert(algorithm.begin(), lastMove[oldId]);

							oldId = predecessor[oldId];

						}

						while (newId != goalId)
						{
							//��ԭ��Ŀ��ħ��״̬��Ҫת���Ĳ���
							algorithm.push_back(inverse(lastMove[newId]));

							newId = predecessor[newId];

						}

						//��ȡ�������

						for (int i = 0; i < (int)algorithm.size(); i++)
						{



							solution.push("UDFBLR"[algorithm[i] / 3]);
							solution.push(algorithm[i] % 3 + 1);

							//��ȡ��Ҫ��ת����ͽǶȣ�1.2.3��ʾ˳ʱ����ת90.180.270
							currentState = applyMove(algorithm[i], currentState);
							//��ת���ֵ����currentState����ǰֵ��
						}


						goto nextPhase;
						//������һ��
					}



					//�����ǰ��δ������״̬(id)��������

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


void Cube3::operation(int i)//��Ϸ����
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

void attachment(int left, int up)//����
{
	rectangle(left, up, left + 30, up - 30);
}

void coloring(int left, int up)//��ɫ
{
	solidrectangle(left, up, left + 30, up - 30);
}



void Cube3:: printcube()//����ħ��ͼ��
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
	//����Ϳɫ

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
	//����Ϳɫ


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

	//�ұ�Ϳɫ


	for (int i = 530; i <= 590; i = i + 30)//���Ϳɫ
	{
		for (int j = 60; j <= 120; j = j + 30)
		{
			fill(3, (j - 60) / 30, (i - 530) / 30);

			coloring(i, j);

			attachment(i, j);
		}
	}
	
	for (int k = 650; k <= 710; k = k + 30)//����Ϳɫ
	{
		for (int l = 60; l <= 120; l = l + 30)
		{
			fill(2, (l - 60) / 30, (k - 650) / 30);

			coloring(k, l);

			attachment(k, l);
		}
	}

	for (int m = 770; m <= 830; m = m + 30)//����Ϳɫ
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
void game(T a, bool begin)//������Ϸ
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
	cout << "1......���¿�ʼ" << endl;
	cout << "0......����" << endl;
	
	cout << "������ʾ��������" << endl;
	cout << "�Ҳ�Ϊ���棬���棬����" << endl;
	cout << "����Ĳ�����" << endl;
	    cout << "'W':Up();"<<endl;
		cout << "'S':Down();"<<endl;
		cout<<"'D':Right();"<<endl;
		cout<<"'A':Left();"<<endl;
		cout<<"'Q':Turnclockwise();"<<endl;
		cout<<"'E':Turnanticlockwise();"<<endl;
	cout << "ͳһ��׼�¼�д��2��ʾ180�ȣ���" << endl;
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
	cout << "���������" << endl;
		cout<<"'p':disorganize();�����ң�"<<endl;
		cout<<"'m':outfile();���洢��"<<endl;
		cout<<"'n':solve()���ⷨ����ʾ���Ҳࣩ;"<<endl;

	cin >> begin;
	initgraph(900, 600);
	Cube3 a(3);

	game(a, begin);

	system("pause");

	return 0;

}

