#pragma once

#include "Algo.h"



namespace algorithms
{


class KNN : public Algo
{
public:
	inline KNN(const ArgsForAlg& args)
	{
		m_k = args.argsLongDouble[0];
	}

	inline string GetAlgName() const override
	{
		return "KNN";
	}

	string GetDescriptionOfModel() const override;

private:
	void Learn(const DataFrame&) override;
	vector <long double> Predict(const DataFrame&) const override;

	int m_k;
	DataFrame m_data;

	vector <long double> FindKNearest(const Obj) const;
};



} // algorithms