#include <limits>

#include "UsefulMath.h"

namespace algorithm_helpers
{

long double UsefulMath::mean(const vector<long double>& v)
{
	long double mean = 0;
	for (size_t i = 0; i < v.size(); ++i)
	{
		mean += (v[i] - mean) / (i + 1);
	}
	return mean;
}

long double UsefulMath::sum(const vector<long double>& v)
{
	long double s = 0;
	for (auto el : v) s += el;
	return s;
}

long double UsefulMath::RMSE(const vector<long double>& a, const vector<long double>& b)
{
	long double rmse = 0;
	for (size_t i = 0; i < a.size(); ++i)
	{
		rmse += (a[i] - b[i])*(a[i] - b[i]);
	}
	return pow(rmse/a.size(), 0.5);
}

long double UsefulMath::MAE(const vector<long double>& a, const vector<long double>& b)
{
	long double mae = 0;
	for (size_t i = 0; i < a.size(); ++i)
	{
		mae += std::abs(a[i] - b[i]);
	}
	return mae / a.size();

}

long double UsefulMath::SE(long double a, const vector<long double>& b)
{
	long double se = 0;
	for (size_t i = 0; i < b.size(); ++i)
	{
		se += (a - b[i])*(a - b[i]);
	}
	return se;
}

long double UsefulMath::SE(const vector<long double>& a , const vector<long double>& b)
{
	long double se = 0;
	for (size_t i = 0; i < a.size(); ++i)
	{
		se += (a[i] - b[i])*(a[i] - b[i]);
	}
	return se;
}

long double UsefulMath::AE(long double a, const vector<long double>& b)
{
	long double ae = 0;
	for (size_t i = 0; i < b.size(); ++i)
	{
		ae += std::abs(a - b[i]);
	}
	return ae;
}

long double UsefulMath::AE(const vector<long double>&a, const vector<long double>& b)
{
	long double ae = 0;
	for (size_t i = 0; i < a.size(); ++i)
	{
		ae += std::abs(a[i] - b[i]);
	}
	return ae;;
}

long double UsefulMath::RootMean(const vector<long double> &a)
{
	long double rm = 0;
	for (size_t i = 0; i < a.size(); ++i)
	{
		rm += a[i];
	}
	rm /= a.size();
	return pow(rm, 0.5);
}

size_t UsefulMath::argmin(const vector<long double>& v)
{
	long double m = minimum(v);
	for (size_t i = 0; i < v.size(); ++i)
	{
		if (v[i] == m) return i;
	}
	return -1;
}

long double UsefulMath::minimum(const vector<long double>& v)
{
	long double m = std::numeric_limits<long double>::max();
	for (size_t i = 0; i < v.size(); ++i)
	{
		if (v[i] < m)
			m = v[i];
	}
	return m;
}

bool UsefulMath::balance(const vector <int>& v)
{
	if (v.size() == 1) return true;
	int t = v[1] - v[0];
	for (size_t i = 1; i < v.size(); ++i)
	{
		if (v[i] - v[i - 1] > t + 1) return false;
	}
	return true;
}

vector<int> UsefulMath::CreateFolds(int n, int k)
{
	vector <int>  f(k+1);
	for (int i = 0; i < k; i++)
		f[i] = i * (n / k);
	f[k] = n;
	int add = 1;
	while (!balance(f))
	{
		for (int i = k-1, t = 0; i > 0 && t < add; i--,t++)
		{
			f[i]++;
		}
		add++;
	}
	return f;
}


} // algorithm_helpers
