#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <map>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

map <string, vector<string> > graph;
vector<string> graphName;
vector<vector<int> > graphV; 
int e = 0;
int W, L, D;

bool is_consistent(string s1, string s2);
void permute(string str);
void initialGraph();
int main(){
	cin >> L >> W >> D;
	// create all sub member
	string s;
	for (int i = 0; i < W; i++)
		s.push_back('1');
	for (int i = 0; i < L - W; i++)
		s.push_back('0');
	permute(s); 
	/*for (auto i = graph.begin(); i != graph.end(); ++i)
	{
		cout << i->first << endl;
	}*/
	initialGraph();
	ofstream file;
   	file.open("cli.mtx", ios::out | ios::trunc);
   	file << "%%MatrixMarket matrix coordinate pattern symmetric" << endl;
   	file << graphV.size() << " " << graphV.size() << " " << e << endl;
   	for (int i = 0; i < graphV.size(); ++i){
   		for (int j = 0; j < graphV[i].size(); ++j)
   		{
   			file << i + 1 << " " << j + 1 << endl;
   		}
   	}
   	file.close();
	return 0;
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
void permute(string str) { 
    sort(str.begin(), str.end());
    do { 
    	vector<string> a;
    	graph[str] = a; 
    } while (next_permutation(str.begin(), str.end())); 
} 
void initialGraph(){
	// convert
	for (auto i = graph.begin(); i != graph.end(); ++i){
		graphName.push_back(i->first);
		graphV.push_back(vector<int>());
	}
	for (int i = 0; i != graphName.size(); ++i){
		for (auto j = 0; j != graphName.size(); ++j){
			if(i != j){
				if(is_consistent(graphName[i], graphName[j])){
					graphV[i].push_back(j);
					e++;
				}
			}
		}	
	}
}