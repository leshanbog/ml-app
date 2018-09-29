#pragma once

#include <limits>
#include <map>
#include "Obj.h"

namespace algorithm_helpers
{


struct StatisticsForNormalization
{
	long double mean = 0;
	long double diffMaxMin = 0;
	long double min = std::numeric_limits<long double>::max();
};


typedef uint32_t featureId;
typedef std::map<string, long double> featureDictionary;

class DataFrame
{
public:
	DataFrame() {};

	DataFrame(const vector<vector<string> > & data);
	DataFrame(vector<vector<string> >&& data);
	
	DataFrame(const vector<Obj>& data);
	DataFrame(vector<Obj>&& data);	

	vector <long double> GetAnswers() const;
	vector<Obj>::const_iterator GetData() const;
	pair <int, int> GetDimention() const;
	vector <string> GetFeatureNames() const;

	void Add(const Obj& obj);
	void Add(Obj&& obj);

	void SetFeatureNames(const vector <string> &);

	void ChangeAnswers(const vector <long double> &);

	void DoNormalization();
	void PerformNormalization(const vector<StatisticsForNormalization>& stats);

public:
	vector<StatisticsForNormalization> m_stats;
	bool m_wasNormalized = false;
private:
	vector<featureId> FindStringFeatures(const vector<vector<string> >& data);
	std::map<featureId, featureDictionary> MakeMappingForTransformation(const vector<vector<string> >& data, const vector<featureId>& stringFeatures);
private:
	vector <Obj> m_data;
	vector <string> m_featureNames;
};


} // algorithm_helpers
