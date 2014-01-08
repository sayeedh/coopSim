#include<iostream>
using namespace std;

#include "sim.h"

Simulation* Simulation::simInstance = 0;

Simulation:: Simulation(int t, int np, int ns, int nr, int nc, double pr, double sr, int cond, int algo, double dl, int ql)
   :userList(np+ns){
    simTime = t;
    nPUser = np;
    nSUser = ns;   
    prate = pr;
    srate = sr;
    chcond = cond;
    alg = algo;
    topo = new Topology(np, ns, nr, nc);
    deadline  = dl;
    qlen = ql;	
}


Simulation* Simulation:: getInstance(int t, int np, int ns, int nr, int nc, double pur, double sur, int cond, int alg,double dl, int ql){
  if (simInstance == NULL){
    simInstance = new Simulation(t, np, ns, nr, nc, pur, sur, cond, alg,dl,ql);
  }
  return simInstance;	
} 

void Simulation::createNodeList()
{
    int i, nUser;
    nUser = nPUser + nSUser;
    for(i = 0;i<nPUser;i++){
      userList[i] = (User *) new PrimaryUser(i,prate,chcond,alg,deadline,qlen);
    }    
    for(;i<nUser;i++){
      userList[i] = (User *) new SecondaryUser(i,srate,alg,deadline,qlen);
    }
}

Simulation::~Simulation()
{
    int i;
    for(i=0;i<nPUser+nSUser;i++){
        if(userList[i] != NULL){
            delete userList[i];
        }
    }
    userList.clear();
    if(topo)
	delete topo;	
}

void Simulation::createNeighborList(bool isPrint = false)
{
  int nU = nPUser + nSUser;
  for(int i=0;i<nPUser;i++){
    for(int j=nPUser;j<nU;j++){
      if(topo->isNeighbor(i,j)){
	if(!isPrint){
	  userList[i]->addNeighbor(userList[j]);
	  userList[j]->addNeighbor(userList[i]);
	}
        //	cout <<"user " << i <<": user " << j<< endl;
      }
    }
  }
}

void Simulation::generatePkts(double t)
{
    int nU = nPUser + nSUser;
    for(int i=0;i<nU;i++){
        userList[i]->generateTraffic(t);
    }
    //    cout <<"At time " << t <<", packets generated" << endl;
}
void Simulation :: exchangeCtrlPkts(double t)
{
    int nU = nPUser + nSUser;
    for(int i = 0; i < nU; i++){
        userList[i]->sendCtrlPkt(t);
    }
    // cout <<"At time " << t <<", ctrl packets sent" << endl;
    for(int i =0;i<nU;i++){
        userList[i]->sendReplyCtrlPkt(t);
    }
    //  cout <<"At time " << t <<", reply packets sent" << endl;
    for(int i =0;i<nU; i++){
        userList[i]->sendFinalDecisionPkt();
    }
    // cout <<"At time " << t <<", decision packets sent" << endl;
}


void Simulation :: sendDataPackets(double t)
{
    int nU = nPUser + nSUser;
    for(int i=0;i<nU;i++){
        userList[i]->sendData();
    }
    // cout <<"At time " << t <<", data packets sent" << endl;
}

void Simulation :: clearAndLog(double curTime)
{
    int nU = nPUser + nSUser;
    for(int i=0;i<nU;i++){
        userList[i]->clearAndlog(curTime);
    }
    //  cout <<"At time " << curTime <<", buffer cleared and result logged" << endl;
}

resultSet Simulation :: show_results()
{
  int i, nU = nPUser + nSUser, ns=0;
  statistics pufinalstats;
  statistics sufinalstats;
  resultSet res;
  pufinalstats.nDirPackets  = 0;
  pufinalstats.nGPackets    = 0;
  pufinalstats.nDropPackets = 0;
  pufinalstats.nRePackets   = 0;

  sufinalstats.nDirPackets  = 0;
  sufinalstats.nGPackets    = 0;
  sufinalstats.nDropPackets = 0;
  sufinalstats.nRePackets   = 0;
  
  //  cout << "result summary" << endl;
  // print the neighborlist
  // createNeighborList(true);
  int avgpunb = 0, avgsunb = 0;

  for(i=0;i<nPUser;i++){
    avgpunb +=userList[i]->numOfNeighbors();
    statistics s = userList[i]->show_results();
    pufinalstats.nDirPackets  += s.nDirPackets;
    pufinalstats.nGPackets    += s.nGPackets;
    pufinalstats.nDropPackets += s.nDropPackets;
    pufinalstats.nRePackets   += s.nRePackets;
  }
  avgpunb = avgpunb/nPUser;
  for(;i<nU;i++){
    if(userList[i]->numOfNeighbors()){
      ns++;
      avgsunb += userList[i]->numOfNeighbors();
      statistics s = userList[i]->show_results();
      sufinalstats.nDirPackets  += s.nDirPackets;
      sufinalstats.nGPackets    += s.nGPackets;
      sufinalstats.nDropPackets += s.nDropPackets;
      sufinalstats.nRePackets   += s.nRePackets;
     
    }
  }
  if(ns){
      avgsunb = avgsunb/ns;
  }
  double pri, su =0;
  pri = (double)(pufinalstats.nDirPackets + pufinalstats.nRePackets)/pufinalstats.nGPackets;
  if (ns){
    su = (double)(sufinalstats.nDirPackets + sufinalstats.nRePackets)/sufinalstats.nGPackets;
  }
  //  cout << pri << " "<< su << endl;
  //  cout <<"end of simulation" << endl;
  res.ns = ns;
  res.avgpunb = avgpunb;
  res.avgsunb = avgsunb;
  res.pupr = pri;
  res.supr = su;
  return res;
  
}
void Simulation::init()
{
  createNodeList();	
  topo->addLocation();
  createNeighborList();
}

    
resultSet Simulation :: run()
{
    // topo = new GridTopology(nrows, ncols, pu+su);
    // place the nodes in the simulation area	
    // topo->addlocation();
    // compute the neighbors list	
    int curTime;
    for (curTime = 0; curTime < simTime; curTime++){
      generatePkts(curTime);
      exchangeCtrlPkts(curTime);
      sendDataPackets(curTime);
      clearAndLog(curTime + 1);
    }
    return show_results();

}

void Simulation::clear(){
  simInstance = 0;
}
