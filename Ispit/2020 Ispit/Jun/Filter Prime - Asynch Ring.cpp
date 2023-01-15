#define EONAT (20001)

chan in, out;
process Filter(int i = 0) {
	// generate array and send out for filtering
	int array[m+1];
	for (int i = 0; i <= m; i++) {
		array[i] = 20000.0 * rand() / (RAND_MAX + 1) + 1;
		send out(array[i]);
	}
	send out(EONAT);
	
	// receive and send filtered array
	int input, k = 0;
	do {
		receive in(input);
		array[k] = input;
		send out(array[k]);
		k++;
	} while (input != EONAT);
}

chan in, out;
process Filter(int i = 1..N-1) {
	int myPrime = ...;
	int array[m+1];
	
	// filter array
	int input;
	do {
		receive in(input);
		if (input != myPrime) {
			send out(input);
		}
	} while (input != EONAT);
	
	// receive final array and pass it forward
	int k = 0;
	do {
		receive in(input);
		array[k] = input;
		// last one doesn't need to pass it forward
		if (i != N-1) {
			send out(array[k]);
		}
		k++;
	} while (input != EONAT);
}








---------------------------
Prsten od N procesa
Asinhroni MP
0 -> generise niz od 1 do 20000
EONAT - sentinel
svaki Node filtrira sve svoje Prime brojeve
Svaki proces cuva ceo niz