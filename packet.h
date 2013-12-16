#ifndef PACKET_H
#define PACKET_H


#define PKT_DATA	0
#define PKT_CTRL	1
#define PKT_OPP         2


#define UNICAST		0
#define BROADCAST	1
#define MULTICAST	2
#define DEADLINE        10.0


class Packet{
 
  int pktType;
  int xmitType;
  int destination;
  int source;
	double deadline;
	double bidRequest;
	double bidOffer;
	double tm;
	public:
 bool isValid;
	Packet(){isValid = false;}
	Packet(double t){
	  isValid = true;
	  tm = t;
	  deadline = DEADLINE;
	  bidRequest = 0; 
	  bidOffer = 0;
	  destination = -1;
	}
	Packet(double t, int type){
	  isValid = true;
	  tm = t; 
	  deadline = DEADLINE;
	  pktType =type;
	  bidRequest = 0; 
	  bidOffer = 0;
	}
	//get and set methods
	int getPktType()const{return pktType;}
	int getDestId()const{return destination;}
	int getSrcId()const{return source;}
	int getDeadline()const {return deadline;}
	double getBidRequest() const {return bidRequest;}
	double getBidOffer()const {return bidOffer;}
	double genTime()const{return tm;}
	
	void setPktType(int type){pktType = type;}
	void setxmitType(int xType){xmitType = xType;}
	void setDest(int id){destination = id;}
	void setSrc(int id){source = id;}
	void setBidReq(double x){bidRequest = x;}
	void setBidOffer(double x){bidOffer = x;}
};

#endif

