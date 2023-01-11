WORKER:: {
	body space[N];
	body result[N];
	int j = 0;
	*{ j != -1 ->
		bag ! getTask();
		body tmp;
		*{ j != -1; bag ? spaceData(tmp, j); ->
			j != -1; -> 
				space[j] = tmp;
		}
		bag ? getData(tmp, j);
		j != -1; ->
			space[j] = tmp;
			CALCULATE::...
			collector ! getData(result[j], j);
	}
}