#include "analiz.h"
#include <iostream> 
#include <stdlib.h> 
#include <string> 
#include <vector> 
#include <exception>
#include <list> 
#include <locale>
#include <stdio.h>
#include <fstream> 
#include <ctype.h>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::list;
using std::getline;
using std::ifstream;

analiz::analiz() {
	ifstream f("test.txt");
	string stroka;
	while (getline(f, stroka)) {
		text.push_back(stroka);
		lines++;
	}
	f.close();
	if (empty(text)) {
		cout << "��������� �� �������";
		exit(1);
	}
	brackets();
	naming(); 
	semicolon_check();
}

void analiz::brackets() {
	typedef vector<string>::size_type len_s;
	typedef string::size_type lens;
	len_s len = size(text);
	len_s i = 0;
	lens j;
	vector<len_s> temp; 
	int count_of_op_brackets = 0, count_of_cl_brackets = 0;
	char sym;
	while (i < len) {
		lens sz = size(text[i]);
		j = 0;
		while (j < sz) {
			sym = text[i][j];
			if (sym == '{') {
				count_of_op_brackets += 1;
				temp.push_back(i);
			}
			if (sym == '}') {
				count_of_cl_brackets += 1;
				if (count_of_cl_brackets > count_of_op_brackets)
					cout << "������ ����������� �������� ������ � ������ " << i + 1 << endl;
			}
			j++;
		}
		i++;
	}
	if (count_of_op_brackets > count_of_cl_brackets) {
		i = count_of_op_brackets - count_of_cl_brackets; 
		j = 0;
		while (j < i) {
			cout << "����� ����������� �������� ������ � ������ " << temp[j] + 1 << endl;
			j++;
		}
	}
}

void analiz::naming() {
	//vector<vector<string>> words;
	vector<string> wor;
	string w, temp, temp1, temp2;
	typedef vector<string>::size_type len_s;
	typedef string::size_type lens;
	len_s i = 0;
	lens sz = 0, j, le;
	int fl=0;
	w.clear();
	char c;
	string deleted;
	deleted.clear();
	temp2.clear();
	//�������� ��������� ����� �� ����� ���������
	while (i < size(text)) {
		sz = size(text[i]);
		j = 0;
		while (j <= sz) {
			if (text[i][j] != ' ' && text[i][j] != '\0') {
				w += text[i][j];
			}
			else {
				if (size(w) >= 1) {
					le = 0;
					while (le < size(w)) {
						c = w[le];
						temp2 += c;
						if (c == ';' || c == '{' || c == '}' || c == '(' || c == ')') {
							fl = 1;
							deleted = (1, c);
							c = ' ';
							temp2.erase(size(temp2) - 1);
							if (size(temp2) >= 1) {
								wor.push_back(temp2);
								if (temp2 == "break")break_position();
								temp2.clear();
							}
							wor.push_back(deleted);
						}
						le++;
					}
					if (size(temp2) >= 1){
						wor.push_back(temp2);
						if (temp2 == "break")break_position();
						temp2.clear();
					}
				}
				deleted.clear();
				w.clear();
				fl = 0;
			}
			j++;
		}
		w.clear();
		words.push_back(wor); 
		wor.clear();
		i++; 
	}
	//�������� �� ���������� ���������� ����������
	vector<vector<string>>::size_type l = 0;
	len_s ls;
	int flag, flag2, flag3;
	lens t;
	vector<string>::size_type var_indx;
	while (l < size(words)) {
		for (j = 0; j < size(words[l]); j++) {
			temp = words[l][j]; 
			fl = 0; 
			ls = 0; 
			while (ls < size(names)) {
				if (temp == names[ls]) {
					fl = 1; 
					break; 
				}
				ls++;
			}
			if (fl == 1) {
				if (size(words[l]) - j != 1) {
					flag = 0;
					flag2 = 0;
					flag3 = 0;
					temp = words[l][j + 1];
					ls = 0;
					while (ls < size(bad_words)) {
						if (temp == bad_words[ls]) {
							flag = 1;
							break;
						}
						ls++;
					}
					if (isalpha(temp[0]) && flag == 0) {
						sz = 0; 
						while (sz < size(temp)) {
							if (!isdigit(temp[sz]) && !isalpha(temp[sz]) && temp[sz] != '_' && temp[sz] != '=') {
								cout << "������ � ���������� � ������ " << l + 1 << endl;
									flag = 1;
							}
							if (temp[sz] == '=') {
								if (size(temp) - sz == 1 || ((size(words[l]) - j + 2) > 1 ? words[l][j + 2] != ";" : 0)) {
									cout << "������ � ���������� � ������ " << l + 1 << endl;
									flag = 1;
								}
								flag2 = 1;
							}
							sz++;
						}
						if (flag == 0) {
							if (size(words[l]) - j + 2 > 1 ? words[l][j + 2] == "=" : 0) {
								if (size(words[l]) - j + 3 == 1 || ((size(words[l]) - j + 3) > 1 ? words[l][j + 3] == ";" : 0)) {
									cout << "������ � ���������� � ������ " << l + 1 << endl;
									continue;
								}
							}
							if (flag2 == 1) { //������� ����� �� ����������
								string::size_type st = 0;
								temp1 = temp;
								temp.clear(); 
								while (temp1[st] != '=') {
									temp += temp1[st]; 
									st++;
								}
							}
							//�������� �� ��������� ������� �������������
							var_indx = 0;
							while (var_indx < size(variables)) {
								if (temp == variables[var_indx]) {
									cout << "������� ������������� ���������� � ��� ������� ������ � ������ " << l + 1 << endl; 
									flag3 = 1;
									break;
								}
								var_indx++;
							}
							if (flag3 == 1) continue;
							variables.push_back(temp);
							if (words[l][j] == "int")int_variables.push_back(temp);
							continue;
						}
					}
					else if (temp[0] == '_') {
						if (size(temp) >= 2)
							if (temp[1] != '_') {
								sz = 0;
								while (sz < size(temp)) {
									if (!isdigit(temp[sz]) && !isalpha(temp[sz]) && temp[sz] != '_') {
										cout << "������ � ���������� (������������ ������) � ������ " << l + 1 << endl;
										flag = 1;
									}
									sz++;
								}
							}
							else {
								cout << "������ � ���������� � ������ " << l + 1 << endl;
								flag = 1;
							}
						if (flag == 0) {
							if (flag2 == 1) { //������� ����� �� ����������
								string::size_type st = 0;
								temp1 = temp;
								temp.clear();
								while (temp1[st] != '=') {
									temp += temp1[st];
									st++;
								}
							}
							//�������� �� ��������� ������� �������������
							var_indx = 0;
							while (var_indx < size(variables)) {
								if (temp == variables[var_indx]) {
									cout << "������� ������������� ���������� � ��� ������� ������ � ������ " << l + 1 << endl;
									flag3 = 1;
									break;
								}
								var_indx++;
							}
							if (flag3 == 1) continue;
							variables.push_back(temp);
							if (words[l][j] == "int")int_variables.push_back(temp);
							continue;
						}
					}
					else
						cout << "������ � ���������� � ������ " << l + 1 << endl;
				}
				else
					cout << "������ � ���������� � ������ " << l + 1 << endl;
			}
		}
		l++;
	}
}

void analiz::semicolon_check() {
	vector<vector<string>>::size_type i = 0;
	vector<string>::size_type j, position;
	int count_of_semi, flag, count_of_op_plus, count_of_op_minus;
	string::size_type iterator;
	string::size_type exep, sup;
	int super_flag = 0;
	vector<string>::size_type sup_iter;
	string super_try; 
	super_try.clear();
	while (i < size(words)) {
		flag = 0;
		count_of_semi = 0;
		for (j = 0; j < size(words[i]); j++) {
			for (exep = 0; exep < size(words[i][j]); exep++) {
				if (words[i][j][exep] == '%') {
					super_flag = 0;
					if (exep == 0 && j >= 1) {
						for (sup_iter = 0; sup_iter < size(int_variables); sup_iter++)
							if (words[i][j - 1] == int_variables[sup_iter])super_flag = 1;
					}
					else if (exep >= 1) {
						sup = 0;
						while (sup < exep) { 
							super_try += words[i][j][sup];
							sup++;
						}
						for (sup_iter = 0; sup_iter < size(int_variables); sup_iter++)
							if (super_try == int_variables[sup_iter])super_flag = 1;
						super_try.clear();
					}
					if (super_flag == 0)cout << "������� ���������� �������� '%' � ����������� ���� �� 'int' � ������ " << i + 1 << endl;
				}
			}
			if (words[i][j] == "for") {
				for (position = 0; position < size(words[i]); position++) {
					if (words[i][position] == "(" && count_of_semi > 0)
						cout << "������ � �������� 'for' � ������ " << i + 1 << endl;
					else if (words[i][position] == ";") {
						count_of_semi++;
						if (position <= 1)
							cout << "������ � ��������� ';' � ������ " << i + 1 << endl;
						else {
							if (count_of_semi == 3) {
								if (words[i][position - 1] == ")" || words[i][position - 1] == "{")
									cout << "������ � �������� 'for' � ������ " << i + 1 << endl;
								else if (words[i][position - 1] == "}" && words[i][position - 2] == "{")
									cout << "������ � �������� 'for' � ������ " << i + 1 << endl;
								else if (words[i][position - 2] == ")" || (words[i][position - 3] == ")" && words[i][position - 2] == "{")) {
									count_of_op_plus = 0;
									count_of_op_minus = 0;
									for (iterator = 0; iterator < size(words[i][position - 1]); iterator++) {
										if (words[i][position - 1][iterator] == '+')
											count_of_op_plus++;
										else if (words[i][position - 1][iterator] == '-')
											count_of_op_minus++;
									}
									if (count_of_op_plus == 0 && count_of_op_minus != 2)
										cout << "������ � �������� 'for' � ������ " << i + 1 << endl;
									if (count_of_op_minus == 0 && count_of_op_plus != 2)
										cout << "������ � �������� 'for' � ������ " << i + 1 << endl;
								}
							}
						}
					}
					else if (words[i][position] == ")" && count_of_semi != 2)
						cout << "������ � �������� 'for' � ������ " << i + 1 << endl;
				}
				if (count_of_semi > 3)
					cout << "������ � ��������� ';' � ������ " << i + 1 << endl;
			}
			else if (words[i][j] == "while") {
				count_of_semi = 0;
				for (position = 0; position < size(words[i]); position++) {
					if ((words[i][position] == ")" || words[i][position] == "(") && count_of_semi > 0)
						cout << "������ � ��������� ';' � ������ " << i + 1 << endl;
					if(words[i][position] == "(" && words[i][position + 1] == ")")
						cout << "������ � �������� 'while' � ������ " << i + 1 << endl;
					else if (words[i][position] == ";") {
						count_of_semi++;
						if (position <= 1)
							cout << "������ � ��������� ';' � ������ " << i + 1 << endl;
						else {
							if (words[i][position - 1] == ")" || words[i][position - 1] == "{")
								cout << "������ � ��������� ';' � ������ " << i + 1 << endl;
							else if (words[i][position - 1] == "}" && words[i][position - 2] == "{")
								cout << "������ � �������� 'while' � ������ " << i + 1 << endl;
							else if (words[i][position - 2] == ")") {
								count_of_op_plus = 0;
								count_of_op_minus = 0;
								for (iterator = 0; iterator < size(words[i][position - 1]); iterator++) {
									if (words[i][position - 1][iterator] == '+')
										count_of_op_plus++;
									else if (words[i][position - 1][iterator] == '-')
										count_of_op_minus++;
								}
								if (count_of_op_plus == 0 && count_of_op_minus != 2)
									cout << "������ � �������� 'while' � ������ " << i + 1 << endl;
								if (count_of_op_minus == 0 && count_of_op_plus != 2)
									cout << "������ � �������� 'while' � ������ " << i + 1 << endl;
							}
						}
					}
				}
			}
			else if (words[i][j] == "if") {
				count_of_semi = 0;
				for (position = 0; position < size(words[i]); position++) {
					if ((words[i][position] == ")" || words[i][position] == "(") && count_of_semi > 0)
						cout << "������ � ��������� ';' � ������ " << i + 1 << endl;
					if (words[i][position] == "(" && words[i][position + 1] == ")")
						cout << "������ � �������� 'if' � ������ " << i + 1 << endl;
					else if (words[i][position] == ";") {
						count_of_semi++;
						if (position <= 1)
							cout << "������ � ��������� ';' � ������ " << i + 1 << endl;
						else {
							if (words[i][position - 1] == ")" || words[i][position - 1] == "{")
								cout << "������ � ��������� ';' � ������ " << i + 1 << endl;
							else if (words[i][position - 1] == "}" && words[i][position - 2] == "{")
								cout << "������ � �������� 'if' � ������ " << i + 1 << endl;
							else if (words[i][position - 2] == ")") {
								count_of_op_plus = 0;
								count_of_op_minus = 0;
								for (iterator = 0; iterator < size(words[i][position - 1]); iterator++) {
									if (words[i][position - 1][iterator] == '+')
										count_of_op_plus++;
									else if (words[i][position - 1][iterator] == '-')
										count_of_op_minus++;
								}
								if (count_of_op_plus == 0 && count_of_op_minus != 2)
									cout << "������ � �������� 'if' � ������ " << i + 1 << endl;
								if (count_of_op_minus == 0 && count_of_op_plus != 2)
									cout << "������ � �������� 'if' � ������ " << i + 1 << endl;
							}
						}
					}
				}
			}
			else {
				count_of_semi = 0;
				if (words[i][j] == ";") count_of_semi++;
				if (size(words[i]) > 2) {

				}
				else if (size(words[i]) == 2) {
					count_of_op_plus = 0;
					count_of_op_minus = 0;
					for (iterator = 0; iterator < size(words[i][j]); iterator++) {
						if (words[i][j][iterator] == '+')
							count_of_op_plus++;
						else if (words[i][j][iterator] == '-')
							count_of_op_minus++;
					}
					if (count_of_op_plus == 0 && count_of_op_minus != 2)
						cout << "������ � ��������� ';' � ������ " << i + 1 << endl;
					if (count_of_op_minus == 0 && count_of_op_plus != 2)
						cout << "������ � ��������� ';' � ������ " << i + 1 << endl;
				}
			}
		}
		i++;
	}
}

void analiz::break_position() {
	vector<vector<string>>::size_type len_w = 0; 
	vector<string>::size_type j;
	int flag1 = 0, flagop = 0;
	for (len_w = 0; len_w < size(words); len_w++) {
		for (j = 0; j < size(words[len_w]); j++) {
			if (words[len_w][j] == "for" || words[len_w][j] == "while")
				flag1 = 1;
			if (words[len_w][j] == "{" && flag1 == 1) {
				flagop = 1; 
			}
			if (words[len_w][j] == "}" && flag1 == 1) {
				if (flagop == 1) {
					flagop = 0; 
					flag1 = 0;
				}
			}
			/*if (words[len_w][j])
			if (words[len_w][j] == "break") {
				if (flag1 == 0) {
					cout << "������������ ������������ break � ������ " << len_w + 1 << endl; 
				}
				else if()
			}*/
		}
	}
}