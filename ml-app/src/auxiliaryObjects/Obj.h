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
	Obj(const string& descr, const char separator);
	Obj();

	long double GetAnswer() const;
	void SetAnswer(const long double );

	bool operator==(Obj&);

	vector <long double> description;
private:
	long double answer;
};


} // algorithm_helpers