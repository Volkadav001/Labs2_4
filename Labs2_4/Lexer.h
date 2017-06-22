#include <iostream>
#include <vector>  // �������
#include <string>  // ������
#include <regex>   // ���������� ���������
#include <fstream> // ������ � �������
using namespace std;

class LEXER {

public:

	void Start (string S)			//�������� ������ � ����� � ��������� ������
	{
		s1 = S;
		lexer();
	}

	vector <string> Get_Tokens()	//���� ���������� ������ � ��������
	{
		return Tokens;
	}

	vector <int> Get_Tokens_ID()	//���� ���������� ������ � ���������������� �������
	{
		return Number_ID;
	}

	int Leng()	//���� ���������� ������ � ���������������� �������
	{
		return max_leng_of_token;
	}

private:

	vector <int> Number_ID;		//������ - ��������� ��������������
	vector <string> Tokens;		//������ - ������
	string s1;					//������ �������� ��������
	int incr=0;					//��������� �� ������ � ������
	int max_leng_of_token=5;	//������������ ����� ������ ��� ����������� ���������� �������

	void lexer()										//���� �������
	{

		//����������������� ����� ������� �����
			/*	IF("if"),
				INT("int"),
				ELSE("else"),
				DO("do"),
				WHILE("while"),							��������� ��� �������
				PR("="),
				SYMBOL("[\(]|[\)]|[\{]|[\}]|[;]"),
				OP1("[\+]|[-]"),
				OP2("[/]|[\*]"),
				LOGIC("[==]|[<]|[>]|[<=]|[>=]"), */

			regex
			DIGIT("[0-9]"),					//������������ � ������� ���������� ���������
			LETTER("[A-Za-z_]"),
			ID("[A-Za-z0-9_]*");
		
		string Token_Buffer;		//������ ��� �������������� �������� ������
		int Leng = 0;
		int Alternative_Flag = 0;	//���� ������ ������ 0 - ����������������� �����, 1 - ����� ������ 2 ��� 3, 2 - �������������, 3 - int �����

		while (incr < s1.size())
		{
			//---------------����� 0----------------
			if (Alternative_Flag==0)
				switch (s1[incr])
				{
				case '=':
					Equality_Plus("=", 1);						//�������� �� = ��� == ������������� 1 ��� 4 ��������������
					break;

				case'+': case'-':								//�������� + � - ������������� ������������� 2					
					Write_Token(s1.substr(incr, 1), 2);
					break;

				case'/': case'*':								//�������� / � * ������������� ������������� 3	
					Write_Token(s1.substr(incr, 1), 3);
					break;

				case'>': case'<':
					Equality_Plus(s1.substr(incr, 1), 4);		//�������� >, <, <=, >= ������������� ������������� 4
					break;

				case'i':										//�������� �� if, int �������������� 6 � 5
					if (IF()) break;
					if (INT()) break;
					Alternative_Flag = 1;						//������� � ����� 1
					break;

				case'e':										//�������� �� else ������������� 7
					if (ELSE()) break;
					Alternative_Flag = 1;						//������� � ����� 1
					break;

				case'd':
					if (DO()) break;							//�������� �� do ������������� 8
					Alternative_Flag = 1;						//������� � ����� 1
					break;

				case'w':										//�������� �� while ������������� 9
					if (WHILE()) break;
					Alternative_Flag = 1;						//������� � ����� 1
					break;

				case';':  case'(': case')': case'{': case'}':
					Write_Token(s1.substr(incr, 1), 12);		//�������� ; { } ( ) ������������� ������������� 12
					break;

				default:
					Alternative_Flag = 1;						//������� � ����� 1
				}

			//---------------����� 2----------------
			if (Alternative_Flag == 2)
			{
				Leng++;
				if (regex_match(s1.substr(incr, 1), ID)) Token_Buffer += s1.substr(incr, 1);
				else
				{
					Write_Token(Token_Buffer, 10);					//����� ���������� ������������� ������������� 10
					Alternative_Flag = 0;							//������� � ����� 0
					incr--;											//��������� ����� �� ���������� ������� ������ �� ��������� �������
					max_leng(Leng);									//������� ����������� ���������
				}
			}

			//---------------����� 3----------------
			if (Alternative_Flag == 3)
			{
				Leng++;
				if (regex_match(s1.substr(incr, 1), DIGIT)) Token_Buffer += s1.substr(incr, 1);
				else
				{
					Write_Token(Token_Buffer, 11);					//����� ���� int ������������� ������������� 11
					Alternative_Flag = 0;							//������� � ����� 0
					incr--;											//��������� ����� �� ���������� ������� ������ �� ��������� �������
					max_leng(Leng);									//������� ����������� ���������
				}
			}

			//---------------����� 1----------------
			if (Alternative_Flag==1) 
			{
				Leng = 1;
				if (regex_match(s1.substr(incr, 1), LETTER))		//�������� ��� ������� ������ - �����
				{
					Alternative_Flag = 2;							//������� � ����� 2
					Token_Buffer = s1.substr(incr, 1);				//������ �������� ������� � �����
				}
				else if (regex_match(s1.substr(incr, 1), DIGIT))	//�������� ��� ������� ������ - �����
				{
					Alternative_Flag = 3;							//������� � ����� 3
					Token_Buffer = s1.substr(incr, 1);				//������ �������� ������� � �����
				}
				else Error(0);										//���� �� ����� � �� ����� - ������ ������!
			}
			incr++;								//��������� ��������� ��� �������� �� ��������� ������ � ������
		}
		incr--;								//��������� ���������, ����� �������� �� ��������� ������ � ������, � �� �� ���
		if (s1[incr] != '}') Error(1);		//������ ���������� ��� � ����� ������ ���� �������� �� ������ }
	}

	void max_leng(int n)										//���� ������ ����� ��� ����������� ����������, ����� ������
	{
		if (n > max_leng_of_token) max_leng_of_token = n;
	}

	bool IF()													//���� �������� �� ����� if
	{
		if ((is_enough(2))&(s1[incr + 1] == 'f'))
		{
			Write_Token("if", 6);
			incr++;
			return 1;
		}
		return 0;
	}

	bool ELSE()													//���� �������� �� ����� else
	{
		if ((is_enough(4))&(s1.substr(incr + 1, 3) == "lse"))
		{
			Write_Token("else", 7);
			incr += 3;
			return 1;
		}
		return 0;
	}

	bool DO()													//���� �������� �� ����� do
	{
		if ((is_enough(2))&(s1[incr + 1] == 'o'))
		{
			Write_Token("do", 8);
			incr++;
			return 1;
		}
		return 0;
	}

	bool WHILE()												//���� �������� �� ����� while
	{
		if ((is_enough(5))&(s1.substr(incr + 1, 4) == "hile"))
		{
			Write_Token("while", 9);
			incr += 4;
			return 1;
		}
		return 0;
	}

	bool INT()													//���� �������� �� ����� int
	{
		if ((is_enough(3))&(s1.substr(incr+1,2)=="nt"))
		{
			Write_Token("int", 5);
			incr += 2;
			return 1;
		}
		return 0;
	}

	bool is_enough(int token_length)							//�������� ��� ������� ������ ������ ����� ������
	{															//����� �� ����� �� ������� ������
		if (token_length < s1.size() - 1 - incr) return 1;
		return 0;
	}

	void Equality_Plus(string Symbol, int Symbol_id)			//������� �������� �� = ��������� �� ������� ��������
	{
		if (s1[incr + 1] == '=')
		{
			Write_Token(Symbol+'=', 4);							//������ Symbol + = ������������� ������������� 4
			incr++;
		}
		else Write_Token(Symbol, Symbol_id);					//������� Symbol ������������� �������� �������������
	}

	void Write_Token(string Token,int Token_ID)					//���� ������ ������� � ���������� � ����� ��������
	{
		Tokens.push_back(Token);
		Number_ID.push_back(Token_ID);
	}

	void Error(bool type)										//���� ������ ������
	{
		if (!type)												//����������� ������
			cout << "\nLexical error! Unexpected symbol - \" " << s1[incr] << " \"\n" << "Number in string: " << incr << "\n\n";
		else													//� ����� ������ ��� �� ������ � ����� ��� ����������
			cout << "\nLexical error! Unexpected symbol at the end of code - \" " << s1[incr] << " \"\n" << "Need symbol \" } \" " << "\n\n";
		system("pause");
		exit(0);
	}
};