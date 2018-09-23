#include "DecisionTreeRepresentation.h"

#include <queue>
#include <string>
#include <cmath>

//TODO: delete
#include <iostream>

namespace algorithm_helpers
{


void DecisionTreeRepresentation::Add(uint32_t pos, uint32_t mainFeature, long double threshold)
{
    m_tree[pos] = std::make_pair(mainFeature, threshold);
}

std::function<bool(const Obj& obj)> DecisionTreeRepresentation::operator[](uint32_t pos) const
{
    auto p = m_tree.find(pos);
    auto arg = p->second;
    return [arg](const Obj& obj) -> bool
    {
        return obj.description[arg.first] < arg.second;
    };
}

bool DecisionTreeRepresentation::HaveThisPos(uint32_t pos) const
{
    return m_tree.find(pos) != m_tree.end() && m_tree.find(pos)->second.first != (~(uint32_t)0);
}

long double DecisionTreeRepresentation::GetValue(uint32_t pos) const
{
    return m_tree.find(pos)->second.second;
}

string DecisionTreeRepresentation::ShowTree(const vector<string>& featureNames) const
{
    string T = "Decision tree:\n";
    
    /*
    uint32_t cur = 0;
    std::queue<uint32_t> treeLevel;

    if (HaveThisPos(cur))
        treeLevel.push(cur);

    while (!treeLevel.empty())
    {
        std::queue<int> nextTreelevel;
        while (!treeLevel.empty())
        {
            cur = treeLevel.front();
            if (HaveThisPos(2*cur+1))
                nextTreelevel.push(2*cur+1);
            if (HaveThisPos(2*cur+2))
                nextTreelevel.push(2*cur+2);
            treeLevel.pop();
            T += " [ ";
            T += featureNames[m_tree.find(cur)->second.first];
            T += " < ";
            T += std::to_string(m_tree.find(cur)->second.second);
            T += " ] ";
        }
        T += '\n';

        while (!nextTreelevel.empty())
        {
            treeLevel.push(nextTreelevel.front());
            nextTreelevel.pop();
        }
    }
    */

   vector<vector<string> >  view(GetTreeHeight(), vector<string> (GetTreeWidth()));
   FillTreeViewWithValues(0, GetTreeWidth() / 2, 0, featureNames, view);

    return T;
}

inline uint32_t DecisionTreeRepresentation::GetTreeHeight() const
{
    auto maxElem = m_tree.end();
    --maxElem;
    return log2(maxElem->first+1) + 1;
}

inline uint32_t DecisionTreeRepresentation::GetTreeWidth() const
{
    auto maxElem = m_tree.end();
    --maxElem;
    return pow(2,floor(log2(maxElem->first+1)));
}

inline string DecisionTreeRepresentation::ConstructNode(uint32_t curValue, const vector<string>& featureNames) const
{
    auto it = m_tree.find(curValue);
	if (it != m_tree.end())
	{
		if (it->second.first == (~(uint32_t)0))
			return "[ " + std::to_string(it->second.second) + " ]";
		else
			return "[ " + featureNames[it->second.first] + " < " + std::to_string(it->second.second) + " ]";
	}
	else
	{
		return "";
	}
}

 void DecisionTreeRepresentation::FillTreeViewWithValues(uint32_t treeLevel, float nodeNumber, uint32_t value, const vector<string>& featureNames, vector<vector<string> >& outView) const
 {
    if (treeLevel == outView.size())
        return;
	std::cout << treeLevel << " : " << nodeNumber << '\n';
    outView[treeLevel][nodeNumber] = ConstructNode(value, featureNames);
    FillTreeViewWithValues(treeLevel+1, nodeNumber - GetTreeWidth() / pow(2,treeLevel+2), value*2+1, featureNames, outView);
    FillTreeViewWithValues(treeLevel+1, nodeNumber + GetTreeWidth() / pow(2, treeLevel + 2), value*2+2, featureNames, outView);
 }

} // algorithm_helpers