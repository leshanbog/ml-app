#pragma once

#include "../auxiliaryObjects.h"

using namespace algorithm_helpers;



namespace algorithms
{

class Algo
{
public:
	virtual void Fit(const DataFrame&) = 0;

	virtual vector <long double> Predict(const DataFrame&) const = 0;

	virtual string GetDescriptionOfModel() const = 0;

	virtual string GetAlgName() const = 0;

protected:
	vector <string> m_featureNames;
};


} // algorithms