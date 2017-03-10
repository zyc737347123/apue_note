#include<apue.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>
#include "ga.h"
using namespace std;

double Random()
{
	double rate;
	rate  = rand()*1.0/RAND_MAX;
	return rate;
}

GenAlg::GenAlg()
{
}

void GenAlg::init(int psize,double Mutrate,double Crossrate,int genL,double Lp,double Rp)
{
	// srand(tiem(NULL));
	popsize = psize;
	mutationRate  = Mutrate;
	crossoverRate = Crossrate;
	chromolength = genL;
	totalF = 0;
	generation = 0;
	bestF = 0.0;
	worstF = 9999999;
	averageF = 0;
	maxP = 0.004;
	leftP = Lp;
	rightP = Rp;

	vecPop.clear();
	for(int i=0;i<popsize;i++){
		vecPop.push_back(Genome());
		for(int j=0;j<chromolength;j++){
			// 基因排列顺序是从左到右
			vecPop[i].vecGenome.push_back(Random()*(rightP-leftP)+leftP);
			//cout<<vecPop[i].vecGenome[0]<<endl;
		}
	}
}

void GenAlg::Reset()
{
	totalF=0;
	averageF=0;
}

void GenAlg::CalculateBestWorstAvTot()
{
	for(int i=0;i<popsize;i++){
		totalF += vecPop[i].fitness;
		if(vecPop[i].fitness >= bestF){
			bestF=vecPop[i].fitness;
			fittestG=vecPop[i];
		}

		if(vecPop[i].fitness <= worstF)
			worstF=vecPop[i].fitness;
	}
	averageF = totalF / popsize;
}

Genome GenAlg::GetChromoRoulette()
{
	double slice = Random() * totalF;
	Genome TheOne;
	double Sofar=0;
	for(int i=0;i<popsize;i++){
		Sofar += vecPop[i].fitness;
		if(Sofar >= slice){
			TheOne = vecPop[i];
			break;
		}
	}
	return TheOne;
}

void GenAlg::Mutate(vector<double> &chromo)
{
	for(int i=0;i<chromo.size();i++){
		if(Random()<mutationRate){
			chromo[i] += ((Random()-0.5) * maxP);

			if(chromo[i] < leftP){
				chromo[i] = rightP;
			}else if(chromo[i] > rightP){
				chromo[i] = leftP;
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
	while(vecNewPop.size() < popsize){
		Genome mum = GetChromoRoulette();
		Genome dad = GetChromoRoulette();
		vector<double> baby1,baby2;
		baby1 = mum.vecGenome;
		baby2 = dad.vecGenome;
		Mutate(baby1);
		Mutate(baby2);
		vecNewPop.push_back(Genome(baby1,0));
		vecNewPop.push_back(Genome(baby2,0));
	}

	if(vecNewPop.size() != popsize){
		cout<<"error: popsize is 2n+1\n";
		return;
	}
}

Genome GenAlg::GetBestFitness()
{
	return fittestG;
}

double GenAlg::GetAverageFitness()
{
	return averageF;
}

void GenEngine::report(const int &genNum)
{
	cout<<"第"<<genNum<<endl
		<<"最佳适应度："<<bestFitness<<endl
		<<"最佳适应度基因取值："<<bestSearch<<endl
		<<"平均适应度："<<averageFitness<<endl;
}

void GenEngine::OnstartGenAlg()
{
	srand(time(NULL));
	genAlg.init(g_popsize,g_dMutationRate,g_dCrossoverRate,g_numGen,g_LeftPoint,g_RightPoint);
	m_population.clear();
	m_population = genAlg.vecPop;
	vector<double> input;
	double output;
	input.push_back(0);
	for(int gen = 0; gen <= g_Generation;gen++){
		for(int i=0;i<g_popsize;i++){
			input = m_population[i].vecGenome;
			output = curve.function(input);
			//cout<<"in:"<<input[0]<<" out: ";
			//cout<<output<<endl;
			m_population[i].fitness = output;
		}

		genAlg.Epoch(m_population);
		bestSearch=genAlg.GetBestFitness().vecGenome[0];
		bestFitness=genAlg.GetBestFitness().fitness;
		averageFitness=genAlg.GetAverageFitness();
		report(gen+1);
	}
}
