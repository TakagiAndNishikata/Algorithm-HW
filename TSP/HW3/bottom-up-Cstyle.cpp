//No Better QQ
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

int main(int argc, char *argv[]){
	if(argc != 2){
		cerr << "Input Error: Input arugement do not match." << endl;
		return 1;
	}
	ifstream readFile(string(argv[1])+"/point.txt");
	if(!readFile){
		cerr << "File Error: Not such file name." << endl;
		return 1;
	}
	
	int X, Y, I, x[25], y[25], size = 0;
	for(int i = 0; readFile >> I >> X >> Y;++size, ++i)
		x[i] = X, y[i] = Y;
	readFile.close();	
	
	float** dp;
	dp = new float*[size];
	for(int i = 0; i < size; ++i){
		dp[i] = new float[(1<<(size-1))];
		for(int j = 0; j < (1<<(size-1)); ++j)
			dp[i][j] = -1;
	}
	
	float dist[size][size];
	for(int i = 0; i < size; ++i){
		for(int j = i+1; j < size; ++j){
			dist[i][j] = dist[j][i] = (float)sqrt(pow(x[i] - x[j],2) + pow(y[i]-y[j],2));
		}
	}
	
	
	for(int currentCity = 0; currentCity < size; ++ currentCity){
		dp[currentCity][0] = dist[currentCity][0];
	}
	
	for(int states = 1; states < (1<<(size-1)); ++states){
		for(int currentCity = 1; currentCity < size; ++currentCity){
			for(int i = 1; i < size; ++i){
				if((1 << (i-1)) & states && (dp[currentCity][states] > dp[i][states-(1<<(i-1))] + dist[currentCity][i] || dp[currentCity][states] == -1)){
					dp[currentCity][states] = dp[i][states-(1<<(i-1))] + dist[currentCity][i];
				}
			}
		}
	}
	
	for(int i = 1; i < size; ++i){
		if(dp[0][(1<<(size-1))-1] > dp[i][(1<<(size-1))-1-(1<<(i-1))] + dist[i][0] || i == 1){
			dp[0][(1<<(size-1))-1] = dp[i][(1<<(size-1))-1-(1<<(i-1))] + dist[i][0];
		}
	}
	
	int permuationSol[size], states = (1<<(size-1))-1;
	permuationSol[0] = 0;
	for(int t= 1; t < size; ++t){
		for(int i = 1; i < size; ++i){
			if(dp[permuationSol[t-1]][states] == dp[i][states-(1<<(i-1))] + dist[i][permuationSol[t-1]]){
				states-=(1<<(i-1));
				permuationSol[t] = i;
				break;
			}
		}
	}
	
	ofstream outputFile(string(argv[1])+"/ans_"+string(argv[1])+"_HW3.txt");
	if(!outputFile){
		cerr << "File Error: create file name failed." << endl;
		return 1;
	}
	
	cout << "distance: " << dp[0][(1<<(size-1))-1] << endl;
	outputFile << "distance: " << dp[0][(1<<(size-1))-1] << endl;
	for(int i = 0; i < size; ++i){
		cout << permuationSol[i]+1 << endl;
		outputFile << permuationSol[i]+1 << endl;
	}
	
	outputFile.close();
	
	for(int i = 0; i < size; ++i)
		delete dp[i];
	delete dp;
	
	return 0;
}
