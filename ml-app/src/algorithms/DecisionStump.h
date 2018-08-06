#pragma once

#include "Algo.h"


namespace algorithms
{


struct DecisionStumpParams
{
	long double threshold;
	long double valueAbove;
	long double valueBelow;
};


struct PassingParameters
{
	int mainFeature;
	long double rmse;
	DecisionStumpParams dsParams;
};


class DecisionStump : public Algo
{
public:
	DecisionStump(const ArgsForAlg& args) {}

	inline string GetAlgName() const override
	{
		return "Decision Stump";
	}

	void Learn(const DataFrame& df) override;

	// make predictions with params p and main feature j
	vector <long double> Predict(const DataFrame& df) const override;

	string GetDescriptionOfModel() const override;
private:
	DecisionStumpParams m_params = { 0.0 ,0.0 ,0.0 };
	int m_mainFeature = 0;

	// find params for j-th feature in DataFrame
	PassingParameters  FindParams(const DataFrame& df, int) const;
};


} // algorithms