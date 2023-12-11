#pragma once
#include <vector> 
#include <iostream> 
#include <string> 
#include <vector> 
//#include <stdio.h>


using std::string;
using std::vector;

class analiz{
public:
	analiz();
	void brackets();
	void naming();
	void semicolon_check();
	void break_continue_position(); 
private:
	int lines = 0;
	vector<string> text; 
	vector<vector<string>> words;
	vector<string> int_variables;
	vector<string> variables;
	vector<string> cls;
	vector<string> tdf;
	vector <string> bad_words = { "int", "float", "double", "string", "char", "bool", "void", "main", "typedef", "long", "signed", "unsigned", "short", "while", "for", "if"};
	vector <string> names = { "int", "float", "double", "string", "char"};
};



