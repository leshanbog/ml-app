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


class RunLogic
{
public:
	string Run(int argc, char *argv[]);

private:
	bool ParseParameters(int argc, char *argv[]);

	void printHelp(int argc, char *argv[]) const;

	inline void printAlgs() const;

	void printInfo(int argc, char *argv[]) const;

	void LoadData();

	vector <string> GetFeatureNames(std::ifstream& fin, const char separator);

	string StartAlg();

	ArgsForAlg ConvertToArgs(const string& params) const;

	string getParam(const char* arg) const;

	template <class TLearner> string BuildAndEstimateModel(const int foldsNum, const ArgsForAlg &args)
	{

		TLearner alg(args);

		CrossValidation<TLearner> CV;
		long double rmse = 0;

		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		alg.Fit(*m_df);
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		long double timeForBuild = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(), timeForCV = 0;

		if (foldsNum == 1)
		{
			rmse = UsefulMath::RMSE(alg.Predict(*m_df), m_df->GetAnswers());
		}
		else
		{
			start = std::chrono::high_resolution_clock::now();
			rmse = CV.Score(*m_df, foldsNum, args);
			end = std::chrono::high_resolution_clock::now();
			timeForCV = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		}

		string descr = "\n+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=\n\n\n";
		descr += alg.GetAlgName() + "\n" + m_parameters["Data file name"] + "\n\n";
		descr += alg.GetDescriptionOfModel() + "\n\nTime taken to build model (sec)  " + std::to_string(timeForBuild / CLOCKS_PER_SEC);
		if (timeForCV != 0)
		{
			descr += "\nFolds number " + std::to_string(foldsNum);
			descr += "\nTime taken for CV (sec) " + std::to_string(timeForCV / CLOCKS_PER_SEC);
		}
		descr += "\n\nRMSE  " + std::to_string(rmse);
		descr += "\n\n\n+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=\n\n";

		return descr;
	}


private:
	DataFrame* m_df;
	map <string,string> m_parameters;
};


} // namespace core
