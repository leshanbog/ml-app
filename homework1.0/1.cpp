#include <iostream>
#include "../ml-app/src/LearningMachineLearning.h"


int main()
{
	core::RunLogic rl;
	rl.LoadData("../datasets/cpu.small.csv", ',');
	ArgsForAlg args;
	args.argsLongDouble = {};
	auto res = rl.BuildAndEstimateModel<algorithms::DecisionStump>(5, args); 
	std::cout << res;
	return 0;
}
