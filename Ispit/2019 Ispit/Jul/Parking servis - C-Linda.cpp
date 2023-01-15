#include <unistd.h>

int VlasnikAutomobila(int parkingTime) {
	int car;
	in("ticket", ?car);
	out("ticket", car+1);
	
	int time;
	rd("time", ?time);
	// uplaceno na neko vreme
	out("uplaceno", car, time+parkingTime, true);
	
	// idem negde...
	
	bool valid;
	in("uplaceno", car, ?time, ?valid);
	
	int kazna = 0;
	if (!valid) {
		// uzmi kaznu ako isteklo
		in("kazna", car, ?kazna);
	}
	
	return kazna;
}

void Kontrolor() {
	int car, time, expiry, kazna = 2000;
	
	while (1) {
		in("uplaceno", ?car, ?time, false);
		
		out("kazna", car, kazna);
	}
}

void Cistac() {
	int time, car;
	
	rd("time", ?time);
	
	while (1) {
		// provera da li istice validni tiket
		while (inp("uplaceno", ?car, ?time, true) == true) {
			out("uplaceno", car, time, false);
		}
		
		// wait for next time interval
		time++;
		rd("time", time);
	}
}


void Tik() {
	sleep(1);
	in("time", ?time);
	out("time", time+1);
}


--------------------------------------------------------
VlasnikAutomobila -> uplatio parkiranje
Kontorlor -> proverava da li je uplaceno parkiranje
Cistac -> cisti prstor torki
Tik -> azurira vreme u prostoru torki