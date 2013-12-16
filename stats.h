#ifndef STATS_H
#define STATS_H

// this class stores the result 
class statistics{
  public:
    statistics(){
        nGPackets = 0;
        nDirPackets = 0;
        nDropPackets = 0;
        nRePackets = 0;
    }

    unsigned int nGPackets;
    unsigned int nDirPackets;
    unsigned int nDropPackets;
    unsigned int nRePackets;
};

#endif
