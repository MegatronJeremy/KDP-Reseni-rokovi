chan P[N];
void Process(int id = 0..2) {
	int arr[N];
	int i = 0, result;
	int buf[3];
	int first, second, max;
	
	while (i < N) {
		buf[0] = arr[i];
		max = arr[i];
		
		// prvo - prikupljanje trenutnog skupa podataka
		for (int j = 1; j <= 3; j++) {
			send P[(id+1)%3](buf[j-1]); // send out previous value
			
			receive P[id](result); // receive next value
			
			buf[j] = result;
			
			if (result > max) max = result;
		}
		
		if (buf[0] == buf[1] && buf[0] == buf[2]) {
			// stop; everyone has the same value (value in result)
			break;
		}
		
		// find first value greater or equal to max of mins
		while (i < N && arr[i] < max) i++;
		
		// and start over
	}
}


---------------------------------------------------------------
Procesi moraju imati isti kod.
Povezani su u prsten.
Posalji vrednost i vidi da li si primio istu.
Ako jesi - onda je to zajednicko za sve.
Ako nisi - salji prvu vecu vrednost.
Pipeline - primi N-1 poruka kako bi poruka napravila krug.