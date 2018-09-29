#include "DataFrame.h"
#include "../auxiliaryObjects.h"
#include <algorithm>



namespace algorithm_helpers
{


DataFrame::DataFrame(const vector<vector<string> >& data)
{
	DEBUG_LOG("DataFrame.cpp:\tCopy construcor from vec<vec<str>>");

	if (data.empty())
		return;

	vector<featureId> stringFeatures = FindStringFeatures(data);
	std::map<featureId, featureDictionary> m = MakeMappingForTransformation(data, stringFeatures);

	// creating m_data
	for (uint32_t i = 0; i < data.size(); ++i)
	{	
		vector <long double> curObject(data[i].size() - 1);
		long double ans = 0;
		featureId j = 0;
		for (; j < data[i].size() - 1; ++j)
		{
			if (m.find(j) != m.end())
			{
				curObject[j] = m[j][data[i][j]];
			}
			else
			{
				curObject[j] = stold(data[i][j]);
			}
		}
		if (m.find(j) != m.end())
		{
			ans = m[j][data[i][j]];
		}
		else
		{
			ans = stold(data[i][j]);
		}
		// TODO: optimize all
		m_data.push_back(Obj(curObject, ans));
	}
}


DataFrame::DataFrame(vector<vector<string> >&& data)
{
	DEBUG_LOG("DataFrame.cpp:\tMove construcor from vec<vec<str>>");
	if (data.empty())
		return;

	vector<featureId> stringFeatures = FindStringFeatures(data);
	std::map<featureId, featureDictionary> m = MakeMappingForTransformation(data, stringFeatures);

	// creating m_data
	for (uint32_t i = 0; i < data.size(); ++i)
	{	
		vector <long double> curObject(data[i].size() - 1);
		long double ans = 0;
		featureId j = 0;
		for (; j < data[i].size() - 1; ++j)
		{
			if (m.find(j) != m.end())
			{
				curObject[j] = m[j][data[i][j]];
			}
			else
			{
				curObject[j] = stold(data[i][j]);
			}
		}
		if (m.find(j) != m.end())
		{
			ans = m[j][data[i][j]];
		}
		else
		{
			ans = stold(data[i][j]);
		}
		// TODO: optimize all
		m_data.push_back(Obj(curObject, ans));
	}
}

DataFrame::DataFrame(const vector<Obj>& data)
{
	DEBUG_LOG("DataFrame.cpp:\tCopy constructor from vec<Obj>");
	m_data = data;

	DEBUG_ASSERT_TRUE(data.size() != 0);
}

DataFrame::DataFrame(vector<Obj>&& data)
{
	DEBUG_LOG("DataFrame.cpp:\tMove constructor from vec<Obj>");
	m_data = std::move(data);

	DEBUG_ASSERT_TRUE(data.empty());
}

vector<Obj>::const_iterator DataFrame::GetData() const
{
	return m_data.begin();
}

void DataFrame::Add(const Obj& obj)
{
	DEBUG_LOG("DataFrame.cpp:\tCopy Add");
	m_data.push_back(obj);
}

void DataFrame::Add(Obj&& obj)
{
	DEBUG_LOG("DataFrame.cpp:\tMove add");
	m_data.push_back(obj);

	DEBUG_ASSERT_TRUE(obj.m_description.empty());
}


pair<int, int> DataFrame::GetDimention() const
{
	if (m_data.empty())
		return std::make_pair(0,0);	
	else
		return std::make_pair(m_data.size(), m_data[0].m_description.size());
}


vector<string> DataFrame::GetFeatureNames() const
{
	return m_featureNames;
}

void DataFrame::SetFeatureNames(const vector<string>& fn)
{
	m_featureNames = fn;
}

void DataFrame::ChangeAnswers(const vector<long double>& v)
{
	for (size_t i = 0; i < m_data.size(); ++i)
	{
		m_data[i].SetAnswer(v[i]);
	}
}


vector<long double> DataFrame::GetAnswers() const
{
	vector <long double> ans(this->m_data.size());
	for (size_t i = 0; i < ans.size(); ++i)
		ans[i] = this->m_data[i].GetAnswer();
	return ans;
}

void DataFrame::DoNormalization()
{
	DEBUG_LOG("DataFrame.cpp:\tFinding stats for normalization");
	m_stats.resize(m_data[0].m_description.size());

	for (size_t i = 0; i < m_data.size(); ++i)
	{
    	for (size_t j = 0; j < m_data[i].m_description.size(); ++j)
    	{
    		m_stats[j].mean += (m_data[i].m_description[j] - m_stats[j].mean) / (i+1);

    		if (m_data[i].m_description[j] > m_stats[j].diffMaxMin)
    			m_stats[j].diffMaxMin = m_data[i].m_description[j];

    		if (m_data[i].m_description[j] < m_stats[j].min)
    			m_stats[j].min = m_data[i].m_description[j];
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
	DEBUG_LOG("DataFrame.cpp:\tPerforming normalization");
	for (size_t i = 0; i < m_data.size(); ++i)
	{
		for (size_t j = 0; j < m_data[i].m_description.size(); ++j)
		{
			m_data[i].m_description[j] = (m_data[i].m_description[j] - stats[j].mean) / stats[j].diffMaxMin;
		}
	}
}

vector<featureId> DataFrame::FindStringFeatures(const vector<vector<string> >& data)
{
	vector<featureId> stringFeatures;
	for (featureId i = 0; i < data[0].size(); ++i)
	{	
		try
		{	
			for (uint32_t j = 0; j < std::min(5, (int)data.size()); ++j)
				stold(data[j][i]);
		}
		catch(const std::invalid_argument& e)
		{
			stringFeatures.push_back(i);
		}
	}
	return stringFeatures;
}

std::map<featureId, featureDictionary> DataFrame::MakeMappingForTransformation(const vector<vector<string> >& data, const vector<featureId>& stringFeatures)
{
	std::map<featureId, featureDictionary> m;
	for (featureId j = 0; j < stringFeatures.size(); ++j)
	{
		long double val = 1;
		for (uint32_t i = 0; i < data.size(); ++i)
		{
			// TODO: use sets here
			featureId idOfStringFeature = stringFeatures[j];
			if (m[idOfStringFeature].find(data[i][j]) == m[idOfStringFeature].end())
			{
				m[idOfStringFeature][data[i][idOfStringFeature]] = val;
				++val;
			}
		}
	}
	return m;
}


} // algorithm_helpers
