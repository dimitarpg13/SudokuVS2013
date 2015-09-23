/*
 * Generator.cpp
 *
 *  Created on: Sep 17, 2015
 *      Author: Dimitar Gueorguiev
 */
#include <algorithm>
#include <stdlib.h>     /* srand, rand */
#include <ctime>       /* time */

#include "Generator.h"
#include "Sudoku.h"

using namespace std;

namespace sudoku
{
	char Generator::symbolTable[] = {
		0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
		0x39, 0x30, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
		0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e,
		0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56,
		0x57, 0x58, 0x59, 0x5a, 0x61, 0x62, 0x63, 0x64,
		0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c,
		0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74,
		0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c,
		0x7d, 0x7e, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
		0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e,
		0x2f, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40,
		0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60
	};

	int rand_func(int idx) { return std::rand() % idx; } // random generator

	void RGenerator::fillRandom(vector<char> & buffer, int n)
	{
		

		std::srand(unsigned(std::time(0)));
		
		vector<int> arr;

		// set some values:
		unsigned short dimSquare = n;
		for (unsigned short i = 0; i < n; ++i) 
			arr.push_back(i % m_iDim); 

		// using built-in random generator:
		random_shuffle(arr.begin(), arr.end());

		// using rand()
		random_shuffle(arr.begin(), arr.end(), rand_func);
		for (unsigned short i = 0; i < n; i++)
		{		
		   buffer.push_back(Parser::symbolTable[arr[i]]);
		}

	}


	Generator::~Generator()
	{		
		if (m_pPuzzle != NULL)
			delete m_pPuzzle;

		if (m_pSol != NULL)
			delete m_pSol;
	}

	void RGenerator::init()
	{
		m_pRows = new HorizLine*[m_iDim];
		m_pCols = new VertLine*[m_iDim];
		m_pRegions = new Region*[m_iDim];
		for (int i = 0; i < m_iDim; i++)
		{
			m_pRows[i] = NULL;
			m_pCols[i] = NULL;
			m_pRegions[i] = NULL;
		}

		m_lError = 0;
	}


	void RGenerator::cleanup(unsigned char rowCount, unsigned char colCount, unsigned char regCount)
	{
	

		if (m_pRows != NULL)
		{
			for (unsigned short i = 0; i < rowCount; i++)
				if (m_pRows[i] != NULL)
					delete m_pRows[i];
			delete[] m_pRows;
		}

		if (m_pCols != NULL)
		{
			for (unsigned short i = 0; i < colCount; i++)
				if (m_pCols[i] != NULL)
					delete m_pCols[i];
			delete[] m_pCols;
		}

		if (m_pRegions != NULL)
		{
			for (unsigned short i = 0; i < regCount; i++)
				if (m_pRegions[i] != NULL)
					delete m_pRegions[i];
			delete[] m_pRegions;
		}

		m_pRows = NULL;
		m_pCols = NULL;
		m_pRegions = NULL;
	}

	// input:
	//   symbolRowIdx - the row index of the current symbol
	//   symbolColIdx - the col index of the current symbol
	// return:
	//   the index of the region in which the current symbol is in
	//
	unsigned char RGenerator::getRegionIdx(unsigned char symbolRowIdx, unsigned char symbolColIdx)
	{
		unsigned char M = m_iDim / m_iRegionDim; // the number of regions spanning a single line
		unsigned char regRowIdx = symbolRowIdx / m_iRegionDim;
		unsigned char regColIdx = symbolColIdx / m_iRegionDim;
		return regRowIdx * M + regColIdx;
	}

	unsigned char RGenerator::getInRegionSeqIdx(unsigned char rowIdx, unsigned char colIdx, unsigned char regIdx)
	{
		unsigned char M = m_iDim / m_iRegionDim;
		unsigned char horizRegIdx = regIdx % M;
		unsigned char vertRegIdx = regIdx / M;
		unsigned char horizOffset = colIdx % m_iRegionDim;
		unsigned char vertOffset = rowIdx - vertRegIdx * m_iRegionDim;
		unsigned char seqIdx = vertOffset * m_iRegionDim + horizOffset;

		return seqIdx;
	}

	bool RGenerator::validate(char c, unsigned char rowIdx, unsigned char colIdx, unsigned char regIdx)
	{

		HorizLine * row = m_pRows[rowIdx];
		VertLine * col = m_pCols[colIdx];
		Region * region = m_pRegions[regIdx];

		if (row == NULL && col == NULL && region == NULL)
			return true;

		if (col != NULL)
		{
			for (int i = 0; i < col->getLastSymbIndx(); i++)
			{
				if (col->getSymbols()[i] != NULL && c == col->getSymbols()[i]->getValue())
					return false;
			}
		}

		if (row != NULL)
		{
			for (int j = 0; j < row->getLastSymbIndx(); j++)
			{
				if (row->getSymbols()[j] != NULL && c == row->getSymbols()[j]->getValue())
					return false;
			}
		}


		if (region != NULL)
		{
			for (unsigned char k = 0; k < region->getLastSymbIndx(); k++)
			{
				if (region->getSymbols()[k] != NULL && c == region->getSymbols()[k]->getValue())
					return false;
			}
		}

		return true;
	}



	char RGenerator::nextChar(unsigned char rowIdx, unsigned char colIdx, unsigned char regIdx, vector<ParserState> & state)
	{
		char c = 0;



		if (rowIdx > 0 && colIdx < m_iRegionDim && (rowIdx + 1) % m_iDim != 0)
		{
			if (!state[rowIdx].firstRegionHandled || !state[rowIdx].secondRegionHandled)
			{
				// we are on a line exposed to the threat of "the two regions" so
				// we should take measures not to fall into the two region's trap:
				//
				// in order to avoid this threat we need to make sure that after colIdx becomes larger than 2
				// each of the remaining two regions which enclose the line does not contain the complement 
				// of the symbol set already filled in the current row.

				unsigned char offset = 3;
				unsigned char reg1Idx = getRegionIdx(rowIdx, offset);
				unsigned char reg2Idx = getRegionIdx(rowIdx, offset + m_iRegionDim);

				if (m_pRegions[reg1Idx] != NULL)
				{
					Symbol ** symbolsR1 = m_pRegions[reg1Idx]->getSymbols();
					unsigned char symbCountR1 = m_pRegions[reg1Idx]->getLastSymbIndx();

					Symbol * cur = NULL;

					if (!state[rowIdx].firstRegionHandled)
					{
						for (int i = 0; i < symbCountR1; i++)
						{
							cur = symbolsR1[i];

							if (cur == NULL || state[rowIdx].isUsedAlready(colIdx, cur->getValue()))
								continue;

							if (validate(cur->getValue(), rowIdx, colIdx, regIdx))
							{
								state[rowIdx].firstRegionHandled = true;
								return cur->getValue();
							}
						}
					}
				}

				if (m_pRegions[reg2Idx] != NULL)
				{
					Symbol ** symbolsR2 = m_pRegions[reg2Idx]->getSymbols();
					unsigned char symbCountR2 = m_pRegions[reg2Idx]->getLastSymbIndx();
					Symbol * cur = NULL;
					if (!state[rowIdx].secondRegionHandled)
					{
						for (int i = 0; i < symbCountR2; i++)
						{
							cur = symbolsR2[i];

							if (cur == NULL || state[rowIdx].isUsedAlready(colIdx, cur->getValue()))
								continue;

							if (validate(cur->getValue(), rowIdx, colIdx, regIdx))
							{
								state[rowIdx].secondRegionHandled = true;
								return cur->getValue();
							}
						}
					}
				}

			}


		}

		if (rowIdx > 0 && !state[rowIdx].vertLineHandled)
		{
			// we are on a line exposed to the threat of "the encroaching vertical line"
			// and we should take measures not to fall into the vertical line's trap.
			//
			//
			unsigned char col1Idx = m_iDim - rowIdx;
			Symbol ** symbolsC1 = m_pCols[col1Idx]->getSymbols();
			unsigned char symbCountC1 = m_pCols[col1Idx]->getLastSymbIndx();
			Symbol * cur = NULL;
			for (int i = 0; i < symbCountC1; i++)
			{
				cur = symbolsC1[i];

				if (cur == NULL || state[rowIdx].isUsedAlready(colIdx, cur->getValue()))
					continue;

				if (validate(cur->getValue(), rowIdx, colIdx, regIdx))
				{
					state[rowIdx].vertLineHandled = true;
					return cur->getValue();
				}
			}

		}

		
		vector<char> buffer;
	
#ifdef _DEBUG
		//init_test_buffer(buffer);
		fillRandom(buffer, m_iDim);
#else
		fillRandom(buffer, m_iDim);	
#endif
		c = buffer.back();
		buffer.pop_back();

		int k = 0, count = 0;
		while (!validate(c, rowIdx, colIdx, regIdx) || state[rowIdx].isUsedAlready(colIdx, c))
		{
			if (buffer.size() > 0)
			{
				c = buffer.back();
				buffer.pop_back();
			}
			else
			{
				c = symbolTable[k++ % m_iDim];
				count++;
			}

			if (count >= m_iDim + 1)
			{
				c = 0;
				break;
			}
		}


		return c;
	}


	bool RGenerator::generate(int grade)
	{
		m_iGrade = grade;

		if (m_iDim != Puzzle::CLASSIC_SUDOKU_DIM || m_iRegionDim != Puzzle::CLASSIC_SUDOKU_REGION_DIM)
		{
			m_lError |= SUDOKU_ERROR_FEATURE_NOT_IMPLEMENTED;
			return false;
		}

		bool res = true;

		init();

		unsigned char c = 0, prev_c = 0;
		unsigned char curRowIdx = 0, curColIdx = 0, curRegIdx = 0;
		unsigned int idx = 0;
		Symbol * curSymbol = NULL;
		HorizLine * curRow = NULL;
		VertLine * curCol = NULL;
		Region * curRegion = NULL;
		vector<ParserState> state(m_iDim);

		while (curRowIdx < m_iDim)
		{
			curRow = m_pRows[curRowIdx];

			if (curRow == NULL)
			{
#ifndef _DEBUG
				curRow = new HorizLine(m_iDim, m_iRegionDim);
#else
				curRow = new HorizLine(m_iDim, m_iRegionDim, curRowIdx);
#endif
				m_pRows[curRowIdx] = curRow;
			}
			if (m_pCols[curColIdx] != NULL)
				curCol = m_pCols[curColIdx];
			else
			{
#ifndef _DEBUG
				curCol = new VertLine(m_iDim, m_iRegionDim);
#else
				curCol = new VertLine(m_iDim, m_iRegionDim, curColIdx);
#endif
				m_pCols[curColIdx] = curCol;
			}

			curRegIdx = getRegionIdx(curRowIdx, curColIdx);
			if (m_pRegions[curRegIdx] != NULL)
				curRegion = m_pRegions[curRegIdx];
			else
			{
				curRegion = new Region(m_iDim, m_iRegionDim);
				m_pRegions[curRegIdx] = curRegion;
			}

		

			while ((c = nextChar(curRowIdx, curColIdx, curRegIdx, state)) == 0)
			{
				// backtrack until it is necessary
				//
				//
				if (curColIdx > 0)
				{
					//if (curColIdx % m_iRegionDim == 0)
					//	curRow->removeLastRegion();

					if (curColIdx + 1 < m_iDim)
					   state[curRowIdx].clear(curColIdx+ 1);
					curColIdx--;
				}
				else
				{
					state[curRowIdx].clear();
					//if (curRowIdx % m_iRegionDim == 0)
					//	curCol->removeLastRegion();

			
					curRowIdx--;
					curColIdx = m_iDim - 1;

				}

				curRegIdx = getRegionIdx(curRowIdx, curColIdx);

				curSymbol = m_pRows[curRowIdx]->getSymbols()[curColIdx];
				prev_c = curSymbol->getValue();
				curCol = m_pCols[curColIdx];
				curCol->removeLastSymbol();
				curRow = m_pRows[curRowIdx];
				curRow->removeLastSymbol();

				unsigned char seqIdx = getInRegionSeqIdx(curRowIdx, curColIdx, curRegIdx);
				m_pRegions[curRegIdx]->removeLastSymbol();

			
			
				delete curSymbol;

				if (!state[curRowIdx].isUsedAlready(curColIdx, prev_c))
					state[curRowIdx].setUsedAlready(curColIdx, prev_c);
			}


		


			curSymbol = new Symbol(c, curRow, curCol, curRegion);

			curRow = m_pRows[curRowIdx];
			curCol = m_pCols[curColIdx];
			curRegion = m_pRegions[curRegIdx];

			curRow->addSymbol(curSymbol);
			if (curColIdx % m_iRegionDim == 0)
				curRow->addRegion(curRegion);

			curCol->addSymbol(curSymbol);
			if (curRowIdx % m_iRegionDim == 0)
				curCol->addRegion(curRegion);

			curRegion->addSymbol(curSymbol);
			if (curColIdx % m_iRegionDim == 0)
				curRegion->addRow(curRow);

			curRegion->addCol(curCol);

			if (curColIdx == m_iDim - 1)
			{

				curRow = NULL;
				curRowIdx++;
				curColIdx = 0;
			}
			else
				curColIdx++;

		
		}


		make_puzzle();

		return res;
	}


	bool RGenerator::make_puzzle()
	{

		bool res = true;
		Solver * solver = NULL;	
		int curGrade = -1;
		bool unique = false;
		
		m_pSol = new Puzzle(m_iDim, m_iRegionDim, m_pRows, m_pCols, m_pRegions);
		m_pPuzzle = m_pSol->getCopy();
		int curIter = 0;
			
		for (curIter = 0; curIter < MAX_ITERATION_COUNT; curIter++)
		{
			
			vector<char> buffer;
			fillRandom(buffer, m_iDim);
			unsigned char colIdx = 0;
			vector<pair<unsigned char,char>> stack;
			char c;
			for (int rowIdx = 0; rowIdx < m_iDim; rowIdx++)
			{
				colIdx = buffer.back() - '1';
				buffer.pop_back();
				c = m_pPuzzle->getRows()[rowIdx]->getSymbols()[colIdx]->getValue();
				stack.push_back(pair<unsigned char, char>(colIdx,c));
			    m_pPuzzle->getRows()[rowIdx]->getSymbols()[colIdx]->setValue(0);
			
			}
			
//#ifdef _DEBUG
			cout << endl << endl << "The puzzle on " << curIter + 1 << "th iteration: " << endl << endl;
			m_pPuzzle->printToConsole();
			cout << endl;
//#endif

			solver = new BTSolver(m_pPuzzle);
			res = solver->solve();
			if (!res)
			{
				m_lError |= solver->getError();
				delete solver;
				return false;
			}

			curGrade = solver->getGrade();
			unique = solver->validate();
			if (!unique || curGrade > m_iGrade) // in case the grade jumps by 2 after the last iteration undo the last iteration
			{
				// undo the last iteration
				unsigned char colIdx = 0;
				for (unsigned char rowIdx = 0; rowIdx < m_iDim; rowIdx++)
				{
					colIdx = stack[rowIdx].first;
					c = stack[rowIdx].second;
					m_pPuzzle->getRows()[rowIdx]->getSymbols()[colIdx]->setValue(c);
				}
				delete solver;

				continue;
			}

			delete solver;

			if (m_iGrade != 1)
			{
				if (curGrade == m_iGrade)
					break;
				
			}
			else
			{
				// we do not want to pick the trivial easy problems - select that one 
				// which is just one iteration before becoming moderate
				if (curGrade == 2)
				{
					// undo the last iteration
					unsigned char colIdx = 0;
					for (unsigned char rowIdx = 0; rowIdx < m_iDim; rowIdx++)
					{
						colIdx = stack[rowIdx].first;
						c = stack[rowIdx].second;
						m_pPuzzle->getRows()[rowIdx]->getSymbols()[colIdx]->setValue(c);
					}

					solver = new BTSolver(m_pPuzzle);
					res = solver->solve();
					curGrade = solver->getGrade();
					unique = solver->validate();
					if (curGrade != 1 || !unique)
					{
						m_lError |= SUDOKU_ERROR_PUZZLE_GENERATION_FROM_RANDOM_INPUT;
						delete solver;					
						return false;
					}

					delete solver;
					break;
				}
			}


		}

		if (curIter < MAX_ITERATION_COUNT)
		{
			return true;
		}
		else
		{
			m_lError |= SUDOKU_ERROR_PUZZLE_GENERATION_ITER_COUNT_EXCEEDED;
			return false;
		}
	
	}

	void RGenerator::printToConsole()
	{
		HorizLine * curRow = NULL;
		Symbol * curSymbol = NULL;
		char sep = ' ';
		for (int i = 0; i < m_iDim; i++)
		{
			curRow = m_pRows[i];
			for (int j = 0; j < m_iDim; j++)
			{
				curSymbol = curRow->getSymbols()[j];
				if (!curSymbol->isEmpty())
					cout << curSymbol->getValue();
				else
					cout << ".";
				if (j < m_iDim - 1)
					cout << sep;
			}
			cout << endl;

		}

	}

#ifdef _DEBUG
	void RGenerator::print_row(vector<char> & v)
	{
		cout << endl;
		for (unsigned int i = 0; i < v.size(); i++)
		{
			cout << v[i] << " ";
			if (i > 0 && (i + 1) % m_iDim == 0)
				cout << endl;
		}
		cout << endl;

	}

	// pseudo random input for testing purposes
	void RGenerator::init_test_buffer(vector<char> & v)
	{		
		v.push_back('5');
		v.push_back('6');
		v.push_back('5');
		v.push_back('6');
		v.push_back('2');
		v.push_back('9');
		v.push_back('4');
		v.push_back('3');
		v.push_back('1');
	}

#endif




}


