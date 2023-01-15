chan in, out;

void Process(int id = 0, int num) {
	int min = num, max = num, t;
	
	synch_send out(min)
	synch_send out(max)
	
	receive in(min)
	receive in(max)
	
	synch_send out(min)
	synch_send out(max)
	
	receive in(t)
	receive in(t)
}

void Process(int id = 1...N, int num) {
	int min, max;
	
	receive in(min)
	receive in(max)
	
	if (num < min) {
		min = num;
	}
	if (num > max) {
		max = num;
	}
	
	synch_send out(min)
	synch_send out(max)
}

----------------------------------------------------------
Sinhrona komunikacija
Dva prolaza
Jedan cvor asimetrican (prvi koji salje - ostali primaju)
Prvi cvor na kraju primi min/max i posalje svima
Svaki cvor ima celobrojnu vrednost

2163?

Prvi salje svoj broj sledecem/na kraju prima min/max i salje svima
||2, / /|| -> ||1, / /|| -> ||6, / /|| -> ||3, / /||
||2, / /|| -> ||1, 1 2|| -> ||6, 1 6|| -> ||3, 1 6|| ->
||2, 1 6|| -> ||1, 1 6|| -> ||6, 1 6|| -> ||3, 1 6|| ->
Ne primaj sledecu poruku!


Prvi ima min/max - i opet treba propagirati
Kako da poslednji ima min/max? Raditi obrnuto?


