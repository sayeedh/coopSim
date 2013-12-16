#ifndef TOPO_H
#define TOPO_H
#include<iostream>
#include<cstdlib>
#include<vector>
using namespace std;

#include "binsearch.h"

typedef struct{
  int x;
  int y;
}Loc;


class Topology{
  int sizex;
  int sizey;
  int pu;
  int su;
  Tree *t;
  double threshold;
  vector<Loc> nodeLocation;
 public:
  Topology(int r,int c,int p,int s);
  ~Topology();
  void  addLocation();
  void printLocation();
  bool isNeighbor(int, int);
 protected:
  void  addPULocation(int i);
  void addSULocation(int i);
  Loc getUserLocation(int i);
  static int randindex(int start, int end);

};
#endif
