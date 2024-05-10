//Sieve of Eratosthenes

import java.io.*;
import java.util.*;

class sieve{
     public static void main(String [] args){
          final int K = 1;
          int n;
          int count=0;
          Scanner cin=new Scanner(System.in);

          System.out.print("Please enter your upper bound: ");
          n=cin.nextInt();

          boolean [] sieve = new boolean[n+1]; // two steps: declare and allocate
          //or write: sieve = new boolean[n+1] on new line

          // initialize each position to TRUE
          for(int i=0; i<sieve.length; i++){
               sieve[i]=true;
          }

          //0 and 1 are not prime
          sieve[0]=sieve[1]=false;
          //the equals operator is right-associative

          for(int p=0;p<=Math.sqrt(n);p++){
               if(sieve[p]){
                    for(int q=p*p; q<sieve.length; q=q+p){
                         sieve[q]=false;
                    }
               }
          }

          for(int i=0; i<sieve.length; i++){
               if(sieve[i]){
                    count++;
               }
          }

          System.out.println("pi("+n+"): "+count);

          int primes[] = new int[count];
          int j=0;

          System.out.print("Primes: ");
          for(int i=0; i<sieve.length; i++){
               if(sieve[i]){
                    primes[j]=i;
                    if(count<50) System.out.print(primes[j] + " ");
                    j++;  
               }
          }
          System.out.println("");

          int prefix[] = new int[count];
          prefix[0]=primes[0];

          System.out.print("Prefix: ");
          for(int i=1; i<count; i++){
               prefix[i]=prefix[i-1]+primes[i];
               System.out.print(prefix[i]+" ");
          }
          System.out.println("");

          int temp;
          System.out.print("Sums: ");
          for(int b=0; b<count-1; b++){
               for(int t=b+1; t<count; t++){
                    temp = prefix[t]-prefix[b];
                    if(temp>n) break;
                    else System.out.print("("+temp+", "+primes[b+1]+") ");
               }
          }
     }
}