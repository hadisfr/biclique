#include "bigraph2.h"
#include "simple_timer.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

typedef int LT;		// type of left column. change the type here according to the data
typedef int RT;		// type of right column.

int main(int argc, char ** argv)
{
  if (argc != 4) {
    cout << "Usage: " << argv[0] 
	 << " <bigraph file> <biclique file> <size dist file>" << endl;
    return 0;
  }

  // change the type here according to the input file.
  SimpleBigraph<LT, RT> SBG;

  ofstream bicliq_fs(argv[2]), size_fs(argv[3]);

  if (!bicliq_fs || !size_fs) {
    cout << "Error output files!" << endl;
    return 0;
  }

  Timer T;

  cout << "Read edges: " << SBG.read(argv[1]) << endl;
  cout << "Left: " << SBG.l_size() << endl;
  cout << "Right: " << SBG.r_size() << endl;

  cout << T.start() << endl;
  //SBG.mica(argv[2], cout);
  SBG.mica(bicliq_fs, size_fs, cout);
  cout << endl;
  cout << T.stop() << endl;
  cout << T.report() << endl;

  bicliq_fs.close();
  size_fs.close();

  return 0;
}
