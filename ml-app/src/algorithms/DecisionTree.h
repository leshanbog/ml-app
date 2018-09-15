#pragma once

#include "Algo.h"


namespace algorithms
{


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

};


} // algorithms