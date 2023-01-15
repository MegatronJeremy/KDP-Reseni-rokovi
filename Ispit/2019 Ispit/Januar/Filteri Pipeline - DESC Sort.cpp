chan in, out;
void Process(int i = 1) {
	int min = INT_MAX;
	int input;
	read in(input);
	while (input != EOS) {
		input = -input;
		if (input < min) {
			min = input;
		} else {
			send out(input);
		}
		read in(input);
	}
	send out(min);
	send out(EOS);
}

void Process(int i = 2..N-1) {
	...kako je opisano u zadatku
}

void Process(int i = N) {
	int min = INT_MAX;
	int input;
	read in(input);
	while (input != EOS) {
		if (input < min) {
			min = input;
		} else {
			send out(-input);
		}
		read in(input);
	}
	send out(-min);
	send out(EOS);
}


----------------------------------------------------------------------
Nisam tacno siguran sta se ovde treba raditi, ali moguce ideja:
Dodamo dva procesa na pocetak i kraj niza.
Prvi pretvara sve brojeve u negativne i prosledjuje dalje.
Drugi pretvara sve negativne brojeve u pozitivne i prosledjuje dalje.
-> Ovo rade uz odgovarajuce trazenje minimuma
Ovako ce krajnji niz biti sortiran opadajuce.