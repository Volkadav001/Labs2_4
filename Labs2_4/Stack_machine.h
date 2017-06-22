#include <iostream>
#include <vector>  // Векторы
#include <string>  // Строки
#include <regex>   // Регулярные выражения
#include <fstream> // Работа с файлами
using namespace std;

class STACK
{

public:

	void init(vector <string> Input_Poliz)
	{
		Poliz = Input_Poliz;
		On_Input();
	}

	vector<int> Output_RAM_INT()
	{
		return RAM_INT;
	}

	vector<string> Output_RAM_ID()
	{
		return RAM_ID;
	}

private:
	vector <string> Poliz, RAM_ID, Machine_Stack;
	vector <int> RAM_INT;
	vector <bool> Id_Flag;

	int n = 0;

	void On_Input()
	{

		regex
			INT("[0-9]+"),
			ID("[A-Za-z0-9_]*");

		for (; n < Poliz.size(); n++)
		{
			if (Poliz[n] == "!INIT") On_Init();
			else if (Poliz[n] == "=") On_Assign();
			else if ((Poliz[n] == "+") || (Poliz[n] == "-") || (Poliz[n] == "*") || (Poliz[n] == "/")) On_Op();
			else if ((Poliz[n] == "<") || (Poliz[n] == ">") || (Poliz[n] == ">=") || (Poliz[n] == "<=") || (Poliz[n] == "==")) On_Log();
			else if (Poliz[n] == "!GOTO") Goto();
			else if (Poliz[n] == "!GOTO_FALSE") Goto_on_false();
			else if (regex_match(Poliz[n], INT)) Push_Back_Machine_Stack(0);	//Число записывается в стек с флагом 0
			else if (regex_match(Poliz[n], ID)) Push_Back_Machine_Stack(1);		//Идентификатор записывается в стек с флагом 1
		}
	}

string a, b;

	void On_Init()								//Инициализируем переменную нулем
	{
		RAM_ID.push_back(Poliz[n+1]);
		RAM_INT.push_back(0);
	}

	void Push_Back_Machine_Stack(bool mode)		//Функция записи в стек
	{
		Machine_Stack.push_back(Poliz[n]);
		Id_Flag.push_back(mode);
	}

	string Pop_Back_Machine_Stack()				//Функция выноса из стека
	{
		string Back = Machine_Stack.back();
		Machine_Stack.pop_back();
		Id_Flag.pop_back();
		return Back;
	}

	int Return_Addr(string ID)					//Функция возвращает адрес ячейки в импровизированном ОЗУ
	{
		for (int i = 0; i < RAM_ID.size(); i++) if (RAM_ID[i] == ID) return i;
	}

	string Return_Int_On_Addr(int Addr)			//Возвращает значение из ОЗУ по адресу
	{
		return to_string(RAM_INT[Addr]);
	}

	string Return_Int(bool Flag)				//Возвращает интовое значение в виде строки в зависимости от флага
	{
		if (Flag) return Return_Int_On_Addr(Return_Addr(Pop_Back_Machine_Stack()));
		else return  Pop_Back_Machine_Stack();
	}

	int Return_Label_Addr(string Label)
	{
		for (int i = 0; i < Poliz.size(); i++)
		{
			if (Poliz[i] == (Label + ':')) return i;
		}
		Error();
	}

	void Goto()
	{
		b = Pop_Back_Machine_Stack();
		n = Return_Label_Addr(b);
	}

	void Goto_on_false()
	{
		a = Pop_Back_Machine_Stack();
		if (stoi(a) == 0) Goto();
		else Pop_Back_Machine_Stack();
	}

	void On_Assign()							//Функция присваивания
	{
		a = Pop_Back_Machine_Stack();
		b = Pop_Back_Machine_Stack();

		RAM_INT[Return_Addr(b)] = stoi(a);
	}

	void On_Op()
	{
		a = Return_Int(Id_Flag.back());
		b = Return_Int(Id_Flag.back());
		switch (Poliz[n][0])
		{
		case '+': Machine_Stack.push_back(to_string(stoi(b) + stoi(a))); break;
		case '-': Machine_Stack.push_back(to_string(stoi(b) - stoi(a))); break;
		case '*': Machine_Stack.push_back(to_string(stoi(b) * stoi(a))); break;
		case '/': Machine_Stack.push_back(to_string(stoi(b) / stoi(a))); break;
		}
		Id_Flag.push_back(0);
	}

	void On_Log()
	{
		a = Return_Int(Id_Flag.back());
		b = Return_Int(Id_Flag.back());
		switch (Poliz[n].size())
		{
		case 1:
			switch (Poliz[n][0])
			{
			case '<': Machine_Stack.push_back(to_string(stoi(b) < stoi(a))); break;
			case '>': Machine_Stack.push_back(to_string(stoi(b) > stoi(a))); break;
			}
			break;
		case 2:
			switch (Poliz[n][0])
			{
			case '<': Machine_Stack.push_back(to_string(stoi(b) <= stoi(a))); break;
			case '>': Machine_Stack.push_back(to_string(stoi(b) >= stoi(a))); break;
			case '=': Machine_Stack.push_back(to_string(stoi(b) == stoi(a))); break;
			}
			break;
		}
		Id_Flag.push_back(0);
	}

	void Error()
	{
		cout << "Translation error in word number - " << n << "\n";
		system("pause");
		exit(0);
	}
};