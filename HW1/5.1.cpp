#include <iostream>
using namespace std;
#include <algorithm>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>
#include <time.h>
#define POPULATION_SIZE 4
#define GENERATION_ROUND 600
#define MUTANT_PROB 7

void createFirstGeneration(vector<vector<int> > &population, vector<pair<long double, long double> > &vertex, int size){
	// each member of population is a Permutation
	vector<int> initialPermutation(vertex.size());
	for (int i = 0; i < initialPermutation.size(); ++i){
		initialPermutation[i] = i;
	}
	for (int i = 0; i < size; ++i){
		vector<int> tmp(initialPermutation);
		random_shuffle(tmp.begin(), tmp.end());
		population[i] = tmp;
	}
}
long double distance(pair<long double, long double> first, pair<long double, long double> second){
	long double xDiff = first.first - second.first;
	long double yDiff = first.second - second.second;
	return sqrt(xDiff * xDiff + yDiff * yDiff);
}
void evaluationFunction(vector<vector<int> > &population, vector<pair<long double, long double> > &vertex, vector<long double> &evaluate){
	// evaluate population base on vertex and output on evaluate vector
	for (int i = 0; i < population.size(); ++i){
		for (int j = 0; j < population[i].size() - 1; ++j){
			// find distance between population[i][j] and population[i][j + 1]
			evaluate[i] += distance(vertex[population[i][j]], vertex[population[i][j + 1]]);
		}
		// add distance between first (population[i][0]) and last one (population[i][population[i].size() - 1])
		evaluate[i] += distance(vertex[population[i][0]], vertex[population[i][population[i].size() - 1]]);
	}
}
pair<vector<int>, vector<int> > createChild(vector<vector<int> > &population, vector<pair<long double,int> > &ev_pop, long double &sum){
	int v = rand() % 100;
	long double choose = (v * sum) / 100;
	long double tmp = 0;
	int element1 = ev_pop.size() - 1;
	for (int i = 0; i < ev_pop.size(); ++i){
		tmp += ev_pop[i].first;
		if(choose <= tmp){
			// found
			element1 = ev_pop.size() - i - 1;
			break;
		}
	}
	v = rand() % 100;
	choose = (v * sum) / 100;
	tmp = 0;
	int element2 = ev_pop.size() - 1;
	for (int i = 0; i < ev_pop.size(); ++i){
		tmp += ev_pop[i].first;
		if(choose <= tmp){
			// found
			element2 = ev_pop.size() - i - 1;
			break;
		}
	}

	// cross over element1 and element2
	int line = rand() % population[ev_pop[element1].second].size();
	vector<int> child1(population[ev_pop[element1].second].size(), -1);
	vector<int> child2(population[ev_pop[element2].second].size(), -1);
	for (int i = 0; i < line; ++i){
		child1[i] = population[ev_pop[element1].second][i];
		child2[i] = population[ev_pop[element2].second][i];
	}
	for (int i = line; i < child1.size(); ++i){
		for (int j = 0; j < population[ev_pop[element2].second].size(); ++j){
			auto p = find(child1.begin(), child1.end(),population[ev_pop[element2].second][j]);
			if(p == child1.end()){
				child1[i] = population[ev_pop[element2].second][j];
				break;
			}
		}
	}
	for (int i = line; i < child2.size(); ++i){
		for (int j = 0; j < population[ev_pop[element1].second].size(); ++j){
			auto p = find(child2.begin(), child2.end(),population[ev_pop[element1].second][j]);
			if(p == child2.end()){
				child2[i] = population[ev_pop[element1].second][j];
				break;
			}
		}
	}
	return make_pair(child1, child2);
}
void mutant(vector<int> &child, int percent){
	int v = rand() % 100;
	if(v < percent){
		// do mutant
		int element1 = rand() % child.size();
		int element2 = rand() % child.size();
		swap(child[element1], child[element2]);
	}
}
int main(){
	srand (time(NULL));
	// use genetic algorithm to solve TSP
	//first of all load data to a graph by reading from file
	cout << "please enter the name of file : " << endl;
	string filename;
	cin >> filename;
	ifstream myfile;
	cout << endl;
	myfile.open(filename);
	vector<pair<long double, long double> > vertex;
	if(myfile.is_open()){
		cout << "File opened successfully" << endl;
		long double index;
		long double x;
		long double y;
		while(myfile >> index){
			myfile >> x;
			myfile >> y;
			vertex.push_back(make_pair(x,y));
		}
		myfile.close();
		// now create first generation
		// each generation consist of 100 sample randomly (5 percent of all samples)
		vector<vector<int> > population(POPULATION_SIZE, vector<int>(vertex.size()));
		createFirstGeneration(population, vertex, population.size());
		
		// 100 round genetic algorithm
		for (int i = 0; i < GENERATION_ROUND; ++i){
			// now evaluate each element of population
			vector<long double> evaluate(population.size());
			evaluationFunction(population, vertex, evaluate);
			cout << "Generation " << i << endl;
			// sort population
			vector<pair<long double,int> > ev_pop(evaluate.size());
			for (int i = 0; i < ev_pop.size(); ++i){
				ev_pop[i] = make_pair(evaluate[i], i);
			}
			sort(ev_pop.begin(), ev_pop.end());
			cout << "	best Path in this generation : " << endl;
			cout << "	" << ev_pop[0].first << endl;

			// create child
			long double sum = 0;
			for (int i = 0; i < ev_pop.size(); ++i){
				sum += ev_pop[i].first;
			}
			vector<vector<int> > childPopulation(population.size(), vector<int>(vertex.size()));
			for (int i = 0; i < population.size() / 2; ++i){
				pair<vector<int>, vector<int> > tmp = createChild(population, ev_pop, sum);
				childPopulation[i] = tmp.first;
				childPopulation[childPopulation.size() - 1 - i] = tmp.second;
			}

			// mutant child with 5 percent probability
			for (int i = 0; i < childPopulation.size(); ++i){
				mutant(childPopulation[i], MUTANT_PROB);
			}

			// evaluate childs
			vector<long double> evaluateChild(childPopulation.size());
			evaluationFunction(childPopulation, vertex, evaluateChild);

			// sort child
			vector<pair<long double,int> > ev_chi(evaluateChild.size());
			for (int i = 0; i < ev_chi.size(); ++i){
				ev_chi[i] = make_pair(evaluateChild[i], i);
			}
			sort(ev_chi.begin(), ev_chi.end());
			cout << "		best Path in this generation childs : " << endl;
			cout << "	" << ev_chi[0].first << endl;

			// create new generation base on parents and childrens
			vector<vector<int> > newPopulation(POPULATION_SIZE, vector<int>(vertex.size()));
			int popIterator = 0;
			int chilIterator = 0; 
			for (int i = 0; i < newPopulation.size(); ++i){
				if(ev_chi[chilIterator] < ev_pop[popIterator]){
					newPopulation[i] = childPopulation[ev_chi[chilIterator].second];
					chilIterator++;
				}else{
					newPopulation[i] = population[ev_pop[popIterator].second];
					popIterator++;
				}
			}
			population = newPopulation;
		}
		// now evaluate each element of population
		vector<long double> evaluate(population.size());
		evaluationFunction(population, vertex, evaluate);
		// sort population
		vector<pair<long double,int> > ev_pop(evaluate.size());
		for (int i = 0; i < ev_pop.size(); ++i){
			ev_pop[i] = make_pair(evaluate[i], i);
		}
		sort(ev_pop.begin(), ev_pop.end());

		cout << "Best Path Evaluation" << endl;
		cout << ev_pop[0].first << endl;
		cout << "Path : " << endl;
		for (int i = 0; i < population[ev_pop[0].second].size(); ++i){
			cout << population[ev_pop[0].second][i] + 1 << " " ;
		}
		cout << endl;
	}else{
		cout << "Some problem happened when opening file" << endl;
	}
	
	return 0;
}