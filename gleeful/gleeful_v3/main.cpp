#include "gleeful.h"
#include "parameters.h"
#include "cprimes.h"

#include <iostream>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <ctime>

using namespace std;

vector<Gleeful> happy(numGleefuls+1);
CPrimes soe(3, sieveSize);
unordered_map<uint64_t, uint32_t> h;
vector<uint64_t> representations(20, 0);

void hashInsert(uint32_t pos){ //insert the gleeful object at happy[pos] into hash table
     auto search = h.find(happy[pos].sum); //does the sum already exist in table?
     if(search!=h.end()){ //if so
          happy[pos].next = search->second;  //link sum-in-table to sum-to-insert
          h.erase(search); //remove sum-in-table
     }
     h.insert(make_pair(happy[pos].sum, pos)); //insert sum-to-insert

}

int main(){
     auto start = chrono::system_clock::now();
	time_t time = chrono::system_clock::to_time_t(start);
     uint64_t sum(0);
     uint32_t len(0);
     uint64_t maxP(0);
     Rollsieve r(1);

     //short init
     for(len = 1; len <= sizeShort; len ++){
          maxP = r.nextprime();
          sum += maxP;
          happy[len] = Gleeful(sum, len, maxP);
     }

     //long init
     for(len = sizeShort + 1; len <= numGleefuls; len ++){
          maxP = r.nextprime();
          sum += maxP;
          happy[len] = Gleeful(sum, len, maxP, len);
     }

     for(int i = 1; i< happy.size(); i++){
          hashInsert(i);
          //cout << happy[i].toString() << endl;
     }

     //now the fun begins!
     for(uint64_t s = 1; s<=bound; s++){
          auto current = h.find(s); //look for s in table
          if(current == h.end()){ //not found
               representations[0]++;
               continue;
          }
          int count(0);
          h.erase(current); //found
          uint32_t headNode = current->second;
          uint32_t tail = happy[headNode].next;
          do{
               count++;
               Gleeful &currObj = happy[headNode];
               //cout << currObj.toString() << endl;
               if(currObj.isShort){ //short, so use rollsieve
                    currObj.maxP = currObj.roll.nextprime();
                    currObj.minP = currObj.dequeue();
                    currObj.sum += (currObj.maxP - currObj.minP);
                    currObj.enqueue(currObj.maxP);
               } else { //long, so use soe[];
                    if(currObj.maxPI==1){
                         currObj.maxPI = 3;
                    } else {
                         currObj.maxP += soe.gap(currObj.maxPI-2);
                    }
                    currObj.maxPI++;

                    currObj.sum += (currObj.maxP - currObj.minP);

                    if(currObj.minPI==1){
                         currObj.minP = 3;
                    } else {
                         currObj.minP += soe.gap(currObj.minPI-2);
                    }
                    currObj.minPI++;
               }
               
               currObj.next = 0;
               if(currObj.sum <= bound) hashInsert(headNode);
               headNode = tail;
               if(headNode!=0) tail = happy[headNode].next;
          } while(headNode!=0);
          representations[count]++;
     }

     auto end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	double comptime = elapsed_seconds.count();
	cout << "Program terminated. " << endl;
	cout << "computation time: " << comptime << "s" << endl;

     for(int i = 0; i <= representations.size(); i++){
          cout << i << "\t" << representations[i] << endl;
     }

     return 0;
}