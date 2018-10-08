#include <iostream>
#include <vector>
#include "../ml-app/src/LearningMachineLearning.h"


using namespace algorithms;

int main()
{
	// data
	core::RunLogic rl;
	auto data = rl.LoadData("../datasets/insurance.csv", ',');

	// arguments
	ArgsForAlg args;
	args.argsLongDouble = { 60, 420, 20 };
	const int foldsNumber = 10;


	// result
	auto res = rl.BuildAndEstimateModel<BaggedAlg<DecisionTree>>(foldsNumber, args); 	
	std::cout << res << "\n\n";
	

	// experiments


	std::cout << "\n\n";
	return 0;
}