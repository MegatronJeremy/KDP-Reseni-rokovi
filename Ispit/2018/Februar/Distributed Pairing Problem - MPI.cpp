struct Msg {
	int id;
	bool wantToPair;
}

mbx N[M]
Proces Node(id:0..M-1) {
	bool links[M][M] = ...;
	int myPair = -1;
	bool st;
	
	// salji i primaj zahteve od procesa sa manjim id-jem
	for (int i = 0; i < id; i++) {
		if (i == id || links[i][id] == false)
			continue;
		
		Msg msg;
		msg.id = id;
		msg.wantToPair = myPair == -1;
		
		mbx_put(msg, N[i]);
		
		// i know this is not a pairing request - but a response
		mbx_get(msg, N[id]);
		if (msg.wantToPair == true) {
			myPair = msg.id;
		}
	}
	
	// primaj od onih sa vecim id-jem - nema kruznih zavisnosti za uparivanje
	for (int i = id+1; i < M; i++) {
		if (i == id || links[i][id] == false)
			continue;
		
		Msg msg;
		
		mbx_get(msg, N[id], INF, st);

		// i know this is a pairing request
		
		if (msg.wantToPair == true && myPair == -1) {
			myPair = msg.id;
			msg.wantToPair = true;
		} else {
			msg.wantToPair = false;
		}
		int sendId = msg.id;
		msg.id = id;
		
		mbx_put(msg, N[sendId]);
	}
}

-----------------------------------------------------------------------------
Manji indeksi salji - pa veci indeksi primaj
Susedni cvorovi upareni.
Samo jedan proces moze da cita iz sanducica.
Slanje je asinhrono ovde.
Posalji svima -> primi od jednog (handshake napravi)
Kako da znam da li primam handshake ili request?