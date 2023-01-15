chan Phil[5];
chan ForkGet[5];
chan ForkPut[5];

void Philosopher(int i = 0..4) {
	int left = (i + i%2) % 5;
	int right = (i + 1 - i%2) % 5;
	
	while (true) {
		// think
	
		synch_send ForkGet[left]("take");
		synch_send ForkGet[right]("take");
	
		// eat
	
		synch_send ForkPut[left]("put");
		synch_send ForkPut[right]("put");
	}
}

void Fork(int i = 0..4) {
	string t;
	
	while (true) {
		receive ForkGet[i](t);
	
		// fork taken
	
		receive ForkPut[i](t);
	}
}