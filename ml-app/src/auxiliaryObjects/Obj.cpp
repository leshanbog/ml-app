#include "Obj.h"


namespace algorithm_helpers
{


Obj::Obj(const string& descr, const char separator)
{
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
	answer = description[description.size() - 1];
	description.erase(description.end() - 1);
}

Obj::Obj() {};

long double Obj::GetAnswer() const
{
	return answer;
}

void Obj::SetAnswer(const long double inValue)
{
	answer = inValue;
}

bool Obj::operator==(Obj& a)
{
	if (a.GetAnswer() != this->GetAnswer())
		return false;

	if (a.description.size() != this->description.size()) 
		return false;

	for (int i = 0; i < a.description.size(); i++)
	{
		if (a.description[i] != this->description[i])
			return false;
	}

	return true;
}


} // algorithm_helpers
