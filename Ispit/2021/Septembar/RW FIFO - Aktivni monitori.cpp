struct Msg {
	int id;
	string op;
}

chan RW;
process ServerRW {
	Queue q;
	Msg msg;
	int nW = 0, nR = 0;
	
	while (true) {
		if (!q.empty() && (nW == 0 && nR == 0 || q.peek().op == "readC" && nW == 0)) {
			msg = q.poll();
		} else {
			receive RW(msg);
		}
		
		switch(msg.type) {
			case "read":
				if (nW > 0 || !q.empty()) {
					msg.op = "readC"
					q.add(msg);
					break;
				} // moze biti u queue citaoc ili pisac pre mene!
			case "readC":
				nR++;
				send R[msg.id](msg); //ok
				break;
			case "write":
				if (nR > 0 || nW > 0) { 
					msg.op = "writeC";
					q.add(msg);
					break;
				} // ako mogu da prodjem queue je prazan!
			case "writeC":
				nW++;
				send W[msg.id](msg); //ok
				break;
			case "endR":
				nR--;
				break;
			case "endW":
				nW--;
				break;
		}
	}
}

chan R[NR]
process Reader(int id = 0..NR) {
	Msg msg;
	
	// start read
	msg.op = "read";
	msg.id = id;
	
	send RW(msg);
	receive RW(msg);
	
	// read
	
	// end read
	msg.op = "endR";
	send RW(msg);
}

chan W[NW]
process Writer(int id = 0..NW) {
	Msg msg;
	
	// start write
	msg.op = "write";
	msg.id = id;
	
	send RW(msg);
	receive RW(msg);
	
	// write
	
	// end write
	msg.op = "endW";
	send RW(msg);
}