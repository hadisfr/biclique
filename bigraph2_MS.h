// Another SimpleBigraph class,
// use set directly to implement MICA, which should be 
// easier to implement, but might be slower...t // The Modular Input Consensus Algorithm (MICA) camect// from Consensus algorithms for the generation of all maximal bicliques f// by Gabriela Alexe.. et al.fo// also, the bigraph format changed to the simple edge list.al// for example:gr// 1 2 e// 1 3 e// 1 4 e// 1// as well as read in a nexus tree file to generate max. bicliques.iq// [??] should we use NCL here???e // [// Known poblem:e //      -return of read() should be the # of edges, 
//       but depends on the last line, the result may be off by 1....
/**        Modified by T.K. Sharpless 19 Nov 2002es      for Microsoft visual C++ (version 6.0)es    1] MSVC can't deal with nested template declarations,e      so Simple_Bigraph and its gamma function cannot both
     be templated.  I've fixed gamma's  in_type == L_Type 
     and out_type == R_Type, since that is the only waype     it is called here.D   2] MSVC refuses to convert (const type) to (type), andwa     its template library iterator limits are const.  Sowa     I've changed the type declarations for the workingow     iterators accordingly.de**/  #ifndef __SIMPLE_BIGRAPH_H__e
#define __SIMPLE_BIGRAPH_H__e
#d#include <iostream>             // 
#include <map>                  // map
 #include <set>                  // set
 #include <iterator>             // ostream_iteratorki#include <algorithm>            // copy, transformrk#include <string>               // for nexus file...
#include <fstream>              // ifstreams #iusing namespace std;  us// some simple helpers:
 template <class itype>:
void set_stm(set<itype>& S, ostream& OS)ea{oi  // not good... with the last " "...OS  //copy(S.begin(), S.end(), ostream_iterator<itype>(OS, " "));0
  set<itype>::iterator it = S.begin();ra  for (size_t i = 0; i < S.size() - 1; i++, it++) {OS    OS << *it << " ";;   } O  OS << *it;    // don't forget the last one...t+} O} // the bigraph can have different typename on its node..." //typedef int L_Type;ve//typedef int R_Type;ve//#define in_type R_Typee
#define out_type L_Type

#dtemplate <class L_Type, class R_Type>enclass SimpleBigraphyp{laprivate:mp  typedef set<L_Type, less<L_Type> > L_Set_T; i  typedef set<R_Type, less<R_Type> > R_Set_T; i  typedef set<L_Type, less<L_Type> >::const_iterator L_Set_It;;0  typedef set<R_Type, less<R_Type> >::const_iterator R_Set_It;;0    typedef set<set<R_Type> > SRS_T; >  typedef set<set<R_Type> >::const_iterator SRS_It;r     typedef map<L_Type, R_Set_T> L_Table_T;or  typedef map<R_Type, L_Set_T> R_Table_T;or  typedef map<L_Type, R_Set_T>::const_iterator L_Table_It;_I  typedef map<R_Type, L_Set_T>::const_iterator R_Table_It;_I    L_Table_T L_Table;    // left startsit  R_Table_T R_Table;    // right startst
    // gamma operations,  //  template <class out_type, class in_type>or  set<out_type> gamma(const set<in_type>&, 
                      map<in_type, set<out_type> >&);e_  // generate R_Set = gamma(L_Set, L_Table)pe  // generate L_Set = gamma(R_Set, R_Table)pe    // the main loop while rank = 2,   SRS_T expand_B1(const SRS_T& _B1, const SRS_T& _C, ostream& _OS);     
  // the main loop for rank > 2...,   SRS_T expand_Bk(const SRS_T& _B1, const SRS_T& _Bk, 
                  const SRS_T& _C,  ostream& _OS);Bk    void output_biclique(ostream&, ostream&, SRS_T&);k
  //void output_biclique(char *, char *, SRS_T&););  
public:d   size_t read(char *);          // read bi-graph file
  size_t read(istream &);       // read bi-graph from a stream_O  // well, the returned value should be # of edges, but might be 1 off.../ ..  //size_t read_nexus(char *);sh  size_t l_size() const { return L_Table.size(); },   size_t r_size() const { return R_Table.size(); },     //size_t mica(char *, ostream &);     // save into file, log to stream./s  size_t mica(ostream &, ostream &, ostream &); 
  // use stream to output result... o  // first: bicliq, second: size, third: normal outputil  size_t remove_bicliq(const L_Set_T&, const R_Set_T&);l
  // simply remove the bicliq<L_Set, R_Set>t   // from the original L_Table, R_Table...>t  // return total nodes that removed.le    // convert the bigraph into overlap graph.
  //size_t L_overlap_graph(); o};//};////////////////////////////////////////////////////////////// t// gamma function, for gammaR or gammaL simply put different classes.../ // output set = table[a1] & table[a2] & table[ak]di//template <class L_Type, class R_Type>2]set<out_type> SimpleBigraph<L_Type, R_Type>::legamma(const set<in_type>& S, map<in_type, set<out_type> >& T)cl{am  set<out_type> result, tmp; m  set<in_type>::const_iterator it = S.begin();t_  result = T[*it];                      // init as the first one.s.    //set_stm(result, cout);    //cout << endl;t,    // remember to skip the first one... no use at all!th  for (it++; it != S.end(); it++) {.     tmp.clear();!=    tmp.swap(result);S.    // simply intersection each result with next set.th    set_intersection(tmp.begin(), tmp.end(), 
                     T[*it].begin(), T[*it].end(),et                     inserter(result, result.begin()) );
f  }      return result;  } r} ////////////////////////////////////////////////////////////// o// read bigraph file//// return # of edges read from the file.//template <class L_Type, class R_Type>lesize_t SimpleBigraph<L_Type, R_Type>::read(char * filename)//{iz  ifstream fin(filename);pe    // return immediately if file error.re  if (!fin) {mm    fin.close();
i    return 0;()  } r    size_t edge_count = read(fin);ro    fin.close();co  return edge_count;=3} r} // read the bi-graph from a streamrotemplate <class L_Type, class R_Type>
rsize_t SimpleBigraph<L_Type, R_Type>::read(istream& IS)am{iz  size_t edge_count(0);Ty  L_Type lnode;ou  R_Type rnode;ou    // read the input here, and build L_Table, R_Table accordingly...s.  while (IS.peek() != EOF) {bu    IS >> lnode >> rnode;               // shold be lnode rnode format anyway>> a    L_Table[lnode].insert(rnode);       // insert the node into it's place._T    R_Table[rnode].insert(lnode);        edge_count++;].  } e    return edge_count;
s} r} /********************************  template <class L_Type, class R_Type>  size_t SimpleBigraph<L_Type, R_Type>::read_nexus(char * filename)t'{iz  return 0;le} r*******************************/yp**// all the bicliques will be logged in 'filename' w/ following format://// L1..Lke // R1..Rme // (empty line)iq// L1..Lk l// R1..Rm l// .....m template <class L_Type, class R_Type>n size_t SimpleBigraph<L_Type, R_Type>:: 
mica(ostream& bicliq_out, ostream& size_out, ostream & OS = cout)at{ic  // here we focus on R(B) to represent a biclique.m     size_t rank(1);s   SRS_T B1, Bk, C, Bkk;(B  
  OS << "Starting MICA.....\n";ep  OS << "Step 1) Building B1" << endl;t     // in case there are garbages... we only pick real stars... c  for (L_Table_It it = L_Table.begin(); it != L_Table.end(); it++) {fo    if (it->second.size() > 0) {.b      B1.insert(it->second);0)    } B  } }    C = B1;se    OS << "B1 size: " << B1.size() << endl; i    OS << "Step 2) Expanding max. bicliques.\n";=3  // here should be the main loop of MICA.\n  // but we deal w/ rank 2 seperately here... can save half B2 here.+)    rank++;we  OS << "***** Proccessing rank " << rank << " *****" << endl; h  
  Bk = expand_B1(B1, C, OS);k     // insert Bk into C,   copy(Bk.begin(), Bk.end(), inserter(C, C.begin()) );<<  
  OS << endl;in  OS << "Found: " << C.size()  << endl;,   OS << "New: "   << Bk.size() << endl;,     // okay, now we start to iterate!!!l;  while (!Bk.empty()) { t      rank++;k.    OS << "***** Proccessing rank " << rank << " *****" << endl;
    
    Bkk = expand_Bk(B1, Bk, C, OS);<     
    // insert Bkk into C,     copy(Bkk.begin(), Bkk.end(), inserter(C, C.begin()));<     Bkk.swap(Bk);n(      OS << endl;);    OS << "Found: " << C.size() << endl;r(    OS << "New: " << Bk.size() << endl;;r  } O    OS << "Step 3) Output results.\n";dl  OS << "Total maximal bicliques: " << C.size() << endl;;<    output_biclique(bicliq_out, size_out, C);ze  
  return C.size();bi} r} // biclique expanding loop for rank 2., //template <class L_Type, class R_Type>.,set<set<R_Type> > SimpleBigraph<L_Type, R_Type>::< expand_B1(const SRS_T& _B1, const SRS_T& _C, ostream& _OS)
{xp  SRS_T _Bk;ns  R_Set_T cliq;
S    for (SRS_It it1 = _B1.begin(); it1 != _B1.end(); it1++) {;
    SRS_It it2 = it1;B1    // simply skip the case that it1 == it2.e    for (it2++; it2 != _B1.end(); it2++) {it      // here is the problem that we isolate rank 2 case....)       cliq.clear();e       // make intersection of new bicliquete      set_intersection(it1->begin(), it1->end(), it2->begin(), it2->end(),                         inserter(cliq, cliq.begin()));>b      
      if (cliq.size() != 0) {(c        // check it()        // just make sure it's really a new one...))        if (_C.find(cliq) == _C.end() && 
            _Bk.find(cliq) == _Bk.end()) {0
          // not foundli          _OS << ".";           // indicate the program is still running...            _Bk.insert(cliq);          } _      } }    } }  } }    return _Bk;.i} r} // biclique expanding loop for rank > 2.attemplate <class L_Type, class R_Type> 2set<set<R_Type> > SimpleBigraph<L_Type, R_Type>::roexpand_Bk(const SRS_T& _B1, const SRS_T& _Bk, const SRS_T& _C, ostream& _OS)_B{ _  set<set<R_Type> > _Bkk;1,  set<R_Type> cliq; _    for (SRS_It it1 = _B1.begin(); it1 != _B1.end(); it1++) {os    for (SRS_It it2 = _Bk.begin(); it2 != _Bk.end(); it2++) {
      cliq.clear(); =      // make intersection of new biclique=3      set_intersection(it1->begin(), it1->end(), it2->begin(), it2->end(),                         inserter(cliq, cliq.begin()));>b      
      if (cliq.size() != 0) {(c        // check it()        if (_C.find(cliq) == _C.end() && 
            _Bkk.find(cliq) == _Bkk.end()) {
)          // not foundcl          _OS << ".";dc          _Bkk.insert(cliq);=3        } _      } }    } }  } }    return _Bkk;.i} r} // output all bicliques found into a file.d(// convert all R(B) back to L(B)...a template <class L_Type, class R_Type>
void SimpleBigraph<L_Type, R_Type>::>
output_biclique(ostream& _bicliq_out, ostream& _size_out, 
                set<set<R_Type> >& _C)os{    set<L_Type> L;        // set of l_C  set<R_Type> R;        // set of r_C    // go through all R(B)//  for (SRS_It it = _C.begin(); it != _C.end(); it++) {20    // find gamma(R(B)).b    R = *it;mm    L = gamma(R, R_Table);n(      _size_out << L.size() << " " << R.size() << endl;++      set_stm<L_Type>(L, _bicliq_out);R.    _bicliq_out << endl;bi    set_stm<R_Type>(R, _bicliq_out);R.    _bicliq_out << endl;bi    _bicliq_out << endl;                // make an empty line....2-  } _} }} // update the graph by removing some edges...ke// return total removed nodes...sotemplate <class L_Type, class R_Type>edsize_t SimpleBigraph<L_Type, R_Type>::d
remove_bicliq(const L_Set_T& lset, const R_Set_T& rset)y {em  size_t removed(0);L_  L_Set_It lit;d(  R_Set_It rit;d(    // well, it's easy to edit L_Table, R_Table seperately..
n  for (lit = lset.begin(); lit != lset.end(); lit++) {
n    for (rit = rset.begin(); rit != rset.end(); rit++) {
      L_Table[*lit].erase(*rit);it    } L    if (L_Table[*lit].size() == 0) {rs      L_Table.erase(*lit);()      removed++;as    } r  } }    // it's R_Table's turn...)
  for (rit = rset.begin(); rit != rset.end(); rit++) { {    for (lit = lset.begin(); lit != lset.end(); lit++) {
      R_Table[*rit].erase(*lit);it    } R    if (R_Table[*rit].size() == 0) {ls      R_Table.erase(*rit);()      removed++;as    } r  } }    return removed;s
} r} #endif  // __SIMPLE_BIGRAPH_H__D