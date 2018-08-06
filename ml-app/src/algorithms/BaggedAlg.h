#pragma once

#include "Algo.h"


namespace algorithms
{


template <class TLearner>
class BaggedAlg : public Algo
{
public:
	inline BaggedAlg(const ArgsForAlg& args)
	{
		m_numberOfAlgs = args.argsInt[0];
		m_sizeOfTrainingSet = args.argsInt[1];

		for (uint16_t i = 2; i < args.argsInt.size(); ++i)
			m_argsForTLearner.argsInt.push_back(args.argsInt[i]);

		m_algsPool.reserve(m_numberOfAlgs);
	}

	inline string GetAlgName() const override
	{
		string ans;
		ans += "Bagged " + m_algsPool[0].GetAlgName();
		if (m_argsForTLearner.argsInt.size() != 0)
		{
			ans += "( ";
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
	DataFrame GenerateTrainingSet(const DataFrame&);

private:
	size_t m_numberOfAlgs;
	size_t m_sizeOfTrainingSet;
	vector <TLearner> m_algsPool;
	ArgsForAlg m_argsForTLearner;
};


template<class TLearner> void BaggedAlg<TLearner>::Learn(const DataFrame& df)
{
	for (uint16_t i = 0; i < m_numberOfAlgs; ++i)
	{
		TLearner alg(m_argsForTLearner);
		DataFrame train = GenerateTrainingSet(df);
		alg.Fit(train);
		m_algsPool.push_back(alg);
	}

}

template<class TLearner> vector<long double> BaggedAlg<TLearner>::Predict(const DataFrame &df) const
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
			ans[i] += (results[j][i] / m_numberOfAlgs);
		}
	}
	return ans;
}

template<class TLearner> string BaggedAlg<TLearner>::GetDescriptionOfModel() const
{
	string ans = "";
	ans += "Number of algorithms: " + std::to_string(m_numberOfAlgs);
	ans += "\nNumber of instances in training set: " + std::to_string(m_sizeOfTrainingSet);
	return ans;

}


template<class TLearner> DataFrame BaggedAlg<TLearner>::GenerateTrainingSet(const DataFrame& df)
{
	vector <Obj> train(m_sizeOfTrainingSet);
	for_each(train.begin(), train.end(), [&df](Obj& ob) {
		ob = df.GetData()[rand() % df.GetDimention().first];
	});

	return DataFrame(move(train));
}


} // algorithms