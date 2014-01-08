#ifndef COMMON_H
#define COMMON_H

// different states of a user
#define ST_IDLE    1
#define ST_DIRECT  2
#define ST_RELAY   3
#define ST_XMIT    4

// types of algorithm
#define ALG_PURE 1
#define ALG_WIN  2
#define ALG_NOCP 3

#define COND_GOOD   1
#define COND_MED    2
#define COND_POOR   3
#define COND_RELAY  4

#define INFTY 1000.0

#define min(x,y) ((x < y) ? x : y)
#define max(x,y) ((x > y) ? x : y)

#endif
