Module dp
	op takeForks(int i)
	op leaveForks(int i)
Body
Process DP {
	int forks[N] = {2};
	while (true) {
		in
		takeForks(int i) and forks(i) == 2 ->
			int left = (i+N-1)%N, right = (i+1)%N;
			forks[left]--; forks[right]--;
		leaveForks(int i) ->
			int left = (i+N-1)%N, right = (i+1)%N;
			forks[left]++; forks[right]++;
		ni
	}
}


void Philosopher(int i = 0..N-1) {
	while (true) {
		// THINK
	
		call takeForks(i);
	
		// EAT
	
		call leaveForks(i);
	}
}