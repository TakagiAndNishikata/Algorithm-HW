#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cfloat>
using namespace std;

class City{
	public:
		int x, y, index;
		City(int X, int Y, int I){
			x = X, y = Y, index = I;
		};
		//use in next_permutation
		bool operator<(City& object){
			return index < object.index;
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
	
	//use next_permuation to get next permuation
	int permuationSol[cityList.size()];
	double minDist = DBL_MAX;
	do{
		//calulus first point and last point distance
		double dist = cityList[cityList.size() - 1] - cityList[0];
		//calulus follow point distance
		for(int i = 1; i < cityList.size(); ++i)
			dist += cityList[i] - cityList[i-1];
		//check current ans is smeller than min distance. if yes, replace than answer 
		if(dist < minDist){
			minDist = dist;
			for(int i = 0; i < cityList.size(); ++i)
				permuationSol[i] = cityList[i].index;
		}
	}while(next_permutation(cityList.begin() + 1, cityList.end()));
	
	//test file open
	ofstream outputFile(string(argv[1])+"/ans_"+string(argv[1])+"_HW1.txt");
	if(!outputFile){
		cerr << "File Error: create file name failed." << endl;
		return 1;
	}
	//output to file and terminal
	cout << "distance: " << minDist << endl;
	outputFile << "distance: " << minDist << endl;
	for(int i = 0; i < cityList.size(); ++i){
		cout << permuationSol[i] << endl;
		outputFile << permuationSol[i] << endl;
	}
	
	outputFile.close();
	return 0;
}
