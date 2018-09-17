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
	uint32_t mainFeature;
	long double rmse;
	DecisionStumpParams dsParams;
};


class DecisionStump : public Algo
{
public:
	DecisionStump() {}
	DecisionStump(const ArgsForAlg& args) {}

	inline string GetAlgName() const override
	{
		return "Decision Stump";
	}

	void Learn(const DataFrame& df) override;

	vector<long double> Predict(const DataFrame& df) const override;

	string GetDescriptionOfModel() const override;

	uint32_t GetMainFeature()
	{
		return m_mainFeature;
	}

	long double GetThreshold()
	{
			return m_params.threshold;
	}
private:
	DecisionStumpParams m_params = { 0.0 ,0.0 ,0.0 };
	uint32_t m_mainFeature = 0;

	// find params for j-th feature in DataFrame
	PassingParameters  FindParams(const DataFrame& df, int) const;
};


} // algorithms