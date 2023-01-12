


[GARAGE::GARAGE||CAR(i:1..NC)::CAR]


GARAGE::[
	int space = N, leaving = 0, entering = 0, M = 2*N;
	int request[M];
	int head = 0, tail = 0, cnt = 0;
	bool rampTaken = false;
	*[ true ->
		car(i)?enter() -> [
			space == 0 && leaving - entering == 0; -> [
				car(i)!false;
			]
			[]
			space != 0 || leaving - entering != 0; -> [
				request[tail] = i;
				tail = (tail + 1) % M;
				entering++;
				cnt++;
				car(i)!true;
			]
		]
		[]
		car(i)?exit() -> [
			request[tail] = i;
			tail = (tail + 1) % M;
			leaving++;
			cnt++;
		]
		[]
		cnt > 0 && rampTaken == false; -> [
			int i = request[head];
			car(i)!ok();
			rampTaken = true;
			cnt--;
			head = (head + 1) % M;
		]
		[]
		car(i)?entered() -> [
			rampTaken = false;
			entering--;
			space--;
		]
		[]
		car(i)?exited() -> [
			rampTaken = false;
			leaving--;
			space++;
		]
	]
]

CAR::[
	bool st;
	
	garage!enter();
	garage?st;

	st == false; -> [
		GO_ELSEWHERE
	]
	
	garage?ok();
	
	GET_GARAGE_TICKET::...
	
	garage!entered();
	
	GO_SHOPPING::...

	garage!exit();
	garage?ok();

	PAY_GARAGE_TICKET::...
	
	garage!exited();
]
