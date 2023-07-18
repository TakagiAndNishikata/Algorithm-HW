#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cfloat>
using namespace std;

class City{
	public:
		int x, y, index;
		City(int X, int Y, int I){
			x = X, y = Y, index = I;
		};
		//Use minus operator to calulus distance
		double operator-(City& object){
			return sqrt(pow(x - object.x,2) + pow(y-object.y,2));
		};
};

int main(int argc, char *argv[]){
	//test for input message
	if(argc != 2){
		cerr << "Input Error: Input arugement do not match." << endl;
		return 1;
	}
	//test file open
	ifstream readFile(string(argv[1])+"/point.txt");
	if(!readFile){
		cerr << "File Error: Not suck file name." << endl;
		return 1;
	}
	
	int X, Y, I;
	vector<City> cityList;
	//read data
	while(readFile >> I >> X >> Y)
		cityList.push_back(City(X, Y, I));
	readFile.close();
	
	//recorder answer and distance
	int Sol[cityList.size()];
	double minDist = DBL_MAX;
	for(int start = 0; start < cityList.size(); ++start){
		//test all node to be start
		bool visited[cityList.size()] = {false};
		visited[start] = true;
		double dist = 0.0;
		int tempSol[cityList.size()], next = start;
		for(int t = 0; t < cityList.size(); ++t){
			//try n time to trival each node
			double temp = DBL_MAX;
			int buf;
			for(int i = 0 ; i < cityList.size(); ++i){
				//find the closest point
				if(next != i && !visited[i] && cityList.at(next) - cityList.at(i) < temp){
					temp = cityList.at(next) - cityList.at(i);
					buf = i;
				}
			}
			//recorder the colosest point and assign it as next start
			dist += cityList.at(next) - cityList.at(buf);
			visited[buf] = true;
			tempSol[t] = next+1;
			next = buf;
		}
		dist += cityList.at(start) - cityList.at(tempSol[cityList.size()-1]-1);
		//test current start is (or not) better than answer
		if(minDist > dist){
			minDist = dist;
			for(int i = 0; i < cityList.size(); ++i){
				Sol[i] = tempSol[i];
			}
		}
	}
	
	//test file open
	ofstream outputFile(string(argv[1])+"/ans_"+string(argv[1])+"_HW2.txt");
	if(!outputFile){
		cerr << "File Error: create file name failed." << endl;
		return 1;
	}
	//output to file and terminal
	cout << "distance: " << minDist << endl;
	outputFile << "distance: " << minDist << endl;
	for(int i = 0; i < cityList.size(); ++i){
		cout << Sol[i] << endl;
		outputFile << Sol[i] << endl;
	}
	
	outputFile.close();
	
	return 0;
}
