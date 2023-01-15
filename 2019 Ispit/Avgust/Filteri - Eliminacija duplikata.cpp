const int N = 10;

chan in(int), out(int);
Process Filter(i:0..N-1) {
	int min = INT_MAX, curr;
	
	receive in(curr);
	while (curr != EOS) {
		if (curr > min) {
			send out(curr);
		} else if (curr != min) {
			send out(min);
			min = curr;
		}
	}
	// svaki eliminise duplikate za 
	// sigurno jedan element (lokalni minimum)
	// niz je na kraju uredjen rastuce
	send out(min);
	send out(EOS);
}

----------------------------------------------------------------------
Poruke - jedan element niza
Asinhrona razmena poruka
Svaki proces identican kod
19 ulaznih vrednosti
Dve lokacije za smestanje elemenata niza
Potrebna jedna lokacija za smestanje prethodnog elementa
Jedan ulaz moguc u tom slucaju -> pa pipeline