// Crux.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fstream>
#include <string>

class Crux {
public:
	Crux();
	~Crux();

	static void runFile(const std::string& filename);
	static void runPrompt();
	
	static bool hadError;
private: 
	static void run(const std::string& source);
	static void error(int line, const std::string& message);
	static void report(int line, const std::string& where, const std::string& message);
};