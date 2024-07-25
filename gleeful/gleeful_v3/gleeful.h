#ifndef _GLEEFUL
#define _GLEEFUL

#include "parameters.h"
#include "rollsieve.h"

#include <queue>
#include <string>

using namespace std;

class Gleeful{
public:
     uint64_t sum;
     bool isShort;
     uint32_t len, next;
     uint64_t minP, maxP, minPI, maxPI;
     queue<uint64_t> list;
     Rollsieve roll{Rollsieve(1)};

     Gleeful(){ //default constructor
          sum = minP = maxP = minPI = maxPI = 0;
          len = next = 0;
          isShort = false;
     }

     Gleeful(uint64_t s, uint32_t l, uint64_t m){ //short
          sum = s;
          next = 0;
          isShort = true;
          len = l;
          minP = 2;
          maxP = m;
          minPI = 0;
          maxPI = 0;
          uint64_t p(0);
          for(p = roll.nextprime(); p<maxP; p = roll.nextprime()){
               list.push(p);
          }
          list.push(p);
     }

     Gleeful(uint64_t s, uint32_t l, uint64_t m, uint64_t i){ //long
          sum = s;
          next = 0;
          isShort = false;
          len = l;
          minP = 2;
          maxP = m;
          minPI = 1;
          maxPI = l;
     }

     inline uint64_t dequeue(){
          uint64_t save = list.front();
          list.pop();
          return save;
     }

     inline void enqueue(uint64_t x){ //might need to actually write code for this
          list.push(x);
     }

     inline std::string toString(){ 
          if(isShort){   
               return (
                    "(S sum: " + to_string(sum) 
                    + "\tlen: " + to_string(len)
                    + "\tnext: " + to_string(next)
                    + "\trollN: " + to_string(roll.getn())
                    + "\tfront: " + to_string(list.front())
                    + "\tback: " + to_string(list.back())
                    + ")"
               );
          } else{
               return (
                    "(L sum: " + to_string(sum) 
                    + "\tlen: " + to_string(len) 
                    + "\tnext: " + to_string(next)
                    + "\tmaxP: " + to_string(maxP) 
                    + "\tmaxPI: " + to_string(maxPI)
                    + "\tminP: " + to_string(minP)
                    + "\tminPI: " + to_string(minPI)
                    + ")"
               );
          }
     }
};

#endif