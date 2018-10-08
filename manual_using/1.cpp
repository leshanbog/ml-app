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
	//args.argsLongDouble = { 60, 420, 25 };
	args.argsLongDouble = { 25 };
	const int foldsNumber = 10;


	// result
	auto res = rl.BuildAndEstimateModel<DecisionTree>(foldsNumber, args); 	
	std::cout << res << "\n\n";
	

	// experiments
	DecisionTree dt(args);
	dt.Fit(data);
	//age, sex, bmi, children, smoker, region
	Obj person1({  60,  1, 41, 2, 1, 1   }, 0);
	Obj person2({  20,  2, 41, 0, 2, 4   }, 0);
	std::vector<Obj> d = {person1,person2};
	DataFrame pred(d);
	auto ans = dt.PredictResult(pred);

	for (auto el : ans)
		std::cout << el << ' ';


	std::cout << "\n\n";
	return 0;
}