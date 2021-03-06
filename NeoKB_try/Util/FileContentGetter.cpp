#include "FileContentGetter.h"

#include <fstream>


using namespace std;
using namespace Util;


string FileContentGetter::GetContentFromFile(char const * fname)
{
	
		string id;

		ifstream infile(fname);
		if (!getline(infile, id))
			id = "unknown";

		if (id[id.size()] == '\0')
			id = id.substr(0, id.size() - 1);

		return id;
}

string FileContentGetter::GetFileContentVariable(char const * file, char const * field)
{
	char buff[256];
	FILE* f = fopen(file, "r");
	while (fgets(buff, sizeof(buff) - 1, f) != nullptr)
	{
		if (strncmp(buff, field, strlen(field)) == 0)
		{
			char* p = strchr(buff, ':');
			if (p)
			{
				p++;
				while (p && isspace(*p))
					p++;
			}
			if (p)
			{
				size_t len = strlen(p);
				if (len > 0)
					p[len - 1] = '\0';
				return string(p);
			}
		}
	}
	return string();
}
