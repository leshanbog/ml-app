#pragma once

#include "Algo.h"
#include <functional>
#include <map>

namespace algorithms
{


class DecisionTreeRepresentation
{
public:
    void add(uint32_t pos, uint32_t mainFeature, long double threshold)
    {
        m_tree[pos] = {mainFeature, threshold};
    }

    std::function<bool(const Obj& obj)> operator[](uint32_t pos) const
    {
        auto p = m_tree.find(pos);
        auto arg = p->second;
        return [&arg](const Obj& obj) -> bool
        {
            return obj.description[arg.first] < arg.second;
        };
    }

    bool HaveThisPos(uint32_t pos) const
    {
        return m_tree.find(pos) != m_tree.end();
    }

private:
    std::map<uint32_t, pair<uint32_t, long double>> m_tree;
};


class DecisionTree : public Algo
{
public:
    DecisionTree(const ArgsForAlg& args);

    inline string GetAlgName() const override
    {
        return "Decision Tree";
    }

    void Learn(const DataFrame& df) override;

    vector<long double> Predict(const DataFrame& df) const override;

    string GetDescriptionOfModel() const override;
private:
    void RecursiveLearn(const DataFrame& df, uint32_t pos);
    bool IsNeedToStop(const DataFrame& df) const;
    void SplitDataFrame(const std::function<bool(const Obj& obj)>& pred, const DataFrame& df, DataFrame& outSubDf1, DataFrame& outSubDf2);
    long double Sift(const Obj& obj) const;

private:
    DecisionTreeRepresentation m_dtr;
};


} // algorithms