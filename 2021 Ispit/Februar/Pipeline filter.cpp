chan in, out;

Process filter(int i:1..n) {
	float buf[2] = {0};
	int cnt = 0;
	
	float input;
	receive in(input);
	while (input != EOS) {
		if (cnt < 2) {
			// cekaj na dva elementa
			buf[cnt++] = input;
		} else if(input < buf[1]) { 
			// manji od veceg (buf[1]) - uvek salji veceg 
			send out(buf[1]);
			buf[1] = input;
		} else {
			// veci od oba (ili jednak)
			send out(input);
		}
		
		// sortiraj koristenjem treceg slota za float
		// buf[1] mora biti veci
		if (buf[0] > buf[1]) {
			input = buf[0];
			buf[0] = buf[1];
			buf[1] = input;
		}
		
		receive in(input);
	}
	
	// ASC sortiran niz od 2*n? posaljemo NA KRAJU buf[1] (veci) pa buf[0] (manji) - obrnuti redosled
	// onaj ispred ce opet poslati ISTO NA KRAJU ALI PRE TOGA SVOJA DVA SORTIRANA MINIMUMA
	// CHANNEL JE FIFO
	
	// ovde samo saljem sumu - pa nisam ni trebao voditi racuna o sortiranosti dva elementa
	send out(buf[0] + buf[1]);
	send out(EOS);
}


Primer (za sortiranje):
Na ulazu je 2413

3 --> ||   || --> ||   ||

1 --> ||3  || --> ||   ||

4 --> ||1 3|| --> ||   ||

2 --> ||1 3|| --> ||4  ||

E --> ||1 2|| --> ||3 4||

/ --> ||1 /|| --> ||2 3|| --> 4

/ --> ||/ /|| --> ||1 2|| --> 3 4 

/ --> ||/ /|| --> ||/ 2|| --> 2 3 4

/ --> ||/ /|| --> ||/ /|| --> 1 2 3 4
--------------------------------------------------------------
-> sortira pa sumira do 2*n ulaznih realnih brojeva
-> sacuvaj 2 maksimalna broja, ostale prosledi
-> saberi 2 maksimalna broja, i prosledi sumu pa EOS
-> sortirano od maksimalne do minimalne sume?
-> sortiranje bi bilo kada bi slali samo MAKS - bez sume
-> maks je vrednost koju posaljemo poslednju
-> za N vrednosti sa N procesa - ili parcijalno sortiramo

izlaz: suma pa EOS