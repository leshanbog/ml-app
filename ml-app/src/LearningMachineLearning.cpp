#include <cstring>

#include "LearningMachineLearning.h"
#include "InfoProvider.h"


namespace core
{


string RunLogic::Run(int argc, char *argv[])
{
	if (ParseParameters(argc, argv))
	{
		LoadData();
		string res = StartAlg();
		return res;
	}
	return "";
}

void RunLogic::printHelp(int argc, char *argv[]) const
{
	if (argc == 0)
	{
		CONSOLE_OUTPUT(Information.find("HelpCommand")->second);
	}
	else
	{
		auto query = Information.find("HelpCommand" + string(argv[0]));
		if (query != Information.end())
		{
			CONSOLE_OUTPUT(query->second);
		}
		else
		{
			throw std::runtime_error(Information.find("WrongCommand")->second + Information.find("GetHelp")->second);
		}
	}
}

void RunLogic::printAlgs() const
{
	CONSOLE_OUTPUT(Information.find("AlgsCommand")->second);
}

void RunLogic::printInfo(int argc, char *argv[]) const
{
	if (argc == 0)
	{
		CONSOLE_OUTPUT(Information.find("HelpCommandinfo")->second);
	}
	else
	{
		int algNumber = atoi(argv[0]);
		if (algNumber > 20 && algNumber < 30)
		{
			CONSOLE_OUTPUT(Information.find("InfoCommand2i")->second);
			algNumber%=10;
		}
		else if (algNumber > 30 && algNumber < 40)
		{
			CONSOLE_OUTPUT(Information.find("InfoCommand3i")->second);
			algNumber%=10;
		}

		auto msg = Information.find("InfoCommand" + std::to_string(algNumber));
		if (msg == Information.end())
			throw std::runtime_error(Information.find("NoSuchAlg")->second + Information.find("GetHelp")->second);
		CONSOLE_OUTPUT(msg->second);
	}
}

bool RunLogic::ParseParameters(int argc, char *argv[])
{
	string command(argv[0]);

	if (command == "help")
	{
		printHelp(argc-1, ++argv);
		return false;
	}
	else if (command == "algs")
	{
		printAlgs();
		return false;
	}
	else if (command == "info")
	{
		printInfo(argc-1, ++argv);
		return false;
	}
	else if (command == "ml")
	{
		if (argc < 3)
		{
			CONSOLE_OUTPUT(Information.find("HelpCommandml")->second);
			return false;
		}
		m_parameters["Data file name"] = argv[1];
		m_parameters["Algorithm"] = argv[2];


		// default values
		m_parameters["Separator"] = ',';
		m_parameters["Feature names provided"] = "1";
		m_parameters["Folds number"] = "5";
		m_parameters["Normalization"] = "0";

		int i = 3;

		while (i < argc)
		{
			if (argv[i][0] == '-')
			{
				m_parameters[getParam(&argv[i][1])] = argv[i+1];
				i+=2;
			}

		}

		return true;
	}
	else
	{
		throw std::runtime_error(Information.find("WrongCommand")->second + Information.find("GetHelp")->second);
	}
}

string RunLogic::getParam(const char* arg) const
{
	if (!strcmp(arg, "fn"))
	{
		return "Folds number";
	}
	if (!strcmp(arg, "sep"))
	{
		return "Separator";
	}
	if (!strcmp(arg, "a"))
	{
		return "Arguments for algorithm";
	}
	if (!strcmp(arg, "f"))
	{
		return "Feature names provided";
	}
	if (!strcmp(arg, "n"))
	{
		return "Normalization";
	}
	throw std::runtime_error(Information.find("WrongArgs")->second + Information.find("GetHelp")->second);
}

void RunLogic::LoadData()
{
	std::ifstream fin(m_parameters["Data file name"]);
	char separator = m_parameters["Separator"][0];
	vector <string> featureNames = GetFeatureNames(fin, separator);

	Obj x;
	string descriptionOfObject;
	vector <Obj> data;

	// reading data file
	while (!fin.eof())
	{
		getline(fin, descriptionOfObject);
		if (descriptionOfObject.size() == 0)
			continue;
		x = Obj(descriptionOfObject, separator);
		data.push_back(x);
	}

	m_df = new DataFrame(data);
	m_df->SetFeatureNames(featureNames);

	fin.close();
}

vector <string> RunLogic::GetFeatureNames(std::ifstream& fin, const char separator)
{
	vector <string> featureNames;
	string featuresList;

	if (m_parameters["Feature names provided"] == "1")
	{
		getline(fin, featuresList);
		size_t i = 0;
		while (i < featuresList.size())
		{
			string name = "";
			while (i < featuresList.size() && featuresList[i] != separator)
			{
				name += featuresList[i];
				++i;
			}
			++i;
			featureNames.push_back(name);
		}
	}
	else
	{
		for (size_t i = 0; i < featureNames.size(); ++i)
		{
			featureNames.push_back("feature" + std::to_string(i));
		}
	}
	return featureNames;
}

ArgsForAlg RunLogic::ConvertToArgs(const string& params) const
{
	ArgsForAlg args;
	string s = "";
	int i = 0;

	while (i < params.size())
	{
		while (i < params.size() && params[i] != ',')
		{
			s += params[i];
			++i;
		}
		args.argsLongDouble.push_back(stold(s));
		++i;
		s = "";
	}

	return args;
}

string RunLogic::StartAlg()
{
	int foldsNum = stoi(m_parameters["Folds number"]);
	if (foldsNum > m_df->GetDimention().first || foldsNum < 1)
	{
		throw std::runtime_error("Wrong number of folds\n");
	}

	bool normalizationNeeded = m_parameters["Normalization"] == "1" ? true : false;

	if (normalizationNeeded)
		m_df->DoNormalization();

	ArgsForAlg args = ConvertToArgs(m_parameters["Arguments for algorithm"]);
	string res;

	switch (stoi(m_parameters["Algorithm"]))
	{
		case 1:
			res = BuildAndEstimateModel<ConstPrediction>(foldsNum, args);
			break;
		case 2:
			res = BuildAndEstimateModel<DecisionStump>(foldsNum, args);
			break;
		case 3:
			if (args.argsLongDouble.empty())
				args.argsLongDouble = {5};
			res = BuildAndEstimateModel<KNN>(foldsNum, args);
			break;
		case 4:
			if (args.argsLongDouble.empty())
				args.argsLongDouble = { 0.00001, 300000 };
			res = BuildAndEstimateModel<LinearRegression>(foldsNum, args);
			break;
		case 21:
			if (args.argsLongDouble.empty())
				args.argsLongDouble = {25, m_df->GetDimention().first * 0.8};
			res = BuildAndEstimateModel<BaggedAlg<ConstPrediction>>(foldsNum, args);
			break;
		case 22:
			if (args.argsLongDouble.empty())
				args.argsLongDouble = {25, m_df->GetDimention().first * 0.8};
			res = BuildAndEstimateModel<BaggedAlg<DecisionStump>>(foldsNum, args);
			break;
		case 23:
			if (args.argsLongDouble.empty())
				args.argsLongDouble = {25, m_df->GetDimention().first * 0.8, 5};
			res = BuildAndEstimateModel<BaggedAlg<KNN>>(foldsNum, args);
			break;
		case 31:
			if (args.argsLongDouble.empty())
				args.argsLongDouble = {50};
			res = BuildAndEstimateModel<BoostedAlg<ConstPrediction>>(foldsNum, args);
			break;
		case 32:
			if (args.argsLongDouble.empty())
				args.argsLongDouble = {50};
			res = BuildAndEstimateModel<BoostedAlg<DecisionStump>>(foldsNum, args);
			break;
		case 33:
			if (args.argsLongDouble.empty())
				args.argsLongDouble = {50, 5};
			res = BuildAndEstimateModel<BoostedAlg<KNN>>(foldsNum, args);
			break;
		default:
			throw std::runtime_error(Information.find("NoSuchAlg")->second);

	}

	return res;
}

}  // namespace core



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

// TODO: change ArgsForAlgs