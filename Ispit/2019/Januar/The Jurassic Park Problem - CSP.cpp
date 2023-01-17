[vis(0..m-1)::VIS||car(0..n-1)::CAR||park::PARK]

VIS::[
	// walk around
	
	int myCar;
	park!getCar();
	park?myCar;
	
	// ride around
	
	car(myCar)!done();
	
	// leave park
]

CAR::*[
	// wait for passenger
	// kome saljem? - direktno je imenovanje - potreban intermedijum
	park!avail();
		
	vis(i)?done();
]

PARK::[
	const int N = ...;
	int carsAvail[N];
	int head = 0, tail = 0, cnt = 0;
	
	*[ true ->
		car(i)?avail()->[
			carsAvail[tail] = i;
			tail = (tail + 1) % N;
			cnt++;
		]
		[]
		cnt > 0; vis(i)?getCar()->[
			vis(i)!carsAvail[head];
			head = (head+1) % N;
			cnt--;
		]
	]
]















-----------------------------------------------------------------------
Park: m posetilaca, n bezbednih vozila -> svako primi jednog putnika

Posetilac i vozilo zasebni procesi.
Naidje slobodno vozilo -> posetilac se vozi po parku.
Na krau voznje -> vozilo slobodno i prima sledeceg posetioca.