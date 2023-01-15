void dispatch() {
	int next = 0;
	int n = ..., x, y;
	
	while (true) {
		in("next", next, ?x, ?y);
		
		// calculate rolling min
		out("customer", x, y, -1, -1, 0);
		
		sleep(n);
		
		int dst, id, cnt;
		in("customer", x, y, ?dst, ?id, ?cnt);
		
		if (cnt > 0) {
			out("chosen", cnt, id);
			in("chosen", 0, id);
		}
		
		out("response", next, id);
		
		next++;
	}
}

bool customer(int x, int y) {
	int myT;
	
	in("ticketC", ?myT);
	out("ticketC", myT+1);
	
	out("next", myT, x, y);
	
	int taxi;
	in("response", myT, ?taxi);
	
	if (taxi == -1) return false;
	
	// wait for taxi to arrive
	in("arrived", taxi);
	
	// board taxi
	out("customerBoarded", taxi);
	
	// ride to location
	in("rideDone", taxi);
}

void taxi(int x, int y) {
	int cX, cY, dst, id, cnt;
	
	int myT;
	in("ticketT", ?myT);
	out("ticketT", myT+1);
	
	while (true) {
		// wait for a customer
		in("customer", ?cX, ?cY, ?dst, ?id, ?cnt);
		int myDst = distance(x, y, cX, cY); // calculates distance from coordinates
		if (dst == -1 || myDst < dst) {
			dst = myDst;
			id = myT;
		}
		out("customer", cX, cY, dst, id, cnt+1);
		
		in("chosen", ?cnt, ?id);
		out("chosen", cnt-1, id);
		
		// i am chosen
		if (id == myT) {
			// go to location...
			out("arrived", myT);
			
			// wait for customer...
			in("customerBoarded", myT);
			
			// ride customer...
			out("rideDone", myT);
		}
	}
}

void init() {
	out("ticketC", 0);
	out("ticketT", 0);
	
	for (...) {
		int x = ..., y = ...;
		eval(customer(x, y));
	}
	for (...) {
		int x = ..., y = ...;
		eval(taxi(x, y));
	}
	eval(dispatch());
}

------------------------------------------------------------------------
Korisnici, dispecer, taksi sa radio stanicama
Redosled:
1. Korisnik zove dispecera i trazi zahtev za voznju
2. Dispecer salje zahtev svim radio taksijima (broadcast)
3. Nakon n jedinica vremena
	-> Ima vozila? dispecer objavljuje da prijavljeni taksi
	najblizi korisniku obavlja prevoz
4.1.1. Javlja korisniku koji taksi ga vozi
4.1.2. Taksi dolazi do korisnika, vozi ga i zavrsava voznju
		-> Nema vozila?
4.2. Javlja korisniku da nema vozila