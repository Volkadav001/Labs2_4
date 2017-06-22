#include <iostream>
#include <vector>  // �������
#include <string>  // ������
#include <regex>   // ���������� ���������
#include <fstream> // ������ � �������
using namespace std;

class PARSER
{

public:

	void Start(vector <string> Input_Tokens, vector <int> Input_IDs, int Start_i)
	{
		i = Start_i;
		Tokens = Input_Tokens;
		Tokens_ID = Input_IDs;
		Dijkstra_Stack.resize(1, "S");
		Begin();
	}

	vector <string> OutputPoliz()
	{
		return Poliz;
	}

private:
	vector <string> Tokens, Poliz, Dijkstra_Stack, RAM_Name;	//�� ������� ������� ����� 1)������ �� ������� 2)�������� �������� ����� 
												//3)���� ��� �������������� � ��������� �������� 4)�������� �������� ����� ���������� � ���
	vector <int> Tokens_ID, RAM_INT;							//�� ������� ������� int 1)�������������� ������� �� ������� 
																//2)�������� �������� ����������
	int i, Label = -1;								//1)��������� �� ������� ������������������ ������� 2)������� ��� �������� �����

	void Error()
	{
		cout << "Syntax error in token number - " << i << "\n";
		system("pause");
		exit(0);
	}

	void Begin()
	{
		if (Tokens[i] == "{") 
		{ 
			i++;
			Statement();
		}
		else Error();
	}

	void Statement()
	{
		for (;;i++)
		switch (Tokens_ID[i])
		{
		case 5:
			Poliz.push_back("!INIT");				//!IN � ����� ������-������ ����� ���������� ������������� ����������
			break;
		case 6:
			If();
			break;
		case 8:
			Do();
			break;
		case 9:
			While();
			break;
		case 10:
			Id();
			break;
		case 12:
			if (Tokens[i] == "{") 
			{ 
				i++; 
				Statement(); 
				break; 
			}
			else if (Tokens[i] == ";") break;
			else if (Tokens[i] == "}") return;
			else Error();
		default:
			Error();
		}
	}

	void Id()
	{
		Poliz.push_back(Tokens[i]);
		if (Tokens_ID[i+1] == 1)
		{
			i+=2;
			Dijkstra_Algorithm();
			Poliz.push_back("=");	
		}
	}

	void Do()
	{
		Label++; int L0 = Label;
		Label++; int L1 = Label;
		Poliz.push_back('L' + to_string(L0) + ':');
		i++;
		if (Tokens[i] == "{") { i++; Statement(); }
		else Error();
		if (Tokens[i] != "}") Error();
		Poliz.push_back('L' + to_string(L1));
		if (Tokens_ID[i + 1] == 9)
		{
			i += 2;
			if (Tokens[i] == "(") Dijkstra_Algorithm();
			else Error();
			if (Tokens[i] != ")") Error();
		}
		else Error();
		Poliz.push_back("!GOTO_FALSE");
		Poliz.push_back('L' + to_string(L0));
		Poliz.push_back("!GOTO");
		Poliz.push_back('L' + to_string(L1) + ':');
	}
	void While()
	{
		Label++; int L0 = Label;
		Label++; int L1 = Label;
		Label++; int L2 = Label;
		Poliz.push_back('L' + to_string(L0) + ':');
		Poliz.push_back('L' + to_string(L1));
		i++;
		if (Tokens[i] == "(") Dijkstra_Algorithm();
		else Error();
		if (Tokens[i] != ")") Error();
		Poliz.push_back("!GOTO_FALSE");
		Poliz.push_back('L' + to_string(L2));
		Poliz.push_back("!GOTO");
		Poliz.push_back('L' + to_string(L1) + ':');
		if (Tokens_ID[i + 1] == 8)
		{
			i += 2;
			if (Tokens[i] == "{") { i++; Statement(); }
			else Error();
			if (Tokens[i] != "}") Error();
		}
		else Error();
		Poliz.push_back('L' + to_string(L0));
		Poliz.push_back("!GOTO");
		Poliz.push_back('L' + to_string(L2) + ':');
	}


	void If()
	{	
		Label++; int L0 = Label;
		Label++; int L1 = Label;
		Poliz.push_back('L'+to_string(L0));
		i++;
		if (Tokens[i] == "(") Dijkstra_Algorithm();
		else Error();
		if (Tokens[i] != ")") Error();
		Poliz.push_back("!GOTO_FALSE");
		i++;
		if (Tokens[i] == "{") { i++; Statement(); }
		else Error();
		if (Tokens[i] != "}") Error();
		Poliz.push_back('L' + to_string(L1));
		Poliz.push_back("!GOTO");
		Poliz.push_back('L'+to_string(L0)+':');
		if (Tokens_ID[i+1] == 7) 
		{
			i+=2;
			if (Tokens[i] == "{") { i++; Statement(); }
			else Error();
			if (Tokens[i] != "}") Error();
		}
		Poliz.push_back('L' + to_string(L1) + ':');
	}

	/*
	��������� �������� ��� ��������� ��������:

	�� ������������� � ������������
		
	   max
		|		6) =
		|		5) ==
		|		4) > >= < <=
		|		3) * /
		|		2) + -
		V		1) ( )
	   min

	   ������� ��������� ��������� ����� � ������� �����������
	
	*/

	void Dijkstra_Algorithm()
	{
		for (;; i++)
		{
			switch (Tokens_ID[i])
			{
			case 2:										//+ - ������� �� ����� ��� ����� ����� ������� (
				if ((Dijkstra_Stack.back() != "+") & (Dijkstra_Stack.back() != "-")) Dijkstra_pop_back(0);
				Dijkstra_Stack.push_back(Tokens[i]);
				break;
			case 3:										//* / ������� �� ����� ��� ����� ����� ( � + -
				if ((Dijkstra_Stack.back() != "*") & (Dijkstra_Stack.back() != "/") & (Dijkstra_Stack.back() != "+") & (Dijkstra_Stack.back() != "-")) Dijkstra_pop_back(0);
				Dijkstra_Stack.push_back(Tokens[i]);
				break;
			case 4:										//== ������ �� �������, > >= < <= ������� ������ ==
				if ((Tokens[i] != "==") & (Dijkstra_Stack.back() == "==")) Dijkstra_pop_back(0);
				Dijkstra_Stack.push_back(Tokens[i]);
				break;
			case 10: case 11:
				Poliz.push_back(Tokens[i]);				//����� ��� ������������� ��������� ����� � �����
				break;
			case 12:
				if (Tokens[i] == "(")					//���� ����������� ( ������� �� � ���� ��������� ��������
				{
					Dijkstra_Stack.push_back("S");
					break;
				}
				else if (Tokens[i] == ")")				//���� ����������� ) ������� �� ����� ��������� �������� ��� �������� ���� �� �������� (
				{
					Dijkstra_pop_back(1);
					if ((Tokens_ID[i + 1] > 4) & (Tokens[i + 1] != ";") & (Tokens[i + 1] != ")")) return;  //���� �� ������� ) �� ���� ���� �������� ��� ; �� ��� ������ ����������� � if do while 
					else break;
				}
				else if (Tokens[i] == ";")				//���� ����������� ; ������� �� ����� ��������� �������� ��� �������� ���� �� �������� ���� ������ S
				{
					Dijkstra_pop_back(1);
					i--;
					return;
				}
				else if (Tokens[i] == "{")
				{
					i--;
					return;
				}
				else Error();
			default:
				Error();
			}
		}

	}

	void Dijkstra_pop_back(bool mode)
	{
		if (!mode)
		{
			if (Dijkstra_Stack.back() != "S")
			{
				Poliz.push_back(Dijkstra_Stack.back());
				Dijkstra_Stack.pop_back();
			}
		}
		else while (Dijkstra_Stack.size() != 1)
		{
			if (Dijkstra_Stack.back() == "S")
			{ 
				Dijkstra_Stack.pop_back(); 
				return;
			}
			Poliz.push_back(Dijkstra_Stack.back());
			Dijkstra_Stack.pop_back();
		}
	}
};
