#pragma once

#include "Algo.h"


namespace algorithms
{


template <class TLearner>
class BoostedAlg : public Algo
{
public:
	inline BoostedAlg(const ArgsForAlg& args)
	{
		m_numberOfAlgs = args.argsLongDouble[0];

		for (int i = 1; i < args.argsLongDouble.size(); ++i)
			m_argsForTLearner.argsLongDouble.push_back(args.argsLongDouble[i]);

		m_algsPool.reserve(m_numberOfAlgs);
	}

	inline string GetAlgName() const override
	{
		string ans = "Boosted " + m_algsPool[0].GetAlgName();
		if (m_argsForTLearner.argsLongDouble.size() != 0)
		{
			ans += "(";
			for (const auto& el : m_argsForTLearner.argsLongDouble)
			{
				ans += std::to_string(el) + ", ";
			}
			ans[ans.size()-2] = ')';
		}
		return ans;
	}

	string GetDescriptionOfModel() const override;

private:
	void Learn(const DataFrame&) override;
	vector <long double> Predict(const DataFrame&) const override;

	size_t m_numberOfAlgs;
	vector <TLearner> m_algsPool;
	ArgsForAlg m_argsForTLearner;
};

template<class TLearner> void BoostedAlg<TLearner>::Learn(const DataFrame& df)
{
	DataFrame train = df;
	for (uint16_t i = 0; i < m_numberOfAlgs; ++i)
	{
		TLearner alg(m_argsForTLearner);
		alg.Fit(train);
		m_algsPool.push_back(alg);

		train.ChangeAnswers(UsefulMath::SubtractVectors(train.GetAnswers(), alg.PredictResult(train) ) );
	}
}

template<class TLearner> vector<long double> BoostedAlg<TLearner>::Predict(const DataFrame& df) const
{
	vector <long double> ans(df.GetDimention().first);
	vector <vector<long double>> results(m_numberOfAlgs);

	DataFrame dfToChange = df;

	for (uint16_t i = 0; i < m_numberOfAlgs; ++i)
	{
		results[i] = m_algsPool[i].PredictResult(dfToChange);
	}

	for (uint16_t i = 0; i < ans.size(); ++i)
	{
		for (uint16_t j = 0; j < m_numberOfAlgs; ++j)
		{
			ans[i] += results[j][i];
		}
	}
	return ans;
}


template<class TLearner> string BoostedAlg<TLearner>::GetDescriptionOfModel() const
{

	return "Number of algorithms: " + std::to_string(m_numberOfAlgs);
}


} // algorithms