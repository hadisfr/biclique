// Another SimpleBigraph class,
// use set directly to implement MICA, which should be 
// easier to implement, but might be slower...
// The Modular Input Consensus Algorithm (MICA) came
// from Consensus algorithms for the generation of all maximal bicliques
// by Gabriela Alexe.. et al.
// also, the bigraph format changed to the simple edge list.
// for example:
// 1 2
// 1 3
// 1 4
//
// as well as read in a nexus tree file to generate max. bicliques.
// [??] should we use NCL here???
//
// Known poblem:
// 	-return of read() should be the # of edges, 
//	 but depends on the last line, the result may be off by 1....

#ifndef __SIMPLE_BIGRAPH_H__
#define __SIMPLE_BIGRAPH_H__

#include <iostream>		// 
#include <map>			// map
#include <set>			// set
#include <iterator>		// ostream_iterator
#include <algorithm>		// copy, transform
#include <string>		// for nexus file...
#include <fstream>		// ifstream

using namespace std;

// some simple helpers:
template <class itype>
void set_stm(set<itype>& S, ostream& OS)
{
  // not good... with the last " "...
  //copy(S.begin(), S.end(), ostream_iterator<itype>(OS, " "));
  typename set<itype>::iterator it = S.begin();
  for (size_t i = 0; i < S.size() - 1; i++, it++) {
    OS << *it << " ";
  }
  OS << *it;	// don't forget the last one...
}

// the bigraph can have different typename on its node...
template <class L_Type, class R_Type>
class SimpleBigraph
{
private:
  //typedef set<L_Type, less<L_Type> > L_Set_T;
  //typedef set<R_Type, less<R_Type> > R_Set_T;
  //typedef set<L_Type, less<L_Type> >::iterator L_Set_It;
  //typedef set<R_Type, less<R_Type> >::iterator R_Set_It;
  //
  //typedef set<set<R_Type> > SRS_T;
  //typedef set<set<R_Type> >::iterator SRS_It;
  //
  //typedef map<L_Type, set<R_Type> > L_Table_T;
  //typedef map<R_Type, set<L_Type> > R_Table_T;
  //typedef map<L_Type, set<R_Type> >::iterator L_Table_It;
  //typedef map<R_Type, set<L_Type> >::iterator R_Table_It;

  map<L_Type, set<R_Type> > L_Table;	// left starts
  map<R_Type, set<L_Type> > R_Table;	// right starts

  // gamma operations,
  template <class out_type, class in_type>
  set<out_type> gamma(const set<in_type>&, map<in_type, set<out_type> >&);
  // generate R_Set = gamma(L_Set, L_Table)
  // generate L_Set = gamma(R_Set, R_Table)

  // the main loop while rank = 2
  set<set<R_Type> > expand_B1(const set<set<R_Type> > &, const set<set<R_Type> > &, ostream &);

  // // the main loop for rank > 2...
  set<set<R_Type> > expand_Bk
    (const set<set<R_Type> > &, const set<set<R_Type> > &, const set<set<R_Type> > &,  ostream &);

  void output_biclique(ostream&, ostream&, set<set<R_Type> >&);
  //void output_biclique(char *, char *, SRS_T&);
  
public:
  size_t read(char *);		// read bi-graph file
  size_t read(istream &);	// read bi-graph from a stream
  // well, the returned value should be # of edges, but might be 1 off...

  //size_t read_nexus(char *);
  size_t l_size() const { return L_Table.size(); }
  size_t r_size() const { return R_Table.size(); }

  //size_t mica(char *, ostream &);	// save into file, log to stream.
  size_t mica(ostream &, ostream &, ostream & = cout);	
  // use stream to output result...
  // first: bicliq, second: size, third: normal output
  size_t remove_bicliq(const set<L_Type> &, const set<R_Type> &);
  // simply remove the bicliq<L_Set, R_Set>
  // from the original L_Table, R_Table...
  // return total nodes that removed.

};

//////////////////////////////////////////////////////////////
// gamma function, for gammaR or gammaL simply put different classes...
// output set = table[a1] & table[a2] & table[ak]
template <class L_Type, class R_Type>
template <class out_type, class in_type>
set<out_type> SimpleBigraph<L_Type, R_Type>::
gamma(const set<in_type>& S, map<in_type, set<out_type> >& T)
{
  set<out_type> result, tmp;
  typename set<in_type>::iterator it = S.begin();
  result = T[*it];			// init as the first one.

  //set_stm(result, cout);
  //cout << endl;

  // remember to skip the first one... no use at all!
  for (it++; it != S.end(); it++) {
    tmp.clear();
    tmp.swap(result);
    // simply intersection each result with next set.
    set_intersection(tmp.begin(), tmp.end(), 
		     T[*it].begin(), T[*it].end(),
		     inserter(result, result.begin()) );
  }

  return result;
}

//////////////////////////////////////////////////////////////
// read bigraph file
// return # of edges read from the file.
template <class L_Type, class R_Type>
size_t SimpleBigraph<L_Type, R_Type>::read(char * filename)
{
  ifstream fin(filename);

  // return immediately if file error.
  if (!fin) {
    fin.close();
    return 0;
  }

  size_t edge_count = read(fin);

  fin.close();
  return edge_count;
}

// read the bi-graph from a stream
template <class L_Type, class R_Type>
size_t SimpleBigraph<L_Type, R_Type>::read(istream& IS)
{
  size_t edge_count(0);
  L_Type lnode;
  R_Type rnode;

  // read the input here, and build L_Table, R_Table accordingly...
  while (IS.peek() != EOF) {
    IS >> lnode >> rnode;		// shold be lnode rnode format anyway

    L_Table[lnode].insert(rnode);	// insert the node into it's place.
    R_Table[rnode].insert(lnode);

    edge_count++;
  }

  return edge_count;
}

/********************************
template <class L_Type, class R_Type>
size_t SimpleBigraph<L_Type, R_Type>::read_nexus(char * filename)
{
  return 0;
}
*******************************/

// all the bicliques will be logged in 'filename' w/ following format:
// L1..Lk
// R1..Rm
// (empty line)
// L1..Lk
// R1..Rm
// .....
template <class L_Type, class R_Type>
size_t SimpleBigraph<L_Type, R_Type>::
mica(ostream& bicliq_out, ostream& size_out, ostream & OS)
{
  // here we focus on R(B) to represent a biclique.

  size_t rank(1);
  set<set<R_Type> > B1, Bk, C, Bkk;
  
  OS << "Starting MICA.....\n";
  OS << "Step 1) Building B1" << endl;

  // in case there are garbages... we only pick real stars...
  for (typename map<L_Type, set<R_Type> >::iterator it = L_Table.begin(); 
       it != L_Table.end(); it++) {
    if (it->second.size() > 0) {
      B1.insert(it->second);
    }
  }

  C = B1;

  OS << "B1 size: " << B1.size() << endl;

  OS << "Step 2) Expanding max. bicliques.\n";
  // here should be the main loop of MICA.
  // but we deal w/ rank 2 seperately here... can save half B2 here.

  rank++;
  OS << "***** Proccessing rank " << rank << " *****" << endl;
  
  Bk = expand_B1(B1, C, OS);

  // insert Bk into C
  copy(Bk.begin(), Bk.end(), inserter(C, C.begin()) );
  
  OS << endl;
  OS << "Found: " << C.size()  << endl;
  OS << "New: "   << Bk.size() << endl;

  // okay, now we start to iterate!!!
  while (!Bk.empty()) {

    rank++;
    OS << "***** Proccessing rank " << rank << " *****" << endl;
    
    Bkk = expand_Bk(B1, Bk, C, OS);
    
    // insert Bkk into C
    copy(Bkk.begin(), Bkk.end(), inserter(C, C.begin()));
    Bkk.swap(Bk);

    OS << endl;
    OS << "Found: " << C.size() << endl;
    OS << "New: " << Bk.size() << endl;
  }

  OS << "Step 3) Output results.\n";
  OS << "Total maximal bicliques: " << C.size() << endl;

  output_biclique(bicliq_out, size_out, C);
  
  return C.size();
}

/* biclique expanding loop for rank 2. */
template <class L_Type, class R_Type>
set<set<R_Type> > 
SimpleBigraph<L_Type, R_Type>::
expand_B1(const set<set<R_Type> > & _B1, 
	  const set<set<R_Type> > & _C1, 
	  ostream & _OS)
{
  set<set<R_Type> > _Bk;
  set<R_Type> cliq;

  for (typename set<set<R_Type> >::iterator it1 = _B1.begin(); it1 != _B1.end(); it1++) {
      typename set<set<R_Type> >::iterator it2 = it1;
    // simply skip the case that it1 == it2
    for (it2++; it2 != _B1.end(); it2++) {
      // here is the problem that we isolate rank 2 case....
      cliq.clear();
      // make intersection of new biclique
      set_intersection(it1->begin(), it1->end(), it2->begin(), it2->end(),
		       inserter(cliq, cliq.begin()));
      
      if (cliq.size() != 0) {
	// check it
	// just make sure it's really a new one...
	if (_C1.find(cliq) == _C1.end() && _Bk.find(cliq) == _Bk.end()) {
	  // not found
	  //_OS << ".";		// indicate the program is still running...
	  _Bk.insert(cliq);
	}
      }
    }
  }

  return _Bk;
}

// biclique expanding loop for rank > 2.
template <class L_Type, class R_Type>
set<set<R_Type> > SimpleBigraph<L_Type, R_Type>::
expand_Bk(const set<set<R_Type> > & _B1, 
	  const set<set<R_Type> > & _Bk, 
	  const set<set<R_Type> > & _C1, 
	  ostream & _OS)
{
  set<set<R_Type> > _Bkk;
  set<R_Type> cliq;

  for (typename set<set<R_Type> >::iterator it1 = _B1.begin(); it1 != _B1.end(); it1++) {
    for (typename set<set<R_Type> >::iterator it2 = _Bk.begin(); it2 != _Bk.end(); it2++) {
      cliq.clear();
      // make intersection of new biclique
      set_intersection(it1->begin(), it1->end(), it2->begin(), it2->end(),
		       inserter(cliq, cliq.begin()));
      
      if (cliq.size() != 0) {
	// check it
	if (_C1.find(cliq) == _C1.end() && 
	    _Bkk.find(cliq) == _Bkk.end()) {
	  // not found
	  //_OS << ".";
	  _Bkk.insert(cliq);
	}
      }
    }
  }

  return _Bkk;
}

// output all bicliques found into a file.
// convert all R(B) back to L(B)...
template <class L_Type, class R_Type>
void SimpleBigraph<L_Type, R_Type>::
output_biclique(ostream& _bicliq_out, 
		ostream& _size_out, 
		set<set<R_Type> >& _C1)
{
  set<L_Type> L;	// set of l
  set<R_Type> R;	// set of r

  // go through all R(B)
  for (typename set<set<R_Type> >::iterator it = _C1.begin(); it != _C1.end(); it++) {
    // find gamma(R(B))
    R = *it;
    L = gamma<L_Type, R_Type>(R, R_Table);

    _size_out << L.size() << " " << R.size() << endl;

    set_stm<L_Type>(L, _bicliq_out);
    _bicliq_out << endl;
    set_stm<R_Type>(R, _bicliq_out);
    _bicliq_out << endl;
    _bicliq_out << endl;		// make an empty line....
  }
}

// update the graph by removing some edges...
// return total removed nodes...
template <class L_Type, class R_Type>
size_t SimpleBigraph<L_Type, R_Type>::
remove_bicliq(const set<L_Type> & lset, 
	      const set<R_Type> & rset)
{
  size_t removed(0);
  typename set<L_Type>::iterator lit;
  typename set<R_Type>::iterator rit;

  // well, it's easy to edit L_Table, R_Table seperately..
  for (lit = lset.begin(); lit != lset.end(); lit++) {
    for (rit = rset.begin(); rit != rset.end(); rit++) {
      L_Table[*lit].erase(*rit);
    }
    if (L_Table[*lit].size() == 0) {
      L_Table.erase(*lit);
      removed++;
    }
  }

  // it's R_Table's turn...
  for (rit = rset.begin(); rit != rset.end(); rit++) {
    for (lit = lset.begin(); lit != lset.end(); lit++) {
      R_Table[*rit].erase(*lit);
    }
    if (R_Table[*rit].size() == 0) {
      R_Table.erase(*rit);
      removed++;
    }
  }

  return removed;
}

#endif	// __SIMPLE_BIGRAPH_H__
