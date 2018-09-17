#pragma once

#include "Algo.h"


namespace algorithms
{


class ConstPrediction : public Algo
{
public:
	ConstPrediction() {}
	ConstPrediction(const ArgsForAlg& args){}

	inline string GetAlgName() const override
	{
		return "Const Prediction";
	}

	void Learn(const DataFrame&) override;

	vector <long double> Predict(const DataFrame&) const override;

	string GetDescriptionOfModel() const override;
private:
	long double m_param = 0;
};


} // algorithms