#include <iostream> 
#include <iomanip>
#include <cmath>
#include <chrono>
#include <fstream>
#include <vector>
#include <random>
#include <string>

using namespace std;

int n;
int p;
int stime;
double massf[100000] = { 0 };
double b[100000];
//std::vector<double> massf(100000);

void FindM(int i, long long& x) {
	long double f;
	long double k;
	int t = 1023;
	f = massf[i];
	x = 0;
	if (f == 0) {
		return;
	}
	f = fabs(f);
	while (f < 1 && t > 0) {
		--t;
		f *= 2.0;
	}
	while (f >= 2 && t < 2048) {
		++t;
		f /= 2.0;
	}
	f -= 1;
	k = 0.5;
	for (int j = 0; j < 52; ++j) {
		x *= 2;
		if (f >= k) {
			x += 1;
			f -= k;
		}
		k /= 2;
	}
}

void FindP(int i, int& x) {
	long double f;
	f = massf[i];
	x = 1023;
	if (f == 0) {
		x = 0;
		return;
	}
	f = fabs(f);
	while (f < 1 && x > 0) {
		--x;
		f *= 2.0;
	}
	while (f >= 2 && x < 2048) {
		++x;
		f /= 2.0;
	}
}

void FindC(int i, int& x) {
	if (massf[i] < 0) {
		x = 1;
	}
	else {
		x = 0;
	}
}

void RadixSortM() {
	int a[256];
	long long x;
	for (int i = 0; i < 256; ++i) a[i] = 0;
	for (int i = 0; i < n; ++i) {
		FindM(i, x);
		++a[x >> p & 255];
	}
	for (int i = 1; i < 256; ++i) a[i] += a[i - 1];
	for (int i = n - 1; i >= 0; --i) {
		FindM(i, x);
		b[--a[x >> p & 255]] = massf[i];
	}
	for (int i = 0; i < n; ++i) {
		massf[i] = b[i];
	}
}

void RadixSortP() {
	int a[256];
	int k = 0;
	int x;
	for (int i = 0; i < 256; ++i) a[i] = 0;
	for (int i = 0; i < n; ++i) {
		FindP(i, x);
		++a[x >> p & 255];
	}
	for (int i = 1; i < 256; ++i) a[i] += a[i - 1];
	for (int i = n - 1; i >= 0; --i) {
		FindP(i, x);
		b[--a[x >> p & 255]] = massf[i];
	}
	for (int i = 0; i < n; ++i) {
		massf[i] = b[i];
	}
}

void RadixSortC() {
	int a[2];
	int k = 0;
	int x;
	for (int i = 0; i < 2; ++i) a[i] = 0;
	for (int i = 0; i < n; ++i) {
		FindC(i, x);
		++a[x];
		if (x == 1) ++k;
	}
	for (int i = 1; i < 2; ++i) a[i] += a[i - 1];
	for (int i = n - 1; i >= 0; --i) {
		FindC(i, x);
		b[--a[x]] = massf[i];
	}
	//for (int i = 0; i < n; ++i) cout << b[i][0] << " ";
	for (int i = 0; i < k; ++i) massf[i] = b[n - i - 1];
	for (int i = k; i < n; ++i) massf[i] = b[i - k];
}

void Radix() {
	p = 0;
	for (int i = 0; i < 7; ++i) {
		RadixSortM();
		p += 8;
	}
	p = 0;
	RadixSortP();
	p = 8;
	RadixSortP();
	RadixSortC();
}

/*
void FindCPM(int i) {
	long double f;
	long double k;
	f = massf[i];
	mass[i][0] = 0;
	mass[i][1] = 1023;
	mass[i][2] = 0;
	if (f == -0) {
		massf[i] = 0;
		mass[i][1] = 0;
		return;
	}
	else if (f == 0) {
		mass[i][1] = 0;
		return;
	}
	if (f < 0) {
		mass[i][0] = 1;
		f *= -1.0;
	}
	while (f < 1 && mass[i][1] > 0) {
		--mass[i][1];
		f *= 2.0;
	}
	while (f >= 2 && mass[i][1] < 2048) {
		++mass[i][1];
		f /= 2.0;
	}
	f -= 1;
	k = 0.5;
	for (int j = 0; j < 52; ++j) {
		mass[i][2] *= 2;
		if (f >= k) {
			mass[i][2] += 1;
			f -= k;
		}
		k /= 2;
	}
}*/
/*
void Out(int i) {
	cout << i << ") ";
	cout << mass[i][0] << " ";// << mass[i][1] << " ";
	for (int j = 10; j >= 0; --j) cout << (mass[i][1] >> j & 1);
	cout << " ";
	for (int j = 51; j >= 0; --j) cout << (mass[i][2] >> j & 1);
	//cout << " " << mass[i][2];
	cout << " = " << massf[i] << endl;
}*/

void RadixMain(ofstream& fout) {

	std::random_device rnd;
	std::mt19937 rng(rnd());
	std::uniform_int_distribution<int> uni1(-9223372000000000000, 9223372000000000000);
	std::uniform_int_distribution<int> uni2(1, 1000000000);

	for (int i = 0; i < n; ++i) {
		//massf[i] = (rand() % 10000000 - 10000) * 1.0 / (rand() % 1000);
		massf[i] = uni1(rng) * 1.0 / uni2(rnd);
		//cin >> massf[i];
		//FindCPM(i);
		//Out(i);
	}

	auto begin = std::chrono::high_resolution_clock::now();
	Radix();
	auto end = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < n - 1; ++i) {
		if (massf[i] > massf[i + 1]) throw "Merge is wrong";
	}
	auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
	//cout << endl;
	//for (int i = 0; i < n; ++i) cout << massf[i] << " ";
	//cout << endl << time.count() << endl;
	stime += time.count();
	fout << "," << time.count();
}

int main() {
	ofstream fout;
	fout.open("Double.txt");
	//cin >> n;
	n = 10;
	int y = 1;
	fout << "\n" << "\n";
	for (int i = 0; i < 100; ++i) {
		//cout << i << " ";
		fout << "," << "," << n;
		stime = 0;
		for (int j = 0; j < 7; ++j) {
			//massf.resize(n);
			RadixMain(fout);
			cout << ++y << " ";
		}
		fout << "," << stime / 7 << "\n";
		if (i < 4) n += 10;
		else if (i < 13) n += 50;
		else n += 500;
	}
	fout.close();
}