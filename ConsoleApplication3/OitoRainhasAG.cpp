// ConsoleApplication3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include "genotype.h"
#include <ctime>
#include <algorithm>
#include <math.h>
using namespace std;


double average_fitness(vector<genotype> genotypes)
{
	double sumFitness = 0;
	for (int i = 0; i < genotypes.size(); i++)
	{
		sumFitness += genotypes[i].fitness;
	}
	return sumFitness / genotypes.size();
}

vector<genotype> initTeacher()
{
	vector<genotype> populacao;

	for (int i = 0; i < 100; i++)
	{
		genotype tempGenotype;
		tempGenotype.initializeRandom();
		populacao.push_back(tempGenotype);
	}

	sort(populacao.begin(), populacao.end(), less_than_key());

	return populacao;
}

vector<genotype> initTeam()
{
	vector<genotype> populacao;

	for (int i = 0; i < 100; i++)
	{
		genotype tempGenotype;
		tempGenotype.initializeRandom();
		populacao.push_back(tempGenotype);
	}
		
	sort(populacao.begin(), populacao.end(), less_than_key_alt());

	return populacao;
}

double average_fitness_alt(vector<genotype> genotypes)
{
	double sumFitness = 0;
	for (int i = 0; i < genotypes.size(); i++)
	{
		sumFitness += genotypes[i].altFitness;
	}
	return sumFitness / genotypes.size();
}

double variance(vector<double> values, double average)
{
	double variance = 0;
	for (int i = 0; i < values.size(); i++)
	{
		variance += pow(values[i] - average,2);
	}
	return variance/values.size();
}

double standdevi(vector<double> values, double average){
	double stdrddev;
	stdrddev = variance(values, average);
	return sqrt(stdrddev);	
}

double average(vector<double> values)
{
	double sum = 0;
	for (int i = 0; i < values.size(); i++)
	{
		sum += values[i];
	}
	return sum / values.size();
}

int parentsRoullette(vector<genotype> genotypes, int ignoreThis=-1)
{
	double sumFitness = 0;
	for (int i = 0; i < genotypes.size(); i++)
	{
		if (i!= ignoreThis)
		{
			sumFitness += genotypes[i].altFitness;
		}		
	}
	int sum = (int) ceil (1000 * sumFitness);
	int max = rand() % sum;
	int index = 0;

	for (int i = 0; i < genotypes.size(); i++)
	{
		if (index != ignoreThis)
		{
			max -= ceil(1000 * genotypes[index].altFitness);
			if (max <= 0){
				index = i;
				break;
			}
		}
	}
	return index;
}

void team_method(){
	//Em quantas execuções o algoritmo convergiu (no/30 execuções);
	//Em que iteração o algoritmo convergiu (média e desvio padrão);
	vector<double> convergedIteraction;

	//Número de indivíduos que convergiram por execução;
	vector<double> numberOfConverged;

	//Fitness médio alcançado nas 30 execuções (média e desvio padrão);
	vector<double> avgFitnesses;

	//Quantas iterações são necessárias para toda a população convergir?
	vector<double> allConverge;

	for (int experimento = 0; experimento < 30; experimento++)
	{
		//cout << "Exec " << experimento << endl;

		bool converged = false;
		vector<genotype> populacao = initTeam();

		for (int i = 0; i < ((2+3)*1000)+1; i++)
		{
			int annealing = 100;
			double averageFitAlt = average_fitness_alt(populacao);
		

			if (populacao[0].altFitness == 1 && !converged)
			{
				convergedIteraction.push_back(i);

				int convNumber = 0;
				for (int i = 0; i < populacao.size(); i++)
				{ 
					if (populacao[i].altFitness == 1){
						convNumber++;
					}
					else{
						break;
					}
				}
				numberOfConverged.push_back(convNumber);

				avgFitnesses.push_back(averageFitAlt);

				converged = true;
			}

			if (averageFitAlt == 1){
				allConverge.push_back(i);
				break;
			}

			vector<genotype> filhos;
			vector<genotype> primeirosN;
			int nPais = 30;
			for (int o = 0; o < nPais; o++){
				primeirosN.push_back(populacao[o]);
			}
			for (int i = 0; i < 10; i += 2){
				int parent1 = parentsRoullette(primeirosN);
				int parent2 = parentsRoullette(primeirosN, parent1);
				while (parent1>nPais || parent2>nPais){
					parent1 = parentsRoullette(primeirosN);
					parent2 = parentsRoullette(primeirosN, parent1);
				}

				if (rand() % 10 < 9){
					vector<genotype> filhoTemp;
					filhoTemp = primeirosN[parent1].cutAndCrossfill(primeirosN[parent2]);
					filhos.push_back(filhoTemp[0]);
					filhos.push_back(filhoTemp[1]);
				}
				else{
					filhos.push_back(primeirosN[parent1]);
					filhos.push_back(primeirosN[parent2]);
				}
			}


			for (int i = 0; i < filhos.size(); i++)
			{
				if (rand() % 100 < annealing){
					filhos[i].swapGenesAlt();
				}
			}

			if (annealing > 40)
			{
				annealing--;
			}


			for (int i = 0; i < filhos.size(); i++)
			{
				populacao.push_back(filhos[i]);
			}


			sort(populacao.begin(), populacao.end(), less_than_key_alt());
			populacao.resize(100);
		}
	}

	double conversionRatio = convergedIteraction.size() / 30;
	double avgConv = average(convergedIteraction);
	double stdrdConv = standdevi(convergedIteraction, avgConv);
	double avgConvIndi = average(numberOfConverged);
	double stdrdConvIndi = standdevi(numberOfConverged,avgConvIndi);
	double avgFitnessOnAllExec = average(avgFitnesses);
	double stdrdFitn = standdevi(avgFitnesses, avgFitnessOnAllExec);
	double allConvAvg = average(allConverge);
	double allConvStdrd = standdevi(allConverge, allConvAvg);

	cout << "Test Results [MODEL2]:" << endl;
	cout << "Conversion/Executions Ratio: " << conversionRatio << endl;
	cout << "Conversion Iteractions [AVERAGE: " << avgConv << " STANDARD DEVIATION: " << stdrdConv << "]" << endl;
	cout << "Individuals that Converged [AVERAGE: " << avgConvIndi << " STANDARD DEVIATION: " << stdrdConvIndi << "]" << endl;
	cout << "Fitness [AVERAGE: " << avgFitnessOnAllExec << " STANDARD DEVIATION: " << stdrdFitn << "]" << endl;
	cout << "All Converged Iteraction [AVERAGE: " << allConvAvg << " STANDARD DEVIATION: " << allConvStdrd << "]";
}

void teacher_method()
{
	//Em quantas execuções o algoritmo convergiu (no/30 execuções);
	//Em que iteração o algoritmo convergiu (média e desvio padrão);
	vector<double> convergedIteraction;

	//Número de indivíduos que convergiram por execução;
	vector<double> numberOfConverged;

	//Fitness médio alcançado nas 30 execuções (média e desvio padrão);
	vector<double> avgFitnesses;

	//Quantas iterações são necessárias para toda a população convergir?
	vector<double> allConverge;


	for (int experimento = 0; experimento < 30; experimento++)
	{
		//cout << "Execution " << experimento << " [MODEL1]" << endl << "Current Iteraction: ";

		bool converged = false;
		vector<genotype> populacao = initTeacher();
		for (int i = 0; i < (2 + 3 * 1000); i++)
		{
			//cout << i << ", ";
			double avgFitness = average_fitness(populacao);

			if (populacao[0].altFitness == 1 && !converged)
			{
				convergedIteraction.push_back(i);

				int convNumber = 0;
				for (int i = 0; i < populacao.size(); i++)
				{
					if (populacao[i].altFitness == 1){
						convNumber++;
					}
					else{
						break;
					}
				}
				numberOfConverged.push_back(convNumber);

				avgFitnesses.push_back(avgFitness);

				converged = true;
			}

			if (avgFitness == 1){
				allConverge.push_back(i);
				break;
			}

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
	}

	double conversionRatio = convergedIteraction.size() / 30;
	double avgConv = average(convergedIteraction);
	double stdrdConv = standdevi(convergedIteraction, avgConv);
	double avgConvIndi = average(numberOfConverged);
	double stdrdConvIndi = standdevi(numberOfConverged, avgConvIndi);
	double avgFitnessOnAllExec = average(avgFitnesses);
	double stdrdFitn = standdevi(avgFitnesses, avgFitnessOnAllExec);
	double allConvAvg = average(allConverge);
	double allConvStdrd = standdevi(allConverge, allConvAvg);

	cout << "Test Results [MODEL 1]:" << endl;
	cout << "Conversion/Executions Ratio: " << conversionRatio << endl;
	cout << "Conversion Iteractions [AVERAGE: " << avgConv << " STANDARD DEVIATION: " << stdrdConv << "]" << endl;
	cout << "Individuals that Converged [AVERAGE: " << avgConvIndi << " STANDARD DEVIATION: " << stdrdConvIndi << "]" << endl;
	cout << "Fitness [AVERAGE: " << avgFitnessOnAllExec << " STANDARD DEVIATION: " << stdrdFitn << "]" << endl;
	cout << "All Converged Iteraction [AVERAGE: " << allConvAvg << " STANDARD DEVIATION: " << allConvStdrd << "]";
}

int _tmain(int argc, _TCHAR* argv[])
{
	int test;
	srand(time(0));

	teacher_method();

	team_method();

	cin >> test;

}
