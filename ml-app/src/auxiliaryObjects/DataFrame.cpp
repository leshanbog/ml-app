#include "DataFrame.h"


// TODO: delete
#include <iostream>

namespace algorithm_helpers
{

DataFrame::DataFrame(const vector<Obj>& v)
{
	data = v;
	if (v.size() != 0)
		dimention = std::make_pair(v.size(), v[0].description.size());
	else
		dimention = std::make_pair( 0,0 );
}


DataFrame::DataFrame(vector<Obj>&& v)
{
	data = move(v);
	if (data.size() != 0)dimention = std::make_pair( data.size(),data[0].description.size() );
	else dimention = std::make_pair( 0,0 );
}

vector<Obj>::const_iterator DataFrame::GetData() const
{
	return data.begin();
}

void DataFrame::Add(const Obj& obj)
{
	std::cout << "DEBUG\tWrong add\n";
	data.push_back(obj);
	dimention.first++;
	dimention.second = obj.description.size();
}

void DataFrame::Add(Obj&& obj)
{
	std::cout << "DEBUG\tMove-add\n";
	data.emplace_back(obj);
	dimention.first++;
	dimention.second = obj.description.size();
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
	m_stats.resize(dimention.second);

	for (size_t i = 0; i < dimention.first; ++i)
	{
    	for (size_t j = 0; j < dimention.second; ++j)
    	{
    		m_stats[j].mean += (data[i].description[j] - m_stats[j].mean) / (i+1);

    		if (data[i].description[j] > m_stats[j].diffMaxMin)
    			m_stats[j].diffMaxMin = data[i].description[j];

    		if (data[i].description[j] < m_stats[j].min)
    			m_stats[j].min = data[i].description[j];
    	}
	}

	for (size_t i = 0; i < m_stats.size(); ++i)
	{
		m_stats[i].diffMaxMin -= m_stats[i].min;
	}

	PerformNormalization(m_stats);
	m_wasNormalized = true;
}

void DataFrame::PerformNormalization(const vector<StatisticsForNormalization>& stats)
{
	for (size_t i = 0; i < dimention.first; ++i)
	{
		for (size_t j = 0; j < dimention.second; ++j)
		{
			data[i].description[j] = (data[i].description[j] - stats[j].mean) / stats[j].diffMaxMin;
		}
	}
}

} // algorithm_helpers
