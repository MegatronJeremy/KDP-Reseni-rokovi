chan in, out;

void process() {
	int buf[2] = {EOS};
	int input;
	
	receive in(input);
	while (input != EOS) {
		if (buf[0] == EOS) {
			buf[0] = input;
		} else if (buf[1] == EOS) {
			buf[1] = input;
		} else if (input < buf[0]) {
			send out(buf[0]);
			buf[0] = input;
		} else if (input > buf[1]) {
			send out(buf[1]);
			buf[1] = input;
		} else {
			send out(input);
		}
		if (buf[0] != EOS && buf[1] != EOS && buf[0] > buf[1]) {
			// swap to keep order
			input = buf[0];
			buf[0] = buf[1];
			buf[1] = input;
		}
		receiv in(input);
	}
	
	if (buf[1] == EOS) {
		send out(buf[0]); // median found
	} // else median still not found
	
	send out(EOS); 
}

-------------------------------------------------------
Samo tri lokacije
Do N ulaznih pozitivnih vrednosti (N je neparno) -> Kraj EOS
N filterskih procesa
Salje se medijana pa EOS


--> Svako filtrira lokalni min/max?

21538

2 -> ||   || -> ||   || -> ||   ||
1 -> ||2  || -> ||   || -> ||   ||
5 -> ||1 2|| -> ||   || -> ||   ||
3 -> ||1 5|| -> ||2  || -> ||   ||
8 -> ||1 5|| -> ||2 3|| -> ||2  ||
E -> ||1 8|| -> ||2 5|| -> ||3  ||
- -> ||1 8|| -> ||2 5|| -> ||   || -> 3 //ok izgleda
// poslednji proces ima samo jednu vrednost (n je neparan broj)