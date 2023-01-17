COLLECTOR::{
	int it = 0;
	body space[N] = ...;
	*{ it < M; ->
		int k = 0, j;
		body tmp;
		*{ k < N; worker(i) ? getData(tmp, j) ->
			space[j] = tmp;
			k++;
		}
		it++;
		it < M; -> {
			int j = 0;
			*{ j < N; ->
				bag ! spaceData(space[j]);
				j++;
			}
		}
	}

}