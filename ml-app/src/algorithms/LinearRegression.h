#pragma once

#include "Algo.h""


namespace algorithms
{


class LinearRegression : public Algo
{
public:
	LinearRegression(const ArgsForAlg& args);

	void Learn(const DataFrame& df) override;

	vector<long double> Predict(const DataFrame&) const override;

	string GetDescriptionOfModel() const override;

	inline string GetAlgName() const override
	{
		return "Linear regression";
	}

private:
	vector<long double> FindCurrentLoss(const DataFrame& df);
	
	vector<long double> FindDerivatives(const DataFrame& df, vector<long double>& loss);

private:
	vector<long double> m_params;
	long double m_learningRate;
};


} // algorithms