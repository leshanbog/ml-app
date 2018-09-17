#pragma once

#include "Obj.h"

namespace algorithm_helpers
{


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
private:
	vector <Obj> data;
	pair <size_t, size_t> dimention;
	vector <string> featureNames;
};


} // algorithm_helpers