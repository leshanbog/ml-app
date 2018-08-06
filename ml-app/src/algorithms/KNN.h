#pragma once

#include "Algo.h"



namespace algorithms
{


class KNN : public Algo
{
public:
	inline KNN(const ArgsForAlg& args)
	{
		m_k = args.argsInt[0];
	}

	inline string GetAlgName() const override
	{
		return "KNN";
	}

	void Learn(const DataFrame&) override;

	vector <long double> Predict(const DataFrame&) const override;

	string GetDescriptionOfModel() const override;

private:
	int m_k;
	DataFrame m_data;

	vector <long double> FindKNearest(const Obj) const;
};



} // algorithms