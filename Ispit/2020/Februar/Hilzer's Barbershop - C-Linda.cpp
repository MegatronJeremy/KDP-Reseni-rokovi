bool customer() {
	int t, myT;
	
	in("cntW", ?t);
	if (t == 20) {
		out("cntW", t);
		// barbershop full
		return false;
	}

	// enter barbershop
	t = t + 1;
	
	// get waiting ticket
	in("ticketC", ?myT);
	out("ticketC", myT+1);
	
	out("cntW", t);
	
	// await barber
	in("next", myT);
	
	// occupy chair
	in("cntW", ?t);
	out("cntW", t-1);
	out("chair", myT);
	
	// get haircur
	in("done", myT);
	
	// pay
	out("customerPaid", myT);
	
	// barber received pay, wait for receipt
	in("receipt", myT);
}

void barber() {
	while (true) {
		int myT = get_next_customer();
		// ...give_haircut
		finish_cut(myT);
	}
}

int get_next_customer() {
	int myT;
	
	// get my ticket
	in("ticketB", ?myT);
	out("ticketB", myT+1);
	
	// get next customer
	out("next", myT);
	
	// wait for chair to be occupied
	in("chair", myT);
}

void finish_cut(int myT) {
	out("done", myT);
	
	// receive pay
	in("customerPaid", myT);
	
	// give receipt
	out("receipt", myT);
}

void init() {
	out("cntW", 0);
	out("ticketC", 0);
	out("ticketB", 0);
	out("next", 0);
	for (int i = 0; i < 3; i++) {
		eval(barber());
	}
	for (...) {
		eval(customer());
	}
	
}