void vehicle() {
	int M, N, slot, ticket;
	rd("args", ?M, ?N);
	
	int lane = M * ((double) rand() / (RAND_MAX + 1)); // 0..M-1
	
	in("tailQ", lane, ?ticket);
	out("tailQ", lane, ticket+1);
	
	in("headQ", lane, ticket);
	in("board", lane, ?slot);
	// boarding...
	if (slot > 1) {
		out("board", lane, slot-1);
	} else {
		out("all aboard");
	}
	out("headQ", lane, ticket+1);
	
	// ride ferry
	
	in("unboard", lane, slot);
	//unboarding...
	if (slot < N) {
		out("unboard", lane, slot+1);
	} else {
		out("all ashore");
	}
}

void ferry() {
	int M, N;
	rd("args", ?M, ?N);
	
	while(1) {
		// ferry arrived in port
		for (int i = 0; i < M; i++) {
			out("board", i, N);
		}
		for (int i = 0; i < M; i++) {
			in("all aboard");
		}
		
		// get to the other side
		
		// ferry arrived at destination
		for (int i = 0; i < M; i++) {
			out("unboard", i, 1);
		}
		for (int i = 0; i < M; i++) {
			in("all ashore");
		}
		
		// travel back to port...
	}
}

void init() {
	int M = ..., N = ...;
	out("args", M, N);
	
	for (int i = 0; i < M; i++) {
		out("tailQ", i, 0);
		out("headQ", i, 0);
	}
	
	eval(ferry());
	for (...) {
		eval(vehicle());
	}
}