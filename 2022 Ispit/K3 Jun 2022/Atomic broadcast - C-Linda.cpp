void init() {
	int C, P, B;
	out("C", C);
	out("P", P);
	out("B", B);
	
	out("tailP", 0);
	out("tailB", 0);
	out("headP", 0);
	
	out("ticket", 0);
	out("next", 0);
	
	for (int i = 0; i < B; i++) {
		out("cnt", i, C);
	}
	for (int i = 0; i < C; i++) {
		eval(consumer(i));
	}
	for (int i = 0; i < P; i++) {
		eval(producer());
	}
}

void producer() {
	int B, C, myT, tailB;
	
	rd("B", ?B);
	rd("C", ?C);

	while (true) {
		T item = produce();
		
		in("ticket", ?myT);
		out("ticket", myT+1);
		
		// wait for turn
		in("next", myT);
		
		in("tailB", ?tailB);
		out("tailB", (tailB+1)%B);
		
		// wait for slot to become available
		in("cnt", tailB, C);
		
		// put item in empty slot
		out("buffer", tailB, item);
		
		// signal all consumers
		for (int i = 0; i < C; i++) {
			out("items", i);
		}
		
		// let next producer through
		out("next", myT+1);
	}
}

void consumer(int id) {
	int B, C;
	int headB = 0, cnt;
	T item;
	
	rd("B", ?B);
	rd("C", ?C);
	
	while (true) {
		// wait for item
		in("items", id);
		
		rd("buffer", headB, ?item);
		
		// increment number of consumers read item
		in("cnt", headB, ?cnt);
		cnt = cnt+1;
		
		if (cnt == C) {
			// take item out of space
			in("buffer", headB, item);
		}
		out("cnt", headB, cnt);
		
		consume(item);
		
		headB = (headB + 1) % B;
	}

}