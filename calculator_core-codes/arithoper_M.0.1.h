
//arithoper£∫arithmetic operation À„ ı‘ÀÀ„ 

#pragma once

#include <cctype>
#include <cmath>
#include <iostream>

#include <stack>


using namespace std;


#include "stack_auxiliary.h"

namespace arithoper
{
	void inreset();

	ios_base::fmtflags coutset(int n);

	int parematch();

	char precede(char operleft, char operight);

	void show(stack<char>& expression);

	void show(stack<char>& expression, stack<double>& medresult, char delimiter = '#');

	bool loadopnd();

	bool unloadopnd();

	double calculate(double opndleft, char optr_, double opndright);

	void showcalpro();

	void evalexpre();

}