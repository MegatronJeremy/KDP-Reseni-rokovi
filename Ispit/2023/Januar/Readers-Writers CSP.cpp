[r(i:1..NR)::R||w(i:1..NW)::W||coord::COORD]


COORD::[
	int nr = 0;
	*[
		(i:1..NR) r(i)?startR() -> nR++;
		[]
		(i:1..NR) r(i)?endR() -> nR--;
		[]
		(i:1..NW) w(i)?startW() -> [
			*[ nR > 0; (j:1..NR) r(j) ? endR() -> 
				nR-- 
			]
			w(i)!ok();
			w(i)?endW();
		]
	]
]

R::*[
	coord!startR();
	//read
	coord!endR();
]

W::*[
	coord!startW();
	coord?ok();
	//write
	coord!endW();
]

------------------------------------------------------
Writers-preferred resenje