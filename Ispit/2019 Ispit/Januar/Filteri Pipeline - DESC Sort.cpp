chan in, out;
void Process(int i = 0..N-1) {
	int min = EOS;
	int input;
	
	receive in(input);
	while (input != EOS) {
		if (min == EOS) {
			min = input;
		} else if (input < min) {
			send out(min);
			min = input;
		} else {
			send out(input);
		}
		receive in(input);
	}
	send out(min);	// niz je nenulte velicine
	send out(EOS);
}


----------------------------------------------------------------------
-> Trazenje minimuma
-> Ako se poslednje izbaci minimalna vrednost, a niz ze prima od PRVOG do POSLEDNJEG
-> Posalji min POSLEDNJE
-> Niz ce NA KRAJU BITI SORTIRAN OPADAJUCE