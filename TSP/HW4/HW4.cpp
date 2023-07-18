/*
last revise time: 2022/12/10
argv: ./HW4 Run Iteration #Ant ShowEachRunResult(y/n) OutputToFile(y/n) ShowRunTime(y/n)
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include <ctime>
using namespace std;

double pheromones[51][51];
double Distance[51][51];
//random seed
random_device rd;
mt19937 gen(rd());

class City{
	public:
		int x, y, index;
		City(int X, int Y, int I):x(X), y(Y), index(I){};
};

class Ant{
	public:
		int current, start, travlList[51], startOrder[51];
		vector<int> unvisitedList;
		double dist;
		Ant():dist(0.0){};
		void visitCity(int target){
			//remove city from unvisitedlist if visited
			for(int i = 0; i < unvisitedList.size(); ++i){
				if(unvisitedList.at(i) == target){
					unvisitedList.erase(unvisitedList.begin()+i);
					break;
				}
			}
		};
		
		void init(vector<City> cityList){
			for(int i = 0; i < cityList.size(); ++i){
				unvisitedList.push_back(i);
				travlList[i] = 0;
			}
			//decide start city
			uniform_int_distribution<int> dir(0, (int)unvisitedList.size()-1);
			start = unvisitedList.at(dir(gen));
			current = start;
			visitCity(current);
		};
		void decideNextCity(int order, vector<City> cityList){
			double totalPheromones = 0.0;
			for(int j = 0; j < unvisitedList.size(); ++j)
				totalPheromones += pheromones[current][unvisitedList.at(j)]/(Distance[current][unvisitedList.at(j)]);
			
			uniform_real_distribution<double> dir(0.0, totalPheromones);
			double probability = dir(gen);
			//decide next city
			int target = unvisitedList.size() - 1;
			for( ; totalPheromones > probability; target--)
				totalPheromones -= pheromones[current][unvisitedList.at(target)]/(Distance[current][unvisitedList.at(target)]);
			
			current = unvisitedList.at(target+1);
			travlList[current] = order;
			visitCity(current);
		};
		void constructSol(vector<City> cityList){
			init(cityList);
			for(int i = 1; i < cityList.size(); ++i)
				decideNextCity(i, cityList);
			for(int i = 0; i < cityList.size(); ++i)
				startOrder[travlList[i]] = i;
			//calculate distance
			dist += Distance[startOrder[0]][startOrder[cityList.size()- 1]];
			for(int i = 0; i < cityList.size() - 1; ++i)
				dist += Distance[startOrder[i]][startOrder[i+1]];
		};
		void undatePheromonesbyAnt(int citySize){
			for(int i = 0; i < citySize - 1; ++i){
				pheromones[startOrder[i]][startOrder[i+1]] += 1/dist;
				pheromones[startOrder[i+1]][startOrder[i]] += 1/dist;
			}
			pheromones[startOrder[0]][citySize-1] += 1/dist;
			pheromones[citySize-1][startOrder[0]] += 1/dist;
		};
		
		void outputSol(int citySize){
			cout << dist << " ";
			for(int i = 0; i < citySize; ++i)
				cout << startOrder[i] << " ";
			cout << endl;
		};
		
		void outputTofile(int citySize,string dataSet){
			ofstream outputFile(dataSet+"/ans_"+dataSet+".txt");
			if(!outputFile){
				cerr << "File Error: create file name failed." << endl;
				exit(1);
			}
			
			outputFile << dist << endl;
			for(int i = 0; i < citySize; ++i)
				outputFile << startOrder[i] + 1 << endl;
			outputFile.close();
		};
};

Ant undatePheromones(int citySize,Ant AntColony[], int antSize){
	//evaporation rate = 0.9
	for(int i = 0; i < citySize; ++i)
		for(int j = 0; j < citySize; ++j)
			pheromones[i][j] *= 0.9;
	//MMAS - find the best ant to update
	int minIndex = 0;
	for(int i = 1; i < antSize; ++i){
		if(AntColony[i].dist < AntColony[minIndex].dist)
			minIndex = i;
	}
	AntColony[minIndex].undatePheromonesbyAnt(citySize);
	return AntColony[minIndex];
}


int main(int argc, char *argv[]){
	double start, end;
	start = clock();
	
	if(argc != 8){
		cerr << "Input Error: Input arugement do not match." << endl;
		return 1;
	}
	ifstream readFile(string(argv[1])+"/point.txt");
	if(!readFile){
		cerr << "File Error: Not suck file name." << endl;
		return 1;
	}
	
	int X, Y, I;
	vector<City> cityList;
	while(readFile >> I >> X >> Y)
		cityList.push_back(City(X, Y, I));
	readFile.close();
	
	for(int i = 0; i < cityList.size(); ++i)
		for(int j = 0; j < cityList.size(); ++j)
			Distance[i][j] = sqrt(pow(cityList.at(i).x - cityList.at(j).x, 2) + pow(cityList.at(i).y - cityList.at(j).y, 2));
	
	double averageDist = 0.0;
	int runSize = stoi(argv[2]), iteration = stoi(argv[3]), antSize = stoi(argv[4]);
	bool outputEachRun = string(argv[5]) == "y", rewritefile = string(argv[6]) == "y", outputRunTime = string(argv[7]) == "y";
	
	for(int run = 0; run < runSize; ++run){
		//initialization
		for(int i = 0; i < cityList.size(); ++i)
			for(int j = i+1; j < cityList.size(); ++j)
				pheromones[i][j] = pheromones[j][i] = 1/Distance[i][j];
		//each ant construct solution
		Ant bestAnt;
		for(int i = 0; i < iteration; ++i){
			Ant AntColony[antSize];
			for(int j = 0; j < antSize; ++j)
				AntColony[j].constructSol(cityList);
			//undate pheromones and find best ant
			bestAnt = undatePheromones(cityList.size(), AntColony, antSize);
		}
		//output answer
		if(outputEachRun || run == runSize - 1){
			cout << "Run " << run + 1 << endl;
			bestAnt.outputSol(cityList.size());
		}
		averageDist += bestAnt.dist;
		if(rewritefile && run == runSize - 1)
			bestAnt.outputTofile(cityList.size(), string(argv[1]));
	}
	
	cout << "Average Distance:" << averageDist/runSize << endl;
	
	end = clock();
	if(outputRunTime)
		cout << "Total cost:" << (end - start)/CLOCKS_PER_SEC << endl;
	
	return 0;
}
