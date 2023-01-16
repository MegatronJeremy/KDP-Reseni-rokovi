void vodja_licitacije() {
	// initialise licitation
	int reserved_bid = ...;
	int licitation_time = ...;
	int n = ...;
	
	for (int i = 0; i < n; i++) {
		eval(ucesnik_u_licitaciji(i, n));
	}
	
	// licitation start
	out("tod", 0);
	
	out("over", false);
	out("current_bid", 0, -1);
	
	// wait for licitation end
	rd("tod", licitation_time);
	
	// end licitation
	in("over", false);
	out("over", true);
	out("awaiting_result", 0);
	
	//wait for everyone to finish last bid
	in("awaiting_result", n);
	
	int x, y;
	rd("current_bid", ?x, ?y);
	if (x >= reserved_bid && y != -1) {
		out("bid_succesful", true);
	} else {
		out("bid_succesful", false);
	}
}

// busy wait dok licitacija nije gotova (ok? - ne radi nikakav drugi posao)
// i samo cita torku ako ne povecava vrednost
bool ucesnik_u_licitaciji(int i, int n) {
	bool over;
	rd("over", ?over);
	
	while (!over) {
		int bid, t;
		
		// look at current bid
		rd("current_bid", ?bid, ?t);
		
		// check if i want to raise the bid (and pass info if I am the leader)
		if (t != i && want_to_raise(bid) == true) {
			in("current_bid", ?bid, ?t);
			
			int raised_val = raise_bid(bid);
			if (val_raised > 0) {
				// bid raised - i am now the leader
				out("current_bid", raised_val, i); 
			} else {
				// leader stays the same - i changed my mind
				out("current_bid", bid, t); 
			}
		} 
		
		rd("over", ?over);
	}
	// it's over - wait for everyone else to finish
	int x;
	in("awaiting_result", ?x);
	out("awaiting_result", x+1);
	
	rd("awaiting_result", n);
	
	bool f;
	rd("bid_succesful", ?f);
	
	bool ret = false;
	if (f) { // licitation succesful
		int y;
		rd("current_bid", ?x, ?y);
		if (y == i) { // i have the last bid
			ret = true;
		}
	}
	return ret;
}

void tick() { // periodicno se zove
	int x = 0;
	in("tod", ?x);
	out("tod", x+1);
}