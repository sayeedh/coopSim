#ifndef SIM_H
#define SIM_H
#include<iostream>
#include<vector>
using namespace std;

#include "user.h"
#include "topo.h"
typedef struct a{
  int ns;
  int avgpunb;
  int avgsunb;
  double pupr;
  double supr;
}resultSet;

class Simulation{

 private:
  Topology *topo;
  vector<User *> userList;
  int curTime;
  int simTime;
  int nPUser;
  int nSUser;
  double prate;
  double srate;
  int chcond;
  int alg;
  double deadline;
  int qlen;
 public:
  Simulation(int t, int np, int ns, int nr, int nc, double pr, double sr, int cond, int algo, double dl, int ql);
  static Simulation* simInstance;
 protected:
  void createNodeList();
  void createNeighborList(bool);
  void generatePkts(double);
  void exchangeCtrlPkts(double);
  void sendDataPackets(double);
  void clearAndLog(double);
  resultSet show_results();
 public:
  static Simulation* getInstance(int t, int np, int ns, int nr, int nc, double pr, double sr, int cond, int algo, double dl, int ql);
  void init();
  resultSet run();
  ~Simulation();
  void clear();
};

#endif
