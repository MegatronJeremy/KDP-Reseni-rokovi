-->Na strani servera se uvek definise interfejs procedura koje ce klijent pozivati.
Klijent uvek poziva serverske procedure na isti nacin (i njemu je taj poziv procedura
potpuno transparentan od nacina implementacije).

-->Prva varijanta (klasicni RPC):
Funkcionise tako sto se na drugoj strani za svaki zahtev klijenta napravi nova nit
koja obradjuje taj zahtev, i na kraju salje odgovor klijentu.
Konkurentnije resenje, ali potrebno sinhronizovati pokrenute procese koji dele adresni
prostor servera.

-->Druga varijanta (rendezvous):
Postoji samo jedna nit na drugoj strani, koja opsluzuje zahteve kada su ispunjeni odredjeni 
uslovi, i kada je taj zahtev primljen (postoji guard nalik na CSP).
Kako samo jedna nit obradjuje zahteve klijenata i salje odgovore, nije potrebna nikakva sinhronizacija
na strani servera.
