void man() {
	// enter
	in("mutex");
	
	if (adultQueued() || !manEnterCond()) {
		waitInAdultQ(0);
	}
	
	in("nM", ?nM);
	out("nM", nM+1);
	in("room", ?room);
	out("room", room-1);
	
	signalCode(); // can signal anyone
	
	// use bathroom
	
	// leave
	in("mutex");
	
	if (!manExitCond()) {
		waitInAdultQ(3);
	}
	in("nM", ?nM);
	in("room", ?room);
	out("nM", nM-1);
	out("room", room+1);
	
	signalCode(); // can signal anyone
}

bool manEnterCond() {
	int nW, nJ, room;
	rd("nW", ?nM);
	rd("nJ", ?nJ);
	rd("room", ?room);
	
	return nJ == 0 && nW == 0 && room != 0;
}

bool manExitCond() {
	int nC, nM;
	rd("nM", ?nM);
	rd("nC", ?nC)
	
	return nM != 1 || nC == 0;
}
// kod za zene je analogan
--------------------------------------------------------------------------------

void janitor() {
	// enter
	in("mutex");
	
	if (adultQueued() || !janitorEnterCond()) {
		waitInAdultQ(2);
	}
	in("nJ", ?nJ);
	out("nJ", nJ+1);
	
	out("mutex"); // no one to signal here
	
	// use bathroom
	
	// exit - no needed condition
	in("mutex");
	
	in("nJ", ?nJ);
	out("nJ", nJ-1);
	
	if (adultQueued()) {
		signalAdult(); // can only signal an adult
	} else {
		out("mutex");
	}
}

bool janitorEnterCond() {
	int nM, nW, nJ;
	rd("nM", ?nM);
	rd("nW", ?nW);
	rd("nJ", ?nJ);
	
	return nJ == 0 && nW == 0 && nM == 0;
}

-----------------------------------------------------------------------------

void child() {
	// enter
	in("mutex");
	
	if (childQueued() || !childEnterCond()) {
		waitInChildQ();
	}
	
	int nC;
	in("nC", ?nC);
	out("nC", nC+1);
	in("room", ?room);
	out("room", room-1);
	
	signalCode(); // can signal anyone
	
	// use bathroom
	
	// exit
	in("mutex");
	
	in("nC", ?nC);
	out("nC", nC-1);
	in("room", ?room);
	out("room", room+1);
	
	signalCode(); // can signal anyone
}

bool childEnterCond() {
	int nW, nM, room;
	rd("nW", ?nW);
	rd("nM", ?nM);
	rd("room", ?room);
	
	return (nM != 0 || nW != 0) && room != 0;
}

----------------------------------------------------------------------------
// BLOCKING CODE

void waitInAdultQ(int cond) {
	int myT, tail;
	
	in("ticket", ?myT);
	out("ticket", myT+1);
	
	in("tail", ?tail);
	out("tail", tail+1);
		
	out("adultQ", tail, myT, cond); 
	// 0 - manEnter, 1 - womanEnter, 2 - janitor, 3 - manExit, 4 - womanExit
	
	out("mutex");
	in("next", myT); // wait(x);
}

void waitInChildQ() {
	int myT, tail;
	
	in("ticket", ?myT);
	out("ticket", myT+1);
	
	in("tailC", ?tail);
	out("tailC", tail+1);
		
	out("childQ", tail, myT); 
	// only condition is waiting for an adult
	
	out("mutex");
	in("next", myT); // wait(x);
}

bool adultQueued() {
	int x, y, z;
	return rdp("adultQ", ?x, ?y, ?z) 
}

bool childQueued() {
	int x, y;
	return rdp("childQ", x, y);
}

-------------------------------------------------------------------------------

// SIGNAL CODE

void signalCode() {
	int minRankA, minRankC, x, y;
	bool adultWaiting = false, childWaiting = false;
	
	adultWaiting = rdp("adultQ", ?x, ?minRankA, ?y);
	childWaiting = rdp("childQ", ?x, ?minRankC);
	
	if (adultWaiting && (!childWaiting || minRankA < minRankC)) {
		signalAdult();
	} else if (childWaiting && childEnterCond()) {
		signalChild();
	} else {
		out("mutex"); // signal(mutex)
	}
}

void signalAdult() {
	int head, next, cond;
	
	rd("head", ?head);
	rd("adultQ", head, ?next, ?cond);
	
	bool valid = false;
	switch (cond) {
		case 0: //man entering
			valid = manEnterCond();
			break;
		case 1: //woman entering
			valid = womanEnterCond();
			break;
		case 2: // janitor entering
			valid = janitorEnterCond();
			break;
		case 3: // man exiting
			valid = manExitCond();
			break;
		case 4: // woman exiting
			valid = womanExitCond();
			break;
	}
	
	if (valid) {
		in("adultQ", head, next, cond);
		in("head", head);
		out("head", head+1);
		out("next", next);
	}
}

void signalChild() {
	int head, next;
	
	in("headC", ?head);
	out("headC", head+1);
	in("childQ", head, ?next);
	out("next", next);
}

-------------------------------------------------------------------------
// INIT CODE

void init() {
	out("head", 0); // dva reda
	out("headC", 0);
	out("tail", 0);
	out("tailC", 0);
	
	out("ticket", 0); // ticket for minranks
	
	out("nW", 0);
	out("nM", 0);
	out("nJ", 0);
	out("nC", 0);
}