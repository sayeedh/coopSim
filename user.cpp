#include "user.h"

void User ::addNeighbor(User* u){
  if(u == this) return;
  neighborList.push_back(u);
}

void User :: generateTraffic(double curTime){
  int x = TrafficGen::generateNumber(getTrafficRate());
  
  stats.nGPackets += x;
  while(x>0){
    Packet *dataPkt = new Packet(curTime);
    dataPkt->setPktType(PKT_DATA);
    dataPkt->setSrc(getUserID());
    que.push_back(dataPkt);
    x--;
  }
}

double PrimaryUser :: assessChnCondition(double t, double dl){
  return TrafficGen::estimateTime(cond,t,dl);
}

double SecondaryUser :: assessChnCondition(double t, double dl){
  return TrafficGen::estimateTime(cond, t, dl);
}

/* secondary user always sends control packets to everyone in its neighborhood*/
void SecondaryUser :: sendCtrlPkt(double curTime){
  double xtime;
  if(state == ST_XMIT || que.empty()){
    return;
  }
  Packet *dataPkt = que.front();
  double dl = dataPkt->getDeadline();
  xtime = assessChnCondition(curTime, dl);
  
  // call all the neighbors receive control packet method and each one 
  // has its own copy.
  Packet ctrlPkt = createCtrlPkt(curTime, dl, xtime);
  if(ctrlPkt.isValid == false){
    return;
  }
  for(unsigned int i=0;i<neighborList.size();i++){
      ctrlPkt.setDest(neighborList[i]->getUserID());
    neighborList[i]->receiveCtrlPkt(ctrlPkt);
  }
  return;
}

void PrimaryUser :: receiveCtrlPkt(Packet p){
  if(state == ST_XMIT || state == ST_DIRECT || (que.empty() && getCoopAlgorithm()!= ALG_NOCP)){
    return;
  }
 CoopOffer.push_back(p);
}
/* primary user always sends reply to a neighbor secondary user */
void PrimaryUser :: sendReplyCtrlPkt(double t){
    if(state == ST_XMIT || state == ST_DIRECT ||(que.empty() && CoopOffer.empty()) ) {
    return;
  }
    if(getCoopAlgorithm() == ALG_NOCP && que.empty() && !CoopOffer.empty()){
      Packet oppPkt(0,PKT_OPP);
      oppPkt.setBidReq(0);
      oppPkt.setSrc(getUserID());
      int opps = rand()%CoopOffer.size();/*assign a random sec neighbor a free channel*/
      oppPkt.setDest(CoopOffer[opps].getSrcId());
      oppPkt.setBidOffer(CoopOffer[opps].getBidRequest());
      for(unsigned int i=0;i<neighborList.size();i++){
	if(oppPkt.getDestId()==neighborList[i]->getUserID()){
            neighborList[i]->receiveReplyCtrlPkt(oppPkt); /*this is always unicast*/
	}
      }
    return;
  }
    if(que.empty()) return;
  Packet *p = que.front();
  double dl = p->getDeadline();
  double xtime = assessChnCondition(t, dl);
  
  Packet reply = createCtrlPkt(t, dl, xtime);
  if (reply.isValid == false){
    return;
  }
  if(state == ST_DIRECT && 
     reply.getBidOffer()== 0 && reply.getBidRequest()==0){
    return;
  }

  for(unsigned int i=0;i<neighborList.size();i++){
    if(reply.getDestId()==neighborList[i]->getUserID()){
      neighborList[i]->receiveReplyCtrlPkt(reply);
    }
  }
  return;
}

void SecondaryUser :: receiveReplyCtrlPkt(Packet p){
 if(state == ST_XMIT || que.empty()){
   return;
  }
 CoopOffer.push_back(p);
}
/* secondary user sends final decision to a neighbor primary user */
void SecondaryUser :: sendFinalDecisionPkt(){
  double CGF = 100;
  int index=-1;
  if(state == ST_XMIT || que.empty() || CoopOffer.empty()){
   return;
  }
 
 for(unsigned int i=0;i<CoopOffer.size();i++){
   if(CGF > (1+CoopOffer[i].getBidOffer())/(1+CoopOffer[i].getBidRequest())){
     CGF = (1 + CoopOffer[i].getBidOffer())/(1 + CoopOffer[i].getBidRequest());
     index = i;
   }
 }
 for(unsigned int i = 0;i<neighborList.size();i++){
   if(CoopOffer[index].getSrcId() == neighborList[i]->getUserID()){
     xcounter = CoopOffer[index].getBidRequest()+CoopOffer[index].getBidOffer();
     state = ST_XMIT;
     neighborList[i]->receiveFinalDecisionPkt(CoopOffer[index]);
   }
 }
 return;
}
/* if it is a direct transmission, the control will not come here */
void PrimaryUser::receiveFinalDecisionPkt(Packet p){
  if(state == ST_XMIT || state == ST_DIRECT || (que.empty() && getCoopAlgorithm()!=PKT_OPP)){
    return;
  }
  /*if pkt_opp, first check whether i */
  if(p.getPktType() == PKT_OPP){
    whoisstealing = p.getSrcId();
    return;
  }  
  else{
      xcounter = p.getBidOffer() + p.getBidRequest();
      state = ST_XMIT;
  }
}
/* creates the reply packet for primary user based on packet type, algorithm and queue state, also selects the destination node */
Packet PrimaryUser :: createCtrlPkt(double t, double dl, double xtime){
  if(state == ST_XMIT || state == ST_DIRECT){
    return Packet();
  }
  if(que.empty() && getCoopAlgorithm() != ALG_NOCP){
    return Packet();
  }

  double remainingTime, addTime=0, offerTime, estimateTime;
  double viaxtime = 0, CGF = 0.0;
  int index=-1;
  double indexReqTime;
  bool isEligible;
  Packet replyPkt(0,PKT_CTRL);
  Packet *p = que.front();
  remainingTime = p->genTime() + p->getDeadline() - t;
  // the packet exceeds the timings
  if(remainingTime < 0){
    cout << "You should not be here" << endl;
  }
  if(getCoopAlgorithm()==ALG_WIN){
    addTime = getAddTime(t);
  }
  if(remainingTime >= xtime && getCoopAlgorithm()==ALG_PURE){
    state = ST_DIRECT;
    xcounter = xtime;
    replyPkt.setBidReq(0.0);
    replyPkt.setBidOffer(remainingTime - xtime);
    if(CoopOffer.size()!=0){
      replyPkt.setDest(CoopOffer[rand()%CoopOffer.size()].getSrcId());
    }
  }
  else if(remainingTime >= xtime){
    state = ST_DIRECT;
    xcounter = xtime;
    replyPkt.setBidReq(0);
    replyPkt.setBidOffer(0);
  }
  else if(getCoopAlgorithm()!=ALG_NOCP && !CoopOffer.empty()){
      state = ST_RELAY;
      //check the offer from secondary users and create the reply packet here
      for(unsigned int i =0;i < CoopOffer.size();i++){
          estimateTime = TrafficGen :: estimateTime(COND_RELAY, t, dl);
          offerTime = remainingTime > addTime? remainingTime:addTime;
          viaxtime = estimateTime + CoopOffer[i].getBidRequest();
          if(remainingTime >= viaxtime && 
             CoopOffer[i].getBidOffer() >= viaxtime &&
             CoopOffer[i].getBidRequest()<= offerTime - viaxtime){
              isEligible = true;
              if (CGF < (1 + viaxtime)/(1 + offerTime - viaxtime)){
                  CGF = (1 + viaxtime)/(1 + offerTime - viaxtime);
                  index = i;
                  indexReqTime = viaxtime;
              }
          }
      }
      if(isEligible == false && getCoopAlgorithm()==ALG_PURE){
          replyPkt.setBidReq(0);
          replyPkt.setBidOffer(remainingTime);
          if(!CoopOffer.empty()){
              replyPkt.setDest(CoopOffer[rand()%CoopOffer.size()].getSrcId());
          }
      }
      else if(isEligible == true){
          replyPkt.setSrc(getUserID());
          replyPkt.setDest(CoopOffer[index].getSrcId());
          replyPkt.setBidReq(indexReqTime);
          replyPkt.setBidOffer(offerTime - indexReqTime);
      }
  }
  return replyPkt;
}

Packet SecondaryUser :: createCtrlPkt(double t, double dl, double xtime){
  double remainingTime,addTime = 0,offerTime; 
  Packet *p = que.front();
  remainingTime = p->genTime()+p->getDeadline()-t;
  Packet ctrlPkt(0,PKT_CTRL);
  if(getCoopAlgorithm()==ALG_WIN){
     addTime = getAddTime(t);
  }
  offerTime = remainingTime > addTime?remainingTime:addTime;
  if(remainingTime < 0){
    cout << "You should not be here" << endl;
    return Packet();
  }
  else if(remainingTime >= xtime && getCoopAlgorithm() != ALG_NOCP){ /* can be delivered if available channel*/
    state = ST_RELAY;
    ctrlPkt.setSrc(getUserID());
    ctrlPkt.setBidReq(xtime);
    ctrlPkt.setBidOffer(offerTime - xtime);
  }
  else if(remainingTime >= xtime && getCoopAlgorithm() == ALG_NOCP){/* no offer since no one caresx*/
    state = ST_RELAY;
    ctrlPkt.setSrc(getUserID());
    ctrlPkt.setPktType(PKT_OPP);
    ctrlPkt.setBidReq(xtime);
  }
  else if(getCoopAlgorithm()==ALG_PURE){/* free donation */
    state = ST_RELAY;
    ctrlPkt.setSrc(getUserID());
    ctrlPkt.setBidOffer(remainingTime);
    ctrlPkt.setBidReq(0);
  }
  return ctrlPkt;
}

double PrimaryUser :: getAddTime(double curTime)
{
  double addTime = 0;
  deque<Packet*>::iterator it;
  Packet* pk = NULL;
  int counter = 0;
  for(it = que.begin(); it != que.end(); it++){
    counter++;
    if(counter == coop_window){
      break;
    }
    pk = *it;
  }
  if(pk != NULL){
    addTime = pk->genTime() + pk->getDeadline()-curTime;
  } 
 
  return addTime;
}

double SecondaryUser :: getAddTime(double curTime)
{
  double addTime = 0;
  deque<Packet*>::iterator it;
  Packet* pk = NULL;
  int counter = 0;
  for(it = que.begin(); it != que.end(); it++){
    counter++;
    if(counter == coop_window){
      break;
    }
    pk = *it;
  }
  if(pk != NULL){
    addTime = pk->genTime() + pk->getDeadline()-curTime;
  } 
  return addTime;
 
}
int PrimaryUser :: sendData(){
    if(state == ST_XMIT || state == ST_DIRECT){
      if(getCoopAlgorithm() == ALG_NOCP && whoisstealing != -1){
	//stop the secondary user's transmission and drop that packet
	neighborList[whoisstealing]->stopstealing();
      }
        xcounter--;
        if(xcounter <= 0){
	  xcounter = 0;
            que.pop_front();
            if(state == ST_DIRECT){
                stats.nDirPackets++; // number of direct packets
            }
            else{
                stats.nRePackets++; // number of relayed packets
            }
            state = ST_IDLE;
        }
    }
    return 0;
}

int PrimaryUser :: recvData(){
    return 0;
}

int SecondaryUser :: sendData(){
 if(state == ST_XMIT){
    xcounter--;
    if(xcounter <= 0){
      xcounter = 0;
      que.pop_front();
      stats.nRePackets++;
      state = ST_IDLE;
      //      cout << getUserID()<< "  One packet delivered " << que.empty()<< endl;
      if (que.empty()) que.clear();
    }
  }
 return 0;
}

int SecondaryUser :: recvData(){
    return 0;
}

void SecondaryUser :: stopstealing(){
  if(xcounter <= 0){
    return;
  }
  xcounter = 0;
  if(!que.empty()){
    que.pop_front();
    stats.nDropPackets++;
    state = ST_IDLE;
  }
}
void PrimaryUser :: clearAndlog(double curTime){
  if(state == ST_DIRECT || state == ST_XMIT)
    return;
  // drop expired packets
  // cout << getUserID()<< "clear "<< endl;
  while(!que.empty()){
    Packet *p = que.front();
    if(p->genTime()+p->getDeadline()> curTime) break;
    que.pop_front();
    stats.nDropPackets++;
  }
  CoopOffer.clear();
  if(getCoopAlgorithm() == ALG_WIN && ((int)curTime)%20 == 0){
    if(stats.nDirPackets == 0){
      coop_window += 1;
      coop_window = min(coop_window, que.size());
    }
    else{
      coop_window = max(1,min((stats.nGPackets/stats.nDirPackets),que.size()));
    }
  }
}

void SecondaryUser :: clearAndlog(double curTime){
  if(state == ST_XMIT)
    return;
  //  cout << getUserID() << " clear" << endl;
  // drop expired packets
  while(que.size() != 0){
    Packet *p = que.front();
    if(p->genTime()+p->getDeadline()> curTime) break;
  
    que.pop_front();
    stats.nDropPackets++;
  }
  
  CoopOffer.clear();
  if(getCoopAlgorithm() == ALG_WIN && ((int)curTime)%20 == 0){
    if(stats.nRePackets == 0){
      coop_window += 1;
      coop_window = min(coop_window,que.size());
    }
    else{
      
      coop_window = max(1,min((stats.nGPackets/stats.nRePackets),que.size()));
    }
  }

}


statistics User :: show_results(){
    cout << "algo: " << getCoopAlgorithm() << ", cond: " <<cond;
    cout << ", id: "<< getUserID()<<", #packets: " << stats.nGPackets << ", #dir packets: " << stats.nDirPackets << ", #relay: " << stats.nRePackets << ", #dropped: "<<stats.nDropPackets << ", FF: " << (double)(stats.nDirPackets + stats.nRePackets)/(stats.nDirPackets + stats.nRePackets+stats.nDropPackets) << endl;

  return stats;
}
