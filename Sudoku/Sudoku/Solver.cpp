/*
 * Solver.cpp
 *
 *  Created on: Sep 7, 2015
 *      Author: Dimitar Gueorguiev
 */

#include <math.h>
#include <limits>
#include "Solver.h"

namespace sudoku
{

#ifdef _DEBUG
   void BTSolver::print_ranked_candidates(vector<rank_list *> & c)
   {
	   cout << "Total number of distinct ranks: " << m_iRankCount << endl;
	 	vector<rank_list *>::iterator itR;
	  	list<pair<Symbol*,list<char>* > >::iterator itRL;
	  	list<char>::iterator itCL;
	  	int indx = 1;
	  	for (itR = c.begin(); itR != c.end(); itR++)
	  	{
	  		if (*itR == NULL)
	  			continue;

            cout << indx++ << " => " << endl;
            for (itRL = (*itR)->begin(); itRL != (*itR)->end(); itRL++)
            {
            	std::cout << "[" << (int) itRL->first->getRow()->getIdx()
            			  << "," << (int) itRL->first->getCol()->getIdx()
            			  << "]\t{";
            	for (itCL = itRL->second->begin(); itCL != itRL->second->end(); itCL++)
            	{
            		std::cout << *itCL << " ";
            	}
            	std::cout << "}" << endl;
            }
	  	}
   }
#endif

  RankNode * BTSolver::init_rank_node_list(vector<rank_list *> & c)
  {
	  RankNode * head = NULL, * cur = NULL, * prev = NULL;

	  vector<rank_list *>::iterator itC;
	  list<rank_pair>::iterator itRL;

	  for (itC = c.begin(); itC != c.end(); itC++)
	  {
		  if (*itC == NULL)
		     continue;

	      for (itRL = (*itC)->begin(); itRL != (*itC)->end(); itRL++)
	      {
              cur = new RankNode(&(*itRL));
              if (prev == NULL)
                head = cur;
              else
            	prev->Next = cur;

              cur->Prev = prev;
              prev = cur;
	      }
	  }

	  return head;
  }

  void BTSolver::initCummDistribEasyLevel(vector<int> & cd)
  {
	  cd.clear();
	  cd.push_back(8);
	  cd.push_back(15); // the count of all nodes with 1 or 2 possible assignments
	  cd.push_back(29); // the count of all nodes with 1,2 or 3 possible assignments
	  cd.push_back(42); // the count of all nodes with 1,2,3 or 4 possible assignments
	  cd.push_back(48); // and so on
	  cd.push_back(49);
	  cd.push_back(49);
	  cd.push_back(49);

  }

  void BTSolver::initCummDistribMediumLevel(vector<int> & cd)
  {
	  cd.clear();
	  cd.push_back(1);
	  cd.push_back(15);
	  cd.push_back(29);
	  cd.push_back(48);
	  cd.push_back(51);
	  cd.push_back(53);
	  cd.push_back(53);
	  cd.push_back(53);
  }

  void BTSolver::initCummDistribHardLevel(vector<int> & cd)
  {
	  cd.clear();
	  cd.push_back(0);
	  cd.push_back(9);
	  cd.push_back(20);
	  cd.push_back(36);
	  cd.push_back(49);
	  cd.push_back(62);
	  cd.push_back(62);
	  cd.push_back(62);
  }

  void BTSolver::initCummDistribSamuraiLevel(vector<int> & cd)
  {
	  cd.clear();
	  cd.push_back(1);
	  cd.push_back(5);
	  cd.push_back(21);
	  cd.push_back(46);
	  cd.push_back(53);
	  cd.push_back(57);
	  cd.push_back(57);
	  cd.push_back(57);
  }

  double BTSolver::cummDistribErrorFunc(vector<int> & cd1, vector<int> & cd2)
  {
	  double res = -1.0;
	  if (cd1.size() != cd2.size())
	  {
		  m_lError |= SUDOKU_ERROR_GRADING;
		  return res;
	  }
	  
	  res = 0.0;
	  for (unsigned short i = 0; i < cd1.size(); i++)
		  res += (cd1[i] - cd2[i]) * i;
	  
	  return res;
  }

  void BTSolver::calcCummDistr(vector<rank_list *> & rankedCandidates, vector<int> & cd)
  {
	  int runningSum = 0, idx = 0;
	  cd.clear();
	  for (vector<rank_list *>::iterator it = rankedCandidates.begin(); it != rankedCandidates.end(); it++)
	  {
		  if ((*it) != NULL)		  
			  runningSum += (*it)->size();

		  cd.push_back(runningSum);

		  idx++;
	  }

	  cd.pop_back();

  }

  // return values: 1 - Easy, 2 - Medium, 3 - Hard, 4 - Samurai
  //
  // details:
  // we define the rank distribution of a puzzle as a map with key the number of possible assignments
  // and a value is the node count of all nodes with this number of possible assignments.
  // we will fit 
  //  the distribution of easy puzzle (example1.txt) is:
  //  1 -> 8
  //  2 -> 7
  //  3 -> 14
  //  4 -> 13
  //  5 -> 6
  //  6 -> 1
  //
  //  the distribution of medium puzzle (example2.txt) is
  //  1 -> 1
  //  2 -> 14
  //  3 -> 14
  //  4 -> 19
  //  5 -> 3
  //  6 -> 2
  //
  // the distribution of hard puzzle (example3.txt) is
  //  2 -> 9
  //  3 -> 11
  //  4 -> 16
  //  5 -> 13
  //  6 -> 8
  //
  // the distribution of samurai puzzle (example4.txt) is
  //
  // 1 -> 1
  // 2 -> 4
  // 3 -> 16
  // 4 -> 25
  // 5 -> 7
  // 6 -> 4
  //
  //
  int BTSolver::createGrade(vector<rank_list *> & rankedCandidates)
  {
	 


	  if (m_pSrc->getDim() == Puzzle::CLASSIC_SUDOKU_DIM &&
		  m_pSrc->getRegionDim() == Puzzle::CLASSIC_SUDOKU_REGION_DIM)
	  {
		  vector<int> easyCD, mediumCD, hardCD, samuraiCD, srcCD;
		  initCummDistribEasyLevel(easyCD);
		  initCummDistribMediumLevel(mediumCD);
		  initCummDistribHardLevel(hardCD);
		  initCummDistribSamuraiLevel(samuraiCD);
		  calcCummDistr(rankedCandidates,srcCD);

		  vector<double> errVect;
		  double res = -1.0;
		  res = cummDistribErrorFunc(srcCD, easyCD);
		  errVect.push_back(res);
		  res = cummDistribErrorFunc(srcCD, mediumCD);
		  errVect.push_back(res);
		  res = cummDistribErrorFunc(srcCD, hardCD);
		  errVect.push_back(res);
		  res = cummDistribErrorFunc(srcCD, samuraiCD);
		  errVect.push_back(res);

		  int grade = 1;
		  for (unsigned char i = 0; i < errVect.size(); i++)
		  {
			  if (errVect[i] > 0)
				  grade = i + 1;
		  }


		 /* double min = numeric_limits<double>::max();
		  int idx_min = -1;
		  for (unsigned short i = 0; i < errVect.size(); i++)
		  {
			  if (min > errVect[i])
			  {
				  min = errVect[i];
				  idx_min = i;
			  }
		  }

		  return idx_min+1;*/
		  return grade;

	  }
	  else
	  {
		  m_lError |= SUDOKU_ERROR_FEATURE_NOT_IMPLEMENTED;
		  return -1;
	  }

	 
  }

  // creates a grade using iter count; return values: 1 - Easy, 2 - Medium, 3 - Hard, 4 - Samurai
  int BTSolver::createGrade(int iterCount)
  {
	  if (iterCount < 1000)
		  return 1;
	  else if (iterCount >= 1000 && iterCount < 8000)
		  return 2;
	  else if (iterCount >= 8000 && iterCount < 150000)
		  return 3;
	  else
	      return 4;

  }

  bool BTSolver::solve()
  {
	  bool res = true;
      res &= assign_rank_to_candidates(m_pSol,m_vRankedCandidates);
      if (!res)
    	  return res;

	  //m_iGrade = createGrade(m_vRankedCandidates);


#ifdef _DEBUG
      print_ranked_candidates(m_vRankedCandidates);
#endif

      m_lstRankedCandidates = init_rank_node_list(m_vRankedCandidates);

	
      res &= solve_internal(m_lstRankedCandidates);
	  
	  m_iGrade = createGrade(m_iIterCount);

	  return res;
  }

  bool BTSolver::process_line_assignments(Line * l, set<char> & assigned)
  {
	  if (l == NULL)
	  {
	     m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
	     return false;
	  }

	  Symbol * cur=NULL;
	  for (int i=0; i < l->getDim(); i++)
	  {
	     cur = l->getSymbols()[i];
	     if (cur != NULL)
	     {
	        if (!cur->isEmpty())
	           assigned.insert(cur->getValue());
	     }
	     else
	     {
	        m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
	        return false;
	     }
	  }

	  return true;
  }

  bool BTSolver::process_region_assignments(Region * r, set<char> & assigned)
  {
	  if (r == NULL)
	  {
	     m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
	     return false;
	  }

	  Symbol * cur=NULL;

	  for (int i=0; i < r->getSymbCount(); i++)
	  {
	     cur = r->getSymbols()[i];
	     if (cur != NULL)
	     {
	        if (!cur->isEmpty())
	           assigned.insert(cur->getValue());
	     }
	     else
	     {
	        m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
	        return false;
	     }
	  }

	  return true;
  }


  bool BTSolver::get_available_assignments(Symbol * s,list<char>&  assignments)
  {
	  if (s == NULL)
		  return false;

      set<char> assigned;

      if (!process_line_assignments(s->getRow(),assigned))
    	  return false;

      if (!process_line_assignments(s->getCol(),assigned))
    	  return false;

      if (!process_region_assignments(s->getRegion(),assigned))
    	  return false;

      const set<char>& symbTable = *m_pSrc->getSymbolTable();

      set<char> diff;
      set_symmetric_difference(assigned.begin(),assigned.end(),symbTable.begin(),symbTable.end(),inserter(diff,diff.end()));

      assignments.assign(diff.begin(), diff.end());


      return true;
  }


  bool BTSolver::assign_rank_to_candidates(Puzzle * puzzle,
		  vector<rank_list *> & rankedCandidates)
  {
     bool res=true;
     if (puzzle != NULL && m_lError == 0)
     {
    	HorizLine *  curRow=NULL;
    	Symbol * curSymbol=NULL;
    	list<char> * curAssignments = NULL;
    	unsigned short curRank=0;
    	rank_list * curRankList = NULL;

        for (int i = 0; i < puzzle->getDim(); i++)
        {
           curRow = puzzle->getRows()[i];
           for (int j = 0; j < puzzle->getDim(); j++)
           {
        	   if (curRow == NULL)
        	   {
        	       m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
        	      return false;
        	   }

               curSymbol = curRow->getSymbols()[j];
               if (curSymbol == NULL)
               {
                  m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
                  return false;
               }

               if (curSymbol->isEmpty())
               {
            	   if (curSymbol->getAssignments() == NULL)
                       curAssignments = new list<char>();
            	   else
            	   {
            		   curAssignments = curSymbol->getAssignments();
            		   curAssignments->clear();
            	   }

                   get_available_assignments(curSymbol,*curAssignments);
                   curSymbol->setAssignments(curAssignments);

                   if (curAssignments->size() > 0)
                      curRank = (unsigned short) curAssignments->size() - 1;
                   else
                   {
                	   m_lError |= SUDOKU_ERROR_UNSOLVABLE_CONFIGURATION;
                	   return false;
                   }



                   curRankList = rankedCandidates[curRank];
                   if (curRankList != NULL)
                   {
                	   curRankList->push_back(rank_pair(curSymbol,curAssignments));
                   }
                   else
                   {
                	   curRankList = new list<rank_pair>();
                	   curRankList->push_back(rank_pair(curSymbol,curAssignments));
                       rankedCandidates[curRank] = curRankList;
                       m_iRankCount++;
                   }
               }
           }

        }
     }
     else
    	 res=false;

     return res;
  }



  bool BTSolver::update_symbols(Symbol* s, Symbol ** arr, unsigned char dim)
  {
	  bool res = true;

	  Symbol * curSymbol = NULL;
	  list<char> * curAssignments = NULL;
	  list<char>::iterator itA;

      for (int i = 0; i < dim; i++)
      {
          curSymbol = arr[i];
          if (curSymbol->isEmpty())
          {
       		  curAssignments = curSymbol->getAssignments();

       		  if (curAssignments != NULL)
       		  {
       			  unsigned short sz = curAssignments->size();
       			  if (sz > 1)
       			  {
                     curAssignments->remove_if(Remover(s->getValue()));
                     if (curAssignments->size() < sz)
                     {
                    	 // there was a removal so we to save the value of the removed
                    	 // in order to be able to restore it afterwards and also we
                    	 // need to update the corresponding rank list
                    	 curSymbol->setLastRemoved(s->getValue());


                     }
       			  }
       			  else
       			  {
       				  if (sz == 1)
       				  {
                         if (curAssignments->front() == s->getValue())
                         {
                             return false;
                         }
       				  }
       				  else
       				  {
       					 m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
       					 return false;
       				  }
       			  }
       		  }
       		  else
       		  {
             	  m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
               	  return false;
       		  }


          }
          //TO DO: handle the case where the current symbol is not an empty symbol
          // and it has the same value as s. In this case update_symbol should return
          // false without registering an error condition. If I do that then example4
          // fails on the backtracking and returns unsolvable configuration error on
          // the 11317th iteration which means that another bug is lurking around
          else
            {
          	  if (curSymbol != s && s->getValue() == curSymbol->getValue())
        		  return false;
            } 

      }

	  return res;

  }


  bool BTSolver::update_line(Symbol * s, Line * l)
  {
	  bool res = true;
	  if (l != NULL && l->getSymbols() != NULL)
	  {
          res &= update_symbols(s,l->getSymbols(),l->getDim());
          if (!res)
        	  return false;
	  }
	  else
	  {
		  m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
		     return false;
	  }

	  return res;
  }

  bool BTSolver::update_region(Symbol * s, Region * r)
  {
	  bool res = true;
	  if (r != NULL && r->getSymbols() != NULL)
	  {
          res &= update_symbols(s,r->getSymbols(),r->getDim());
          if (!res)
        	  return false;
	  }
	  else
	  {
		  m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
		     return false;
	  }

	  return res;
  }


  // called in case the current symbol was empty but now it has been assigned a value.
  // this means that the other symbols in the containing row, column and region need
  // have update in their candidate lists. The update in the candidate lists will
  // trigger an update in the map with the list of ranked pairs since the rank of all
  // empty symbols in the containing row, column and region will be decremented.
  //

  bool BTSolver::update_assignments(Symbol * s)
  {
	  bool res = true;

	  res &= update_line(s,s->getRow());
      if (!res)
    	  return false;

      res &= update_line(s,s->getCol());
      if (!res)
          return false;


      res &= update_region(s,s->getRegion());
      if (!res)
    	  return false;

	  return res;
  }


  bool BTSolver::restore_symbol(Symbol* s, Symbol ** arr, unsigned char dim)
  {
  	  bool res = true;

  	  if (s == NULL)
  	  {
  		  m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
  		  return false;
  	  }


  	  Symbol * curSymbol = NULL;
  	  list<char> * curAssignments = NULL;
  	  list<char>::iterator itA;

        for (int i = 0; i < dim; i++)
        {
            curSymbol = arr[i];
            if (curSymbol->isEmpty())
            {
         		  curAssignments = curSymbol->getAssignments();
         		  if (curAssignments != NULL)
         		  {
         			  if (curSymbol->getLastRemoved() == s->getValue() && !s->isEmpty())
         			  {
         				 unsigned short sz = curAssignments->size();

         				 list<char>::iterator it = find(curAssignments->begin(),curAssignments->end(),s->getValue());

		                 // restore the last removed symbol value only in case it has not been done already from
		                 // previous restore line invocation
         				 if (it == curAssignments->end())
         				 {
         				    curAssignments->push_back(s->getValue());
							curSymbol->popLastRemoved();
							
         				 }

						 


         			  }
         		  }
         		  else
         		  {
               	     m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
                 	  return false;
         		  }
            }

        }

  	  return res;

  }


  bool BTSolver::restore_line(Symbol * s, Line * l)
  {
  	  bool res = true;
  	  if (l != NULL && l->getSymbols() != NULL)
  	  {
            res &= restore_symbol(s,l->getSymbols(),l->getDim());
            if (!res)
          	  return false;
  	  }
  	  else
  	  {
  		  m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
  		     return false;
  	  }

  	  return res;
   }

   bool BTSolver::restore_region(Symbol * s, Region * r)
   {
  	  bool res = true;
  	  if (r != NULL && r->getSymbols() != NULL)
  	  {
            res &= restore_symbol(s,r->getSymbols(),r->getDim());
            if (!res)
          	  return false;
  	  }
  	  else
  	  {
  		  m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
  		     return false;
  	  }

  	  return res;
   }



  bool BTSolver::restore_assignment(Symbol * s)
  {
	  bool res = true;

	 	  res &= restore_line(s,s->getRow());
	       if (!res)
	     	  return false;

	       res &= restore_line(s,s->getCol());
	       if (!res)
	           return false;


	       res &= restore_region(s,s->getRegion());
	       if (!res)
	     	  return false;

	 	  return res;
  }


  bool BTSolver::solve_internal(RankNode * head)
  {
	  m_iIterCount=0;

	  bool res = true;

	  RankNode * curNode = head;

	  Symbol * curSymbol=NULL;
	  list<char> * curAssignments=NULL;
	  char curChar= 0;
	  bool processed = false;
	  while (curNode != NULL)
	  {
	        curSymbol = curNode->Val->first;


	        if (curSymbol == NULL)
	        {
	           m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
	           return false;
	        }



	        curAssignments = curSymbol->getAssignments();
	        if (!curSymbol->isEmpty())
	        {

               // this symbol is not empty because the algorithm has returned to it due to backtracking
	           // so we need to make it empty and restore the state of the enclosing containers
		       //
	           // updating the assignments with the new change
			   // failed which indicates infeasible configuration
			   // was reached so restore the assignments before the
			   // last attempt and check for a different value to be
			   // assigned to curSymbol
			   if (!restore_assignment(curSymbol))
				   return false;


			   if (!curSymbol->isEmpty())
			   {
				   curAssignments->push_back(curSymbol->getValue());
				   curSymbol->setValue(0);
				   curSymbol->popLastRemoved();
			   }
			   // increment the fail count because the whole sequence after it failed so it is a failure for
			   // the current symbol as well.
			   curSymbol->incrementFailedCount();


	        }



			if (curAssignments != NULL)
			{
				if (!curAssignments->empty())
				{
				   processed = false;
				   int idx = 0, sz = (int) curAssignments->size();
                   int newSz = sz - (int) curSymbol->getFailedCount();

				   while (idx++ < newSz)
				   { // while loop start
					  curChar = curAssignments->front();
					  if (curChar != 0)
					  {
						curAssignments->pop_front();
						curSymbol->setValue(curChar);
						curSymbol->setLastRemoved(curChar);

						m_iIterCount++;
#ifdef _DEBUG
				//		cout << "Iteration count : " << m_iIterCount << endl;

#endif
	
						res = update_assignments(curSymbol);


#ifdef _DEBUG
				//		cout << endl << "[" << (int) curSymbol->getRow()->getIdx()
				//		     << "," << (int) curSymbol->getCol()->getIdx() << "] with "
				//		     << sz << " candidates. trying " << curChar << ".. "  << endl;
#endif


						if (!res)
						{
							if (m_lError == SUDOKU_NO_ERROR)
							{

							   // updating the assignments with the new change
							   // failed which indicates infeasible configuration
							   // was reached so restore the assignments before the
							   // last attempt and check for a different value to be
							   // assigned to curSymbol
							   if (!restore_assignment(curSymbol))
								   return false;

							   if (!curSymbol->isEmpty())
							   {
							      curAssignments->push_back(curSymbol->getValue());
                                  curSymbol->popLastRemoved();
							      curSymbol->setValue(0);
							   }
							   curSymbol->incrementFailedCount();

#ifdef _DEBUG
				//		       cout << "[" << (int) curSymbol->getRow()->getIdx()
				//		    		<< "," << (int) curSymbol->getCol()->getIdx()
				//		    		<< "] failed to assign " << curChar << ".. "  << endl;
#endif


							 }
							 else
							   return false;
						 }
						 else
						 {
							 // done with the current symbol. proceed one level further
							 // down onto the solution tree

							 if (idx == sz)
								 curSymbol->setCanChoose(false);

#ifdef _DEBUG
				//		    cout << "[" << (int) curSymbol->getRow()->getIdx()
				//		    	 << "," << (int) curSymbol->getCol()->getIdx()
				//		    	 << "] Assigned " << curChar << endl;


#endif
							processed = true;
							break;
						 }
					   }
					   else
					   {
						  m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
						  return false;
					   }



					 } // while loop end

				     if (idx >= sz)
				 			curSymbol->setCanChoose(false);
				     else
				    	  curSymbol->setCanChoose(true); // TO DO: check the validity of this 9-16-15 6:00am

					 if (!processed)
					 {
						 // No feasible assignment was found for the current symbol where
						 // all attempts lead to an infeasible configuration. So take a step
						 // back (backtracking) one level up the solution tree if the stack
						 // is not empty
						 curSymbol->resetFailedCount();

					     restore_assignment(curSymbol);

					     if (!curSymbol->isEmpty())
					     {
						    curSymbol->getAssignments()->push_back(curSymbol->getValue());
					        curSymbol->setValue(0);
					        curSymbol->popLastRemoved(); 
					     }
						   if (curSymbol->getAssignments()->size() > 1)
								curSymbol->setCanChoose(true);

						  

						 curNode = curNode->Prev;

				

						 while ( curNode != NULL && (!curNode->Val->first->getCanChoose() || curNode->Val->first->getFailedCount() == curNode->Val->first->getAssignments()->size()))
						 {

							 if (curNode->Val->first->getFailedCount() == curNode->Val->first->getAssignments()->size())  // TO DO: check the validity of this 9-16-15 6:00am
								 curNode->Val->first->setCanChoose(true);

							 curNode->Val->first->resetFailedCount();

							 restore_assignment(curNode->Val->first);

							 if (!curNode->Val->first->isEmpty())
							 {

							   curNode->Val->first->getAssignments()->push_back(curNode->Val->first->getValue());
                               curNode->Val->first->popLastRemoved();
                               curNode->Val->first->setValue(0);
							 }

							 curNode = curNode->Prev;
						 }

						 if (curNode == NULL)
						 {
							 // we cannot backtrack from the head of the list with the rank_pairs so it has to be
							 // infeasible configuration
							 m_lError |= SUDOKU_ERROR_UNSOLVABLE_CONFIGURATION;
							 return false;
						 }



						 continue;
					 }
				}
				else
				{
				   //m_lError |= SUDOKU_ERROR_UNSOLVABLE_CONFIGURATION;
				   //return false;



					 // No feasible assignment was found for the current symbol where
					 // all attempts lead to an infeasible configuration. So take a step
					 // back (backtracking) one level up the solution tree if the stack
					 // is not empty

					 restore_assignment(curSymbol);

					 if (!curSymbol->isEmpty())
					 {
						 curSymbol->resetFailedCount();


						curSymbol->getAssignments()->push_back(curSymbol->getValue());
						curSymbol->setValue(0);
						curNode->Val->first->popLastRemoved();
					 }


					 curNode = curNode->Prev;




					 while ( curNode != NULL && (!curNode->Val->first->getCanChoose() || curNode->Val->first->getFailedCount() == curNode->Val->first->getAssignments()->size()))
					 {

						 if (curNode->Val->first->getFailedCount() == curNode->Val->first->getAssignments()->size())  // TO DO: check the validity of this 9-16-15 6:00am
							 curNode->Val->first->setCanChoose(true);

						 curNode->Val->first->resetFailedCount();

						 restore_assignment(curNode->Val->first);

						 if (!curNode->Val->first->isEmpty())
						 {

						   curNode->Val->first->getAssignments()->push_back(curNode->Val->first->getValue());
						   curNode->Val->first->popLastRemoved();
						   curNode->Val->first->setValue(0);
						 }

						 curNode = curNode->Prev;

					 }

					 if (curNode == NULL)
					 {
						 // we cannot backtrack from the head of the list with the rank_pairs so it has to be
						 // infeasible configuration
						 m_lError |= SUDOKU_ERROR_UNSOLVABLE_CONFIGURATION;
						 return false;
					 }

					 continue;

				}
			}
			else
			{
			   m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
			   return false;
			}

	        curNode = curNode->Next;


	  }

#ifdef _DEBUG
	  cout << endl << "Total iteration count: " << m_iIterCount << endl << endl;
#endif

	  return res;

  }

  void BTSolver::cleanup(RankNode * head)
  {
	
	  // clean up the nodes of the RankNode list
	  RankNode * next = NULL;
	  while (head != NULL)
	  {
		  next = head->Next;
		  delete head;
		  head = next;
	  }


  }

  void BTSolver::cleanup(Puzzle * puzzle, vector<rank_list *>& ranks, RankNode * head)
  {
	  delete puzzle;

	  for (vector<rank_list *>::iterator it = ranks.begin(); it != ranks.end(); it++)
		 if (*it != NULL)
		   delete *it;

	  // clean up the nodes of the RankNode list
	  cleanup(head);


  }

  // validate a different configuration obtained by modifying the assignment list 
  // removing a single character which is the value of the current symbol in the original solution
  // if the new config still can be solved then the original solution is not unique
  bool BTSolver::validate_internal(char val, int idx)
  {
	  bool res = true;
	  m_lError = SUDOKU_NO_ERROR; // clear the errors from the previous iteration
	  Puzzle * srcCopy = m_pSrc->getCopy();

	  vector<rank_list *> rankedCandidatesCopy(m_pSrc->getDim(), NULL);
	  res &= assign_rank_to_candidates(srcCopy, rankedCandidatesCopy);
	  if (!res)
		  return res;
	  
	  // the second list serves as a scratch pad in the validation process
	  RankNode * head = init_rank_node_list(rankedCandidatesCopy);
	  RankNode * cur = head;

	  Symbol *curSymbSrc = NULL;
	  list<char> * curListSrc = NULL;
	  list<char>::iterator itA;
	  int i = 0;	  
	  while (cur != NULL && i++ < idx)	  		  
		 cur = cur->Next;
	  		  
	  curSymbSrc = cur->Val->first;
      curListSrc = curSymbSrc->getAssignments();

	  itA = find(curListSrc->begin(), curListSrc->end(), val);
	  if (itA == curListSrc->end())
      {
		  cleanup(srcCopy, rankedCandidatesCopy, head);
		 m_lError |= SUDOKU_ERROR_INCONSISTENT_INTERNAL_STATE;
		 return false;
	  }
	 
      curListSrc->erase(itA);
	 
	  res &= solve_internal(head);
		
	  cleanup(srcCopy, rankedCandidatesCopy, head);

	  return res;

  }

  // validates the solution uniqueness
  bool BTSolver::validate()
  {
	  bool res = true;
    
	  int iterCount = m_iIterCount;
      RankNode * head = m_lstRankedCandidates;
	  int idx = 0;
      while (head != NULL)
      {
		 if (validate_internal(head->Val->first->getValue(), idx++))
		   return false;
       	
    	 head = head->Next;
      }
	  m_iIterCount = iterCount; // restore the correct gr
	  cleanup(m_lstRankedCandidates);
	  m_lError &= ~SUDOKU_ERROR_UNSOLVABLE_CONFIGURATION; // clean the spurious errors from the validation process

	  return res;
  }






  void Solver::printToConsole()
  {
	  if (m_pSol != NULL)
		  m_pSol->printToConsole();
  }





}



