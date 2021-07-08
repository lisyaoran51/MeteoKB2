#include "FileSplitCombiner.h"

#include <fstream>
#include "Log.h"

using namespace std;
using namespace Util;


int FileSplitCombiner::Combine(string destinationPath, vector<string> splitPaths)
{

	fstream destinationStream(destinationPath, ios::binary | ios::out);

	if (!destinationStream.is_open()) {
		// error
		return -1;
	}

	for (int i = 0; i < splitPaths.size(); i++) {

		LOG(LogLevel::Debug) << "FileSplitCombiner::Combine() : combine split [" << splitPaths[i] << "].";

		fstream splitStream(splitPaths[i], ios::binary | ios::in);

		if (!splitStream.is_open()) {
			// error
			return -1;
		}


		splitStream.seekg(0, std::ios::end);
		size_t length = splitStream.tellg();

		if (length > 16 * 1024) {
			// error
			return -1;
		}

		char buffer[16 * 1024] = { 0 };

		splitStream.seekg(0, std::ios::beg);
		splitStream.read(buffer, length);

		splitStream.close();
		destinationStream.write(buffer, length * sizeof(char));
	}

	destinationStream.close();

	return 0;
}
