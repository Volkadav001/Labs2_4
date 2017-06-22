#include <iostream>
#include <vector>  // Векторы
#include <string>  // Строки
#include <regex>   // Регулярные выражения
#include <fstream> // Работа с файлами
using namespace std;

class LEXER {

public:

	void Start (string S)			//Передаем строку в класс и запускаем лексер
	{
		s1 = S;
		lexer();
	}

	vector <string> Get_Tokens()	//Блок возвращает вектор с токенами
	{
		return Tokens;
	}

	vector <int> Get_Tokens_ID()	//Блок возвращает вектор и идентификаторами токенов
	{
		return Number_ID;
	}

	int Leng()	//Блок возвращает вектор и идентификаторами токенов
	{
		return max_leng_of_token;
	}

private:

	vector <int> Number_ID;		//Вектор - численные идентификаторы
	vector <string> Tokens;		//Вектор - Токены
	string s1;					//Строка исходных символов
	int incr=0;					//Указатель на символ в строке
	int max_leng_of_token=5;	//Максимальная длина токена для визуального оформелния таблицы

	void lexer()										//Тело лексера
	{

		//Зарезервированные слова данного языка
			/*	IF("if"),
				INT("int"),
				ELSE("else"),
				DO("do"),
				WHILE("while"),							Выражения для справок
				PR("="),
				SYMBOL("[\(]|[\)]|[\{]|[\}]|[;]"),
				OP1("[\+]|[-]"),
				OP2("[/]|[\*]"),
				LOGIC("[==]|[<]|[>]|[<=]|[>=]"), */

			regex
			DIGIT("[0-9]"),					//Используемые в расчете регулярные выражения
			LETTER("[A-Za-z_]"),
			ID("[A-Za-z0-9_]*");
		
		string Token_Buffer;		//Строка для промежуточного хранения токена
		int Leng = 0;
		int Alternative_Flag = 0;	//Флаг режима работы 0 - Зарезервированные слова, 1 - выбор режима 2 или 3, 2 - Идентификторы, 3 - int число

		while (incr < s1.size())
		{
			//---------------Режим 0----------------
			if (Alternative_Flag==0)
				switch (s1[incr])
				{
				case '=':
					Equality_Plus("=", 1);						//Проверки на = или == идентификатор 1 или 4 соответственно
					break;

				case'+': case'-':								//Символам + и - присваивается идентификатор 2					
					Write_Token(s1.substr(incr, 1), 2);
					break;

				case'/': case'*':								//Символам / и * присваивается идентификатор 3	
					Write_Token(s1.substr(incr, 1), 3);
					break;

				case'>': case'<':
					Equality_Plus(s1.substr(incr, 1), 4);		//Символам >, <, <=, >= присваивается идентификатор 4
					break;

				case'i':										//Проверка на if, int идентификаторы 6 и 5
					if (IF()) break;
					if (INT()) break;
					Alternative_Flag = 1;						//Переход в режим 1
					break;

				case'e':										//Проверка на else идентификатор 7
					if (ELSE()) break;
					Alternative_Flag = 1;						//Переход в режим 1
					break;

				case'd':
					if (DO()) break;							//Проверка на do идентификатор 8
					Alternative_Flag = 1;						//Переход в режим 1
					break;

				case'w':										//Проверка на while идентификатор 9
					if (WHILE()) break;
					Alternative_Flag = 1;						//Переход в режим 1
					break;

				case';':  case'(': case')': case'{': case'}':
					Write_Token(s1.substr(incr, 1), 12);		//Символам ; { } ( ) присваивается идентификатор 12
					break;

				default:
					Alternative_Flag = 1;						//Переход в режим 1
				}

			//---------------Режим 2----------------
			if (Alternative_Flag == 2)
			{
				Leng++;
				if (regex_match(s1.substr(incr, 1), ID)) Token_Buffer += s1.substr(incr, 1);
				else
				{
					Write_Token(Token_Buffer, 10);					//Имени переменной присваивается идентификатор 10
					Alternative_Flag = 0;							//Переход в режим 0
					incr--;											//Декремент чтобы не пропустить текущий символ на следующеи проходе
					max_leng(Leng);									//Функция визуального офрмления
				}
			}

			//---------------Режим 3----------------
			if (Alternative_Flag == 3)
			{
				Leng++;
				if (regex_match(s1.substr(incr, 1), DIGIT)) Token_Buffer += s1.substr(incr, 1);
				else
				{
					Write_Token(Token_Buffer, 11);					//Числу типа int присваивается идентификатор 11
					Alternative_Flag = 0;							//Переход в режим 0
					incr--;											//Декремент чтобы не пропустить текущий символ на следующеи проходе
					max_leng(Leng);									//Функция визуального офрмления
				}
			}

			//---------------Режим 1----------------
			if (Alternative_Flag==1) 
			{
				Leng = 1;
				if (regex_match(s1.substr(incr, 1), LETTER))		//Проверка что текущий символ - буква
				{
					Alternative_Flag = 2;							//Переход в режим 2
					Token_Buffer = s1.substr(incr, 1);				//Запись текущего символа в буфер
				}
				else if (regex_match(s1.substr(incr, 1), DIGIT))	//Проверка что текущий символ - цифра
				{
					Alternative_Flag = 3;							//Переход в режим 3
					Token_Buffer = s1.substr(incr, 1);				//Запись текущего символа в буфер
				}
				else Error(0);										//Если не цифра и не буква - значит ошибка!
			}
			incr++;								//Инкремент указателя для перехода на следующий символ в потоке
		}
		incr--;								//Декремент указателя, чтобы указывал на последний символ в строке, а не за нее
		if (s1[incr] != '}') Error(1);		//Ошибка означающая что в конце строки была получена не скобка }
	}

	void max_leng(int n)										//Блок записи длины для визуального оформления, можно забить
	{
		if (n > max_leng_of_token) max_leng_of_token = n;
	}

	bool IF()													//Блок проверки на токен if
	{
		if ((is_enough(2))&(s1[incr + 1] == 'f'))
		{
			Write_Token("if", 6);
			incr++;
			return 1;
		}
		return 0;
	}

	bool ELSE()													//Блок проверки на токен else
	{
		if ((is_enough(4))&(s1.substr(incr + 1, 3) == "lse"))
		{
			Write_Token("else", 7);
			incr += 3;
			return 1;
		}
		return 0;
	}

	bool DO()													//Блок проверки на токен do
	{
		if ((is_enough(2))&(s1[incr + 1] == 'o'))
		{
			Write_Token("do", 8);
			incr++;
			return 1;
		}
		return 0;
	}

	bool WHILE()												//Блок проверки на токен while
	{
		if ((is_enough(5))&(s1.substr(incr + 1, 4) == "hile"))
		{
			Write_Token("while", 9);
			incr += 4;
			return 1;
		}
		return 0;
	}

	bool INT()													//Блок проверки на токен int
	{
		if ((is_enough(3))&(s1.substr(incr+1,2)=="nt"))
		{
			Write_Token("int", 5);
			incr += 2;
			return 1;
		}
		return 0;
	}

	bool is_enough(int token_length)							//Проверка что остаток строки больше длины токена
	{															//Чтобы не выйти за границу строки
		if (token_length < s1.size() - 1 - incr) return 1;
		return 0;
	}

	void Equality_Plus(string Symbol, int Symbol_id)			//Функция проверки на = следующее за текущим символом
	{
		if (s1[incr + 1] == '=')
		{
			Write_Token(Symbol+'=', 4);							//Токену Symbol + = присваивается идентификатор 4
			incr++;
		}
		else Write_Token(Symbol, Symbol_id);					//Символу Symbol присваивается входящий идентификатор
	}

	void Write_Token(string Token,int Token_ID)					//Блок записи токенов и указателей в конец векторов
	{
		Tokens.push_back(Token);
		Number_ID.push_back(Token_ID);
	}

	void Error(bool type)										//Блок вывода ошибок
	{
		if (!type)												//Неизвестный символ
			cout << "\nLexical error! Unexpected symbol - \" " << s1[incr] << " \"\n" << "Number in string: " << incr << "\n\n";
		else													//В конце строки был не символ а число или переменная
			cout << "\nLexical error! Unexpected symbol at the end of code - \" " << s1[incr] << " \"\n" << "Need symbol \" } \" " << "\n\n";
		system("pause");
		exit(0);
	}
};