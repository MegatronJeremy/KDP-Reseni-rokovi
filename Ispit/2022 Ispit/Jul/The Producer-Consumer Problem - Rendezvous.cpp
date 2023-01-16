----------------------------------------------------------------------
Server side:

module BoundedBuffer
	op put(T);
	op get(result T);
Body
	Process Buffer {
		T buf[N];
		int head = 0, tail = 0, cnt = 0;
		while (true) {
			in 
			put(item) and cnt < N ->
				buf[tail] = item;
				tail = (tail + 1) % N;
				cnt++;
			[]
			get(item) and cnt > 0 ->
				item = buf[head];
				head = (head+1) % 1;
				cnt--;
			ni
		}
	}
End BoundedBuffer

-----------------------------------------------------------------------
Client side:

Process Consumer {
	// get:
	T item;
	call get(item);
	consume(item);
}

Process Producer 
	// put:
	T item = produce();
	call put(item);
}