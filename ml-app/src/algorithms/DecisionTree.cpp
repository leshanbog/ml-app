#include "DecisionTree.h"
#include "DecisionStump.h"

// TODO: delete
#include <iostream>

namespace algorithms
{


void DecisionTree::Learn(const DataFrame& df)
{
    RecursiveLearn(df, 0);
}

void DecisionTree::RecursiveLearn(const DataFrame& df, uint32_t pos)
{
    std::cout << "Size of " << pos << " df = " << df.GetDimention().first << "\n";
    if (!IsNeedToStop(df))
    {
        DecisionStump ds;
        ds.Learn(df);

        m_dtr.Add(pos, ds.GetMainFeature(), ds.GetThreshold());

        DataFrame subDf1, subDf2;
        SplitDataFrame(m_dtr[pos], df, subDf1, subDf2);
        RecursiveLearn(subDf1, 2 * pos + 1);
        RecursiveLearn(subDf2, 2 * pos + 2);
    }
    else
    {
        m_dtr.Add(pos, ~0, UsefulMath::mean(df.GetAnswers()));
    }
}

bool DecisionTree::IsNeedToStop(const DataFrame& df) const
{
    return df.GetDimention().first <= 10;
}

void DecisionTree::SplitDataFrame(const std::function<bool(const Obj& obj)>& pred, const DataFrame& df, DataFrame& outSubDf1, DataFrame& outSubDf2)
{
    for (uint32_t i = 0; i < df.GetDimention().first; ++i)
    {
        if (pred(df.GetData()[i]))
        {
            // TODO: check performance
            outSubDf1.Add(df.GetData()[i]);
        }
        else
        {
            outSubDf2.Add(df.GetData()[i]);
        }
    }
}

vector<long double> DecisionTree::Predict(const DataFrame& df) const
{
    vector<long double> ans(df.GetDimention().first);

    for (uint32_t i = 0; i < ans.size(); ++i)
    {
        ans[i] = SiftAndPredict(df.GetData()[i]);
    }
    return ans;
}

long double DecisionTree::SiftAndPredict(const Obj& obj) const
{
    uint32_t cur = 0;
    while (m_dtr.HaveThisPos(cur))
    {
        if (m_dtr[cur](obj))
        {
            cur = cur * 2 + 1;
        }
        else
        {
            cur = cur * 2 + 2;
        }
    }
    return m_dtr.GetValue(cur);
}

string DecisionTree::GetDescriptionOfModel() const
{
    return m_dtr.ShowTree(m_featureNames);
}

} // algorithms
