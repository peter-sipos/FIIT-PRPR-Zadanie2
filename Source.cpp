#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct zaznam //definovanie struktury
{
	char kategoria[53];
	char znacka[53];
	char predajca[103];
	int cena;
	int rok;
	char stav[203];
	struct zaznam *dalsi; 
} ZAZNAM;

int funkcia_pocet_zaznamov(FILE **subor)
{
	int pocet = 0;
	char naj_riadok[203];
	while (fgets(naj_riadok, 203, *subor) != NULL) 
		if (naj_riadok[0] != '\n') 
			pocet++;
	rewind(*subor);
	return pocet / 7;	
	/* nacitavam po riadku az do konca suboru, na konci delim 7  lebo v subore ma 
	zaznam 7 riadkov (6 s udajmi, jeden ktory to oddeluje. zaroven mam podmienku,
	ze ak som na prazdnom riadku (teda na konci suboru je enter navyse), tak sa to nezaratava*/
}

void nacitaj(FILE **subor, ZAZNAM **prvy, ZAZNAM **aktualny, int *pocet_zaznamov)
{
	if ((*subor = fopen("auta.txt", "r")) == NULL)	
		printf("Zaznamy neboli nacitane\n");
	else
	{
		int i, pocitadlo = 0, udaj_int;
		char udaj_string[203];
		*pocet_zaznamov = funkcia_pocet_zaznamov(subor);

		if (*prvy != NULL) //ak existuje nejaky list, tak ho najprv uvolnim
		{
			for (i = 1; i <= *pocet_zaznamov; i++)  //lebo potrebujem vymazat cely list, teda kazdy zaznam
			{
				if ((*prvy)->dalsi == NULL) 
				{
					free(*prvy);
					break;	//ak bol v liste uz len jeden zaznam, tak ho uvolni a pokracuj vo funkcii
				}
				*aktualny = (*prvy)->dalsi;	
				free(*prvy);
				*prvy = *aktualny;
			}
		}
		
		if (*pocet_zaznamov == 0)	//ak subor neobsahuje ani jeden zaznam
		{
			printf("Nacitalo sa 0 zaznamov\n");
			return;
		}


		if ((*prvy = (ZAZNAM*)malloc(sizeof(ZAZNAM))) == NULL) {
			printf("Nedostatok pamate\n");
			return;	}

		fgets(udaj_string, 203, *subor);
		fgets(udaj_string, 203, *subor);
		strcpy((*prvy)->kategoria, udaj_string);
		fgets(udaj_string, 203, *subor);
		strcpy((*prvy)->znacka, udaj_string);
		fgets(udaj_string, 203, *subor);
		strcpy((*prvy)->predajca, udaj_string);
		fscanf(*subor, "%d", &udaj_int);
		getc(*subor);
		(*prvy)->cena = udaj_int;
		fscanf(*subor, "%d", &udaj_int);
		getc(*subor);
		(*prvy)->rok = udaj_int;
		fgets(udaj_string, 203, *subor);
		if (strchr(udaj_string, '\n') == NULL)	
			strcat(udaj_string, "\n");
		/*zisti, ci je v poslednom riadku enter. ak nie, 
		tak ho tam pridaj (osetrenie pre riadok na konci suboru)*/
		strcpy((*prvy)->stav, udaj_string);
		pocitadlo++;

		*aktualny = *prvy;
		for (i = 2; i <= *pocet_zaznamov; i++)
		{
			if (((*aktualny)->dalsi = (ZAZNAM*)malloc(sizeof(ZAZNAM))) == NULL) {
				printf("Nedostatok pamate\n");
				return;	}

			*aktualny = (*aktualny)->dalsi;
			fgets(udaj_string, 203, *subor);
			fgets(udaj_string, 203, *subor);
			strcpy((*aktualny)->kategoria, udaj_string);
			fgets(udaj_string, 203, *subor);
			strcpy((*aktualny)->znacka, udaj_string);
			fgets(udaj_string, 203, *subor);
			strcpy((*aktualny)->predajca, udaj_string);
			fscanf(*subor, "%d", &udaj_int);
			getc(*subor);
			(*aktualny)->cena = udaj_int;
			fscanf(*subor, "%d", &udaj_int);
			getc(*subor);
			(*aktualny)->rok = udaj_int;
			fgets(udaj_string, 203, *subor);		
			if (strchr(udaj_string, '\n') == NULL)	
				strcat(udaj_string, "\n");
			strcpy((*aktualny)->stav, udaj_string);
			pocitadlo++;
		}

		rewind(*subor);
		(*aktualny)->dalsi = NULL;
		printf("Nacitalo sa %d zaznamov\n", pocitadlo);
	}
}

void vypis(ZAZNAM *prvy, int pocet_zaznamov) 
{
	int  i;
	ZAZNAM *aktualny = prvy; 
	for (i = 1; i <= pocet_zaznamov; i++)
	{
		printf("%d.\n", i);
		printf("kategoria: %s", aktualny->kategoria);
		printf("znacka: %s", aktualny->znacka);
		printf("predajca: %s", aktualny->predajca);
		printf("cena: %d\n", aktualny->cena);
		printf("rok_vyroby: %d\n", aktualny->rok);
		printf("stav_vozidla: %s", aktualny->stav);
		aktualny = aktualny->dalsi;
	}
}

void zmaz(ZAZNAM **prvy, int *pocet_zaznamov)
{
	ZAZNAM *docasny, *pred;
	char manufacturer[53], znacka_aktualna[53];
	int pocitadlo = 0;
	scanf("%s", manufacturer); 

	if (*prvy == NULL) //ak zoznam neobsahuje ziaden zaznam, tak rovno vypis ze sa ich vymazalo 0
	{
		printf("Vymazalo sa %d zaznamov\n", pocitadlo);
		return;
	}

	docasny = *prvy;
	strcpy(znacka_aktualna, docasny->znacka);

	while (docasny != NULL) //kym nebudes na konci zoznamu
	{
		strcpy(znacka_aktualna, (*prvy)->znacka);
		if (*prvy != NULL && (strstr(_strupr(znacka_aktualna), _strupr(manufacturer)) != NULL))
		{
			//ak sa podretazec nachadza uz v prvom zazname
			if (docasny->dalsi == NULL)
			{
				free(docasny);
				*prvy = NULL;
				docasny = NULL;
				pocitadlo++;
				*pocet_zaznamov = *pocet_zaznamov - 1;
				break;
				/*ak bol 1. zaznam zaroven aj posedlny, 
				tak ho uvolni (cim ho zaroven aj vzmaze) a pokracuj vo funkcii*/
			}
			else
			{
				(*prvy) = docasny->dalsi;
				free(docasny);
				docasny = *prvy;
				pocitadlo++;
				*pocet_zaznamov = *pocet_zaznamov - 1;
				continue;
				/*ak 1.zaznam nebol zaroven posledny,
				tak ho uvolni a zaciatok zoznamu nasmeruj na byvaly 2. zaznam*/
			}
		}
		pred = docasny;
		docasny = docasny->dalsi; 
		if (docasny == NULL)	
			break;
		/*ak som podretazec nenasiel ani v poslednom zazaname, tak som ho aj tak posunul
		na dalsi, dalsi zaznam, ktory je ale NULL. docasny je teda NULL. v tom pripade pokracuj vo funckii*/
		strcpy(znacka_aktualna, docasny->znacka);
		if (docasny != NULL && (strstr(_strupr(znacka_aktualna), _strupr(manufacturer)) != NULL))
		{
			pred->dalsi = docasny->dalsi;
			free(docasny);
			pocitadlo++;
			*pocet_zaznamov = *pocet_zaznamov - 1;

			if (pred->dalsi == NULL) 
				break;
			/*ak som vymazal posledny prvok, a teda predchadzajuci dalsi 
			ukazuje na NULL, som logicky na konci listu a teda mozem ukoncit cyklus*/
			docasny = pred;
		}
	}
	printf("Vymazalo sa %d zaznamov\n", pocitadlo);
}

void pridaj(ZAZNAM **prvy, int *pocet_zaznamov) 
{
	int i, pozicia, udaj_int;
	char udaj_string[203], enter[3];
	ZAZNAM *aktualny, *novy;
	if ((novy = (ZAZNAM*)malloc(sizeof(ZAZNAM))) == NULL) {
		printf("Nedostatok pamate\n");
		return;
	}

	scanf("%d", &pozicia);	//nacitavanie
	getchar();
	fgets(udaj_string, 203, stdin);
	strcpy(novy->kategoria, udaj_string);
	fgets(udaj_string, 203, stdin);
	strcpy(novy->znacka, udaj_string);
	fgets(udaj_string, 203, stdin);
	strcpy(novy->predajca, udaj_string);
	scanf("%d", &udaj_int);
	getchar();							
	novy->cena = udaj_int;
	scanf("%d", &udaj_int);
	getchar(); 
	novy->rok = udaj_int;
	fgets(udaj_string, 203, stdin);
	ungetc('\n', stdin);
	strcpy(novy->stav, udaj_string);

	if (*prvy == NULL) //ak je zoznam prazdny, pridavany prvok daj na zaciatok
	{
		*prvy = novy;
		novy->dalsi = NULL;
		*pocet_zaznamov = *pocet_zaznamov + 1;
		return;
	}
		
	if (pozicia > *pocet_zaznamov) //pridavanie na koniec
	{
		aktualny = *prvy;
		while (aktualny->dalsi != NULL)
			aktualny = aktualny->dalsi;
		novy->dalsi = NULL;
		aktualny->dalsi = novy;
		*pocet_zaznamov = *pocet_zaznamov + 1;
	}
	else
	{
		aktualny = *prvy;
		if (pozicia == 1) {
			novy->dalsi = *prvy;
			(*prvy) = novy;
			*pocet_zaznamov = *pocet_zaznamov + 1;
		}
		else {
			for (i = 1; i < pozicia-1; i++)
				aktualny = aktualny->dalsi;
			novy->dalsi = aktualny->dalsi;
			aktualny->dalsi = novy;
			*pocet_zaznamov = *pocet_zaznamov + 1;
		}
	}
}

void hladaj(ZAZNAM *prvy)
{
	int price, pocitadlo=0;
	ZAZNAM *aktualny = prvy;
	scanf("%d", &price);
	while (aktualny != NULL)
	{
		if (aktualny->cena <= price)
		{
			pocitadlo++;
			printf("%d.\n", pocitadlo);
			printf("kategoria: %s", aktualny->kategoria);
			printf("znacka: %s", aktualny->znacka);
			printf("predajca: %s", aktualny->predajca);
			printf("cena: %d\n", aktualny->cena);
			printf("rok_vyroby: %d\n", aktualny->rok);
			printf("stav_vozidla: %s", aktualny->stav);
		}
		aktualny = aktualny->dalsi;
	}
	if (pocitadlo == 0)
		printf("V ponuke su len auta s vyssou cenou\n"); 
}

void zmena(ZAZNAM *prvy, int pocet_zaznamov)
{
	getchar(); //potrebujem to tu, lebo inak prvy fgets berie este enter po a-cku
	char znacka_hladana[53], udaj_string[203];
	int cena_hladana, udaj_int, pocitadlo=0, i;
	ZAZNAM *aktualny = prvy, *novy;

	fgets(znacka_hladana, 53, stdin); 
	scanf("%d", &cena_hladana);
	getchar();
	if ((novy = (ZAZNAM*)malloc(sizeof(ZAZNAM))) == NULL) {
		printf("Nedostatok pamate\n");
		return;
	}
	//nacitavam nove udaje
	fgets(udaj_string, 203, stdin);
	strcpy(novy->kategoria, udaj_string);
	fgets(udaj_string, 203, stdin);
	strcpy(novy->znacka, udaj_string);
	fgets(udaj_string, 203, stdin);
	strcpy(novy->predajca, udaj_string);
	scanf("%d", &udaj_int);
	getchar();
	novy->cena = udaj_int;
	scanf("%d", &udaj_int);
	getchar();
	novy->rok = udaj_int;
	fgets(udaj_string, 203, stdin);
	ungetc('\n', stdin);
	strcpy(novy->stav, udaj_string);

	for (i = 1; i <= pocet_zaznamov; i++)
	{
		if ((strcmp(aktualny->znacka, znacka_hladana)==0) && (aktualny->cena == cena_hladana))
		{
			//ak existuje zaznam s hladanou znackou a cenou tak nahrad stare udaje
			strcpy(aktualny->kategoria, novy->kategoria);
			strcpy(aktualny->znacka, novy->znacka);
			strcpy(aktualny->predajca, novy->predajca);
			aktualny->cena = novy->cena;
			aktualny->rok = novy->rok;
			strcpy(aktualny->stav, novy->stav);
			pocitadlo++;
		}
		aktualny = aktualny->dalsi;
	}
	printf("Aktualizovalo sa %d zaznamov\n", pocitadlo);
}

int main()
{
	FILE *auta=NULL;
	char znak_prikazu;
	ZAZNAM *prv=NULL, *akt=NULL;
	int pocet_prvkov = 0, i;

	scanf("%c", &znak_prikazu);
	while (znak_prikazu != 'k')
	{
		switch (znak_prikazu)
		{
		case 'n': nacitaj(&auta, &prv, &akt, &pocet_prvkov); getchar(); break;
		case 'v': vypis(prv, pocet_prvkov); getchar(); break;
		case 'p': pridaj(&prv, &pocet_prvkov); getchar(); break; 
		case 'z': zmaz(&prv, &pocet_prvkov); getchar(); break;
		case 'h': hladaj(prv); getchar(); break;
		case 'a': zmena(prv, pocet_prvkov); getchar(); break;
		default: getchar(); printf("neexistujuca funkcia\n");
		}
		scanf("%c", &znak_prikazu);
	}

	if (auta != NULL)
		fclose(auta);

	if (prv != NULL)
	{
		for (i = 1; i <= pocet_prvkov; i++)  //lebo potrebujem vymazat cely list, teda kazdy zaznam
		{
			if (prv->dalsi == NULL)
			{
				free(prv);
				break;
			}
			akt = prv->dalsi;
			free(prv);
			prv = akt;
		}
	}

	return 0;
}