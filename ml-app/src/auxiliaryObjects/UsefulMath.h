#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
using std::vector;


namespace algorithm_helpers
{

class UsefulMath
{
public:
	static long double mean(const vector <long double>&);
	static long double sum(const vector <long double>&);
	static long double RMSE(const vector <long double> &, const  vector <long double> &);
	static long double MAE(const vector <long double> &, const  vector <long double> &);
	static long double SE(long double, const  vector <long double> &);
	static long double SE(const  vector <long double> &, const  vector <long double> &);
	static long double AE(long double, const  vector <long double> &);
	static long double AE(const vector <long double> &, const vector <long double> &);
	static long double RootMean(const vector <long double> &);
	static size_t argmin(const vector <long double> &);
	static long double minimum(const vector <long double>&);
	static vector <int> CreateFolds(int, int);
	template <typename T> static vector<T> SubtractVectors(const vector<T> &, const vector<T> &);
private:
	static bool balance(const vector <int>&);
};

template<typename T> vector<T> UsefulMath::SubtractVectors(const vector<T>& a, const vector<T>& b)
{
	vector<T> ans(a.size());
	uint16_t i = 0;
	for_each(ans.begin(), ans.end(), [&i, &a, &b](auto&el) { el = a[i] - b[i]; ++i; });
	return ans;
}

} // algorithm_helpers
