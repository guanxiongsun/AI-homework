#include "iostream"  
#include "stdlib.h"  
#include "conio.h"  
#include <list>   

#define Size 3  
using namespace std;
int Threshold = 10000;

//定义二维数组来存储数据表示某一个特定状态  
typedef int state[Size][Size];

//存储状态序列，状态序列包括当前状态，和动作序列链表
//4628分别代表左右下上
typedef struct StateNode
{
	state s;
	list<int> acts_done;//做完的动作
	list<int> acts_can;//可以做的动作
	int x;//0所在的位置x，y
	int y;
}StateNode,*PSNode;

//建立fringe表来储存状态序列的序列
typedef struct NODE
{
	PSNode Head;
	int g;
	int h;
	int f;
	struct NODE* piror;
	struct NODE* next;
}NODE,*PNode;

PNode fringe = new(NODE);//fringe不变作为队头
PNode Ptill = fringe;//Ptill作为队尾
PNode BestNode = fringe;//出队的f最小NODE
PSNode start = new(StateNode);
state S = { 2,3,4,1,0,5,7,6,8};
state D = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };

int Hesititon(state s){
	int num = 0;
	for (int i = 0; i < Size; i++)
	{
		for (int j = 0; j < Size;j++)
		{
			if (s[i][j]!=D[i][j])
			{
				num++;
			}
		}
	}
	return num;
}

bool Isequal(state s, state d){
	for (int i = 0; i < Size;i++)
	{
		for (int j = 0; j < Size;j++)
		{
			if (s[i][j]!=d[i][j])
			{
				return false;
			}
		}
	}
	return true;
}

void Findzero(PSNode s){
	for (int i = 0; i < Size; i++)
	{
		for (int j = 0; j < Size; j++)
		{
			if (s->s[i][j] == 0)
			{
				s->x = i;
				s->y = j;
				return;
			}
		}
	}
}

void ActsCan(PSNode s){
	int x = s->x;
	int y = s->y;
	if (x > 0 && (s->acts_done.empty()||s->acts_done.back() != 2))
	{
		s->acts_can.push_back(8);
	}
	if (x < 2 && ( s->acts_done.empty()|| s->acts_done.back() != 8 ))
	{
		s->acts_can.push_back(2);
	}
	if (y > 0 && ( s->acts_done.empty()|| s->acts_done.back() != 6))
	{
		s->acts_can.push_back(4);
	}
	if (y < 2 && ( s->acts_done.empty()|| s->acts_done.back() != 4))
	{
		s->acts_can.push_back(6);
	}
	return;
}

void ShuzuFuzhi(state a, state b){
	for (int i = 0; i < Size; i++)
	{
		for (int j = 0; j < Size; j++)
		{
			a[i][j] = b[i][j];
		}
	}
	return;
}

//做一个动作产生一个新的PSNode
PSNode DoAnAction(PSNode s, int act){
	PSNode PSNode_new = new(StateNode);
	int x = s->x;
	int y = s->y;
	state s_new = {};
	ShuzuFuzhi(s_new, s->s);
	switch (act)
	{
	case 4:
		s_new[x][y] = s_new[x][y - 1];
		s_new[x][y - 1] = 0;
		break;
	case 6:
		s_new[x][y] = s_new[x][y + 1];
		s_new[x][y + 1] = 0;
		break;
	case 2:
		s_new[x][y] = s_new[x + 1][y];
		s_new[x + 1][y] = 0;
		break;
	case 8:
		s_new[x][y] = s_new[x - 1][y];
		s_new[x - 1][y] = 0;
		break;
	}
	ShuzuFuzhi(PSNode_new->s, s_new);
	if (!s->acts_done.empty())
	{
		std::list<int>::iterator rit = s->acts_done.begin();
		std::list<int>::iterator rend = s->acts_done.end();
		for (rit; rit != rend; rit++)
		{
			PSNode_new->acts_done.push_back(*rit);
		}
		PSNode_new->acts_done.push_back(act);
	}
	else
	{
		PSNode_new->acts_done.push_back(act);
	}
	
	Findzero(PSNode_new);
	ActsCan(PSNode_new);
	return PSNode_new;
}

void Initall(){
	fringe->Head = start;
	ShuzuFuzhi(start->s, S);
	start->acts_done = {};
	Findzero(start);
	ActsCan(start);
	for (int i = 0; i < Size;i++)
	{
		for (int j = 0; j < Size; j++){
			start->s[i][j] = S[i][j];
		}
	}
	fringe->next = fringe->piror = NULL;
	fringe->g = 0;
	fringe->h = Hesititon(fringe->Head->s);
	fringe->f = fringe->g + fringe->h;
}



int main(){
	Initall();
	int loop = 0;
	while (fringe!=NULL&&!Isequal(BestNode->Head->s,D))
	{
		loop++;
		//执行BestNODE可以做的动作BN.head.actcan
		//	产生一个新的NODE
		//将所有产生的NODE加入到NODE队列
		while (!BestNode->Head->acts_can.empty())
		{
			PNode Node_new = new (NODE);
			Node_new->Head = DoAnAction(BestNode->Head, BestNode->Head->acts_can.front());
			//cout << BestNode->Head->acts_can.front() << endl;
			BestNode->Head->acts_can.pop_front();
			Node_new->g = BestNode->g+1;
			Node_new->h = Hesititon(Node_new->Head->s);
			Node_new->f = Node_new->g + Node_new->h;
			Node_new->piror = Ptill;
			Node_new->next = NULL;
			Ptill->next = Node_new;
			Ptill = Node_new;
		}
		//删除BestNODE
		//分三种情况，BestNode是头，BestNode是尾，BestNode在中间
		if (fringe==BestNode)
		{
			fringe = BestNode->next;
		}
		else if (Ptill == BestNode)
		{
			Ptill = BestNode->piror;
		}
		else
		{
			BestNode->piror->next = BestNode->next;
			BestNode->next->piror = BestNode->piror;
		}
		free(BestNode);
				
		//找到最小的f所在的Node
		PNode temp = fringe;
		PNode p_min = fringe;
		int min = 100;
		while (temp->next != NULL){
			if (temp->f < min)
			{
				min = temp->f;
				p_min = temp;
			}
			temp = temp->next;
		}
		if (Ptill->f<min)
		{
			min = Ptill->f;
			p_min = Ptill;
		}
		//将最小值所在的NODE赋值为BestNODE
		BestNode = p_min;
		//cout << BestNode->g << "    " << BestNode->h<<endl;
		//cout << BestNode->Head->acts_done.size() << endl;
		if (loop > Threshold)
		{
			cout << "Unsolveble!!" << endl;
			system("pause");
			return 0;
		}
	}
	//输出BestNode的动作序列、
	cout << "the action sequences is:" << endl;
	while (!BestNode->Head->acts_done.empty())
	{
		cout << BestNode->Head->acts_done.front() << " ";
		BestNode->Head->acts_done.pop_front();
	}
	system("pause");
	return 0;
}