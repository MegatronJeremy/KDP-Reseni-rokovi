--------------------------------------------
Postoji samo jedna rampa za ulaz i izlaz (get ramp)
N mesta
FIFO

bool enterParking() {
	wait(mutex); 
	// koordinator? sve unutar krit sekcije mutexa je jedna grana
	if (space == 0 && leaving - entering == 0) {
		// u jednom trenutku: N zeli da izadje i N zeli da udje
		// max 2*N zahteva
		signal(mutex);
		return false;
	}
	entering++;
	ticket = getAndIncrementTicket();
	
	signal(mutex);
	
	wait(ramp, ticket);
	
	// enter parking lot
	
	wait(mutex);
	entering--;
	space--;
	signal(mutex);
	
	// let next pass through ramp
	signal(ramp);
}

bool exitParking() {
	wait(mutex);
	leaving++;
	ticket = getAndIncrementTicket();
	
	signal(mutex);
	wait(ramp, ticket);
	
	exit parking lot
	
	wait(mutex);
	leaving--;
	space++;
	signal(mutex);
	
	// let next pass through
	signal(ramp);
}