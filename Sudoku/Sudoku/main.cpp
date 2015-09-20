//============================================================================
// Name        : main.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "stdafx.h"

#include <iostream>
#include <fstream>


#include "Sudoku.h"
#include "Solver.h"
#include "Generator.h"


using namespace std;
using namespace sudoku;

void test_solver()
{
	Puzzle * p = new Puzzle();
	Solver * s = NULL;

	cout << "example1:" << endl;
	p->load("example1.txt");
	s = new BTSolver(p);
	if (!s->solve())
	{
		if (s->getError() > 0)
		{
			if (s->getError() & SUDOKU_ERROR_UNSOLVABLE_CONFIGURATION)
			{
				cout << endl << "Unsolvable configuration!!!" << endl;
			}
			else if (s->getError() & SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE)
			{
				cout << endl << "Inconsistent internal state!!!" << endl;
			}
		}
	}

	if (!s->validate())
	{
		cout << "The solution is not unique!" << endl;
	}
	else
	{
		cout << endl << "This is unique solution." << endl;
	}

	if (s->getGrade() > 0)
	{

		cout << "The grade of the puzzle is : " << s->getGrade() << endl;
	}

	cout << endl << "The solution for example1 is:" << endl;
	s->printToConsole();
	delete s;
	cout << endl << endl;



	cout << "example2:" << endl;
	p->load("example2.txt");
	s = new BTSolver(p);
	if (!s->solve())
	{
		if (s->getError() > 0)
		{
			if (s->getError() & SUDOKU_ERROR_UNSOLVABLE_CONFIGURATION)
			{
				cout << endl << "Unsolvable configuration!!!" << endl;
			}
			else if (s->getError() & SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE)
			{
				cout << endl << "Inconsistent internal state!!!" << endl;
			}
		}
	}

	if (!s->validate())
	{
		cout << "The solution is not unique!" << endl;
	}
	else
	{
		cout << endl << "This is unique solution." << endl;
	}

	if (s->getGrade() > 0)
	{
		cout << "The grade of the puzzle is : " << s->getGrade() << endl;
	}

	cout << endl << "The solution for example2 is:" << endl;
	s->printToConsole();
	delete s;
	cout << endl << endl;

	cout << "example3:" << endl;
	p->load("example3.txt");
	s = new BTSolver(p);
	if (!s->solve())
	{
		if (s->getError() > 0)
		{
			if (s->getError() & SUDOKU_ERROR_UNSOLVABLE_CONFIGURATION)
			{
				cout << endl << "Unsolvable configuration!!!" << endl;
			}
			else if (s->getError() & SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE)
			{
				cout << endl << "Inconsistent internal state!!!" << endl;
			}
		}
	} 

	if (!s->validate())
	{
		cout << "The solution is not unique!" << endl;
	}
	else
	{
		cout << endl << "This is unique solution." << endl;
	}

	if (s->getGrade() > 0)
	{
		cout << "The grade of the puzzle is : " << s->getGrade() << endl;
	}

	cout << endl << "The solution for example3 is:" << endl;
	s->printToConsole();
	delete s;
	cout << endl << endl;


	cout << "example4:" << endl;
	p->load("example4.txt");
	s = new BTSolver(p);
	if (!s->solve())
	{
		if (s->getError() > 0)
		{
			if (s->getError() & SUDOKU_ERROR_UNSOLVABLE_CONFIGURATION)
			{
				cout << endl << "Unsolvable configuration!!!" << endl;
			}
			else if (s->getError() & SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE)
			{
				cout << endl << "Inconsistent internal state!!!" << endl;
			}
		}
	}

	if (!s->validate())
	{
		cout << "The solution is not unique!" << endl;
	}
	else
	{
		cout << endl << "This is unique solution." << endl;
	}

	if (s->getGrade() > 0)
	{
		cout << "The grade of the puzzle is : " << s->getGrade() << endl;
	}

	cout << endl << "The solution for example4 is:" << endl;
	s->printToConsole();

	delete s;
	cout << endl << endl;
	

	delete p;

}

void test_generator()
{
	Generator * g = new RGenerator(Puzzle::CLASSIC_SUDOKU_DIM, Puzzle::CLASSIC_SUDOKU_REGION_DIM);

	g->generate(1);

	if (g->getError() > 0)
	{
		if (g->getError() & SUDOKU_ERROR_PUZZLE_GENERATION_FROM_RANDOM_INPUT)
		{
			cout << endl << "Error in the puzzle generation from random input!!!" << endl;
		}
		else if (g->getError() & SUDOKU_ERROR_UNSOLVABLE_CONFIGURATION)
		{
			cout << endl << "Unsolvable configuration!!!" << endl;
		}
		else if (g->getError() & SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE)
		{
			cout << endl << "Inconsistent internal state!!!" << endl;
		}
	}
	cout << "The puzzle is " << endl;


	g->getPuzzle()->printToConsole();

	cout << "The Solution is: " << endl;
	g->printToConsole();
	

}

int _tmain(int argc, _TCHAR* argv[]) {
	
	cout << "SudokuQlik" << endl << endl; // prints SudokuQlik

	//test_generator();

	test_solver();







	return 0;
}
