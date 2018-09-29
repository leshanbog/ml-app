#include "Obj.h"
#include "../auxiliaryObjects.h"

namespace algorithm_helpers
{


Obj::Obj(const vector<long double>& descr, const long double& ans)
{
	DEBUG_LOG("Obj.cpp\tcopy construcor");
	/*
	int i = 0;
	while (i < descr.size())
	{
		string t = "";
		while (i < descr.size() && descr[i] != separator)
		{
			if ( (descr[i] >= '0' && descr[i] <= '9') || descr[i] == '.' || descr[i] == '-' || descr[i] == 'e')
				t += descr[i];
			i++;
		}

		description.push_back(stold(t));
		i++;
	}
	*/
	m_description = descr;
	m_answer = ans;

	
}

Obj::Obj() {};

long double Obj::GetAnswer() const
{
	return m_answer;
}

void Obj::SetAnswer(const long double inValue)
{
	m_answer = inValue;
}

bool Obj::operator==(Obj& a)
{
	if (a.GetAnswer() != this->GetAnswer())
		return false;

	if (a.m_description.size() != this->m_description.size()) 
		return false;

	for (int i = 0; i < a.m_description.size(); i++)
	{
		if (a.m_description[i] != this->m_description[i])
			return false;
	}

	return true;
}


} // algorithm_helpers
