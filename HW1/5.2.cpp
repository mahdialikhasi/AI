#include <iostream>
using namespace std;
#include <algorithm>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <time.h>
#include <math.h>
#define Initial_T 5000
#define MAX_ROUND 10000
int t = Initial_T;
void updateTemp(int &t){
	t -= 1;
}
long long int evaluate(vector<int> &s,vector<vector<int> > &clause){
	long long counter = 0;
	for (int i = 0; i < clause.size(); ++i){
		int status = 0;
		for (int j = 0; j < clause[i].size(); ++j){
			if(clause[i][j] > 0 && s[clause[i][j] - 1] == 1){
				status = 1;
				break;
			}
			if(clause[i][j] < 0 && s[(clause[i][j] * -1) - 1] == 0){
				status = 1;
				break;	
			}
		}
		if(status == 0)
			counter++;
	}
	return counter;
}
int main(){
	srand (time(NULL));
	// use SA to solve SAT
	// first of all open file
	cout << "please enter the name of file : " << endl;
	string filename;
	cin >> filename;
	ifstream myfile;
	cout << endl;
	myfile.open(filename);
	vector<vector<int> > clause;
	if(myfile.is_open()){
		cout << "File opened successfully" << endl;
		long long int literal;
		clause.push_back(vector<int>());
		int counter = 0;
		int max = 1;
		while(myfile >> literal){
			if(literal == 0){
				clause.push_back(vector<int>());
				counter++;
			}else{
				clause[counter].push_back(literal);
			}
			if(abs(literal) > max){
				max = literal;
			}
		}
		myfile.close();
		//initial first
		vector<int> s(max);
		for (int i = 0; i < max; ++i){
			int v = rand() % 2;
			s[i] = v;
		}
		while(true){
			// evaluate s
			long long int eval = evaluate(s,clause);
			if(eval == 0){
				break;
			}
			if(t > 0){
				// do random select
				// choose random neighbour
				int choose = 0;
				while(!choose){
					int x = rand() % s.size();
					vector<int> tmp(s);
					if(tmp[x] == 0){
						tmp[x] = 1;
					}else{
						tmp[x] = 0;
					}
					long long int evSelectedNeighbour = evaluate(tmp, clause);
					if(evSelectedNeighbour < eval){
						// choose this neighbour
						s = tmp;
						choose = 1;
					}else{
						long int prob = exp((evSelectedNeighbour - eval) / t) * 100;
						int random = rand() % 100;
						if(random < prob){
							// choose
							s = tmp;
							choose = 1;	
						}
					}
				}
				updateTemp(t);
			}else{
				// do hill climbing
				// check all neighbours and choose best neighbour
				int x = -1;
				int min = eval;
				for (int i = 0; i < s.size(); ++i){
					vector<int> tmp(s);
					if(tmp[i] == 0){
						tmp[i] = 1;
					}else{
						tmp[i] = 0;
					}
					long long int tmpEval = evaluate(tmp, clause);
					if(tmpEval < min){
						min = tmpEval;
						x = i;
					}
				}
				if(x == -1){
					break;
				}else{
					cout << "	" <<  min << " clauses left unsatisfied" << endl;
					if(s[x] == 0){
						s[x] = 1;
					}else{
						s[x] = 0;
					}	
				}
			}
		}
		// evaluate s
		int finalEv = evaluate(s, clause);
		if(finalEv){
			cout << finalEv << " clause unsatisfied" << endl;
			for (int i = 0; i < s.size(); ++i){
				cout << "X" << i + 1 << " = " << s[i] << ", ";
			}
		}else{
			cout << "all clause satisfied" << endl;
			for (int i = 0; i < s.size(); ++i){
				cout << "X" << i + 1 << " = " << s[i] << ", ";
			}
		}
		cout << endl;
	}else{
		cout << "Some problem happened when opening file" << endl;
	}
	return 0;
}