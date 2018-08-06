#include "ConstPrediction.h"


namespace algorithms
{


void ConstPrediction::Fit(const DataFrame& df)
{
	m_featureNames = df.GetFeatureNames();
	auto ans = FindBestParams(df);
	m_param = ans[0];
}


vector<long double> ConstPrediction::FindBestParams(const DataFrame& df) const
{
	return { UsefulMath::mean(df.GetAnswers()) };
}


vector<long double> ConstPrediction::Predict(const DataFrame& df) const
{
	vector<long double> ans(df.GetDimention().first, m_param);
	return ans;
}


string ConstPrediction::GetDescriptionOfModel() const
{
	return "\nBest value = " + std::to_string(m_param);
}


} // algorithms