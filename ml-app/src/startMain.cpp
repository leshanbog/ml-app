#include "LearningMachineLearning.h"
#include "InfoProvider.h"

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		CONSOLE_OUTPUT(core::Information.find("GetHelp")->second);
	}
	else
	{
		string res;
		core::RunLogic logicProcessing;
		try
		{
			res = logicProcessing.Run(argc-1, ++argv);
		}
		catch (const std::exception& err)
		{
			res = "Error: ";
			res += err.what();
			res += "\n";
		}

		CONSOLE_OUTPUT(res);
	}

	return 0;
}
