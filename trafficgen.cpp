#include "trafficgen.h"
#include <math.h>
#include<cstdlib>
using namespace std;

int TrafficGen::generateNumber(double param){
  double L = exp(-param);
  // cout << "L:" << L << endl;
  int k = 0;
  double p = 1;
  do {
      k++;
      p *= ((double)rand()/(RAND_MAX));
      //   cout << "p :"<< p << endl;
  } while( p > L);
  //cout << k-1 ;
  return (k-1);
}

double TrafficGen::estimateTime(int cond, double t, double dline)
{
  double xtime = INFTY;
  int range;
  switch(cond){
  case COND_GOOD:
    range = (int)(dline*3/4);
    xtime = rand()%range;
    break;
  case COND_MED:
    range = dline*2;
    xtime = rand()%range;
    break;
  case COND_POOR:
    break;
    range = dline *5/2;
    xtime = rand()%range;
    break;
  case COND_RELAY:
    range = dline/3; /* we need to change it */
    xtime= rand()%range;
    break;
  }
  return xtime;
}
/*
int main()
{
  TrafficGen *tr = new PoissonGen(2);
  tr->generateNumber();
  return 0;
}
*/
