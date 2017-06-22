#include <iostream>
#include <vector>  // Векторы
#include <string>  // Строки
#include <regex>   // Регулярные выражения
#include <fstream> // Работа с файлами
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
	vector <string> Tokens, Poliz, Dijkstra_Stack, RAM_Name;	//По порядку векторы строк 1)Токены из лексера 2)Выходное значение Полиз 
												//3)Стек для преобразований в алгоритме Дейкстры 4)Выходное значение имена переменных в ОЗУ
	vector <int> Tokens_ID, RAM_INT;							//По порядку векторы int 1)Идентификаторы токенов из лексера 
																//2)Исходные значения переменных
	int i, Label = -1;								//1)Указатель на входную последовательность токенов 2)Счетчик для создания меток

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
			Poliz.push_back("!INIT");				//!IN в нашем псевдо-полизе будет обозначать инициализацию переменной
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
	Приоритет операций для алгоритма Дейкстры:

	От максимального к минимальному
		
	   max
		|		6) =
		|		5) ==
		|		4) > >= < <=
		|		3) * /
		|		2) + -
		V		1) ( )
	   min

	   Меньший приоритет вытесняет знаки с большим приоритетом
	
	*/

	void Dijkstra_Algorithm()
	{
		for (;; i++)
		{
			switch (Tokens_ID[i])
			{
			case 2:										//+ - выносят из стека все знаки кроме скобоки (
				if ((Dijkstra_Stack.back() != "+") & (Dijkstra_Stack.back() != "-")) Dijkstra_pop_back(0);
				Dijkstra_Stack.push_back(Tokens[i]);
				break;
			case 3:										//* / выносят из стека все знаки кроме ( и + -
				if ((Dijkstra_Stack.back() != "*") & (Dijkstra_Stack.back() != "/") & (Dijkstra_Stack.back() != "+") & (Dijkstra_Stack.back() != "-")) Dijkstra_pop_back(0);
				Dijkstra_Stack.push_back(Tokens[i]);
				break;
			case 4:										//== ничего не выносит, > >= < <= выносят только ==
				if ((Tokens[i] != "==") & (Dijkstra_Stack.back() == "==")) Dijkstra_pop_back(0);
				Dijkstra_Stack.push_back(Tokens[i]);
				break;
			case 10: case 11:
				Poliz.push_back(Tokens[i]);				//Число или идентификатор заносится сразу в полиз
				break;
			case 12:
				if (Tokens[i] == "(")					//Если встречается ( заносим ее в стек алгоритма Дейкстры
				{
					Dijkstra_Stack.push_back("S");
					break;
				}
				else if (Tokens[i] == ")")				//Если встречается ) выносим из стека алгоритма Дейкстры все элементы пока не встретим (
				{
					Dijkstra_pop_back(1);
					if ((Tokens_ID[i + 1] > 4) & (Tokens[i + 1] != ";") & (Tokens[i + 1] != ")")) return;  //Если за скобкой ) не идет знак операции или ; то это скобка прикреплена к if do while 
					else break;
				}
				else if (Tokens[i] == ";")				//Если встречается ; выносим из стека алгоритма Дейкстры все элементы пока не встретим стоп сигнал S
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
