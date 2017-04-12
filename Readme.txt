12.04.2017.
Promjene:
			-Naziv devAddr u devID jer je taj broj u sustini ID senzora a ne adresa.
			-Napravljena callback f-ja proccesFrame koja poziva f-je updateData ili updateCmd iz gornjeg sloja (apl sloj).
			-Izbacena f-ja aplReadData iz apl sloja (nije bila potrebna).
			-Napravljene posebne f-je za prijem od PC-a i od CC2530 u apl (updateData i updateCmd).
			-U strukturu ubacen clan cmd koji predstavlja komandu(podatke) od strane PC-a.
Problemi:
			-Dostupnost porta apl sloju (da li je dostupan, ako jeste kako mu pristupiti sa apl, ako nije napraviti da bude)
			-CommunicationThread(void const* arg), kako se prosledjuje arg.
			-Da li se mogu mijenjati nazivi clanova strukture u DLLPacket_t?
			
Rijesen RECEPTION dio od najviseg do najnizeg sloja!!!!!!!!!!!!!!! 