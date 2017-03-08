//#include<apue.h>
#include<math.h>
#include<iostream>
#include<vector>
using namespace std;

const double pai = 3.1415926;

class Genome
{
	private:
		vector<double> vecGenome;
		double fitness; // 个体适应度
	public:
		friend class GenAlg;
		friend class GenEngine;
		Genome():fitness(0){}
		Genome(vector<double> vec,double f):vecGenome(vec),fitness(f){}
};

class GenAlg
{
	public:
		vector<Genome> vecPop;
		int popsize;
		int chromolength; // 基因长度
		double totalF;
		double bestF;
		double averageF;
		double worstF;
		Genome fittestG;
		double mutationRate; // 基因突变
		double crossoverRate; // 基因交叉
		int generation;
		double maxP; // 最大变异步长
		double leftP;
		double rightP;

		GenAlg();
		void Reset();
		void init(int size,double MutRate,double CrossRate,int genomeL,double LP,double RP);
		void CalculateBestWorstAvTot(); // 统计，记录数据函数
		Genome GetChromoRoulette(); // 轮盘选择函数
		void Mutate(vector<double> &chrome); //基因突变函数
		void Epoch(vector<Genome> &vecNewPop);
		Genome GetBestFitness(); // 返回对应的最优个体
		double GetAverageFitness();
};

class Curve // 曲线
{
	public:
		double function(vector<double> &input)
		{
			// input 为一个个体
			double x=input[0];
			double output;
			output = x*(sin(10*pai*x))+2;
			return output;
		}
};

class GenEngine
{
	private:
		GenAlg genAlg;
		Curve curve;
		vector<Genome> m_population;  // 种群
		int g_popsize;
		double g_dMutationRate;
		double g_dCrossoverRate;
		int g_numGen;
		int g_Generation;
		double g_LeftPoint;
		double g_RightPoint;
		double bestFitness;
	    double bestSearch;
		double averageFitness;

	public:
		GenEngine(const int& popsize,const double& mutationRate,const double& crossoverRate,const int&numGen,const int&generation
						,const double& leftPoint, const double& rightPoint):genAlg(),curve(),m_population()
		{
			g_popsize = popsize;
			g_dMutationRate = mutationRate;
			g_dCrossoverRate = crossoverRate;
			g_numGen = numGen;
			g_Generation = generation;
			g_LeftPoint = leftPoint;
			g_RightPoint = rightPoint;
			bestFitness = 0;
			bestSearch = 0;
		}

		void OnstartGenAlg();

		void report(const int &genNum);

};
