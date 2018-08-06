#include "DataFrame.h"

#include <limits>


namespace algorithm_helpers
{

DataFrame::DataFrame(const vector<Obj>& v)
{
	data = v;
	if (v.size() != 0 )dimention = {v.size(),v[0].description.size()};
	else dimention = { 0,0 };
}


DataFrame::DataFrame(vector<Obj>&& v)
{
	data = move(v);
	if (data.size() != 0)dimention = { data.size(),data[0].description.size() };
	else dimention = { 0,0 };
}

vector<Obj>::const_iterator DataFrame::GetData() const
{
	return data.begin();
}


pair<int, int> DataFrame::GetDimention() const
{
	return dimention;
}


vector<string> DataFrame::GetFeatureNames() const
{
	return featureNames;
}

void DataFrame::SetFeatureNames(const vector<string>& fn)
{
	featureNames = fn;
}

void DataFrame::ChangeAnswers(const vector<long double>& v)
{
	for (size_t i = 0; i < dimention.first; ++i)
	{
		data[i].SetAnswer(v[i]);
	}
}


vector<long double> DataFrame::GetAnswers() const
{
	vector <long double> ans(this->dimention.first);
	for (size_t i = 0; i < ans.size(); ++i)
		ans[i] = this->data[i].GetAnswer();
	return ans;
}

void DataFrame::DoNormalization()
{
	struct Statistics
	{
		long double mean = 0;
		long double max = 0;
		long double min = std::numeric_limits<long double>::max();
	};

	vector<Statistics> v(dimention.second);

	for (size_t i = 0; i < dimention.first; ++i)
	{
    	for (size_t j = 0; j < dimention.second; ++j)
    	{
    		v[j].mean += (data[i].description[j] - v[j].mean) / (i+1);

    		if (data[i].description[j] > v[j].max)
    			v[j].max = data[i].description[j];

    		if (data[i].description[j] < v[j].min)
    			v[j].min = data[i].description[j];
    	}
	}

	for (size_t i = 0; i < v.size(); ++i)
	{
		v[i].max -= v[i].min;
	}

	for (size_t i = 0; i < dimention.first; ++i)
	{
		for (size_t j = 0; j < dimention.second; ++j)
		{
			data[i].description[j] = (data[i].description[j] - v[j].mean) / v[j].max;
		}
	}
}

} // algorithm_helpers