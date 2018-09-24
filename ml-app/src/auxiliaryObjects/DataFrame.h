#pragma once

#include <limits>
#include "Obj.h"

namespace algorithm_helpers
{


struct StatisticsForNormalization
{
	long double mean = 0;
	long double diffMaxMin = 0;
	long double min = std::numeric_limits<long double>::max();
};

class DataFrame
{
public:
	DataFrame(const vector<Obj>&);
	DataFrame(vector<Obj>&&);
	vector <long double> GetAnswers() const;
	DataFrame() {};

	vector<Obj>::const_iterator GetData() const;
	void Add(const Obj& obj);
	void Add(Obj&& obj);
	pair <int, int> GetDimention() const;
	vector <string> GetFeatureNames() const;
	void SetFeatureNames(const vector <string> &);
	void ChangeAnswers(const vector <long double> &);
	void DoNormalization();
	void PerformNormalization(const vector<StatisticsForNormalization>& stats);

	vector<StatisticsForNormalization> m_stats;
	bool m_wasNormalized = false;
private:
	vector <Obj> data;
	pair <size_t, size_t> dimention;
	vector <string> featureNames;
};


} // algorithm_helpers
