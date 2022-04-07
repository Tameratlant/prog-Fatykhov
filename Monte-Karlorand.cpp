#include<iostream>
#include<string>
#include <vector>
#include <algorithm>
#include <functional>
#include <future>
#include <numeric>
#include <thread>

const int N = 0; const int size = 1000000, R =50;
void counter(int start, int end, int n, std::vector<int> vec) {
	int i = 0;
	vec[n] = 0;
	while (start < end) {
		int x = rand() % (2 * R) - 50;
		int y = rand() % (2 * R) - 50;
		if (sqrt(x * x + y * y) < R) vec[n] ++;
		i++;
	}
}

int main() {
	std::vector<int> vec(N);

	for (int i = 0; i < N; i++) {
		std::thread t[i](counter, i * size, size * (i + 1), N, vec);
	}
	t.join();

	int sum = 0;
	for (int i = 0; i < N; i++) {
		//sum = sum + vec[i];
		std::cout << vec[i] << std::endl;

	}
	
	std::cout << sum;
	return 0;
}
