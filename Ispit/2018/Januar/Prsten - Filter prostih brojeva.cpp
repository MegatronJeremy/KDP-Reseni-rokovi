#define EONAT 20001
#define MAX 20000
#define M 100

chan in(int), out(int);
Process Filter(int id = 0) {
	int array[M+1]; // one more for termination
	
	// generate array
	for (int i = 0; i < M; i++) {
		send out((double) rand() / (RAND_MAX+1) * MAX + 1); 
		// vr. od 1 do 20000
	}
	send out(EONAT);
	
	int input;
	receive in(input);
	
	int k = 0;
	do {
		// receive filtered array and send it out
		receive in(input);
		array[k++] = input;
		send out(input); // pass it along
	} while (input != EONAT);
}

Process Filter(int id = 1..N-1) {
	int array[M+1]; // one more for sentinel
	int myPrime = ...;
	
	int input;
	receive in(input);
	while (input != EONAT) {
		// filter out prime value
		if (input != myPrime) {
			send out(input);
		}
		receive in(input);
	}
	// sentinel
	send out(EONAT);
	
	// and receive the final array
	int k = 0;
	do {
		receive in(input);
		array[k++] = input;
		if (id != N-1) { // don't send it back if on the end
			send out(input);
		}
	} while (input != EONAT);
}



------------------------------------------------------------------------
N procesa - asinhroni message passing
Proces 0 generator niza (od 1 do 20.000) pa EONAT
N-1 odstranjuju jedan prost broj
Modifikovan niz svim ostalim procesima

