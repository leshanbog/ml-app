#pragma once

#include "Algo.h"
#include "../auxiliaryObjects/DecisionTreeRepresentation.h"
#include <functional>

namespace algorithms
{


class DecisionTree : public Algo
{
public:
    DecisionTree(const ArgsForAlg& args) {}

    inline string GetAlgName() const override
    {
        return "Decision Tree";
    }

    string GetDescriptionOfModel() const override;
private:
	void Learn(const DataFrame& df) override;
	vector<long double> Predict(const DataFrame& df) const override;

    void RecursiveLearn(const DataFrame& df, uint32_t pos);
    bool IsNeedToStop(const DataFrame& df) const;
    void SplitDataFrame(const std::function<bool(const Obj& obj)>& pred, const DataFrame& df, DataFrame& outSubDf1, DataFrame& outSubDf2);
    long double SiftAndPredict(const Obj& obj) const;

private:
    DecisionTreeRepresentation m_dtr;
};


} // algorithms