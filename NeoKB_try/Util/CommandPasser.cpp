#include "CommandPasser.h"


using namespace Util;


std::string CommandPasser::PassCommand(char const * cmd)
{
	FILE* fp;
	char buffer[256] = {0};
	fp = popen(cmd, "r");
	if (fp == NULL)
	{
		return std::string("unknown");
	}
	fgets(buffer, sizeof(buffer) - 1, fp);
	pclose(fp);
	return std::string(buffer);
}
