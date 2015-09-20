/*
 * Generator.h
 *
 *  Created on: Sep 17, 2015
 *      Author: Dimitar Gueorguiev
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_


#include "Sudoku.h"
#include "Solver.h"


using namespace std;

namespace sudoku
{

	// this is the class which will generate Sudoku puzzle at random
	// which has unique solution
	class Generator
	{
	protected:	
		Puzzle * m_pPuzzle;
		Puzzle * m_pSol; // to obtain the solution using depth-first search

		unsigned char m_iDim;
		unsigned char m_iRegionDim;

		HorizLine ** m_pRows;
		VertLine ** m_pCols;
		Region ** m_pRegions;

		long long m_lError;

		int m_iGrade;
		

	public:
	
		virtual ~Generator();	
		static char symbolTable[];

		virtual bool generate(int) = 0;
		virtual void printToConsole() = 0;

		Puzzle * getPuzzle() { return m_pPuzzle;  }
		long long getError() { return m_lError;  }


	};

	// generates the puzzle by filling rows in consecutive order and then opening holes on specific
	// places such that the puzzle matches the desired level of difficulty
	class RGenerator : public Generator
	{
	private:				
		void fillRandom(vector<char> &, int);
	public:
	
		RGenerator(unsigned char dim, unsigned char regionDim) 
		{
			m_iDim = dim;
			m_iRegionDim = regionDim;
			m_pSol = NULL;
			m_pPuzzle = NULL;
		}

		~RGenerator()
		{
			
		}

		bool generate(int grade);
		 	
	
		unsigned char getRegionIdx(unsigned char rowIdx, unsigned char colIdx);
		void printToConsole();
		
	private:
	
		void init();
		void cleanup(unsigned char rowCount, unsigned char colCount, unsigned char regCount);
		bool validate(char c, unsigned char rowIdx, unsigned char colIdx, unsigned char regIdx);
		char nextChar(unsigned char rowIdx, unsigned char colIdx, unsigned char regIdx, vector<ParserState> &);
		unsigned char getInRegionSeqIdx(unsigned char rowIdx, unsigned char colIdx, unsigned char regIdx);
		bool make_puzzle();

	

	

#ifdef _DEBUG
		void print_row(vector<char> &);
		void init_test_buffer(vector<char> &);
#endif

	};

}

#endif /* GENERATOR_H_ */
