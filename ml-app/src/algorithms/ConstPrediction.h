#pragma once

#include "Algo.h"


namespace algorithms
{


class ConstPrediction : public Algo
{
public:
	ConstPrediction(const ArgsForAlg& args){}

	inline string GetAlgName() const override
	{
		return "Const Prediction";
	}

	void Fit(const DataFrame&) override;

	vector <long double> FindBestParams(const DataFrame&) const;

	vector <long double> Predict(const DataFrame&) const override;

	string GetDescriptionOfModel() const override;
private:
	long double m_param = 0;
};


} // algorithms