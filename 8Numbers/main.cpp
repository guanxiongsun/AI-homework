#include "iostream"  
#include "stdlib.h"  
#include "conio.h"  
#include <list>   

#define Size 3  
using namespace std;
int Threshold = 10000;

//�����ά�������洢���ݱ�ʾĳһ���ض�״̬  
typedef int state[Size][Size];

//�洢״̬���У�״̬���а�����ǰ״̬���Ͷ�����������
//4628�ֱ������������
typedef struct StateNode
{
	state s;
	list<int> acts_done;//����Ķ���
	list<int> acts_can;//�������Ķ���
	int x;//0���ڵ�λ��x��y
	int y;
}StateNode,*PSNode;

//����fringe��������״̬���е�����
typedef struct NODE
{
	PSNode Head;
	int g;
	int h;
	int f;
	struct NODE* piror;
	struct NODE* next;
}NODE,*PNode;

PNode fringe = new(NODE);//fringe������Ϊ��ͷ
PNode Ptill = fringe;//Ptill��Ϊ��β
PNode BestNode = fringe;//���ӵ�f��СNODE
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

//��һ����������һ���µ�PSNode
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
		//ִ��BestNODE�������Ķ���BN.head.actcan
		//	����һ���µ�NODE
		//�����в�����NODE���뵽NODE����
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
		//ɾ��BestNODE
		//�����������BestNode��ͷ��BestNode��β��BestNode���м�
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
				
		//�ҵ���С��f���ڵ�Node
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
		//����Сֵ���ڵ�NODE��ֵΪBestNODE
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
	//���BestNode�Ķ������С�
	cout << "the action sequences is:" << endl;
	while (!BestNode->Head->acts_done.empty())
	{
		cout << BestNode->Head->acts_done.front() << " ";
		BestNode->Head->acts_done.pop_front();
	}
	system("pause");
	return 0;
}