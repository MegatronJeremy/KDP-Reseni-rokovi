void getNextCustomer() {
	int x;
	in("next", ?x);
	out("next", x+1);

	out("barberA", x);

	in("chairOcc");
}

void giveHaircut() {
	...
}

void finishCut() {
	out("doorOpen");
	
	in("customerLeft");
}

void barber() {
	while (true) {
		getNextCustomer();
		giveHaircut();
		finishCut();
	}
}


---------------------------------------

bool customer() {
	// sit in chair
	int n, m, myT;
	rd("chairsN", ?m);
	
	// mutex.wait()
	in("chairsTaken", ?n);
	if (n == m) {
		out("chairsTaken", n);
		return false;
	}

	in("ticket", ?myT);
	out("ticket", myT+1);
	
	// mutex.signal()
	out("chairsTaken", n+1);
	
	// sit in barber chair
	in("barberA", myT);
	
	// signal barber
	out("chairOcc");
	
	// take haircut
	
	// wait for end
	in("doorOpen");
	
	// exit
	out("customerLeft");
}

----------------------------------

void init() {
	out("ticket", 0);
	out("next", 0);
	out("chairsN", 5);
	out("chairsTaken", 0);
	eval(barber());
	for (...)
		eval(customer());
}