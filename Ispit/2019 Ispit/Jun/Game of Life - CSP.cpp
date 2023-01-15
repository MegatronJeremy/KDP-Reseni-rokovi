[cell(0..N-1)(0..N-1)::CELL||board::BOARD]

BOARD::[
	// potrebno baferovati zahteve
	const int N = ..., B = 16;
	int buf[N][N][B][2] = {0}; // N * N for every cell -> 2 for each iteration -> 8 max pending per iteration
	int head[N][N] = {0};
	int tail[N][N] = {0};
	int cnt[N][N] = {0};
	
	*[ true->
		cell(i)(j)?sendInfo(k, p, status, iter)->[
			int wi = tail[k][p];

			buf[k][p][wi][0] = status;
			buf[k][p][wi][1] = iter;
			
			tail[k][p] = (wi + 1) % B;
			cnt[k][p]++;
		]
		[]
		cnt[i][j] > 0; cell(i)(j)?getInfo()->[
			int ri = head[i][j];
			
			cell(i)(j)!(buf[i][j][ri][0], buf[i][j][ri][1]);
			
			head[i][j] = (ri + 1) % B;
			cnt[i][j]--;
		]
	]
]

CELL::[
	int N = ...;
	int i = ..., j = ...;
	int M = GET_NUM_OF_NEIGHBORS;
	int nb[M][] = GET_NEIGHBORS; 
	int status = 2 * ((double) rand() / (RAND_MAX + 1));
	int myIter = 0, cellIter, cellStatus;
	int alive[2] = {0};
	int cnt[2] = {0};
	
	*[ true->
		active = myIter%2;
		
		// posalji podatke o statusu svim susedima
		[ k < M->
			board!sendInfo(nb[k][0], nb[k][1], status, myIter);
			k++;
		]
		
		// primi podatke o tekucuj iteraciji
		[ cnt[active] < M->
			board!getInfo();
			board?(cellStatus, cellIter);
			
			alive[cellIter%2]+=cellStatus;
			cnt[cellIter%2]++;
		]
		
		[
		status == 1; alive[active] < 2;->
			status = 0;
		[]
		status == 1; alive[active] > 3;->
			status = 0;
		[]
		status == 0; alive[active] == 3;->
			status = 1;
		]
		alive[active] = 0;
		cnt[active] = 0;
		myIter++;
	]
]




--------------------------------------------------------------------
GLAVNI PROBLEM
Slanje je sinhrono - Potrebno uvesti koordinatora
Ne funkcionise parcijalno uredjenje jer nije full mesh
Potreban bafer.