[CROSSWALK::CROSSWALK
	|| CAR(i:1..N)::CAR
	|| PEDESTRIAN(i:1..T)::PEDESTRIAN
]

CROSSWALK::[
	int nP = 0, nC = 0, cInterval = 0;
	int wP = 0, wC = 0;
	bool greenLight = false;
	int pReq[T];
	int cReq[N];
	int phead = 0, ptail = 0;
	int chead = 0, ctail = 0;
	int timeToGreen = system_current_time + K;
	int timeToRed;
	
	*[ true->
		wP > 0 && !greenLight && timeToGreen - system_current_time == K;->GREEN_ON
		[]
		greenLight && timeToRed - system_current_time == G; -> [
			greenLight = false;
			*[ nP > 0;->
				wP > 0;->PEDESTRIAN_CROSS
				[]
				PEDESTRIAN(i)?crossed()->[
					nP--;
				]
			]
			timeToGreen = system_current_time + K;
		]
		[]
		PEDESTRIAN(i)?cross()->[
			wP++; nP++;
			pReq[phead] = i;
			phead = (phead+1) % T;
			[ !greenLight && wP == 1;->
				timeToGreen = system_current_time;
			]
		]
		[]
		CAR(i)?cross()->[
			wC++; nC++;
			cReq[chead] = i;
			chead = (chead+1) % N;
		]
		[]
		greenLight && wP > 0;->PEDESTRIAN_CROSS
		[]
		!greenLight && wC > 0;->CAR_CROSS
		[]
		PEDESTRIAN(i)?crossed()->[
			nP--;
		]
		[]
		CAR(i)?crossed()->[
			nC--;
			cInterval++;
			[ cInterval == C; ->
				cInterval = 0;
				GREEN_ON
			]
		]
	]
]

GREEN_ON::[
	greenLight = true;
	*[ nC > 0; ->
		wC > 0; -> CAR_CROSS
		[]
		CAR(i)?crossed() -> nC--;
	]
	timeToRed = system_current_time + G;
]

CAR_CROSS::[
	wC--;
	int i = cReq[ctail];
	ctail = (ctail+1) % N;
	CAR(i)!ok();
]

PEDESTRIAN_CROSS::[
	wP--;
	int i = pReq[ptail];
	ptail = (ptail+1) % T;
	PEDESTRIAN(i)!ok();
]

CAR::[
	CROSSWALK!cross();
	CROSSWALK?ok();
	
	// cross
	
	CROSSWALK!crossed();
]


PEDESTRIAN::[
	CROSSWALK!cross();
	CROSSWALK?ok();
	
	// cross
	
	CROSSWALK!crossed();
]

