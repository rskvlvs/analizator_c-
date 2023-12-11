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
		cout << "Программа не введена";
		exit(1);
	}
	brackets();
	naming(); 
	semicolon_check();
	break_continue_position();
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
	int sup_flag;
	while (i < len) {
		lens sz = size(text[i]);
		j = 0;
		while (j < sz) {
			sym = text[i][j];
			if (sym == '{') {
				sup_flag = 1;
				count_of_op_brackets += 1;
				temp.push_back(i);
			}
			if (sym == '}') {
				sup_flag = 2;
				count_of_cl_brackets += 1;
				if (count_of_cl_brackets > count_of_op_brackets)
					cout << "Лишняя закрывающая фигурная скобка в строке " << i + 1 << endl;
			}
			j++;
		}
		i++;
	}
	if (sup_flag == 1) {
		cout << "Нужна закрывающая фигурная скобка в строке " << temp[size(temp) - 1] << endl;
		count_of_cl_brackets += 1;
	}
	if (count_of_op_brackets > count_of_cl_brackets) {
		i = count_of_op_brackets - count_of_cl_brackets; 
		j = 0;
		while (j < i) {
			cout << "Нужна закрывающая фигурная скобка в строке " << temp[j] + 1 << endl;
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
	//Считываю отдельные слова из строк программы
	while (i < size(text)) {
		sz = size(text[i]);
		j = 0;
		while (j <= sz) {
			if (text[i][j] != ' ' && text[i][j] != '\0' && text[i][j] != '\t') {
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
								temp2.clear();
							}
							wor.push_back(deleted);
						}
						le++;
					}
					if (size(temp2) >= 1){
						wor.push_back(temp2);
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
	//Проверка на корректное объявление переменных
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
								cout << "Ошибка в объявлении в строке " << l + 1 << endl;
									flag = 1;
							}
							if (temp[sz] == '=') {
								if (size(temp) - sz == 1 || ((size(words[l]) - j + 2) > 1 ? words[l][j + 2] != ";" : 0)) {
									cout << "Ошибка в объявлении в строке " << l + 1 << endl;
									flag = 1;
								}
								flag2 = 1;
							}
							sz++;
						}
						if (flag == 0) {
							if (size(words[l]) - j + 2 > 1 ? words[l][j + 2] == "=" : 0) {
								if (size(words[l]) - j + 3 == 1 || ((size(words[l]) - j + 3) > 1 ? words[l][j + 3] == ";" : 0)) {
									cout << "Ошибка в объявлении в строке " << l + 1 << endl;
									continue;
								}
							}
							if (flag2 == 1) { //Отделяю равно от переменной
								string::size_type st = 0;
								temp1 = temp;
								temp.clear(); 
								while (temp1[st] != '=') {
									temp += temp1[st]; 
									st++;
								}
							}
							//Проверка на повторную попытку инициализации
							var_indx = 0;
							while (var_indx < size(variables)) {
								if (temp == variables[var_indx]) {
									cout << "Попытка инициализации переменной с уже занятым именем в строке " << l + 1 << endl; 
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
										cout << "Ошибка в объявлении (недопустимый символ) в строке " << l + 1 << endl;
										flag = 1;
									}
									sz++;
								}
							}
							else {
								cout << "Ошибка в объявлении в строке " << l + 1 << endl;
								flag = 1;
							}
						if (flag == 0) {
							if (flag2 == 1) { //Отделяю равно от переменной
								string::size_type st = 0;
								temp1 = temp;
								temp.clear();
								while (temp1[st] != '=') {
									temp += temp1[st];
									st++;
								}
							}
							//Проверка на повторную попытку инициализации
							var_indx = 0;
							while (var_indx < size(variables)) {
								if (temp == variables[var_indx]) {
									cout << "Попытка инициализации переменной с уже занятым именем в строке " << l + 1 << endl;
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
						cout << "Ошибка в объявлении в строке " << l + 1 << endl;
				}
				else
					cout << "Ошибка в объявлении в строке " << l + 1 << endl;
			}
		}
		l++;
	}
}

void analiz::semicolon_check() {
	vector<vector<string>>::size_type i = 0, checked1 = 0, checked2 = 0;
	vector<string>::size_type j, position, it;
	int count_of_semi, flag, count_of_op_plus, count_of_op_minus, fl;
	string::size_type iterator;
	string::size_type exep, sup;
	int super_flag = 0;
	vector<string>::size_type sup_iter;
	string super_try; 
	super_try.clear();
	vector<string>::size_type new_it;
	//Провести изменения сайзов на константные значения
	while (i < size(words)) {
		flag = 0;
		count_of_semi = 0;
		for (j = 0; j < size(words[i]); j++) {
			if (size(words[i]) == 0)
				break;
			if (i == checked1) {
				for (exep = 0; exep < size(words[i][j]); exep++) {
					if (words[i][j][exep] == '%') {
						super_flag = 0;
						if (exep == 0 && j >= 1) {
							it = 0;
							while (it < size(words[i][j - 1])) {
								if (!isdigit(words[i][j - 1][it])) {
									super_flag = 0;
									break;
								}
								else super_flag = 1;
								it++;
							}
							if (super_flag == 0) {
								for (sup_iter = 0; sup_iter < size(int_variables); sup_iter++) {
									if (words[i][j - 1] == int_variables[sup_iter]) {
										super_flag = 1;
										break;
									}
									else super_flag = 0;
								}
							}
						}
						else if (exep >= 1) {
							sup = 0;
							while (sup < exep) {
								super_try += words[i][j][sup];
								sup++;
							}
							it = 0;
							while (it < size(super_try)) {
								if (!isdigit(super_try[it])) {
									super_flag = 0;
									break;
								}
								else super_flag = 1;
								it++;
							}
							if (super_flag == 0) {
								for (sup_iter = 0; sup_iter < size(int_variables); sup_iter++)
									if (super_try == int_variables[sup_iter]) {
										super_flag = 1;
										break;
									}
									else super_flag = 0;
							}
							super_try.clear();
						}
						if (super_flag == 0)cout << "Попытка проведения операции '%' с переменными типа не 'int' в строке " << i + 1 << endl;
					}
				}
				checked1 += 1;
			}
			if (words[i][j] == "for") {
				for (position = 0; position < size(words[i]); position++) {
					if (words[i][position] == "(" && count_of_semi > 0)
						cout << "Ошибка в описании 'for' в строке " << i + 1 << endl;
					else if (words[i][position] == ";") {
						count_of_semi++;
						if (position <= 1)
							cout << "Ошибка в написании ';' в строке " << i + 1 << endl;
						else {
							if (count_of_semi == 3) {
								if (words[i][position - 1] == ")" || words[i][position - 1] == "{")
									cout << "Ошибка в описании 'for' в строке " << i + 1 << endl;
								else if (words[i][position - 1] == "}" && words[i][position - 2] == "{")
									cout << "Ошибка в описании 'for' в строке " << i + 1 << endl;
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
										cout << "Ошибка в описании 'for' в строке " << i + 1 << endl;
									if (count_of_op_minus == 0 && count_of_op_plus != 2)
										cout << "Ошибка в описании 'for' в строке " << i + 1 << endl;
								}
							}
						}
					}
					else if (words[i][position] == ")" && count_of_semi != 2)
						cout << "Ошибка в описании 'for' в строке " << i + 1 << endl;
				}
				if (count_of_semi > 3)
					cout << "Ошибка в написании ';' в строке " << i + 1 << endl;
				j = position;
			}
			else if (words[i][j] == "while") {
				count_of_semi = 0;
				for (position = 0; position < size(words[i]); position++) {
					if ((words[i][position] == ")" || words[i][position] == "(") && count_of_semi > 0)
						cout << "Ошибка в написании ';' в строке " << i + 1 << endl;
					if(words[i][position] == "(" && words[i][position + 1] == ")")
						cout << "Ошибка в описании 'while' в строке " << i + 1 << endl;
					else if (words[i][position] == ";") {
						count_of_semi++;
						if (position <= 1)
							cout << "Ошибка в написании ';' в строке " << i + 1 << endl;
						else {
							if (words[i][position - 1] == ")" || words[i][position - 1] == "{")
								cout << "Ошибка в написании ';' в строке " << i + 1 << endl;
							else if (words[i][position - 1] == "}" && words[i][position - 2] == "{")
								cout << "Ошибка в описании 'while' в строке " << i + 1 << endl;
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
									cout << "Ошибка в описании 'while' в строке " << i + 1 << endl;
								if (count_of_op_minus == 0 && count_of_op_plus != 2)
									cout << "Ошибка в описании 'while' в строке " << i + 1 << endl;
							}
						}
					}
				}
				j = position;
			}
			else if (words[i][j] == "if") {
				count_of_semi = 0;
				for (position = 0; position < size(words[i]); position++) {
					if ((words[i][position] == ")" || words[i][position] == "(") && count_of_semi > 0)
						cout << "Ошибка в написании ';' в строке " << i + 1 << endl;
					if (words[i][position] == "(" && words[i][position + 1] == ")")
						cout << "Ошибка в описании 'if' в строке " << i + 1 << endl;
					else if (words[i][position] == ";") {
						count_of_semi++;
						if (position <= 1)
							cout << "Ошибка в написании ';' в строке " << i + 1 << endl;
						else {
							if (words[i][position - 1] == ")" || words[i][position - 1] == "{")
								cout << "Ошибка в написании ';' в строке " << i + 1 << endl;
							else if (words[i][position - 1] == "}" && words[i][position - 2] == "{")
								cout << "Ошибка в описании 'if' в строке " << i + 1 << endl;
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
									cout << "Ошибка в описании 'if' в строке " << i + 1 << endl;
								if (count_of_op_minus == 0 && count_of_op_plus != 2)
									cout << "Ошибка в описании 'if' в строке " << i + 1 << endl;
							}
						}
					}
				}
				j = position;
			}
			else {
				if (i == checked2) {
					count_of_semi = 0;
					for (iterator = 0; iterator < size(words[i]); iterator++) {
						if (words[i][iterator] == ";")count_of_semi++;
						if ((words[i][iterator] == "{" || words[i][iterator] == "}") && count_of_semi >= 1)
							cout << "Ошибка в написании ';' в строке " << i + 1 << endl;
					}
				/*	if (size(words[i]) > 2) {
						for (iterator = 0; iterator < size(words[i]); iterator++) {
							if (words[i][iterator] == ";")count_of_semi++;
						}
						if (words[i][size(words[i]) - 1] == "{") {
							if (count_of_semi != 0) cout << "Ошибка в написании ';' в строке " << i + 1 << endl;
						}
						else
							if (count_of_semi != 1) cout << "Ошибка в написании ';' в строке " << i + 1 << endl;
					}
					else if (size(words[i]) == 2) {
						count_of_semi = 0;
						fl = 0;
						for (iterator = 0; iterator < size(words[i][j]); iterator++) {
							if ((words[i][j][iterator] == '{' || words[i][j][iterator] == '}') && count_of_semi >= 1) {
								cout << "Ошибка в написании ';' в строке " << i + 1 << endl;
								fl = 1;
							}
							else if (words[i][j][iterator] == ';')count_of_semi++;
						}
						if(count_of_semi != 1 && fl == 0)
							cout << "Ошибка в написании ';' в строке " << i + 1 << endl;
					}*/
					checked2 += 1;
				}
			}
		}
		i++;
		checked1 = i; 
		checked2 = i;
	}
}

void analiz::break_continue_position() {
	vector<vector<string>>::size_type i, position = 0;
	vector<string>::size_type j;
	int flag1 = 0, flagop = 0;
	for (i = 0; i < size(words); i++) {
		for (j = 0; j < size(words[i]); j++) {
			if (words[i][j] == "for" || words[i][j] == "while") {
				flag1 = 1;
				position = i;
			}
			else if (words[i][j] == "{" && flag1 == 1) {
				flagop = 1;
			}
			else if (words[i][j] == "}" && flag1 == 1) {
				if (flagop == 1) {
					flagop = 0;
					flag1 = 0;
				}
			}
			else if (words[i][j] == "break")
				if (flagop == 0 && i - position != 1)
					cout << "Неправильное использование оператора 'break' в строке " << i + 1 << endl;
				else;
			else if (words[i][j] == "continue")
				if (flagop == 0 && i - position != 1)
					cout << "Неправильное использование оператора 'continue' в строке " << i + 1 << endl;
		}
	}
}