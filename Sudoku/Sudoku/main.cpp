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


using namespace std;
using namespace sudoku;


int _tmain(int argc, _TCHAR* argv[]) {
	
	cout << "SudokuQlik" << endl << endl; // prints SudokuQlik


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


  /*  cout << "example2:" << endl;
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

   cout << endl << "The solution for example4 is:" << endl;
   s->printToConsole();

   delete s;
   cout << endl << endl*/;
   



   delete p;




	return 0;
}
