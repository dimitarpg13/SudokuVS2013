/*
 * Generator.h
 *
 *  Created on: Sep 17, 2015
 *      Author: Dimitar Gueorguiev
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_


#include "Sudoku.h"


using namespace std;

namespace sudoku
{

	// this is the class which will generate Sudoku puzzle at random
	// which has unique solution
	class Generator
	{
	protected:	
		Puzzle * m_pSrc;
		Puzzle * m_pSol; // to obtain the solution using depth-first search

		unsigned char m_iDim;
		unsigned char m_iRegionDim;

		HorizLine ** m_pRows;
		VertLine ** m_pCols;
		Region ** m_pRegions;

		long long m_lError;

	public:
	
	
		
		static char symbolTable[];

		virtual bool generate() = 0;

		virtual void printToConsole() = 0;

	};

	// generates the puzzle by filling rows in consecutive order and then opening holes on specific
	// places such that the puzzle matches the desired level of difficulty
	class RGenerator : public Generator
	{
	private:		
		Puzzle * m_pSrc;
		void fillRandom(vector<char> &, int);
	public:
	
		RGenerator(unsigned char dim, unsigned char regionDim) 
		{
			m_iDim = dim;
			m_iRegionDim = regionDim;
			m_pSrc = new Puzzle(dim, regionDim);

		}

		~RGenerator()
		{
			delete m_pSrc;
		}

		bool generate();

		bool parse(vector<char>& buffer);
	
		HorizLine ** getRows() { return m_pRows; }
		VertLine **  getCols() { return m_pCols; }
		Region ** getRegions() { return m_pRegions; }
		long long getError() { return m_lError; }
		const set<char> * getSymbolTable() { return m_pSymbols; }
		unsigned char getRegionIdx(unsigned char rowIdx, unsigned char colIdx);
		void printToConsole();
		
	private:
	
		void init();
		void cleanup(unsigned char rowCount, unsigned char colCount, unsigned char regCount);
		bool validate(char c, unsigned char rowIdx, unsigned char colIdx, unsigned char regIdx);
		char nextChar(unsigned char rowIdx, unsigned char colIdx, unsigned char regIdx, vector<char> &, vector<ParserState> &);
		unsigned char getInRegionSeqIdx(unsigned char rowIdx, unsigned char colIdx, unsigned char regIdx);

		set<char> * m_pSymbols;
		unsigned char m_iDim, m_iRegionDim;
		char m_cSep;
		char m_cEol;
		long long m_lError;
		HorizLine ** m_pRows;
		VertLine ** m_pCols;
		Region ** m_pRegions;

#ifdef _DEBUG
		void print_row(vector<char> &);
		void init_test_buffer(vector<char> &);
#endif

	};

}

#endif /* GENERATOR_H_ */
