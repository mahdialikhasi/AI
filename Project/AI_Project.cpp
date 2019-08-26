#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <set>
#include <algorithm>
#include <math.h>
using namespace std;
int W, L, D;
double T = 1000;

string neighbour (string main) {
	vector<int> zeros_index, ones_index;
	for (int i = 0; main[i] != 0; i++) {
		if (main[i] == '0')
			zeros_index.push_back(i);
		else
			ones_index.push_back(i);
	}
	for (int i = 0; i < min(W, L - W); i++) {
		int r0 = rand() % (L - W);
		int r1 = rand() % W;
		int t = zeros_index[r0];
		zeros_index[r0] = zeros_index[L - W - r0 - 1];
		zeros_index[L - W - r0 - 1] = t;
		t = ones_index[r1];
		ones_index[r1] = ones_index[W - r1 - 1];
		ones_index[W - r1 - 1] = t;
	}
	for (int i = 0; i < D / 2; i++) {
		char t = main[zeros_index[i]];
		main[zeros_index[i]] = main[ones_index[i]];
		main[ones_index[i]] = t;
	}
	return main;
}
vector<string> PartNeighbour(string st){
	vector<int> zeros_index, ones_index;
	for (int i = 0; st[i] != 0; i++) {
		if (st[i] == '0')
			zeros_index.push_back(i);
		else
			ones_index.push_back(i);
	}
	vector<string> v;
	for (int i = 0; i < 500; ++i)
	{
		v.push_back(neighbour(st));
	}
	return v;
}
bool is_consistent(string s1, string s2) {
	string f;
	for (int i = 0; i < s1.length(); i++)
		f.push_back(((s1[i] - '0') ^ (s2[i] - '0')) + '0');
	int c = 0;
	for (int i = 0; i < f.length(); i++) 
		if (f[i] == '1')
			c++;
	if (c >= D)
		return true;
	else
		return false;
}
int main() {
	srand(time(NULL));
	cin >> W >> L >> D;
	int max = 0;
	vector<string> best;
	for (int i = 0; i < 1000; i++) {	
		T = 10000;	
		string s;
		for (int i = 0; i < W; i++)
			s.push_back('1');
		for (int i = 0; i < L - W; i++)
			s.push_back('0');
		for (int i = 0; i < L; ++i)
		{
			int a = rand() % L;
			int b = rand() % L;
			swap(s[a], s[b]);
		}
		vector<string> all;
		while (T > 1) {
			string st = neighbour(s);
			bool cons = true;
			int m = 0;
			for (auto x : all) {
				if (!is_consistent(st, x)) {
					cons = false;
				}else{
					m++;
				}
			}
			if (cons == true) {
				s = st;
				all.push_back(s);
			} else {
				int a = m - all.size();
				double c = exp(a / T);
				T -= 0.5 * c;
				//cout << T << endl;
				int b = rand() % 100;
				//if (b < a)
					s = st;
			}
		}
		//cout << all.size() << ":";
		int count = 0;
		/*while(1){
			vector<string> a = PartNeighbour(s);
			int status = 0;
			int max = 0;
			for (int i = 0; i < a.size(); ++i){
				bool cons = true;
				int m = 0;
				for (auto x : all) {
					if (!is_consistent(a[i], x)) {
						cons = false;
					}else{
						m++;
					}
				}
				if(m > max){
					max = m;
					s = a[i];
				}
				if(cons){
					all.push_back(a[i]);
					status = 1;
					break;
				}
			}
			if(status == 0)
				count++;
			if(count == 200)
				break;
			//cout << "here" << count << status << endl;
		}*/
		
		cout << all.size() << endl;
		if(all.size() > max){
			best = all;
			max = all.size();
		}
	}
	cout << "-------------------" << endl;
	cout << max << endl;
	
	return 0;
}