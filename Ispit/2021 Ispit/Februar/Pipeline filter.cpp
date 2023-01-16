chan in, out;

// sortira? opadajuce - po sumama koje posalje pre EOS svaki filter - radi do 2*N brojeva
// u svakom filteru se velicina niza smanjuje za do 2
Process filter(int i:1..n) {
	float buf[2] = {EOS};
	float input;
	
	receive in(input);
	while (input != EOS) {
		if (buf[0] == EOS) {
			buf[0] = input;
		} else if(buf[1] == EOS) {
			// saberi prva dva primljena broja
			bud[0] += input;
			
			// primi sledeceg
			receive in(input);
			if (input == EOS) break; // nema vise
						
			// swap ako treba
			if (input < buf[0]) {
				buf[1] = buf[0];
				buf[0] = input;
			} else {
				buf[1] = input;
			}
		} else if (input > buf[1]) {
			// veci od oba - posalji manjeg
			send out(buf[0]);
			buf[0] = buf[1];
			buf[1] = input;
		} else if (input > buf[0]) {
			// veci od prvog - posalji manjeg
			send out(buf[0]);
			buf[0] = input;
		} else {
			// manji od oba
			send out(input);
		}
		
		receive in(input);
	}
	
	if (buf[1] == EOS) buf[1] = 0;
	// mora imati bar jedan element - buf[0] (niz je neprazan)
	
	send out(buf[0] + buf[1]);
	send out(EOS);
}
----------------------------------------------------------------------------------------------

Primer (za sortiranje - nevezano za ovo resenje):
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
-> sacuvaj 2 maksimalna broja, ostale prosledi
-> saberi 2 maksimalna broja, i prosledi sumu pa EOS
-> sortirano od maksimalne do minimalne sume?
-> sortiranje bi bilo kada bi slali samo MAKS - bez sume
-> maks je vrednost koju posaljemo poslednju
-> za N vrednosti sa N procesa - ili parcijalno sortiramo

izlaz: suma pa EOS