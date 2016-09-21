// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "genotype.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int test; 
	//Inicializacao das amostras
	vector<genotype> populacao;
	
	for (int i = 0; i < 100; i++)
	{
		genotype tempGenotype;
		tempGenotype.initializeRandom();
		populacao.push_back(tempGenotype);
	}

	sort(populacao.begin(), populacao.end(), less_than_key());


	for (int i = 0; i < populacao.size(); i++)
	{
		vector<int> boolConfig = populacao[i].boolGenotype();
		for (int o = 0; o < boolConfig.size(); o++){
			cout << boolConfig[o] << " ";
		}
		cout << "\n";
		vector<int> intConfig = populacao[i].decryptFromBool();
		for (int o = 0; o < intConfig.size(); o++){
			cout << intConfig[o] << " ";
		}
		cout << "\n";
		cout << "Fitness of " << i+1 << ":" << populacao[i].fitness << " \n";
	}

	cin >> test;
}

