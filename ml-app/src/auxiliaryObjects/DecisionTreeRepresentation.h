#pragma once

#include "Obj.h"

#include <functional>
#include <map>


namespace algorithm_helpers
{

class DecisionTreeRepresentation
{
public:
    void Add(uint32_t pos, uint32_t mainFeature, long double threshold);

    std::function<bool(const Obj& obj)> operator[](uint32_t pos) const;

    bool HaveThisPos(uint32_t pos) const;

    long double GetValue(uint32_t pos) const;

    std::string ShowTree(const vector<string>& featureNames) const;

private:
    inline uint32_t GetTreeHeight() const;
    inline uint32_t GetTreeWidth() const;
    inline string ConstructNode(uint32_t curValue, const vector<string>& featureNames) const;
    void FillTreeViewWithValues(uint32_t treeLevel, float nodeNumber, uint32_t value, const vector<string>& featureNames, vector<vector<string> >& outView) const;
	void FormOutput(string& T, const vector<vector<string> >& view) const;
private:
    std::map<uint32_t, std::pair<uint32_t, long double> > m_tree;
};


} // algorithm_helpers