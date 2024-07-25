#include <iostream>
#include <cstring>
#include <cstdint>
#include <vector>
#include <fstream>

using namespace std;

const uint64_t bound = 1e9L;
const uint64_t sieveSize = 1e6L;

vector<bool> isPrime(sieveSize+1, true);
vector<uint64_t> soe;
string fname = "parameters.h";


int main(){
     for(uint64_t p = 2; p*p<=sieveSize; p++)
          if(isPrime[p])
               for(uint64_t q = p*p; q<=sieveSize; q+=p)
                    isPrime[q]=false;
     
     for(uint64_t p = 2; p<=sieveSize; p++)
          if(isPrime[p])
               soe.push_back(p);

     uint64_t sum(0), ng(0), ss(0);
     while(sum <= bound){
          sum += soe[ng++];
     };
     ng--;

     sum = 0;
     uint64_t piX = soe.size();
     while(sum <= bound){
          sum += soe[piX-(ss++)];
     };
     ss--;
     
     ofstream fout;
     fout.open(fname);
     fout 
          << "#ifndef _PARAMETERS" << endl
          << "#define _PARAMETERS" << endl
          << endl
          << "#include <cstdint>" << endl
          << endl
          << "const uint64_t bound = " << bound << "L;" << endl
          << "const uint64_t sieveSize = " << sieveSize << "L;" << endl
          << endl
          << "const uint32_t numGleefuls = " << ng << "L;" << endl
          << "const uint32_t sizeShort = " << ss << "L;" << endl
          << endl
          << "#endif";
     fout.close();
     cout << "Saved parameters to: " << fname << endl;
     return 0;
}