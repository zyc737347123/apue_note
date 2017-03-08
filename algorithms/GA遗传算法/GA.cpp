#include<vector>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>
#include"GA.h"
	
using namespace std;


//srand((unsigned) time(NULL));

double rando()
{
	double randNum;
	randNum=rand()*1.0/RAND_MAX;
	return randNum;
}

GenAlg::GenAlg()
{

}



void GenAlg::init(int popsize, double MutRate, double CrossRate, int GenLenght,double LeftPoint,double RightPoint)

{

	popSize = popsize;

	mutationRate = MutRate;

	crossoverRate = CrossRate;

	chromoLength = GenLenght;

	totalFitness = 0;

	generation = 0;

	//fittestGenome = 0;

	bestFitness = 0.0;

	worstFitness = 99999999;

	averageFitness = 0;

	maxPerturbation=0.004;

	leftPoint=LeftPoint;

	rightPoint=RightPoint;


	vecPop.clear();

	for (int i=0; i<popSize; i++)

	{     


		vecPop.push_back(Genome());

		for (int j=0; j<chromoLength; j++)

		{
			
			vecPop[i].vecGenome.push_back(rando() * 

				(rightPoint - leftPoint) + leftPoint);

		}

	}

}

void GenAlg::Reset()
{
	totalFitness=0;
	//bestFitness=0;
    //worstFitness=9999;
	averageFitness=0;
	
}

void GenAlg::CalculateBestWorstAvTot()
{
	for (int i=0; i<popSize; ++i)

	{

		totalFitness+= vecPop[i].fitness;

		if(vecPop[i].fitness>=bestFitness)
		{
			bestFitness=vecPop[i].fitness;
			fittestGenome=vecPop[i];
		}

		if(vecPop[i].fitness<=worstFitness)
			worstFitness=vecPop[i].fitness;
		
	}

	averageFitness=totalFitness/popSize;



}



Genome GenAlg:: GetChromoRoulette()

{



	double Slice = (rando()) * totalFitness;


	Genome TheChosenOne;


	double FitnessSoFar = 0;


	for (int i=0; i<popSize; ++i)

	{


		FitnessSoFar += vecPop[i].fitness;


		if (FitnessSoFar >= Slice)

		{

			TheChosenOne = vecPop[i];

			break;

		}

	}

	

	return TheChosenOne;

}

void GenAlg::Mutate(vector<double> &chromo)
{	

	

	for (int i=0; i<chromo.size(); ++i)

	{

		

		if (rando() < mutationRate)

		{

			

			chromo[i] += ((rando()-0.5) * maxPerturbation);

			

			if(chromo[i] < leftPoint)

			{

				chromo[i] = rightPoint;

			}

			else if(chromo[i] > rightPoint)

			{

				chromo[i] = leftPoint;

			}

			

		}


	}
}


	
void GenAlg::Epoch(vector<Genome> &vecNewPop)

{

	

	vecPop = vecNewPop;

	

	Reset();

	

	CalculateBestWorstAvTot();

	

	vecNewPop.clear();       



	while (vecNewPop.size() < popSize)

	{

		

		Genome mum = GetChromoRoulette();

		Genome dad = GetChromoRoulette();


		vector<double> baby1, baby2;

		

		baby1 = mum.vecGenome;

		baby2 = dad.vecGenome;

		

		Mutate(baby1);

		Mutate(baby2);

		

		vecNewPop.push_back( Genome(baby1, 0) );

		vecNewPop.push_back( Genome(baby2, 0) );

	}

	

	if(vecNewPop.size() != popSize)

	{

		cout<<"error"<<endl;

		return;

	}

}

Genome GenAlg::GetBestFitness()
{
	return fittestGenome;
}

double GenAlg::GetAverageFitness()
{
	return averageFitness;
}

void GenEngine::report(const int&genNum)
{
	cout<<""<<genNum<<""<<endl;
    cout<<""<<bestFitness<<endl;
	cout<<""<<bestSearch<<endl;
	cout<<""<<averageFitness<<endl<<endl;
}


void GenEngine:: OnStartGenAlg()
	
{
	

	

	srand( (unsigned)time( NULL ) );

	

	genAlg.init(g_popsize, g_dMutationRate, g_dCrossoverRate, g_numGen,g_LeftPoint,g_RightPoint);

	

	m_population.clear();

	

	m_population = genAlg.vecPop;


	vector <double> input;
	double output;

	input.push_back(0);

	for(int Generation = 0;Generation <= g_Generation;Generation++)

	{

		
		for(int i=0;i<g_popsize;i++)

		{

			input = m_population[i].vecGenome;

			

			

			output = (double)curve.function(input);
		
			m_population[i].fitness = output;

		}


		genAlg.Epoch(m_population);

		
		//if(genAlg.GetBestFitness().fitness>=bestFitness)
		bestSearch=genAlg.GetBestFitness().vecGenome[0];
		bestFitness=genAlg.GetBestFitness().fitness;
		averageFitness=genAlg.GetAverageFitness();
		//cout<<bestSearch<<endl;
		report(Generation+1);
	}

	//return bestSearch;

}

