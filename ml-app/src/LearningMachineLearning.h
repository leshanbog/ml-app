#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <ctime>

#include "algorithms.h"
#include "auxiliaryObjects.h"
#include "CrossValidation.h"

using std::map;


namespace core
{


using namespace algorithm_helpers;
using namespace algorithms;

struct MainHelper
{
	bool ParseParameters(int argc, char *argv[]);

	void printHelp(int argc, char *argv[]);

	inline void printAlgs();

	void printInfo(int argc, char *argv[]);

	ArgsForAlg ConvertToArgs(const string& params);

	string getParam(const char* arg);
	map <string,string> m_parameters;
};



class RunLogic
{
public:
	string Run(int argc, char *argv[]);

	// for usage in lib
	void LoadData(string fileName, char separator = ',');

	template <class TLearner> string BuildAndEstimateModel(const int foldsNum, const ArgsForAlg &args)
	{

		TLearner alg(args);

		CrossValidation<TLearner> CV;
		long double rmse = 0;

		auto start = std::chrono::high_resolution_clock::now();
		alg.Fit(*m_df);
		auto end = std::chrono::high_resolution_clock::now();
		long double timeForBuild = (end - start).count(), timeForCV = 0;

		if (foldsNum == 1)
		{
			rmse = UsefulMath::RMSE(alg.Predict(*m_df), m_df->GetAnswers());
		}
		else
		{
			start = std::chrono::high_resolution_clock::now();
			rmse = CV.Score(*m_df, foldsNum, args);
			end = std::chrono::high_resolution_clock::now();
			timeForCV = (end - start).count();
		}

		string descr = "\n+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=\n\n\n";
		descr += alg.GetAlgName() + "\n" + mainHelper.m_parameters["Data file name"] + "\n\n";
		descr += alg.GetDescriptionOfModel() + "\n\nTime taken to build model (sec)  " + std::to_string(timeForBuild / 1'000'000'000.0);
		if (foldsNum > 1)
		{
			descr += "\nFolds number " + std::to_string(foldsNum);
			descr += "\nTime taken for CV (sec) " + std::to_string(timeForCV / 1'000'000'000.0);
		}
		descr += "\n\nRMSE  " + std::to_string(rmse);
		descr += "\n\n\n+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=\n\n";

		return descr;
	}

private:
	void LoadData();

	vector <string> GetFeatureNames(std::ifstream& fin, const char separator);

	string StartAlg();

private:
	DataFrame* m_df;
	MainHelper mainHelper;
};



} // namespace core
