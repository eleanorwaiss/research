#include "gleeful.h"
#include "cprimes.h"
#include <iostream>

using namespace std;

CPrimes smallprimes(3, sieveSize);

int main(){
     for(int i = 0; i<10; i++){
          cout << i << "\t" << smallprimes.gap(i) << endl;
     }
}