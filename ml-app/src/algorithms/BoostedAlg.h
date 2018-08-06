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
		m_numberOfAlgs = args.argsInt[0];

		for (int i = 1; i < args.argsInt.size(); ++i)
			m_argsForTLearner.argsInt.push_back(args.argsInt[i]);

		m_algsPool.reserve(m_numberOfAlgs);
	}

	inline string GetAlgName() const override
	{
		string ans = "Boosted " + m_algsPool[0].GetAlgName();
		if (m_argsForTLearner.argsInt.size() != 0)
		{
			ans += "(";
			for (const auto& el : m_argsForTLearner.argsInt)
			{
				ans += std::to_string(el) + ", ";
			}
			ans[ans.size()-2] = ')';
		}
		return ans;
	}

	void Learn(const DataFrame&) override;

	vector <long double> Predict(const DataFrame&) const override;

	string GetDescriptionOfModel() const override;

private:
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

		train.ChangeAnswers(UsefulMath::SubtractVectors(train.GetAnswers(), alg.Predict(train) ) );
	}
}

template<class TLearner> vector<long double> BoostedAlg<TLearner>::Predict(const DataFrame& df) const
{
	vector <long double> ans(df.GetDimention().first);
	vector <vector<long double>> results(m_numberOfAlgs);

	for (uint16_t i = 0; i < m_numberOfAlgs; ++i)
	{
		results[i] = m_algsPool[i].Predict(df);
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