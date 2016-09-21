#pragma once
#include <map>
#include <algorithm>
#include <vector>
#include <ctime>
using namespace std;

class genotype
{
private:
	vector<bool> genes;
	vector<vector<bool> > genetypes;


public:
	double fitness = -1;
	double altFitness = -1;

	genotype(){
		//Inicializando os genes//
		vector<bool> gene;
		gene.push_back(false); gene.push_back(false); gene.push_back(false);
		genetypes.push_back(gene); gene.clear();
		gene.push_back(false); gene.push_back(false); gene.push_back(true);
		genetypes.push_back(gene); gene.clear();
		gene.push_back(false); gene.push_back(true); gene.push_back(false);
		genetypes.push_back(gene); gene.clear();
		gene.push_back(false); gene.push_back(true); gene.push_back(true);
		genetypes.push_back(gene); gene.clear();
		gene.push_back(true); gene.push_back(false); gene.push_back(false);
		genetypes.push_back(gene); gene.clear();
		gene.push_back(true); gene.push_back(false); gene.push_back(true);
		genetypes.push_back(gene); gene.clear();
		gene.push_back(true); gene.push_back(true); gene.push_back(false);
		genetypes.push_back(gene); gene.clear();
		gene.push_back(true); gene.push_back(true); gene.push_back(true);
		genetypes.push_back(gene);

		//Inicializando genotipo//
		for (int i = 0; i < 24; i++)
		{
			genes.push_back(false);
		}

	};

	vector<bool> getConfiguration(){
		return genes;
	}

	void setPosition(int position, vector<bool> new_genes)
	{
		for (int i = 0; i < 3; i++)
		{
			genes[(position)* 3 + i] = new_genes[i];
		}
	}

	vector<int> decryptFromBool(){
		vector<int> inInt;
		for (int i = 0; i < 8; i++)
		{
			int valor = 0;
			if (genes[i * 3 + 2] == true)
			{
				valor += 1;
			}
			if (genes[i * 3 + 1] == true){
				valor += 2;
			}
			if (genes[i * 3] == true){
				valor += 4;
			}
			inInt.push_back(valor);
		}
		return inInt;
	}

	vector<int> boolGenotype(){
		vector<int> inInt;
		for (int i = 0; i < 24; i++)
		{
			if (genes[i] == true){
				inInt.push_back(1);
			}
			else{
				inInt.push_back(0);
			}
		}
		return inInt;
	}

	void initializeRandom()
	{
		int order[] = { 0, 1, 2, 3, 4, 2 + 3, 2 * 3, 7 };
		random_shuffle(order, order + 7);
		for (int i = 0; i < 8; i++)
		{
			setPosition(i, genetypes[order[i]]);
		}
		calcFitness();
	}

	void swapGenes(int position1, int position2)
	{
		vector<bool> tempGene;
		for (int i = 0; i < 3; i++)
		{
			tempGene.push_back(genes[(position1 * 3) + i]);
		}
		for (int i = 0; i < 3; i++)
		{
			genes[(position1 * 3) + i] = genes[(position2 * 3) + i];
			genes[(position2 * 3) + i] = tempGene[i];
		}
		calcFitness();
	}

	vector<genotype> cutAndCrossfill(genotype other)
	{

		genotype offspring1, offspring2;
		vector<bool> parent1, parent2;
		parent1 = getConfiguration();
		parent2 = other.getConfiguration();

		int cutPoint = 1 + rand() % (7 - 1);
		for (int i = 0; i < cutPoint; i++){
			for (int o = 0; o < 3; o++)
			{
				offspring1.setSpecificGene(i * 3 + o, parent2[i * 3 + o]);
				offspring2.setSpecificGene(i * 3 + o, parent1[i * 3 + o]);
			}
		}

		vector<int> existInC1, existInC2;
		vector<int> configC1, configC2, configC3, configC4;
		configC1 = offspring1.decryptFromBool();
		configC2 = offspring2.decryptFromBool();
		configC3 = decryptFromBool();
		configC4 = other.decryptFromBool();


		configC1.resize(cutPoint);
		configC2.resize(cutPoint);

		for (int i = cutPoint; i < 8; i++)
		{
			for (int o = 0; o < 8; o++)
			{
				if (find(configC1.begin(), configC1.end(), configC3[o]) == configC1.end()){
					configC1.push_back(configC3[o]);
				}
			}
			for (int o = 0; o < 8; o++)
			{
				if (find(configC2.begin(), configC2.end(), configC4[o]) == configC2.end()){
					configC2.push_back(configC4[o]);
				}
			}
		}

		for (int i = 0; i < 8; i++)
		{
			offspring1.setPosition(i, genetypes[configC1[i]]);
			offspring2.setPosition(i, genetypes[configC2[i]]);
		}

		vector<genotype> returnedOffsprings;
		offspring1.calcFitness();
		offspring2.calcFitness();


		returnedOffsprings.push_back(offspring1);
		returnedOffsprings.push_back(offspring2);

		return returnedOffsprings;
	}

	bool getSpecificGene(int position)
	{
		return genes[position];
	}

	void setSpecificGene(int position, bool state)
	{
		genes[position] = state;
	}

	double calcFitness()
	{
		calcFitnessAlt();
		double collisions = 0;
		vector<int> configuration = decryptFromBool();
		for (int i = 0; i < 8; i++)
		{
			for (int o = i + 1; o < 8; o++)
			{
				if ((configuration[i] == configuration[o] + (o - i)) || (configuration[i] == configuration[o] + (i - o)))
				{
					collisions++;
				}
			}
		}
		fitness = 1 / (1 + collisions);
		return fitness;
	}

	double calcFitnessAlt()
	{
		double longestStreakNoCollision = 0;
		double noCollision = 0;
		vector<int> configuration = decryptFromBool();
		for (int i = 0; i < 8; i++)
		{
			for (int o = i + 1; o < 8; o++)
			{
				if ((configuration[i] == configuration[o] + (o - i)) || (configuration[i] == configuration[o] + (i - o)))
				{
					noCollision = 0;
				}
			}
			noCollision++;
			if (longestStreakNoCollision < noCollision)
			{
				longestStreakNoCollision = noCollision;
			}
		}
		altFitness = longestStreakNoCollision/8;
		return altFitness;
	}
};

struct less_than_key
{
	inline bool operator() (const genotype& genotype1, const genotype& genotype2)
	{
		return (genotype1.fitness > genotype2.fitness);
	}
};

struct less_than_key_alt
{
	inline bool operator() (const genotype& genotype1, const genotype& genotype2)
	{
		return (genotype1.altFitness > genotype2.altFitness);
	}
};
