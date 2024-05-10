#include <iostream>
#include <cstdlib>
#include <ctime>
#include <complex>
#include <cmath>
#include <vector>
#include <string.h>
#include <fstream>
#include <bits/stdc++.h>
#include <complex>
#include <iomanip>
#include <iostream>
#include <numbers>
using namespace std::complex_literals;
using namespace std;

// Generates a random complex number with length=bound
complex<double> randBound(double bound){
	double x = (rand()%20000)*0.0001-1;
	double y = (rand()%20000)*0.0001-1;
	complex<double> temp(x,y);
	return complex<double>(temp.real()*bound/abs(temp),temp.imag()*bound/abs(temp));
}

int main(int argc, char *argv[]){
	srand((unsigned)time(0)); //seed rng
	
	time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    char buffer[80];
    strftime (buffer,80,"000%Y%m%d-%H-%M-%S.txt",now); //make buffer file name with current time
	
	//Variable init
	complex<double> c(-1.01,0.03), d(-0.39,0.14);
	complex<double> p, q, alpha, beta;
	ofstream fo;
	
	fo.open(buffer); //open log file
	for(double bound(0.015); bound<1; bound+=0.005){
		fo << "==========" << endl << bound << endl << "==========" << endl;
		cout << bound << endl;
		for(int counter(0); counter<100; counter++){ //perturbations with bound
			p=c+randBound(bound); q = d+randBound(bound);
			alpha = 0.5*(complex<double>(-1,0)-sqrt(complex<double>(-3,0)-(complex<double>(4,0)*p))); //attracting periodic 2-cycle of p(z)=z^2+p, assuming p in correct domain
			beta = 0.5*(complex<double>(-1,0)+sqrt(complex<double>(-3,0)-(complex<double>(4,0)*p)));
			
			fo << endl << "=====" << endl;
			fo << "Counter:\t" << counter << "\tp:\t" << p << "\tq:\t" << q << endl;
			fo << "alpha:\t" << alpha << "\tbeta:\t" << beta << endl;
			
			complex<double> g(0,0), temp, limpz;	
			
			for(int j(0); j<250; j++){ //g orbit testing
				limpz = temp = g;
				for(int i(0); i<250; i++){ //basin testing
					limpz = pow(limpz, 2) + p;
					if(abs(limpz)>2){
						fo << "Failure on limpz: " << limpz << "\ti: " << i << endl; fo.close(); 
						cout << "Program terminated (LIMPZ OUT OF BOUNDS).\ni="<<i<<endl; return 0;
					}
				}
				if(abs(limpz-alpha)<0.001){ fo << "j: "<< j << "\t" << g << "\twithin basin of alpha\n"; g = alpha + pow(g,2)+q; }
				else if(abs(limpz-beta)<0.001){fo << "j: "<< j << "\t" << g << "\twithin basin of beta\n";g = beta + pow(g,2)+q;}
				else {
					fo << g << "\tnot in basin of either alpha nor beta" << endl; fo.close();
					cout << "Program terminated (LIMPZ NONCONVERGENCE).\n"; return 0;
				}
				
				if(abs(g)>2){
					fo << "Failure on g:\t" << g << endl; fo.close();
					cout << "Program terminated (G OUT OF BOUNDS.\n"; return 0;
				} else if(temp==g){
					fo << "Short circuiting due to fixed point." << endl;
					break;
				}
			}
		}
	}
	return 0;
}