#pragma once

#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>

#include "auxiliaryObjects.h"


enum DataRegime { TRAIN, TEST };

template <class TLearner>
class CrossValidation
{
public:
	long double Score(const DataFrame &df, int foldsNum, const ArgsForAlg& args);

private:
	vector <int> m_folds;
	vector <int> m_cv;

	std::mutex m_mtx;

	// make vector<Obj> for train DataFrame if Regime r == TRAIN, else for test
	vector<Obj> MakeVecForDf(const DataFrame& df, int k, DataRegime r);
	void Init(size_t elemNumber, int foldsNum);
	void StepScore(const DataFrame &df, long double& ansRMSE, int i, const ArgsForAlg& arg);
};




template<class TLearner> inline void CrossValidation<TLearner>::Init(size_t elemNumber, int foldsNum)
{
	// shuffling data
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	m_cv.resize(elemNumber);
	for (size_t i = 0; i < m_cv.size(); ++i)
		m_cv[i] = i;
	shuffle(m_cv.begin(), m_cv.end(), std::default_random_engine(seed));

	// making folds
	m_folds = UsefulMath::CreateFolds(elemNumber, foldsNum);
}

template<class TLearner> void CrossValidation<TLearner>::StepScore(const DataFrame &df, long double& ansRMSE, int i, const ArgsForAlg& args)
{
	TLearner alg(args);

	DataFrame
		train(MakeVecForDf(df, i, TRAIN), df.m_stats),
		test(MakeVecForDf(df, i, TEST), df.m_stats);

	if (train.m_wasNormalized)
		train.PerformNormalization(train.m_stats);

	vector <long double> answersTest = test.GetAnswers();
	alg.Fit(train);
	long double tempRmse = (UsefulMath::SE(alg.PredictResult(test), answersTest) / (m_folds.size()-1)) / test.GetDimention().first;

	std::unique_lock<std::mutex> m_lck(m_mtx, std::defer_lock);

	m_lck.lock();
	ansRMSE += tempRmse;
	m_lck.unlock();
}

template<class TLearner> long double CrossValidation<TLearner>::Score(const DataFrame &df, int foldsNum, const ArgsForAlg& args)
{
	Init(df.GetDimention().first, foldsNum);

	long double ansRMSE = 0;
	vector <std::thread*> threads;

	auto f = [this, &df, &ansRMSE, &args](int i) { this->StepScore(df, ansRMSE, i, args); };

	for (int i = 0; i < foldsNum; ++i)
	{
		std::thread* thr = new std::thread(f,i);
		threads.push_back(thr);
	}
	for (int i = 0; i < foldsNum; ++i)
	{
		threads[i]->join();
	}

	return pow(static_cast<long double>(ansRMSE), 0.5);
}

template<class TLearner> vector<Obj> CrossValidation<TLearner>::MakeVecForDf(const DataFrame& df, int k, DataRegime r)
{
	vector <Obj> ans;

	if (r == TRAIN) // train, i.e. except k-th fold
	{
		for (int i = 0; i < m_folds[k]; ++i)
		ans.push_back(df.GetData()[m_cv[i]]);
		for (int i = m_folds[k + 1]; i < df.GetDimention().first; ++i)
		ans.push_back(df.GetData()[m_cv[i]]);
	}
	else // test, i.e. only k - th fold
	{
		for (int i = m_folds[k]; i < m_folds[k + 1]; ++i)
		ans.push_back(df.GetData()[m_cv[i]]);
	}
	return ans;
}