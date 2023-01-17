void putnik() {
	string dst = ..., src = ...;
	int brLeta = ..;
	int vrSletanja = ...;
	int datSletanja = ...;
	int vrUzletanja = ...;
	int datUzletanja = ...;
	int ukupnoSedista = ...;
	int rbSed = ...;
	int ticket;
	
	// neke parametre bira sam korisnik
	in(?brLeta, src, dst, datUzletanja, vrUzletanja, datSletanja, vrSletanja, ?rbSed, ?ticket);
	
	in("ticket", ?ticket);
	out("ticket", ticket+1); // moj id
	
	out(brLeta, src, dst, vrSletanja, rbSed, ticket);
	
	in("ok", ticket); // sacekaj da bude obradjen zahtev o kupovini
	
	out(brLeta, src, dst, vrSletanja, rbSed, ticket);
}

void kreator_leta() {
	string aerodromUzl = ..., aerodromSl = ...;
	int brLeta;
	int datumUzletanja = ..., vrUzletanja = ...;
	int datumSletanja = ..., vrSletanja = ...;
	int ukupnoSedista = ...;
	int brSed = ...;

	for (int i = 0; i < brSed; i++) {
		in("brLeta", ?brLeta); // autoincrement
		out("brLeta", brLeta+1);
		out(brLeta, aerodromUzl, aerodromSl, datumUzletanja, vrUzletanja, datumSletanja, vrSletanja, rbSed, -1);
		// na kraju -1 - niko nije vlasnik karte
	}
}

struct let_t {
	string aerodromUzl, aerodromSl;
	int brLeta;
	int datumUzletanja, vrUzletanja;
	int datumSletanja, vrSletanja;
	int rbSed;
}

void ukidac_letova() {
	string aerodromUzl, aerodromSl;
	int brLeta;
	int datumUzletanja, vrUzletanja;
	int datumSletanja, vrSletanja;
	int rbSed;
	
	int next = 0;
	struct let_t letSrc, letDst, letDir;
	while (1) {
		// sacekaj da neko kupi kartu
		in(?brLeta, ?aerodromUzl, ?aerodromSl, ?datumUzletanja, ?vrUzletanja, ?datumSletanja, ?vrSletanja, ?rbSed, next);
		
		letDir = new Let(...); // sa ovim gore
		
		int ciljVrUzl = vrUzletanja;
		int ciljDatSlet = datumSletanja;
		
		bool srcFound = false, ok = false;
		// potrebno proveriti moguce letove
		while (!ok) {
			bool passed = false;
			if (!inp(?brLeta, aerodromUzl, aerodromSl, ?datumUzletanja, ?vrUzletanja, ?datumSletanja, ?vrSletanja, ?rbSed, -1)) {
				break;
			}
			
			let_t tmp = new Let(...); // sa podacima gore
			
			if (!dstFound && aps(vrUzletanja  - ciljVrUzl) / 60 <= 3) {
				if (!srcFound) {
					srcFound = true;
					ciljVrUzl = vrUzletanja;
					letSrc = tmp;
					passed = true;
				} else if (datSletanja == ciljDatSlet) {
					letDst = tmp;
					ok = true;
					passed = true;
				}
			}
			
			if (!passed) {
				out(tmp.brLeta, ..., -1); // izbaci sva polja iz leta
			}
		}
		
		if (!ok) {
			if (srcFound) {
				out(letSrc.brLeta, ..., -1);
			}
			out(letDir.brLeta,...);
		}
		
		if (ok) {
			out(letSrc.brLeta, ..., next);
			out(letDst.brLeta, ..., next);
		}
		
		out("ok", next);
		next++;
	}
}

void init() {
	out("ticket", 0);
	out("brLeta", 0);
}

--------------------------------------------------------------------------------
Zadatak kao baze podataka.
U prostor torki - torke za svako sediste svakog leta.

Torka za let sadrzi:
(broj leta, aerodrom uzletanja, aerodrom sletanja, 
datum uzletanja, vreme uzletanja, datum sletanja, vreme sletanja, rb sedista)

Napisati procese (vise) za kreiranje direktnog leta->
	specificira sedista, ukupan broj sedista, kupovinu karte
	
Napisati proces (jedan) koji pokusava da ukine let->
	--> putnici koji su kupili karte za direktan let se rasporedjuju na druge letove
		sa najvise jednim presedanjem (uzletanje -> sletanje)
	--> pritom, uzletanje drugih letova u simetricnom opsegu oko predvidjenog
		trenutka uzletanja leta +-3 sata, i sletanje na odredistu unutar istog dana
	--> putnici se ne moraju obavestavati o promeni leta
	--> nije moguce ukinuti let? potrebno vratiti sve na staro stanje (transakcija)
	
Sistem moze sadrzati druge torke.
