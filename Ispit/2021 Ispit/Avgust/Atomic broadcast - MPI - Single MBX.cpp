struct Msg {
	int id;
	string type;
	T val;
}

mbx AB;
Process Coordinator() {
	const int B = ..., N = ...;
	T buf[B];
	int slots[B] = {N};
	int items[N] = {0};
	int tail = 0;
	int head[N] = {0};
	
	Msg msg, msg_prod = NULL;
	boolean st;
	Queue q;
	
	while (true) {
		if (msg_prod != NULL && slots[tail] == N) {
			msg = msg_prod;
			msg_prod = NULL;
		} else if (!q.empty() && items[q.peek().id] > 0) {
			msg = q.poll();
		} else {
			mbx_get(msg, AB, INF, st);
		}
		switch(msg.type) {
			case "put":
				if (slots[tail] < N) {
					msg.type = "putC";
					msg_prod = msg;
					break;
				}
			case "putC":
				slots[tail] = 0;
				buf[tail] = msg.val;
				tail = (tail + 1) % B;
				
				for (int i = 0; i < N; i++) {
					items[i]++;
				}
				
				mbx_put(msg, P);
				break;
			case "get":
				if (items[msg.id] == 0) {
					msg.type = "getC";
					q.add(msg);
					break;
				}
			case "getC":
				items[msg.id]--;
				int ri = head[msg.id];
				msg.val = buf[ri];
				slots[ri]++;
				
				mbx_put(msg, C[msg.id]);
				break;
		}
	}
}

mbx P;
Process producer() {
	Msg msg;
	boolean st;
	
	while (true) {
		T item = produce();
		
		msg.val = item;
		msg.type = "put";
		
		mbx_put(msg, AB);
		
		// wait for OK
		mbx_get(msg, AB, INF, st);
	}
}

mbx C[N];
Process consumer(int id = 0...N-1) {
	Msg msg;
	boolean st;
	
	while (true) {
		msg.type = "get";
		msg.id = id;
		
		mbx_get(msg, AB, INF, st);
		
		T item = msg.val;
		
		consume(item);
	}
}