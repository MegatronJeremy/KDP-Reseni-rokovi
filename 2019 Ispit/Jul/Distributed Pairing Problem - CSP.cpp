node(i:0..N-1)::[
	int myPair = -1;
	bool links[N][N] = ...;
	bool st;
	
	// prvo salji onima sa manjim indeksom
	int j = 0;
	[ j < i;->
		links[i][j] == true;->[
			bool st = myPair == -1;
			node(j)!pairStatus(st);
		
			node(j)?st;
			[st == false && myPair == -1;-> 
				myPair = j;
			]
		]
		j++;
	]
	
	// pa primaj od vecih indeksa
	j = j+1;
	[ j < N;->
		links[i][j] == true; node(j)?pairStatus(st)->[
			[myPair == -1 && st == false;->
				myPair = j;
				node(j)!true;
			]
			[]
			[myPair != -1 || st == true;->
				node(j)!false;
			]
		]
		j++;
	]
]


---------------------------------------------------------------------
Sve isti problem kao C-Linda: videti jun 2020