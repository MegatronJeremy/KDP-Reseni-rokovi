int reserve(int id, int spaceNeeded) {
	int N, myT;
	rd("N", ?N);
	
	int spaceAvailable[N] = {0};

	in("ticket", ?myT);
	out("ticket", myT+1); // get ticket for reservation
	
	// get space availability info
	for (int i = 0; i < N; i++) {
		if (i == id) continue;
		
		read("space", i, ?spaceAvailable[i]);
	}
	
	int reservation = 0, i = 0;
	while (i < N && spaceNeeded > 0) {
		// reserve with my ticket
		int spaceReserved = 0, start = i;
		while (i < N && spaceReserved < spaceNeeded) {
			if (spaceAvailable[i] == 0) continue;
			
			if (spaceNeeded-spaceReserved < spaceAvailable[i]) {
				reservation = spaceNeeded-spaceReserved;
			} else {
				reservation = spaceAvailable[i];
			}
			out("reserve", i, reservation, myT);
			
			spaceReserved += reservation;
			i++;
		}
		
		// get confirmation from everyone that I reserved from
		for (int j = start; j < i; j++) {
			if (spaceAvailable[j] == 0) continue;
			
			int spaceConfirmed;
			in("confirmed", j, ?spaceConfirmed, myT);
			spaceNeeded -= spaceConfirmed;
		}
		
		// continue reserving from others if more space needed
		// stop if reserved all space I needed or not enough space left on all PCs
	}
	
	in("next", myT);
	out("next", myT+1); // PCs confirming reservation can accept the next PC
	
	return spaceReserved; // return how much space was allocated succesfully
}

void confirmReservation(int id) {
	int next, reservation, totalSpace;
	
	while (true) {
		rd("next", ?next);
		
		in("reserve", id, ?reservation, next);
		
		in("space", id, ?totalSpace);
		
		if (reservation > totalSpace) {
			reservation = totalSpace;
		}
		totalSpace -= reservation;
	
		out("space", id, totalSpace);
	
		out("confirm", id, reservation, next);
	}
}

void init() {
	int N = ...;
	int space[N] = ...;
	
	out("next", 0);
	out("ticket", 0);
	out("N", N);
	
	for (int i = 0; i < N; i++) {
		out("space", i, space[i]);
	}
	for (int i = 0; i < N; i++) {
		eval(confirmReservation(i));
	}
	for (...) {
		eval(reserve(i, SPACE_NEEDED));
	}
	
}


---------------------------------------------------------------------
N racunara - N diskova
Prvo samo vidim informaciju -> koliko ko ima raspolozivog prostora
Dalje zatrazim prostor (uniformno?) od koga mogu -> rezervacija
Potrebna potvrda od svih -> ako preostalo prostora, pokusati ponovo

Ako nema dovoljno prostora ukupno - nema prostora, alociram koliko mogu - 
u suprotnom potrebno ili raditi rollback od pocetka svim racunarima ILI
atomicno rezervisati prostor na svim racunarima (u dva prolaza)? 
mozda bi i moglo - ali mnogo manje konkurentno

Ma valjda je ovako u redu nmg