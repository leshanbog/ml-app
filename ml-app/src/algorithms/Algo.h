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
		if (df.m_wasNormalized)
			m_statsForNormalization = df.m_stats;

		Learn(df);
	}

	vector <long double> PredictResult(DataFrame& df) const
	{
		if (!m_statsForNormalization.empty())
		{
			DEBUG_LOG("Algo.h:\tStarting normalization...");
			df.PerformNormalization(m_statsForNormalization);
		}
		return Predict(df);
	}

	virtual string GetDescriptionOfModel() const = 0;

	virtual string GetAlgName() const = 0;

protected:
	virtual void Learn(const DataFrame& df) = 0;
	virtual vector <long double> Predict(const DataFrame& df) const = 0;

	vector <string> m_featureNames;
	vector<StatisticsForNormalization> m_statsForNormalization;
};


} // algorithms