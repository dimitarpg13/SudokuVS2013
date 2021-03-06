//============================================================================
// Name        : main.cpp
// Author      : Dimitar Gueorguiev
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "stdafx.h"

#define _CRTDBG_MAP_ALLOC

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include <stdlib.h>
#include <crtdbg.h>

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

	cout << "Total iteration count : " << s->getIterCount() << endl;

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

	cout << "Total iteration count : " << s->getIterCount() << endl;
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

	cout << "Total iteration count : " << s->getIterCount() << endl;

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

	cout << "Total iteration count : " << s->getIterCount() << endl;

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

	g->generate(4);

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

	cout << endl << "The Solution is: " << endl;
	g->printToConsole();
	
	delete g;
}


void generate(int grade)
{


	Generator * g = new RGenerator(Puzzle::CLASSIC_SUDOKU_DIM, Puzzle::CLASSIC_SUDOKU_REGION_DIM);

	g->generate(grade);

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
		else if (g->getError() & SUDOKU_ERROR_PUZZLE_GENERATION_ITER_COUNT_EXCEEDED)
		{
			cout << "The maximum iteration count of " << Generator::MAX_ITERATION_COUNT << " was exceeded and puzzled with the desired grade was not found!" << endl;
		}

	}

	if (!(g->getError() & SUDOKU_ERROR_PUZZLE_GENERATION_ITER_COUNT_EXCEEDED))
	{
		cout << "The puzzle is " << endl;
		g->getPuzzle()->printToConsole();
	}

	cout << endl << "The Solution is: " << endl;

	g->printToConsole();

	delete g;
}

void solve(string fileName)
{
	Puzzle * p = new Puzzle();
	Solver * s = NULL;

	cout << fileName << ":" << endl;
	if (!p->load(fileName))
	{
		if (p->getError() & SUDOKU_ERROR_INCORRECT_INPUT_FORMAT)
		{
			cout << endl << "Incorrect input format!!!" << endl;
		}
		else if (p->getError() & SUDOKU_ERROR_INCORRECT_INPUT_ROW_COUNT)
		{
			cout << endl << "Incorrect input row count!!!" << endl;
		}
		else if (p->getError() & SUDOKU_ERROR_INCORRECT_INPUT_COL_COUNT)
		{
			cout << endl << "Incorrect input column count!!!" << endl;
		}
		else if (p->getError() & SUDOKU_ERROR_MISSING_INPUT_FILE)
		{
			cout << endl << "Missing input file!!!" << endl;
		}

		return;
	}

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

			return;
		}
	}

	cout << "Total iteration count : " << s->getIterCount() << endl;

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

}

int _tmain(int argc, _TCHAR* argv[]) {
	
	cout << "SudokuQlik" << endl << endl; // prints SudokuQlik
	

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(58302);
#endif

	//test_generator();

	//test_solver();

	cout << "\t\t\t\tMENU:" << endl << endl << endl << endl;
	cout << "\t\t1 - solve puzzle" << endl << endl;
	cout << "\t\t2 - generate a puzzle" << endl << endl;
	cout << "\t\t3 - exit the program" << endl << endl;
	int i;
	
	while (1)
	{
		cin >> i;

		if (i == 1)
		{
			cout << "Enter a text file name : " << endl;
			cout << "(available input files: example1.txt, example2.txt, example3.txt, example4.txt)" << endl;
			string fileName;
			cin >> fileName;
			solve(fileName);

		}
		else if (i == 2)
		{
			cout << "Enter the desired puzzle grade: " << endl;
			int grade;
			cin >> grade;
			while (1)
			{
				if (grade != 1 && grade != 2 && grade != 3 && grade != 4)
					cout << endl << endl << "Incorrect choise.. try again" << endl;
				else
					break;
			}
			generate(grade);

		}
		else if (i == 3)
		{
			

			break;
		}
		else
			cout << endl << endl << "Incorrect choise.. try again" << endl;
	}

	
	return 0;
}
