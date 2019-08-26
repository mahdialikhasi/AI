#include <iostream>
using namespace std;
#include <algorithm>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>
void hiuristic(vector<long double> &h, vector<pair<long double, long double> > &position, int d){
	for (int i = 0; i < h.size(); ++i)
	{
		h[i] = sqrt(pow((position[i].first - position[d].first), 2) + pow((position[i].second - position[d].second), 2));
	}
}
void updateq(vector<vector<pair<int, long double> > > &graph, vector<pair<long double, int> > &q, pair<long double, int> &last, vector<int> &mark, vector<int> &parent, vector<long double> &h){
	//long double cost = last.first;
	long double cost = last.first - h[last.second];
	//cout << last.second << " is " << cost << endl;
	mark[last.second] = 1;
	for (int i = 0; i < graph[last.second].size(); ++i){
		// graph[last.second][i].first
		// graph[last.second][i].second
		int st = 0;
		for (int j = 0; j < q.size(); ++j)
		{
			if(q[j].second == graph[last.second][i].first){
				st = 1;
				//q[j].first = cost + graph[last.second][i].second;
				q[j].first = cost + graph[last.second][i].second + h[q[j].second];
				parent[q[j].second] = last.second;
				break;
			}
		}
		if(st == 0){
			if(mark[graph[last.second][i].first] == 0){
				q.push_back(make_pair(cost + graph[last.second][i].second + h[graph[last.second][i].first], graph[last.second][i].first));
				//q.push_back(make_pair(cost + graph[last.second][i].second, graph[last.second][i].first));
				parent[graph[last.second][i].first] = last.second; 
			}
		}
	}
}
void printq(vector<pair<long double, int> > &q){
	for (int i = 0; i < q.size(); ++i)
	{
		cout << q[i].second << " " << q[i].first << endl;
	}
	cout << endl << endl << endl << endl << endl;
}
long double aStar(vector<vector<pair<int, long double> > > &graph, vector<int> &mark, vector<int> &parent,vector<pair<long double, long double> > &position){
	int s,d;
	cout << "enter source id : " << endl;
	cin >> s;
	s = s - 1;
	cout << "enter destination id : " << endl; 
	cin >> d;
	d = d - 1;
	vector<long double> h(graph.size());
	hiuristic(h, position, d);
	vector<pair<long double, int> > q;
	q.push_back(make_pair(h[s], s));
	while(1){
		sort(q.begin(), q.end());
		reverse(q.begin(), q.end());
		//printq(q);
		if(q.empty()){
			cout << "cant reach that destination" << endl;
			return -1;
		}
		pair<long double, int> last = q[q.size() - 1];
		q.pop_back();
		if(last.second == d){
			cout << " the path is (reverse): " << endl;
			int k = d;
			cout << d + 1 << ", ";
			while(parent[k] != s){
				cout << parent[k] + 1 << ", ";
				k = parent[k];
			}
			cout << s + 1 << ", " << endl;
			return last.first;
		}else{
			//cout << last.second << " visited with cost " << last.first << endl;
			updateq(graph, q, last, mark, parent, h);
		}
	}
}
int main(){
	cout << "please enter the name of file : " << endl;
	string filename;
	cin >> filename;
	ifstream myfile;
	cout << endl;
	myfile.open(filename);
	if(myfile.is_open()){
		cout << "File opened successfully" << endl;
		int m,n;
		myfile >> n;
		myfile >> m;
		vector<vector<pair<int, long double> > > graph(n);
		vector<int> mark(n);
		vector<int> parent(n);
		vector<pair<long double, long double> > position(n);
		for (int i = 0; i < n; ++i)
		{
			int x;
			myfile >> x >> position[i].first >> position[i].second;
		}
		for (int i = 0; i < m; ++i)
		{
			long double l;
			int s,d;
			myfile >> s >> d >> l;
			graph[s - 1].push_back(make_pair(d - 1,l)); 
		}
		myfile.close();
		long double cost;
		cost = aStar(graph, mark, parent, position);
		cout << "cost :" <<  cost << endl;
	}else{
		cout << "Some problem happened when opening file" << endl;
	}
	return 0;
}