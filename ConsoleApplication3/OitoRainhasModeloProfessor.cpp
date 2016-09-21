// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "genotype.h"
#include <ctime>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int test;
	srand(time(0));
	//Inicializacao das amostras
	vector<genotype> populacao;

	for (int i = 0; i < 100; i++)
	{
		genotype tempGenotype;
		tempGenotype.initializeRandom();
		populacao.push_back(tempGenotype);
	}

	vector<genotype> filhos = populacao[0].cutAndCrossfill(populacao[1]);

	populacao.push_back(filhos[0]);
	populacao.push_back(filhos[1]);

	for (int i = 0; i < 700 || populacao[99].fitness==1; i++)
	{
		cout << "Geracao " << i << "\n";
		vector<genotype> filhos;
		vector<genotype> cincoAleatorios;
		for (int o = 0; o < 2 + 3; o++){
			cincoAleatorios.push_back(populacao[rand() % 100]);
		}
		if (rand() % 10 == 9){
			sort(cincoAleatorios.begin(), cincoAleatorios.end(), less_than_key());
			filhos = cincoAleatorios[0].cutAndCrossfill(cincoAleatorios[1]);		
		}
		else{
			filhos.push_back(cincoAleatorios[0]);
			filhos.push_back(cincoAleatorios[1]);
		}

		
		for (int i = 0; i < 2; i++)
		{
			for (int o = 0; o < 8; o++)
			{
				if (rand() % 10 < 4){
					filhos[i].swapGenes(rand() % 8, rand() % 8);
				}
			}		
		}

		populacao.push_back(filhos[0]);
		populacao.push_back(filhos[1]);

		sort(populacao.begin(), populacao.end(), less_than_key());
		populacao.resize(100);
	}
	

	for (int i = 0; i < 100; i+=10)
	{
		/*vector<int> boolConfig = populacao[i].boolGenotype();
		for (int o = 0; o < boolConfig.size(); o++){
			cout << boolConfig[o] << " ";
		}
		cout << "\n";*/
		vector<int> intConfig = populacao[i].decryptFromBool();
		for (int o = 0; o < intConfig.size(); o++){
			cout << intConfig[o] << " ";
		}
		cout << "\n";
		cout << "Fitness of " << i+1 << ":" << populacao[i].fitness << " \n";
	}

	cin >> test;
}

