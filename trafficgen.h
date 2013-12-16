#ifndef TRAFFICGEN_H
#define TRAFFICGEN_H
#include<iostream>
//#include<random>
#define COND_GOOD   1
#define COND_MED    2
#define COND_POOR   3
#define INFTY 1000.0
#define COND_RELAY  4
class TrafficGen
{
 private:
  TrafficGen(){}
 public:
  TrafficGen* getinstance();
  static int generateNumber(double p);
  static double estimateTime(int cond, double tm, double deadline);
};
/*
class PoissonGen:public TrafficGen
{
 private:
  //	std::default_random_engine generator;
  //std::poisson_distribution<int> distribution;
  
 public:
 PoissonGen(){ }
  //inline int generatePacket(){return distribution(generator);}
  // inline int generatePacket(){return 0;}
  int generateNumber();
};
*/
#endif
