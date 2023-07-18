#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

class City{
	public:
		int x, y, index;
		City(int X, int Y, int I){
			x = X, y = Y, index = I - 1;
		};
		float operator-(City& object){
			return (float)sqrt(pow(x - object.x,2) + pow(y-object.y,2));
		};
};

float** dp;
vector<City> cityList;

float TSP(int currentCity, int states, int size){
	if(dp[currentCity][states] == -1)
		if(states == 0)
			dp[currentCity][0] = (cityList[currentCity]-cityList[0]);
		else
			for(int i = 0; i < size; ++i)
				if(i != currentCity && (1 << (i-1)) & states){
					float temp = TSP(i, states-(1<<(i-1)), size)+(cityList[currentCity]-cityList[i]);
					if(dp[currentCity][states] > temp || dp[currentCity][states] == -1)
						dp[currentCity][states] = temp;
				}
	
	return dp[currentCity][states];
}

int main(int argc, char *argv[]){
	if(argc != 2){
		cerr << "Input Error: Input arugement do not match." << endl;
		return 1;
	}
	ifstream readFile(string(argv[1])+"/point.txt");
	if(!readFile){
		cerr << "File Error: Not suck file name." << endl;
		return 1;
	}
	
	int X, Y, I;
	while(readFile >> I >> X >> Y)
		cityList.push_back(City(X, Y, I));
	readFile.close();	
	
	int size = cityList.size();
	
	dp = new float*[size];
	for(int i = 0; i < size; ++i){
		dp[i] = new float[(1<<(size-1))];
		for(int j = 0; j < (1<<(size-1)); ++j)
			dp[i][j] = -1;
	}
	
	float minDist=TSP(0, (1<<(size-1))-1,size);
	
	int permuationSol[size], states = (1<<(size-1))-1;
	permuationSol[0] = 0;
	for(int t= 1; t < size; ++t)
		for(int i = 1; i < size; ++i)
			if(dp[permuationSol[t-1]][states] == dp[i][states-(1<<(i-1))] + (cityList[i]-cityList[permuationSol[t-1]])){
				cout << t << " " << i << " " << dp[permuationSol[t-1]][states] == dp[i][states-(1<<(i-1))] << " " << dp[i][states-(1<<(i-1))] + (cityList[i]-cityList[permuationSol[t-1]]) << endl;
				states-=(1<<(i-1));
				permuationSol[t] = i;
				break;
			}
	cout << "distance: " << dp[0][(1<<(size-1))-1] << endl;
	for(int i = 0; i < size; ++i)
		cout << permuationSol[i]+1 << endl;
	for(int i = 0; i < size; ++i)
		delete dp[i];
	delete dp;
	return 0;
}
