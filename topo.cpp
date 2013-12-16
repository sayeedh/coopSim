#include<iostream>
#include<cstdlib>
#include<vector>
using namespace std;
#include "topo.h"
#define DEBUG 0
Topology::Topology(int p,int s,int r,int c):nodeLocation(p+s)
{
    sizex = r;
    sizey = c;
    pu = p;
    su = s;
    srand(time(NULL));
    t = new Tree();
    
    /* distance between any corner and the mid point of the area*/
    threshold = (sizex-1)*(sizex-1)/4+(sizey-1)*(sizey-1)/4;
    t->insert((sizex-1)*(sizex-1)/2 + (sizey - 1)/2);
}

Topology:: ~Topology()
{
    delete t;
}

void Topology::printLocation()
{
  cout << "nodes absolute location" << endl;
  t->inorder();
}

void Topology::addLocation()
{
  for(int i=0;i<pu+su; i++){
    if(i<pu){
      addPULocation(i);
    }
    else {
      addSULocation(i);
    }
  }
}

bool Topology :: isNeighbor(int i,int j)
{
  double distance;
  distance = (nodeLocation[i].x - nodeLocation[j].x)*(nodeLocation[i].x - nodeLocation[j].x)+ (nodeLocation[i].y - nodeLocation[j].y)*(nodeLocation[i].y - nodeLocation[j].y);
  if(distance < threshold){
    return true;
  }
  return false;
}

void Topology::addPULocation(int id)
{
  int x, locx, locy;
  bool notLocated = true;
  while(notLocated){
    x = Topology::randindex(0,1);
    if(x){
      locx = Topology::randindex(0,1)==0? 0:sizex-1;
      locy = Topology::randindex(0,sizey-1);
     }
    else{
      locy = Topology::randindex(0,1) == 0? 0 : sizey -1;
      locx = Topology::randindex(0,sizex-1);
    }
    int value = locx*(sizex-1) + locy;
#if DEBUG
    cout << "value PU "<<id <<" Location: " << value << endl;
#endif
    if(t->search(value)== NULL){
      nodeLocation[id].x = locx;
      nodeLocation[id].y = locy;
#if DEBUG
      cout << "Primary user " <<id << "loc " << locx << " " << locy << endl;
#endif
      t->insert(value);
      notLocated = false;
    }
  }
}

void Topology::addSULocation(int id)
{
  int locx, locy;
  bool notLocated = true;
  while(notLocated){
    locx = Topology::randindex(1,sizex-2);
    locy = Topology::randindex(1,sizey-2);
    int value = locx*(sizex-1) + locy;
    if(t->search(value)==NULL){
      nodeLocation[id].x=locx;
      nodeLocation[id].y=locy;
      t->insert(value);
      notLocated =false;
#if DEBUG
      cout << "Secondary user " <<id << "loc " << locx << " " << locy << endl;
#endif

    }
  }
}

Loc Topology::getUserLocation(int in)
{
  return nodeLocation[in];
}

int Topology::randindex(int s, int e)
{

  int y = s+(int)(rand()%(e -s+1));
  return y;
}


