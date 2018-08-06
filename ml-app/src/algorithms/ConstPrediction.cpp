#include "ConstPrediction.h"


namespace algorithms
{


void ConstPrediction::Learn(const DataFrame& df)
{
	m_param = UsefulMath::mean(df.GetAnswers());
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