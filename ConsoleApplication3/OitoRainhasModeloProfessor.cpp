// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "genotype.h"
#include <ctime>
#include <algorithm>
#define professor 0
using namespace std;


double mean_fitness(vector<genotype> genotypes)
{
	double sumFitness = 0;
	for (int i = 0; i < genotypes.size(); i++)
	{
		sumFitness += genotypes[i].fitness;
	}
	return sumFitness / genotypes.size();
}

double mean_fitness_alt(vector<genotype> genotypes)
{
	double sumFitness = 0;
	for (int i = 0; i < genotypes.size(); i++)
	{
		sumFitness += genotypes[i].altFitness;
	}
	return sumFitness / genotypes.size();
}


int _tmain(int argc, _TCHAR* argv[])
{

#if(professor)
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

	sort(populacao.begin(), populacao.end(), less_than_key_alt());

	for (int i = 0; i < (2+3*1000) && populacao[0].fitness<1.0; i++)
	{
		cout << "Geracao " << i << " [AVG FIT: " << mean_fitness(populacao) << "]\n";
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
	

	for (int i = 0; i < 3; i++)
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
#else
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

	sort(populacao.begin(), populacao.end(), less_than_key_alt());
	int annealing = 100;

	for (int i = 0; i < ((2 + 3) * 1000); i++)
	{
		double meanFit = mean_fitness_alt(populacao);
		cout << "Geracao " << i << " [MEAN FIT: " << meanFit << "]\n";
		if (meanFit == 1){
			break;
		}
		vector<genotype> filhos;
		vector<genotype> primeirosN;
		int nPais = 30;
		for (int o = 0; o < nPais; o++){
			primeirosN.push_back(populacao[o]);
		}
		for (int i = 0; i < nPais; i += 2){
			if (rand() % 10 == 9){
				vector<genotype> filhoTemp;
				filhoTemp = primeirosN[i].cutAndCrossfill(primeirosN[i+1]);
				filhos.push_back(filhoTemp[0]);
				filhos.push_back(filhoTemp[1]);
			}
			else{
				filhos.push_back(primeirosN[i]);
				filhos.push_back(primeirosN[i+1]);
			}
		}


		for (int i = 0; i < filhos.size(); i++)
		{
			for (int o = 0; o < 8; o++)
			{
				if (rand() % 100 < annealing){
					filhos[i].swapGenes(rand() % 8, rand() % 8);
				}
			}
		}
		
		if (annealing > 10)
		{
			annealing/=2;
		}
		

		for (int i = 0; i < 30; i++)
		{
			populacao.push_back(filhos[i]);
		}


		sort(populacao.begin(), populacao.end(), less_than_key_alt());
		populacao.resize(100);
	}


	for (int i = 0; i < 3; i++)
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
		cout << "Fitness of " << i + 1 << ":" << populacao[i].fitness << " \n";
	}

	cin >> test;

#endif

}
