void init() {
	int C, P, B;
	int tailP = 0, tailB = 0;
	sem pSem[P] = {1};
	sem mutexP = 1;
	sem mutexB = 1;
	sem mutexC = 1;
	int buffer[B];
	int cnt[B] = {0};
	int headP = 0;
}

void producer() {
	while (true) {
		int item = produce();
		
		// get pass
		mutexP.wait();
		int id = tailP; // get id
		tailP = (tailP+1) % P;
		mutexP.signal();
		
		pSem[id].wait(); // wait for turn (and slot to become available)
		
		mutexB.wait();
		int ind = tailB;
		tailB = (tailB+1) % B;
		mutexB.signal();
		
		buffer[ind] = item;
		
		// signal all producers
		for (int i = 0; i < C; i++) {
			items[i].signal();
		}
	}
}


void consumer(int i) {
	int headB = 0;

	while (true) {
		items[i].wait();
		
		int item = buffer[headB];
		
		mutexC.wait();
		cnt[headB]++;
		if (cnt[headC] == 5) {
			cnt[headC] = 0;
			pSem[headP].signal();
			headP = (headP + 1) % P;
		}
		mutexC.signal();
		
		consume(item);
		
		headB = (headB + 1) % B;
	}
}