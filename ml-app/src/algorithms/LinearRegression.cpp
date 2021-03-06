#include "LinearRegression.h"

#include <cstdlib>


namespace algorithms
{


LinearRegression::LinearRegression(const ArgsForAlg& args)
{
	m_learningRate = args.argsLongDouble[0];
	m_numberOfIterations = args.argsLongDouble[1];
}

void LinearRegression::Learn(const DataFrame& df)
{
	// last element - interceptor term
	m_params.resize(df.GetDimention().second + 1);

	// init with random values between -1 and 1
	for (size_t i = 0; i < m_params.size(); ++i)
		m_params[i] = (long double)(rand() % 201 - 100) / (long double)100.0;

	long double trainingSetSize = df.GetDimention().first;

	for (int i = 0; i < m_numberOfIterations; ++i)
	{
		vector <long double> currentLoss = FindCurrentLoss(df);
		vector <long double> currentDerivatives = FindDerivatives(df, currentLoss);

		DEBUG_COND_TRACE4(i % (m_numberOfIterations/50) == 0, i, " iteration...", "\tloss of 1st component = ", currentLoss[0]);

		for (size_t j = 0; j < m_params.size(); ++j)
		{
			m_params[j] -= m_learningRate / trainingSetSize * currentDerivatives[j];
		}
	}
}

vector <long double> LinearRegression::Predict(const DataFrame& df) const
{
	vector<long double> ans(df.GetDimention().first, 0);

	for (size_t i = 0; i < ans.size(); ++i)
	{
		for (size_t j = 0; j < m_params.size() - 1; ++j)
			ans[i] += df.GetData()[i].m_description[j] * m_params[j];

		ans[i] += m_params[m_params.size() - 1];
	}

	return ans;
}

string LinearRegression::GetDescriptionOfModel() const
{
	string descr = "Learning rate = ";
	descr += std::to_string(m_learningRate);
	descr += "\nNumber of iterations = ";
	descr += std::to_string(m_numberOfIterations);
	descr += "\nCoeficients = ";
	for (size_t i = 0; i < m_params.size(); ++i)
		descr += std::to_string(m_params[i]) + ' ';
	descr += '\n';
	return descr;
}

vector <long double> LinearRegression::FindCurrentLoss(const DataFrame& df)
{
	vector<long double> loss(df.GetDimention().first);

	for (size_t i = 0; i < loss.size(); ++i)
	{
		long double currentPredictedValue = 0;
		for (size_t j = 0; j < m_params.size() - 1; ++j)
			currentPredictedValue += df.GetData()[i].m_description[j] * m_params[j];

		// interceptor term
		currentPredictedValue += m_params[m_params.size() - 1];
		loss[i] = currentPredictedValue - df.GetData()[i].GetAnswer();
	}

	return loss;
}

vector<long double> LinearRegression::FindDerivatives(const DataFrame& df, vector<long double>& loss)
{
	vector<long double> derivatives(m_params.size());
	size_t trainingSetSize = df.GetDimention().first;
	size_t inteceptorTermIndex = derivatives.size() - 1;

	for (size_t i = 0; i < trainingSetSize; ++i)
	{
		for (size_t j = 0; j < inteceptorTermIndex; ++j)
		{
			derivatives[j] += loss[i] * df.GetData()[i].m_description[j];
		}

		// interceptor term
		derivatives[inteceptorTermIndex] += loss[i];
	}

	return derivatives;
}


} // algorithms
