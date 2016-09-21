#pragma once
#include <map>
#include <algorithm>
#include <vector>
using namespace std;

class genotype
{
private:
	vector<bool> genes;
	vector<vector<bool> > genetypes;
	

public:
	double fitness = -1;

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
			genes[(position) * 3 + i] = new_genes[i];
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

	double calcFitness()
	{
		double collisions = 0;
		if (fitness < 0){
			vector<int> configuration = decryptFromBool();
			for (int i = 0; i < 8; i++)
			{
				for (int o = i+1; o < 8; o++)
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
		else{
			return fitness;
		}
	}

};

struct less_than_key
{
	inline bool operator() (const genotype& genotype1, const genotype& genotype2)
	{
		return (genotype1.fitness < genotype2.fitness);
	}
};
