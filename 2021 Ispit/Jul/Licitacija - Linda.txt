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


bool ucesnik_u_licitaciji(int i, int n) {
	bool over;
	rd("over", over);
	
	while (!over) {
		int bid, t;
		in("current_bid", ?bid, ?t);
		
		if (raise_bid(bid) == true) {
			out("current_bid", bid, i); 
			// bid raised - i am now the leader
		} else {
			out("current_bid", bid, t); 
			// don't want to raise - leader stays the same
		}
		rd("over", over);
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

void tick() {
	int x = 0;
	in("tod", ?x);
	out("tod", x+1);
}