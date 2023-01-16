Enum Color {
	HEART = 0, SPADE, DIAMOND, CLUB
}

struct Card {
	Color color;
	int num;
}

void Player(int id) {
	int left = id;
	int right = (id + 1) % 4;
	Card cards[4];
	
	// get initial 4 cards
	for (int i = 0; i < 3; i++) {
		in("table", ?cards[i]);
	}
	
	bool gameEnd;
	
	rd("end", ?gameEnd);
	
	// play while no winner is found
	while (!gameEnd) {
		bool colorSet[4] = {false}
		int num = -1;
	
		// check if i am the winner
		bool winner = true;
		
		for (int i = 0; i < 4; i++) {
			if (num == -1) {
				num = cards[i].num;
				colorSet[cards[i].color] = true;
			} else if (colorSet[cards[i]] == true || num != cards[i].num) {
				// not the winner
				winner = false;
				break;
			} else {
				colorSet[cards[i]] = true;
			}
		}
		
		in("end", ?gameEnd);
		if (winner || gameEnd) {
			if (gameEnd) {
				// somebody announced winner before me
				winner = false;
			}
			out("end", true);
			
			// ensure that next person is not waiting before finishing
			out("deck", left, deck[0]);
			
			break;
		} else {
			// game not over
			out("end", false);
		}
		
		int i = 3.0 * rand() / RAND_MAX
		out("deck", left, deck[i]); // uvek ce moci jedan proces da se izvrsava DOK TRAJE IGRA
		in("deck", right, ?deck[i]); 
	}
}

void init() {
	Card deck[24];
	
	for (int i = 0; i < 16; i++) {
		out("table", deck[i]);
	}
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; i++) {
			out("deck", i, deck[16+i*2+j]);
		}
	}
	
	for (int i = 0; i < 3; i++) {
		eval(Player(i));
	}
}


---------------------------------------------------------
Spil od 24 karte, podeljene u 4 boja (pik, karo, tref, herc),
sa po 6 brojeva

4 igraca za stolom -> svako po 4 karte (16 ukupno inicijalno)
Izmedju dva igraca gomila karata -> 2x4 karte (8 ukupno)

Igrac sve 4 karte istog broja -> u razlicitim bojama -> igra se
zavrsava i svi prekidaju igru

//Nema pobede u iteraciji, izbaci kartu na levu stranu, uzmi sa
desne strane.

//Sacekaj sledecu iteraciju, vidi da li ima pobednika. (barijera)
Ima pobednika? Prekini, inace nastavi. 
Nema ovde nikakvih iteracija!!
//Rashod 1 iteracija najvise.
