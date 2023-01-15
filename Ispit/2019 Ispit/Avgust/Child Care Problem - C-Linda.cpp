void Roditelj(int dovodi) {
	// bring children - wait(mutex)
	in("mutex");
	int brD, brV;
	rd("brD", ?brD);
	rd("brV", ?brV);
	if (brD + dovodi > 3*brV) {
		int myT;
		in("tail", ?myT);
		out("tail", myT+1);
		out("who", dovodi, myT);
		
		out("mutex"); // signal(mutex)
		in("continue", myT); // wait(x)
	}
	in("brD", ?brD);
	out("brD", brD+dovodi);
	signalCode();
	
	// working...
	
	// take children
	in("mutex");
	in("brD", ?brD);
	out("brD", brD-dovodi);
	signalCode();
}

void Vaspitacica() {
	// start work
	in("mutex");
	int brD, brV;
	rd("brV", ?brV);
	out("brV", brV+1);
	signalCode();
	
	// working
	
	// end work
	in("mutex");
	rd("brD", ?brD):
	rd("brV", ?brV);
	if (brD > 3 * (brV-1)) {
		int myT;
		in("tail", ?myT);
		out("tail", myT+1);
		out("who", 0, myT);
		
		out("mutex");
		in("continue", myT);
	}
	brV--;
	signalCode();
}


// kako implementirati queue?
void signalCode() {
	int who;
	int brD, brV;
	int next;
	rd("head", ?next);
	rd("brD", ?brD);
	rd("brV", ?brV);
	
	if (rdp("who", ?who, next) == true  //who.queue()
		&& (who == 0 && brD <= 3*(brV-1)  // conditions
		|| who != 0 && who + brD <= 3*brV)) {
		in("who", who, next);
		
		in("head", next);
		out("head", next+1);
		
		out("continue", next); // signal(x)
	} else {
		out("mutex"); //mutex.signal()
	}
}

void init() {
	// nadam se da nisam nesto zaboravio
	out("head", 0);
	out("tail", 0);
	out("mutex");
	out("brD", 0);
	out("brV", 0);
	
	for (...) {
		eval(Roditelj(..));
	}
	for (...) {
		eval(Vaspitacica());
	}
}

-----------------------------------------------------------------------
Roditelj i vaspitacica
Na svaka tri deteta barem jedna vaspitacica 
Roditelj ceka ukoliko nema mesta
Vaspitacica napusta zabaviste ako moze
Boze daj mi snage da zavrsim ovaj zadatak