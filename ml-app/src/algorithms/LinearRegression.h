#pragma once

#include "Algo.h"


namespace algorithms
{


class LinearRegression : public Algo
{
public:
	LinearRegression(const ArgsForAlg& args);

	string GetDescriptionOfModel() const override;

	inline string GetAlgName() const override
	{
		return "Linear regression";
	}

private:
	void Learn(const DataFrame& df) override;
	vector<long double> Predict(const DataFrame&) const override;

	vector<long double> FindCurrentLoss(const DataFrame& df);	
	vector<long double> FindDerivatives(const DataFrame& df, vector<long double>& loss);

private:
	vector<long double> m_params;
	long double m_learningRate;
	uint32_t m_numberOfIterations;
};


} // algorithms