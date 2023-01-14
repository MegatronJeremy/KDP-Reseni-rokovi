struct Msg {
	int id;
	String op;
};

chan DP(Msg);
process ServerDP {
	Queue q;
	int N = ...;
	int forks[N] = {2};
	
	while (true) {
		Msg msg;
		if (!q.empty() && forks[q.peek().id] == 2) {
			msg = q.poll();
		} else {
			receive DP(msg);
		}
		switch (msg.op) {
			case "get":
				if (!q.empty() || forks[msg.id] < 2) {
					msg.op = "getC";
					q.add(msg);
					break;
				}
			case "getC":
				forks[(msg.id + 1) % N]--;
				forks[(msg.id + N - 1) % N]--;
				send phil[msg.id](msg);
				break;
			case "put":
				forks[(msg.id + 1) % N]++;
				forks[(msg.id + N - 1) % N]++;
				break;
		}
	}
}

chan phil[N](Msg);
process Phil(int id:0..N-1) {
	Msg msg;
	msg.id = id;
	while (true) {
		// think
		
		msg.op = "get";
		send DP(msg);
		receive DP(msg);
		
		// eat
		
		msg.op = "put";
		send DP(msg);
	}
}