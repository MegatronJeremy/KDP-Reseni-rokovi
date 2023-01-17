[p::P||s(i:0..2)::S||k(i:0..99)::K]

P::[
	int served[3] = {0};
	bool servingRequest[3] = {false};
	int next = 0;
	
	// pass requests
	*[ true ->
		servingRequest[next] == false; client(i)?request()->[
			servingRequest[next] = true;
			server(next)!clientRequest(i);
			client(i)!next;
		]
		[]
		server(i)?requestDone()->[
			servingRequest[i] = false;
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

S::*[
	p?clientRequest(i)-> [
		// serve request...
		
		client(i)!served();
		p!requestDone();
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