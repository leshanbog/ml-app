#pragma once

#include "../auxiliaryObjects.h"

using namespace algorithm_helpers;


namespace algorithms
{

class Algo
{
public:
	void Fit(const DataFrame& df)
	{
		m_featureNames = df.GetFeatureNames();
		Learn(df);
	}

	virtual void Learn(const DataFrame&) = 0;

	virtual vector <long double> Predict(const DataFrame&) const = 0;

	virtual string GetDescriptionOfModel() const = 0;

	virtual string GetAlgName() const = 0;

protected:
	vector <string> m_featureNames;
};


} // algorithms