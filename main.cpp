#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <cstring>
using namespace std;

#define N 4
#define NUM_STEPS 200000000

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char** argv) {

	// PI Example 
	
	// Parallel Version 1.0
	
	double pi = 0;
	double step = 1.0 / (double) NUM_STEPS;
	double time1 = omp_get_wtime();
	double sum = 0;
	
	
	#pragma omp parallel num_threads(N) 
	{
		double x = 0;
		double result = 0;
		int id = omp_get_thread_num();
		for(int i = id ; i < NUM_STEPS;i+=N) {
			x = (i + 0.5) * step;
			result += + 4.0 / (1.0 + (x * x));
		}
		
		#pragma omp critical 
		{
			sum += result;
		}
	
		
	}
	
	
	pi = sum * step;
	cout << "PI : " << pi << endl; // 3.14
	cout << "Time Taken : " << omp_get_wtime() - time1 << " ms" << endl; // 2.902


	cout << "******************************************************" << endl;
	/* ######################################################################################################################################################### */

	// Parallel Version 1.1
	
	double sum2 = 0;
	double pi2 = 0;
	double x2 = 0;
	double time3 = omp_get_wtime();
	#pragma omp parallel num_threads(N) private(x2) 
	{
		#pragma omp for reduction(+:sum2) schedule(static,N/4)
			for(int i = 0 ; i < NUM_STEPS; i++) {
				x2 = (i + 0.5) * step;
				sum2 += 4.0 / (1.0 + (x2 * x2));
			}
	}
	
	
	pi2 = sum2 * step;
	
	
	cout << "PI (V1.1) : " << pi2 << endl; 
	cout << "Time Taken (V1.1) : " << omp_get_wtime() - time3 << " ms" << endl; // 2.905ms	
	
	
	cout << "******************************************************" << endl;
	


	/* ######################################################################################################################################################## */


	// Serialized Version
	double xSerialized = 0;
	double sumSerialized = 0;
	double piSerialized = 0;
	double time2 = omp_get_wtime();
	
	
	for(int i = 0 ; i < NUM_STEPS;i++) {
		xSerialized = (i + 0.5) * step;
		sumSerialized += 4.0 / (1.0 + (xSerialized * xSerialized));
	}
	
	piSerialized = sumSerialized * step;
	
	
	cout << "PI (Serialized Version) : " << piSerialized << endl; // 3.14ms
	cout << "Time Taken (Serialized Version) : " << omp_get_wtime() - time2 << " ms" << endl; // 5.579




	return 0;
}
