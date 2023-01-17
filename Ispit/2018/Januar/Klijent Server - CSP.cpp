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
		server(i)?requestDone()->[
			served[i]++;
			
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

S::[
	const int N = 100; 
	// max number of requests
	
	int requestBuf[N];
	int head = 0, tail = 0;
	int cnt = 0;
	
	*[ p?clientRequest(i)-> [
			// serve request...
			
			client(i)!served();
			p!requestDone();
		]
	]
]

K::*[
	int server;
	p!request();
	p?server;
	
	//...
	
	s(server)?served();
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