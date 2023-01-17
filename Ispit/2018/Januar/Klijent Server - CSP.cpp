[p::P||s(i:0..2)::S||k(i:0..99)::K]

P::[
	int served[3] = {0};
	int next = 0;
	
	// pass requests
	*[ true ->
		client(i)?request()->[
			server(next)!clientRequest(i);
			client(i)!next;
		]
		[]
		client(i)?serverServed(j)->[
			served[j]++;
			
			// calculate the next minimum (look at notes)
			[ served[2] < served[1] ->
				[ served[2] < served[0] ->
					next = 2;
				[]
				served[2] >= served[0] ->
					next = 0;
				]
			[]
			served[2] >= served[1] ->
				[ served[1] < served[0]; ->
					next = 1;
				[]
				served[1] >= served[0]; ->
					next = 0;
				]
			]
		]
	]
]

S::*[
	int M = 100;
	int requests[M];
	int head = 0, tail = 0;
	int cnt = 0;
	
	// nedeterministicki ulazi u jednu od ove dve grane
	//pa moze u jednom navratu primiti vise zahteva (kao i drugi serveri)
	p?clientRequest(i)-> [
		request[tail] = i;
		tail = (tail + 1) % M;
		cnt++;
	]
	[] 
	cnt > 0;->[ 
		int i = request[head];
		head = (head + 1) % M;
		cnt--;
		
		// serve request...
		
		client(i)!served();
	]
]

K::*[
	int server;
	p!request();
	p?server;
	
	//...
	
	s(server)?served();
	p!serverServed(served);
]

---------------------------------------------------------------------------------------
Stotinu klijenata K , tri servera S
Koordinator: P
Klijentu salju zahteve procesu P, koji ih naizmenicno prosledjuje serverima
	-> Zahtev dobije server koji je obradio najmanje zahteva
	-> Klijenta obavestava kom serveru je zahtev prosledjen
	-> Podjednaki broj zahteva? Fifo redosled: S1 > S2 >S3
	-> Klijent prima odgovor direktno od servera
	
Trazenje maksimuma tri broja...
Uz red prioriteta
Sta ako su jednaki?
Idem sa desna na levo -> (ako poredim na STROGU nejednakost)
x, y, z --> s3, s2, s1
min = (x < y) ? (x < z ? x : z) : (y < z ? y : z)