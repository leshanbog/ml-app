#include <cstring>
#include <sstream>

#include "LearningMachineLearning.h"
#include "InfoProvider.h"


namespace core
{


string RunLogic::Run(int argc, char *argv[])
{
	if (m_mainHelper.ParseParameters(argc, argv))
	{
		LoadData();
		string res = StartAlg();
		return res;
	}
	return "";
}

void MainHelper::printHelp(int argc, char *argv[])
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

void MainHelper::printAlgs()
{
	CONSOLE_OUTPUT(Information.find("AlgsCommand")->second);
}

void MainHelper::printInfo(int argc, char *argv[])
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

bool MainHelper::ParseParameters(int argc, char *argv[])
{
	string command(argv[0]);

	if (command == "help")
	{
		MainHelper::printHelp(argc-1, ++argv);
		return false;
	}
	else if (command == "algs")
	{
		MainHelper::printAlgs();
		return false;
	}
	else if (command == "info")
	{
		MainHelper::printInfo(argc-1, ++argv);
		return false;
	}
	else if (command == "ml")
	{
		if (argc < 3)
		{
			CONSOLE_OUTPUT(Information.find("HelpCommandml")->second);
			return false;
		}
		MainHelper::m_parameters["Data file name"] = argv[1];
		MainHelper::m_parameters["Algorithm"] = argv[2];


		// default values
		MainHelper::m_parameters["Separator"] = ',';
		MainHelper::m_parameters["Feature names provided"] = "1";
		MainHelper::m_parameters["Folds number"] = "5";
		MainHelper::m_parameters["Normalization"] = "0";

		int i = 3;

		while (i < argc)
		{
			if (argv[i][0] == '-')
			{
				MainHelper::m_parameters[getParam(&argv[i][1])] = argv[i+1];
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

string MainHelper::getParam(const char* arg)
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


vector<std::string> MainHelper::split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}


void RunLogic::LoadData()
{
	std::ifstream fin(m_mainHelper.m_parameters["Data file name"]);
	if (!fin)
		throw std::runtime_error("No such file!\n");
	char separator = m_mainHelper.m_parameters["Separator"][0];
	vector <string> featureNames = GetFeatureNames(fin, separator);

	string descriptionOfObject;
	vector <vector<string> > data;

	// reading data file
	while (!fin.eof())
	{
		getline(fin, descriptionOfObject);
		if (descriptionOfObject.size() == 0)
			continue;
		data.push_back(m_mainHelper.split(descriptionOfObject, separator));
	}

	m_df = new DataFrame(data);
	m_df->SetFeatureNames(featureNames);

	fin.close();
}

DataFrame RunLogic::LoadData(string fileName, char separator)
{
	std::ifstream fin(fileName);
	if (!fin)
		throw std::runtime_error("No such file!\n");

	m_mainHelper.m_parameters["Feature names provided"] = "1";
	vector <string> featureNames = GetFeatureNames(fin, separator);

	string descriptionOfObject;
	vector <vector<string> > data;

	// reading data file
	while (!fin.eof())
	{
		getline(fin, descriptionOfObject);
		if (descriptionOfObject.size() == 0)
			continue;
		data.push_back(m_mainHelper.split(descriptionOfObject, separator));
	}

	m_df = new DataFrame(data);
	m_df->SetFeatureNames(featureNames);

	fin.close();
	return *m_df;
}

vector <string> RunLogic::GetFeatureNames(std::ifstream& fin, const char separator)
{
	vector <string> featureNames;
	string featuresList;

	if (m_mainHelper.m_parameters["Feature names provided"] == "1")
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

ArgsForAlg MainHelper::ConvertToArgs(const string& params)
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
	int foldsNum = stoi(m_mainHelper.m_parameters["Folds number"]);
	if (foldsNum > m_df->GetDimention().first || foldsNum < 1)
	{
		throw std::runtime_error("Wrong number of folds\n");
	}

	ArgsForAlg args = m_mainHelper.ConvertToArgs(m_mainHelper.m_parameters["Arguments for algorithm"]);
	string res;

	switch (stoi(m_mainHelper.m_parameters["Algorithm"]))
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
		case 5:
			if (args.argsLongDouble.empty())
				args.argsLongDouble = { m_df->GetDimention().first * 0.02 };
			res = BuildAndEstimateModel<DecisionTree>(foldsNum, args);
			break;
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
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
		case 24:
			if (args.argsLongDouble.empty())
				args.argsLongDouble = {25, m_df->GetDimention().first * 0.8, 0.00001, 300000 };
			res = BuildAndEstimateModel<BaggedAlg<LinearRegression>>(foldsNum, args);
			break;
		case 25:
			if (args.argsLongDouble.empty())
				args.argsLongDouble = {25, m_df->GetDimention().first * 0.8, m_df->GetDimention().first * 0.02};
			res = BuildAndEstimateModel<BaggedAlg<DecisionTree>>(foldsNum, args);
			break;
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
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
		case 34:
			if (args.argsLongDouble.empty())
				args.argsLongDouble = {50, 5, 0.00001, 300000};
			res = BuildAndEstimateModel<BoostedAlg<LinearRegression>>(foldsNum, args);
			break;
		case 35:
			if (args.argsLongDouble.empty())
				args.argsLongDouble = {50, 5, m_df->GetDimention().first * 0.02};
			res = BuildAndEstimateModel<BoostedAlg<DecisionTree>>(foldsNum, args);
			break;
		default:
			throw std::runtime_error(Information.find("NoSuchAlg")->second);

	}

	return res;
}

}  // namespace core


// TODO: change ArgsForAlgs, LR with Normalization Prediction