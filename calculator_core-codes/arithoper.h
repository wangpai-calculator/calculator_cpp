
//arithoper£∫arithmetic operation À„ ı‘ÀÀ„ 

#pragma once

#include <cctype>
#include <cmath>
#include <iostream>

#include "stack_templet.h"

namespace arithoper
{
	void inreset();

	ios_base::fmtflags coutset(int n);

	int parematch();

	char precede(char operleft, char operight);

	void show(WangpaiStack<char>& expression);

	void show2(WangpaiStack<char>& expression, WangpaiStack<double>& medresult, char delimiter = '#');

	bool loadopnd();

	bool unloadopnd();

	double calculate(double opndleft, char optr_, double opndright);

	void showcalpro();

	void evalexpre();

}