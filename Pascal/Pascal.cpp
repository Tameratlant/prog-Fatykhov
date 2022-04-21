#include <iostream>

template <int n, int k>
struct F
{
	enum {
		result = F<n - 1, k - 1>::result +F <n - 1, k>::result
	};
};

template <>
struct F<1, 1>
{
	static const int result = 1;
};

template <int n>
struct F<n, n>
{
	static const int result = 1;
};

template <int n>
struct F<n, 1>
{
	static const int result = n;
};

int main() {
	std::cout << F<5, 2>::result;
	return 0;
}
