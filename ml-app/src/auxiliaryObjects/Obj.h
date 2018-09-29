#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;
using std::pair;


namespace algorithm_helpers
{

class Obj
{
public:
	Obj(const vector<long double>& descr, const long double& ans);
	Obj();

	long double GetAnswer() const;
	void SetAnswer(const long double );

	bool operator==(Obj&);

	vector <long double> m_description;
private:
	long double m_answer;
};


} // algorithm_helpers