------------------------------------------------------------
Jedan pesacki prelaz.
Pesak dosao (T pesaka):
	-> Zeleno svetlo -> predji ulicu.
	-> Crveno svetlo -> Ceka zeleno svetlo
		Prvi pesak 
			-> Nakon K sekundi se pali zeleno svetlo (broj automobila je 0)
		
Automobil dosao (N automobila):
	-> Crveno svetlo -> inkrementiran broj automobila koji su prosli
		-> Proslo C automobila i pesak ceka? -> Upali zeleno svetlo
		
Crveno svetlo?
	-> Zeleno svetlo trajalo G sekundi
		-> Sacekaj da prodju svi pesaci
	
Zeleno svetlo?
-> Pesak ceka i (proslo C automobila ili K sekundi od proslog paljenja) ->
	-> Sacekaj da prodju sva auta
	
Dostupan system_current_time