#include "Algo.h"

#include <thread>



namespace algorithms
{


constexpr int THREADS_NUMBER = 5;

void Algo::Fit(const DataFrame& df)
{
	m_featureNames = df.GetFeatureNames();
	Learn(df);
}

vector <long double> Algo::Predict(const DataFrame& df) const
{
	std::thread* threads[THREADS_NUMBER];
	vector<long double> ans(df.GetDimention().first);

	int d = df.GetDimention().first / THREADS_NUMBER;

	for (int i = 0; i < THREADS_NUMBER; ++i)
	{
		threads[i] = new std::thread(PredictForRange, df, i, i + d, ans);
	}

}


} // algorithms