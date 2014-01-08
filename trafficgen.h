#ifndef TRAFFICGEN_H
#define TRAFFICGEN_H

#include<iostream>
using namespace std;
#include "common.h"

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
