Queue q, who;

void Roditelj(int dovodi) {
	// bring children
	wait(mutex);
	if (brD + dovodi > 3*brV) {
		Sem x = 0;
		q.put(x);
		who.put(dovodi);
		signal(mutex);
		wait(x);
	}
	brD += dovodi;
	signalCode();
	
	// working
	
	// take children
	wait(mutex);
	brD -= dovodi;
	signalCode();
}

void Vaspitacica() {
	// start work
	wait(mutex);
	brV++;
	signalCode();
	
	// working
	
	// end work
	wait(mutex);
	if (brD > 3 * (brV-1)) {
		Sem x = 0;
		q.put(x);
		who.put(0);
		signal(mutex);
		wait(x);
	}
	brV--;
	signalCode();
}

void signalCode() {
	if (who.queue()
		&& (who.peek() != 0 && who.peek() + brD <= 3*brV 
		|| who.peek() == 0 && brD <= 3*(brV-1))) {
		who.poll();
		q.poll().signal();
	} else {
		mutex.signal();
	}
}