
//arithoper��arithmetic operation �������� 

#pragma once

#include <cctype>
#include <cmath>
#include <iostream>

#include "stack_templet_M.0.0.1.h"

namespace arithoper
{
	void inreset();

	ios_base::fmtflags coutset(int n);

	int parematch();

	char precede(char operleft, char operight);

	void show(WangpaiStack<char>& expression);

	void show(WangpaiStack<char>& expression, WangpaiStack<double>& medresult, char delimiter = '#');

	bool loadopnd();

	bool unloadopnd();

	double calculate(double opndleft, char optr_, double opndright);

	void showcalpro();

	void evalexpre();

}