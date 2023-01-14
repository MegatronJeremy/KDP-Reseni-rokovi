int node(int id) {
	int N;
	rd("N", ?N);
	
	int links[N][N], pair = -1;
	rd("links", ?links);
	
	out("paired", id, false);
	
	bool flag;
	
	// phase one
	for (int i = 0; i < id; i++) {
		if (links[id][i] != true) {
			continue;
		}
		// try to acquire node atomically if unpaired
		if (pair == -1) {
			// acquiring multiple resources, is this ok? YES - NO CIRCULAR ACQUISITION EXISTS
			in("paired", id, false); // nobody can acquire ME after taking this - I AM ACQUIRING (downstram acquisition)
			in("paired", i, ?flag); // i have the correct value and NOBODY can acquire THIS NODE - I HAVE ACQUIRED
			if (flag == false) {
				flag = true;
				pair = i;
			}
			out("paired", i, flag); // i have acquired this node - now the value is correct
			out("paired", id, pair == i); // same here - this is atomic
		}
		
		// send out link-status regardless - NODE NEEDS TO READ
		// ALL LINK-STATUSES, IN CASE PAIRING IN PHASE ONE WAS
		// UNSUCCESSFUL
		out("link-status", id, i, pair == i);
	}
	
	if (pair != -1) // pairing successful in phase one - no need to continue
		return pair;
		
	// phase two
	for (int i = id+1; i < N; i++) {
		if (links[i][id] != true) {
			continue;
		}
		
		// check link-status
		in("link-status", i, id, ?flag);
		if (flag == true) {
			// pair found!
			return i;
		}
	}
	
	return -1; // no pair was found
}

void init() {
	// bidirectional graph
	int N = ...;
	int links[N][N] = ...;
	out("links", links);
	out("N", N);
	for (int i = 0; i < N; i++) {
		eval(node(i));
	}
}



------------------------------------------------------
Cvorovi u grafu koji komuniciraju samo sa susedima
Uparuje se cvor sa jednim susedom
Ostaje neuparen ako su svi ostali susedi upareni

Iz dve faze sa parcijalnim uredjenjem:
(enumerisani cvorovi - eliminacija kruznih zavisnosti)

Prva faza - komunikacija za nizim id-jem:
1.1 Probaj nezauzeti cvor da zauzmes atomicno.
1.2 Posalji svim cvorovima status zauzeca.

Druga faza - komunikacija sa visim id-jem (ako nemam para nakon prve faze):
2.1 Primi sve statuse zauzeca od visih cvorova
2.2 Ako je status potvrdan - cvor je nasao para

