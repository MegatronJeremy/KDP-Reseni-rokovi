struct Msg {
	int id;
	string op;
}

mbx SB;
Process SingleBathroom {
	Msg msg;
	Deque q;
	int nM = 0, nW = 0, nJ = 0, nC = 0, cnt = 0;
	
	while (true) {
		bool cond;
		if (!q.empty()) {
			switch(q.get_front().op) {
				case "enterMC":
					cond = nW == 0 && nJ == 0 && cnt < N;
					break;
				case "enterWC":
					cond = nM == 0 && nJ == 0 && cnt < N;
					break;
				case "enterJC":
					cond = nW == 0 && nM == 0 && nJ == 0;
					break;
				case "enterCC":
					cond = cnt < N && (nW != 0 || nM != 0);
					break;
				case "exitWC" | "exitMC":
					cond = nC == 0;
					break;
			}
		} else {
			cond = false;
		}
		
		if (cond) {
			msg = q.get_front();
			q.pop_front();
		} else {
			mbx_get(msg, SB, INF, st);
		}
		switch (msg.op) {
			bool childInQueueFront = !q.empty() && q.get_front().op == "enterCC";
			// child in queue front only if no men or women currently executing
			
			case "enterM":
				if (!q.empty() || nW > 0 || nJ > 0 || cnt == N) {
					msg.op = "enterMC";
					if (childInQueueFront) {
						// let blocked child after me
						q.push_front(msg):
					} else {
						q.push_back(msg);
					}
					break;
				}
			case "enterMC":
				nM++;
				cnt++;
				mbx_put(msg, M[msg.id]);
				break;
			case "enterW":
				if (!q.empty() || nM > 0 || nJ > 0 || cnt == N) {
					msg.op = "enterWC";
					if (childInQueueFront) {
						// let blocked child after me
						q.push_front(msg):
					} else {
						q.push_back(msg);
					}
					break;
				}
			case "enterWC":
				nW++;
				cnt++;
				mbx_put(msg, W[msg.id]);
				break;
			case "enterJ":
				if (!q.empty() || nJ > 0 || nW > 0 || nM > 0) {
					msg.op = "enterJC";
					q.push_back(msg);
					break;
				}
			case "enterJC":
				nJ++;
				mbx_put(msg, J);
				break;
			case "enterC":
				if (!q.empty() || nW == 0 && nM == 0 || cnt == N) {
					msg.op = "enterCC";
					q.push_back(msg);
					break;
				}
			case "enterCC":
				nC++;
				cnt++;
				mbx_put(msg, C[msg.id]);
				break;
			case "exitJ":
				nJ--;
				break;
			case "exitC":
				nC--;
				cnt--;
				break;
			case "exitW":
				if (nW == 1 && nC > 0) {
					msg.op = "exitWC";
					// give priority of exiting over everything else
					// only let children exit
					q.push_front(msg);
					break;
				}
			case "exitWC";
				nW--;
				cnt--;
				break;
			case "exitM":
				if (nM == 1 && nC > 0) {
					msg.op = "exitMC";
					// give priority of exiting over everything else
					// only let children exit
					q.push_front(msg);
					break;
				}
			case "exitMC";
				nM--;
				cnt--;
				break;
		}
		
	}
}


-------------------------------------------------------------------------
Pozivanje monitora je standardno... citalac moze napisati za vezbu