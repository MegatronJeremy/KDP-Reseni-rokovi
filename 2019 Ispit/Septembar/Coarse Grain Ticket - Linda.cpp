void Process() {
	int myT;
	in("ticket", ?myT);
	out("ticket", myT+1);
	
	in("next", myT);
	
	// critical section
	
	out("next", myT+1);
}

void init() {
	out("ticket", 0);
	out("next", 0);
}

--------------
lmao