[BAG::BAG || WORKER(0...NW-1)::WORKER || COLLECTOR::COLLECTOR]

BAG::{
	int it = 0;
	body space[N] = ...;
	int request[N] = {0};
	*{ it < M; ->
		int k = 0;
		*{ k < N; ->
			worker(i) ? getTask(); -> {
				request[i] == 0; ->
					request[i] = 1;
					*{ j < N; ->
						worker(i) ! spaceData(space[j], j);
						j++;
					}
				worker(!) ! spaceData(-1, -1);
				worker(i) ! getData(space[k], k);
			}
			k++;
		}
		it++;
		it < M; -> {
			int j = 0;
			*{ j < N; ->
				collector ? spaceData(space[j]);
				request[j] = 0;
				j++;
			}
		}
	}
	*{ worker(i) ? getTask(); ->
		worker(i) ! spaceData(-1, -1);
		worker(i) ! getData(-1, -1;
	}
}