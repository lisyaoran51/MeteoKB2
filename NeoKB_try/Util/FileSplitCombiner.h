#ifndef FILE_SPLIT_COMBINER_H
#define FILE_SPLIT_COMBINER_H


#include <vector>

using namespace std;


namespace Util {


	class FileSplitCombiner {

	public:

		static int Combine(string destinationPath, vector<string> splitPaths);

	};


}



#endif