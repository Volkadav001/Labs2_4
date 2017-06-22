#include <iostream>
#include <vector>  // Векторы
#include <string>  // Строки
#include <fstream> // Работа с файлами

#include "Lexer.h"
#include "Parser.h"
#include "Stack_machine.h"

using namespace std;

void Symbols(int n, char Ch)
{
	for(int m = n; m > 0; m--) cout << Ch;
}


int getCountsOfDigits(int number) 
{
	int count = (number == 0) ? 1 : 0;
	while (number != 0) {
		count++;
		number /= 10;
	}
	return count;
}

int Biggest(int one, int two)
{
	if (one > two) return one;
	else return two;
}

void main()
{

	ifstream in("Sample.txt");				//Открываем файл для считывания информации 
	string s1, s2;							//Переменная будет считываться в строку
	while (in >> s2) s1 += s2;				//Считываем пока можем
	in.close();								// Закрываем файл

	//Вывод и визуальное оформление 2 лабы

	LEXER lex;
	lex.Start(s1);
	vector <string> Tokens = lex.Get_Tokens();
	vector <int> ID = lex.Get_Tokens_ID();
	int Max_Size = lex.Leng();

	int Leng = getCountsOfDigits(ID.size());
	Leng += 2;

	cout << "LAB RAB NUMBER 2 - LEXER\n\n\xDA";
	Symbols(Leng+Max_Size+7,'\xC4');
	cout << "\xBF\n\xB3 TOKENS";
	Symbols(Leng+Max_Size,' ');
	cout << "\xB3\n\xC3";

	Symbols(Leng, '\xC4');
	cout << "\xC2";

	Symbols(4, '\xC4');
	cout << "\xC2";
	Symbols(Max_Size + 1, '\xC4');
	cout << "\xB4\n\xB3 N ";
	Symbols(Leng - 3, ' ');

	cout << "\xB3 ID \xB3 LEXEM";
	Symbols(Max_Size - 5, ' ');
	cout << "\xB3\n\xC6";

	Symbols(Leng, '\xCD');
	cout << "\xD8";

	Symbols(4,'\xCD');
	cout << "\xD8";
	Symbols(Max_Size+1,'\xCD');
	cout << "\xB5\n";



	for (int n = 0; n < ID.size(); n++)		//Вывод лексем в консоль
	{
		cout <<"\xB3 " << n;
		Symbols(Leng - getCountsOfDigits(n)-1, ' ');
		cout <<"\xB3 " << ID[n];
		if (ID[n] < 10) cout << "  \xB3 ";
		else cout << " \xB3 ";
		cout << Tokens[n];
		Symbols(Max_Size-Tokens[n].size(),' ');
		cout << "\xB3\n";
	}
	cout << "\xC0";

	Symbols(Leng, '\xC4');
	cout << "\xC1";

	Symbols(4, '\xC4');
	cout << "\xC1";

	Symbols(Max_Size + 1, '\xC4');
	cout << "\xD9\n";

	//Вывод и визуальное оформление 3 лабы

	PARSER Pars;
	Pars.Start(Tokens,ID,0);
	vector <string> Poliz = Pars.OutputPoliz();

	cout << "\nLAB RAB NUMBER 3 - PARSER\n\nReverse Polish notation:\n";


	for (int i = 0; i < Poliz.size(); i+=10)
	{
		cout << '\xD6';
		for (int n = i;(n < i+10)&(n< Poliz.size()); n++)
		{
			Symbols(Biggest(Poliz[n].size(), getCountsOfDigits(n)), '\xC4');
			cout << '\xD2';
		}
		cout << "\x08\xB7\n\xBA";
		for (int n = i; (n < i + 10)&(n< Poliz.size()); n++)
		{
			cout << n;
			Symbols(Poliz[n].size() - getCountsOfDigits(n), ' ');
			cout << '\xBA';
		}
		cout << "\n\xC7";

		for (int n = i; (n < i + 10)&(n< Poliz.size()); n++)
		{
			Symbols(Biggest(Poliz[n].size(), getCountsOfDigits(n)), '\xC4');
			cout << '\xD7';
		}

		cout << "\x08\xB6\n\xBA";
		for (int n = i; (n < i + 10)&(n< Poliz.size()); n++)
		{
			cout << Poliz[n];
			Symbols(Biggest(Poliz[n].size(), getCountsOfDigits(n)) - Poliz[n].size(), ' ');
			cout << '\xBA';
		}
		cout << "\n\xD3";
		for (int n = i; (n < i + 10)&(n< Poliz.size()); n++)
		{
			Symbols(Biggest(Poliz[n].size(), getCountsOfDigits(n)), '\xC4');
			cout << '\xD0';
		}
		cout << "\x08\xBD\n";
	}

	//Вывод и визуальное оформление 4 лабы

	STACK Maсhine;
	Maсhine.init(Poliz);
	vector <int> RAM_INT = Maсhine.Output_RAM_INT();
	vector <string> RAM_ID = Maсhine.Output_RAM_ID();

	cout << "\nLAB RAB NUMBER 4 - StackMachine\n\nOutput:\n\xD6";

	int Id_size=0, Int_size=0;
	for (int n = 0; n < RAM_INT.size(); n++)
	{
		if (Id_size < RAM_ID[n].size()) Id_size = RAM_ID[n].size();
		if (Int_size < getCountsOfDigits(RAM_INT[n])) Int_size = getCountsOfDigits(RAM_INT[n]);
	}
	Symbols(Id_size, '\xC4');
	cout << '\xD2';
	Symbols(Int_size, '\xC4');
	cout << "\xB7\n";

	for (int n = 0; n < RAM_INT.size(); n++) //Вывод значений в консоль после расчетов
	{
		cout << '\xBA' << RAM_ID[n];
		Symbols(Id_size-RAM_ID[n].size(), ' ');
		cout << '\xBA' << RAM_INT[n];
		Symbols(Int_size - getCountsOfDigits(RAM_INT[n]), ' ');
		cout << "\xBA\n";
	}
	cout << "\xD3";
	Symbols(Id_size, '\xC4');
	cout << '\xD0';
	Symbols(Int_size, '\xC4');
	cout << "\xBD\n";

	//Вывод в конце

	cout << "\n\xC9";
	Symbols(38, '\xCD');
	cout << "\xBB\n\xBA Made by Andrey Pahomov IVBO-08-14(c) \xBA\n\xBA All rights reserved 2017             \xBA\n\xC8";
	Symbols(38, '\xCD');
	cout << "\xBC\n\n";

	system("pause");
	return;
}