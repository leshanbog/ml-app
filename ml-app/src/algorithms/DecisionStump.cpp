#include "DecisionStump.h"

#include <algorithm>
#include <set>
#include <random>
#include <chrono>
#include <limits>


namespace algorithms
{


void DecisionStump::Learn(const DataFrame& df)
{
	PassingParameters bestParams;
	bestParams.rmse = std::numeric_limits<long double>::max();
	for (int i = 0; i < df.GetDimention().second; i++)
	{
		PassingParameters tempParams = FindParams(df, i);
		if (bestParams.rmse > tempParams.rmse)
		{
			bestParams = tempParams;
		}
	}

	m_params = bestParams.dsParams;
	m_mainFeature = bestParams.mainFeature;
}

PassingParameters DecisionStump::FindParams(const DataFrame& df, int j) const
{
	vector <pair <long double, long double> > data(df.GetDimention().first);
	long double
		errorAbove = 0, errorBelow = 0,
		currentThreshold = 0,
		errorGeneral = 0, errorGeneralBest = std::numeric_limits<long double>::max(),
		sumTargetAbove = 0, sumTargetBelow = 0,
		sumSquareTargetAbove = 0, sumSquareTargetBelow = 0;
	PassingParameters bestParams;

	for (size_t i = 0; i < data.size(); i++)
	{
		data[i] = { df.GetData()[i].description[j], df.GetData()[i].GetAnswer() };
		sumTargetAbove += df.GetData()[i].GetAnswer();
		sumSquareTargetAbove += df.GetData()[i].GetAnswer() * df.GetData()[i].GetAnswer();
	}

	long double meanTargetAbove = sumTargetAbove / data.size();

	for (size_t i = 0; i < data.size(); i++)
		errorAbove += (data[i].second - meanTargetAbove) * (data[i].second - meanTargetAbove);

	sort(data.begin(), data.end(), [](auto el1, auto el2) { return el1.first < el2.first;  });

	for (size_t i = 0; i < data.size() - 1; i++)
	{
		currentThreshold = (data[i].first + data[i + 1].first) / 2;

		sumTargetBelow += data[i].second;
		sumSquareTargetBelow += data[i].second*data[i].second;
		errorBelow = sumSquareTargetBelow - sumTargetBelow * sumTargetBelow / (i + 1);

		sumTargetAbove -= data[i].second;
		sumSquareTargetAbove -= data[i].second*data[i].second;
		errorAbove = sumSquareTargetAbove - sumTargetAbove * sumTargetAbove / (data.size() - i - 1);

		if (data[i].first == data[i + 1].first)
			continue;

		errorGeneral = pow((errorBelow + errorAbove) / data.size(), 0.5);
		if (errorGeneral < errorGeneralBest)
		{
			bestParams.dsParams = DecisionStumpParams({currentThreshold, sumTargetAbove / (data.size() - i - 1), sumTargetBelow / (i + 1) });
			errorGeneralBest = errorGeneral;
		}
	}

	bestParams.rmse = errorGeneralBest;
	bestParams.mainFeature = j;
	return bestParams;
}

vector<long double> DecisionStump::Predict(const DataFrame& df) const
{
	vector<long double> ans(df.GetDimention().first);

	for (size_t i = 0; i < ans.size(); ++i)
	{
		if (df.GetData()[i].description[m_mainFeature] < m_params.threshold)
		{
			ans[i] = m_params.valueBelow;
		}
		else
		{
			ans[i] = m_params.valueAbove;
		}
	}
	return ans;
}

string DecisionStump::GetDescriptionOfModel() const
{
	string ans = "";
	ans += "Main feature  " + m_featureNames[m_mainFeature];
	ans += "\nThreshold  " + std::to_string(m_params.threshold);
	ans += "\nValue below  " + std::to_string(m_params.valueBelow);
	ans += "\nValue above   " + std::to_string(m_params.valueAbove);
	return ans;
}


} // algorithms