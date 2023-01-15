struct msg {
	int id;
	string op;
	T data[M];
}


chan PR(msg);
Process prihvatnik() {
	T arr[N];
	int head = 0, tail = 0, cnt = 0;
	const int M = (N-1)/2+1;
	msg m;
	Queue q;
	
	while (true) {
		if (!q.empty() && 
		(m.op == "putC" && cnt != N || m.op == "getC" && cnt >= M)) {
			m = q.pop();
		} else {
			receive PR(m);
		}
		
		switch (m.op) {
			case "put":
				if (cnt == N) {
					m.op = "putC";
					q.put(m);
					break;
				}
			case "putC":
				arr[tail] = m.data[0];
				tail = (tail + 1) % N;
				cnt++;
				send P[m.id](m);
				break;
			case "get":
				if (cnt < M) {
					m.op = "getC";
					q.put(m);
					break;
				}
			case "getC":
				for (int i = 0; i < M; i++) {
					m.data[i] = arr[head];
					head = (head + 1) % N;
					cnt--;
				}
				send P[m.id](m);
				break;
		}
	}
}

chan P[NP](msg);
Process p(int i = 0..NP-1) {
	msg m;
	
	m.id = i;
	
	// put
	m.op = "put";
	m.data[0] = ...;
	send PR(m);
	receive P[i](m);
	
	...
	
	// get
	m.op = "get";
	send PR(m);
	receive P[i](m);
}

------------------------------------------------------------------------
Upis pojedinacnih podataka.
Citanje samo kada se napuni do polovine, pa se odjednom ocita pola 
prihvatnika.