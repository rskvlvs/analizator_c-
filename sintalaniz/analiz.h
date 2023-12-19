#pragma once
#include <iostream> 
#include <string> 
#include <vector> 

class analiz{
public:
	analiz();
	void brackets();
	void naming();
	void semicolon_check();
	void break_continue_position(); 
	bool isOperation(const std::string&, int);
private:
	int lines = 0;
	std::vector<std::string> text; 
	std::vector<std::vector<std::string>> words;
	std::vector<std::string> int_variables;
	std::vector<std::string> variables;
	std::vector<std::string> cls;
	std::vector<std::string> tdf;
	std::vector <std::string> bad_words = { "int", "float", "double", "string", "char", "bool", "void", "main", "typedef", "long", "signed", "unsigned", "short", "while", "for", "if"};
	std::vector <std::string> names = { "int", "float", "double", "string", "char"};
};



