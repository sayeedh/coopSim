#include<iostream>
#include<fstream>
using namespace std;

#include "sim.h"

int main(int argc, char** argv)
{
  int time = 100, np, ns, nrow, ncol, chcond, algo;
  double purate,surate;
  ofstream resFile;
  resFile.open("n_m_med.txt");
  if (argc == 11){
      time = atoi(argv[2]);
      np = atoi(argv[3]);
      ns = atoi(argv[4]);
      nrow = atoi(argv[5]);
      ncol = atoi(argv[6]);
      purate = atof(argv[7]);
      //      surate = (double)(atoi(argv[8]))/10;
      surate = atof(argv[8]);
      chcond = atoi(argv[9]);
      algo = atoi(argv[10]);
    }
  else{
    nrow = 11;
    ncol = 11;
  }
  
  for(chcond = 2; chcond <= 2; chcond++){
    for(np = 3; np <= 3; np +=1){
      for(ns = 5; ns <= 10; ns+=2){
	for(purate = 0.50; purate <= 0.5 ; purate += 0.1){
	  for(double ratio = 0.5; ratio <= 1.5; ratio += 0.5){
	    surate = purate *ratio;
	    for(algo = 1; algo <= 3; algo++){
	      resultSet resTotal;
	      int count = 0;
	      for(int r = 1; r <= 5; r++){
		Simulation* sim = new  Simulation(time,np,ns,nrow,ncol,purate,surate,chcond,algo);
		/* create nodes, place them, calculate neighbors */
		sim->init();	
		resultSet res = sim->run();
		if (res.ns){
		  count++;
		  if(count == 1){
		    resTotal.ns = res.ns;    resTotal.avgpunb = res.avgpunb; 	    resTotal.avgsunb = res.avgsunb;
		    resTotal.pupr = res.pupr; 		    resTotal.supr = res.supr;
		  }
		  else{
		    resTotal.ns += res.ns;		    resTotal.avgpunb += res.avgpunb;		    resTotal.avgsunb += res.avgsunb;
		    resTotal.pupr += res.pupr;		    resTotal.supr += res.supr;
		  }
		}
		sim->clear();
		delete sim;
	      }//end run
	      if(count){
		resFile <<np<<","<<ns<<","<< resTotal.ns/count<<","<<purate<<","<<surate<<","<<chcond<<","<<algo<<","<<resTotal.avgpunb/count<<","<<resTotal.avgsunb/count<<","<<resTotal.pupr/count<<","<<resTotal.supr/count<<", " << (resTotal.pupr*0.6 + resTotal.supr*0.4)/count << endl;
	      }	    
	    }// end algo
	  }//end ratio
	}// end purate
      }//end ns
    }// end np
  }// end chn

resFile.close();
return 0;
}
