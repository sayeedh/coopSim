#ifndef USER_H
#define USER_H
#include<vector>
#include<queue>
using namespace std;

#include "trafficgen.h"
#include "topo.h"
#include "packet.h"
#include "stats.h"

// different states of a user
#define ST_IDLE    1
#define ST_DIRECT  2
#define ST_RELAY   3
#define ST_XMIT    4

// types of algorithm
#define ALG_PURE 1
#define ALG_WIN  2
#define ALG_NOCP 3

#define min(x,y) ((x < y) ? x : y)
#define max(x,y) ((x > y) ? x : y)



class User
{
 private:
  int userID;
  Loc userPos;
  double trafficRate;
  int coopAlgorithm;
  
 public:
  User(int i, double r, int alg){
    userID = i;
    trafficRate = r;
    state = ST_IDLE;
    coopAlgorithm = alg;
    coop_window = 1;
    cond = COND_GOOD;
  }
  User(int i, double r, int cnd, int alg):stats(){
    userID = i; 
    trafficRate = r; 
    state = ST_IDLE; 
    coopAlgorithm = alg;
    coop_window = 1;
    cond = cnd;
  }
  int getUserID()const{return userID;}
  double getTrafficRate()const{return trafficRate;}
  int getCoopAlgorithm()const{return coopAlgorithm;}
  void addNeighbor(User*);
  bool isOpp;
  inline  int numOfNeighbors(){return neighborList.size();}
  virtual void generateTraffic(double t);
  virtual double assessChnCondition(double t, double dl)=0;
  virtual void sendCtrlPkt(double a){}
  virtual void receiveCtrlPkt(Packet p){}
  virtual void sendReplyCtrlPkt(double a){}
  virtual void receiveReplyCtrlPkt(Packet p){}
  virtual void sendFinalDecisionPkt(){}
  virtual void receiveFinalDecisionPkt(Packet p){}
  virtual int sendData()=0;	
  virtual int recvData()=0;
  virtual void clearAndlog(double)=0;
  statistics show_results();
  virtual void stopstealing(){};
 protected:
  virtual Packet createCtrlPkt(double, double, double)=0;
  virtual double getAddTime(double curTime){return 0;}

  std:: vector<User*> neighborList;
  std:: deque<Packet*> que;
  std:: vector<Packet> CoopOffer;
  int coop_window;
  int state;
  int cond;
  double xcounter;
  statistics stats;
};

class PrimaryUser:public User
{
 public:
 PrimaryUser(int i, double r, int c, int a):User(i,r,c, a){whoisstealing = -1;}
  // PrimaryUser(int i, double r, int c, int alg):User(i,rate,alg){}
  double assessChnCondition(double, double);
  void receiveCtrlPkt(Packet);
  void sendReplyCtrlPkt(double);
  void receiveFinalDecisionPkt(Packet);
  int sendData();
  int recvData();
  void clearAndlog(double);
 protected:
  Packet createCtrlPkt(double, double, double);
  double getAddTime(double curTime);
  int whoisstealing;
};

class SecondaryUser:public User
{
 public:
  /* SecondaryUser(int i):User(i,0.1){
    cond = COND_RELAY;
    }*/
 SecondaryUser(int i, double rate, int alg):User(i,rate,alg){cond = COND_RELAY;}
  double assessChnCondition(double, double);
  void sendCtrlPkt(double);
  void receiveReplyCtrlPkt(Packet);
  virtual void sendFinalDecisionPkt();
  int sendData();
  int recvData();
  void clearAndlog(double);
  void stopstealing();
   protected:
  Packet createCtrlPkt(double, double, double);
  double getAddTime(double curTime);
};
#endif
