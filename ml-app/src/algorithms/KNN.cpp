#include <map>

#include "KNN.h"

using std::multimap;



namespace algorithms
{


void KNN::Fit(const DataFrame &df)
{
	m_featureNames = df.GetFeatureNames();
	m_data = df;
}


vector<long double> KNN::Predict(const DataFrame &df) const
{
	vector <long double> ans(df.GetDimention().first);
	vector <long double> currentKNearest;

	for (uint16_t i = 0; i < ans.size(); ++i)
	{
		 currentKNearest = FindKNearest(df.GetData()[i]);
		 ans[i] = UsefulMath::mean(currentKNearest);
	}

	return ans;
}


string KNN::GetDescriptionOfModel() const
{
	return "K = " + std::to_string(m_k) + "\n";
}


vector<long double> KNN::FindKNearest(const Obj x) const
{
	multimap <long double, int> kNearest; // (measure of nearness, index of obj)

	auto objDescr = x.description;

	for (int i = 0; i < m_k; ++i)
	{
		kNearest.insert({ UsefulMath::RMSE(objDescr, m_data.GetData()[i].description), i });
	}

	int n = m_data.GetDimention().first;
	long double currentRmse = 0;
	auto lastNearest = kNearest.end();
	lastNearest--;

	for (int i = m_k; i < n; ++i)
	{
		currentRmse = UsefulMath::RMSE(objDescr, m_data.GetData()[i].description);

		if (lastNearest->first > currentRmse)
		{
			kNearest.erase(lastNearest);
			kNearest.insert({ currentRmse, i });

			lastNearest = kNearest.end();
			lastNearest--;
		}
	}

	vector <long double> kNearestObjs;
	for (auto el : kNearest)
		kNearestObjs.push_back(m_data.GetData()[el.second].GetAnswer());

	return kNearestObjs;
}



} // algorithms