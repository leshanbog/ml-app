#pragma once

#include "../auxiliaryObjects.h"

using namespace algorithm_helpers;


namespace algorithms
{


class Algo
{
public:
	void Fit(const DataFrame& df);

	vector <long double> Predict(const DataFrame& df) const;

	virtual void Learn(const DataFrame&) = 0;

	virtual void PredictForRange(const DataFrame& df, int start, int end, vector<long double>& outAns) const = 0;

	virtual string GetDescriptionOfModel() const = 0;

	virtual string GetAlgName() const = 0;

protected:
	vector <string> m_featureNames;
};


} // algorithms