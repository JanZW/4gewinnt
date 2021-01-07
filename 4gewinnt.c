/*	Jan Philipp Zwank - Benotung erwünscht
	Thomas Labermeyer - Benotung erwünscht


	1 2 3 4 5 6		
	- - - - - -		
	- - - - - -			
	- - - - - -		
	O O X - - - 
	X O X X X O		
	X X O O X O			
	
*/
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct spieler{			// Datentyp  "spieler"
	char name[20];
	int zuege;
};
//--------------------------------------------------------------------------------
// Funktions-Prototypen

void highscore (int sz, int az, char* sp1, char* sp2, int* st);
int nochmal (void);
void pipe (void);
void spielerabfrage (char* s1, char* s2, int* z, int* s);
int ausgabe (char** m,int z, int s, char* s1, char*s2, int az, char* info, int* st);
int check (char** m, int z0, int s0, int z, int s, int* h);
int rekursion (char** m,  int zeilen, int spalten, int* h);

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// Funktion main

int main (void)
{
	int zeilen=4, spalten=4;
	char spieler1[20]= "Jan" , spieler2[20]= "Tom";
	int amzug=0;//c
	int spalteneingabe;//c
	int status;
	//int* hoehe=calloc(spalten, sizeof(int));
	char info[40]="";
	int spielzuege;

Anfang:
; // nach Label muß eine Anweisung stehen, keine Deklaration

		int* hoehe=calloc(spalten, sizeof(int));
		memset(hoehe,0,spalten*sizeof(int));
		//for(int i=0;i<spalten;i++) printf("%d ",hoehe[i]); //Testausgabe

		status=0;
		spielzuege=0;
		amzug=0;

	spielerabfrage(spieler1,spieler2,&zeilen, &spalten);

					//Erstellen einer zweidimensionalen Matrix
	char** matrix=(char**)calloc(zeilen, sizeof(char*));
	for(int i=0;i<zeilen;i++) 
		matrix[i]=(char*)calloc(spalten, sizeof(char));	
	
	for(int i=0; i<zeilen; i++)				//belegung mit '-'
		for(int j=0;j<spalten; j++) matrix[i][j]='-';

	while(status==0)		//Beginn Spielschleife, 0=Spiel läuft, 1=Gewinner steht fest, evtl. 2=nichts geht mehr
	{
		spalteneingabe=ausgabe(matrix, zeilen, spalten, spieler1, spieler2,amzug, info, &status);

		spielzuege++;						//Spielzuege fuer Highscore hochzaehlen

		strncpy(info,"",39);				//Fehlermeldung löschen bzw auf LEER setzen

		if(hoehe[spalteneingabe-1]==zeilen)		//Neubelegung Fehlermeldung
		{	strncpy(info,"Spalte voll, nochmal",39);
			continue;
		}
		
		if(spalteneingabe<0 || spalteneingabe>spalten)	//Neubelegung Fehlermeldung
		{	strncpy(info,"Ungueltige Eingabe, nochmal",39);
			continue;
		}
								//Matrix füllen
		if(amzug==0)
			 matrix[zeilen-1-hoehe[spalteneingabe-1]][spalteneingabe-1]='X';
		else 	 matrix[zeilen-1-hoehe[spalteneingabe-1]][spalteneingabe-1]='O';

		hoehe[spalteneingabe-1]++;			//aktuelle Befüllhöhe - Array aktuallisieren

		status=check(matrix, hoehe[spalteneingabe-1], spalteneingabe, zeilen, spalten, hoehe);	// Spielende ??		
		if (status ==0 && spielzuege > zeilen*spalten - 3)     status=rekursion(matrix, zeilen, spalten, hoehe);


		if(amzug==0) amzug=1;				//Spielerwechsel 1/2
		else amzug=0;
	}
	//printf("%d",status);	
	
	ausgabe(matrix, zeilen, spalten, spieler1, spieler2,amzug, info, &status);	//Gewinnerbildschirm

	if(zeilen==6 && spalten==6) highscore(spielzuege, amzug, spieler1, spieler2, &status);


	
	status=nochmal();	
	if(status==5) goto Anfang;

}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// Funktion rekursion
/*
Es werden zwei Kopien des aktuellen Spielfeldes erstellt. das nächste freie Feld wird gesucht
und einmal mit X und einmal mit O bestückt. Es wird geprüft, ob ein Spielende (Sieg) erreicht ist.
Ist dies der Fall wird der Status=0 an main übermittelt und das Spiel wird fortgesetzt. 
Falls noch kein Sieg vorliegt, wird dieses Verfahren rekursiv weitergeführt. Wird Spielfeldes
voll bestückt und es liegt kein Sieg vor wird der Status=2 weitergegeben. Erhält der vorangegangene
Rekursionsschritt beide Male eine 2, so wird Status=2 an main übergeben, was einen Spielabbruch mit
Unentschieden bewirkt.
*/
int rekursion (char** m,  int zeilen, int spalten, int* h)
{	int status1, status2;

	int* hoehe=calloc(spalten, sizeof(int));
	for (int i = 0; i<spalten; i++)
		hoehe[i]= h[i];

					//Erstellen einer zweidimensionalen Matrix
	char** matrix1=(char**)calloc(zeilen, sizeof(char*));
	for(int i=0;i<zeilen;i++) 
		matrix1[i]=(char*)calloc(spalten, sizeof(char));	
					//Erstellen einer zweidimensionalen Matrix
	char** matrix2=(char**)calloc(zeilen, sizeof(char*));
	for(int i=0;i<zeilen;i++) 
		matrix2[i]=(char*)calloc(spalten, sizeof(char));	

	for (int i=0; i<zeilen; i++)
		for (int j=0; j<spalten; j++) {
			matrix1[i][j] = m[i][j];
			matrix2[i][j] = m[i][j];
		}


	int i, j;
	for (i=zeilen-1; i>=0; i--){
		for (j=0; j<spalten; j++) 
			if (matrix1[i][j] == '-')  break;
		if (matrix1[i][j] == '-')  break;
	}
	if(i==-1) return 2;

	matrix1[i][j]='X';
	matrix2[i][j]='O';
	hoehe[j]++;

	status1 = check(matrix1, zeilen-i, j+1 , zeilen, spalten, h);	// Spielende ??	
	if (status1==1) 
		return 0;	// spiel geht weiter
	status2 = check(matrix2, zeilen-i, j+1 , zeilen, spalten, h);	// Spielende ??	
	if (status2==1) 
		return 0;	// spiel geht weiter
	if (status1==2 && status2==2)
		return 2;

	status1=rekursion (matrix1, zeilen, spalten, hoehe);

	status2=rekursion (matrix2, zeilen, spalten, hoehe);

	if (status1==0 || status2==0)
		return 0;
	if (status1==2 && status2==2)
		return 2;

	return 3;		//wird nie erreicht
}
//--------------------------------------------------------------------------------
// Funktion highscore

void highscore (int sz, int az, char* sp1, char* sp2, int* st){
	struct spieler* liste=calloc(3, sizeof(struct spieler)); 
	FILE *fh=fopen("highscore.txt", "r");
		for(int i=0; i<3;i++){			// Array initialisieren, egal wieviel bereits in Datei
			strncpy(liste[i].name,"Dummy",19);
			liste[i].zuege=99;
		}
	if(fh){	for(int i=0; i<3; i++){			// Auslesen Datei
			fscanf(fh, "%19s %d", liste[i].name, &liste[i].zuege);
			if(feof(fh)) break;
		}	
		fclose(fh);
	}
	

	if(*st==1)			// nur falls tatsächlich ein Sieger existiert
	if (sz < liste[2].zuege) {
		char winner[20];			// falls besser als Dritter
		if (az==1) strncpy(winner, sp1, 19);
		else strncpy(winner, sp2, 19);
		strncpy(liste[2].name, winner, 19);
		liste[2].zuege=sz;
		if (sz < liste[1].zuege) {
			strncpy (liste[2].name, liste[1].name,19); // zus. falls besser als Zweiter
			liste[2].zuege=liste[1].zuege;
			strncpy(liste[1].name, winner, 19);
			liste[1].zuege=sz;
			if (sz < liste[0].zuege) {
				strncpy (liste[1].name, liste[0].name,19); // zus. falls besser als Erster
				liste[1].zuege=liste[0].zuege;
				strncpy(liste[0].name, winner, 19);
				liste[0].zuege=sz;
			}
		}
	}
	
	FILE *fh2=fopen("highscore.txt", "w");
	if (!fh2) printf("Datei-Fehler (schreiben)\n");
	else  {	 for(int i=0; i<3; i++)			
			fprintf(fh2, "%-20s %3d\n", liste[i].name, liste[i].zuege);
		fflush(fh2);
		fclose(fh2);
	      }



	printf("\n");
 	for(int i=0; i<3; i++)			
			printf("Name: %-20s %3d\n", liste[i].name, liste[i].zuege);

	free(liste);	// Array Speicherplatz freigeben
}
//--------------------------------------------------------------------------------
// Funktion  nochmal

int nochmal (void){
	char c;
	printf("       möchtet ihr nochmal spielnen? (j/n)");
	scanf(" %c", &c);  
	if(c=='j') {
		pipe();
		return 5;
	}	
	return 0;	// wegen Rückgabewert derFkt.: int
}
//--------------------------------------------------------------------------------
// Funktion pipe - Löscht überflüssige Daten im Eingabestream

void pipe (void){
	char c='a';
	while(c!='\n') scanf("%c",&c);		// Auslesen input-stream ins "NICHTS"
 }

//--------------------------------------------------------------------------------
// Funktion spielerabfrage

void spielerabfrage (char* s1, char* s2,int* z, int* s){
	system("clear");
	printf("Fuer den Spielmodus 6x6 existiert eine Highscoreliste!\n\n");
	printf("Spieler 1 Namen eingeben:");
		scanf("%19s",s1); 
		pipe();							// alles was über 19 Zeichen ist loeschen
	printf("Spieler 2 Namen eingeben:");
		scanf("%19s",s2); 
		pipe();							// ales was über 19 Zeichen ist loeschen
		
	printf("Feldgroesse: Zeilen: ");
	int err=0;	
	while(err!=1){
		err=scanf(" %d",z);
		if(*z>9 || *z<4){
			printf("Feldgroese zwischen 4 und 9! ");
			err=0;
		}
		pipe();							// alles was über 19 Zeichen ist loeschen
	}
	printf("Feldgroesse: Spalten: ");
	err=0;
	while(err!=1){					// Eingabebe bis gültier Wert
		err=scanf(" %d",s);
		if(*s>9 || *s<4){			//Eingabebegrenzung
			printf("Feldgroese zwischen 4 und 9! ");
			err=0;
		}
		pipe();							// alles was über 19 Zeichen ist loeschen
	}
}

//--------------------------------------------------------------------------------
// Funktion ausgabe

int ausgabe (char** m,int z, int s, char* s1, char*s2, int az, char* info, int* st){
	int ein, err;
	fflush(stdout);
	system("clear");
	for(int k=0;k<s;k++)			//Numerierungszeile
		printf("%d ",k+1);
		printf("   4 g E w I n N t");
		printf("\n");

	for(int i=0; i<z;i++){			//Zeilenweise Ausgabe
		for(int j=0; j<s;j++)		// google: terminal colour codes
		    if (m[i][j]=='O') 	
			 printf("%c%s%c%s%c ", 0x1b, "[40m", 0x1b, "[34m", m[i][j]);
		    else if (m[i][j]=='X') printf("%c%s%c%s%c ", 0x1b, "[40m", 
								0x1b, "[93m", m[i][j]);
			else printf("%c%s%c%s%c ", 0x1b, "[40m", 0x1b, "[31m", m[i][j]); 
		printf("%c%s%c%s", 0x1b, "[49m", 0x1b, "[39m");	  // nach Zeile auf default


		if(i==0)	printf("   %s = X, %s = O",s1, s2);
		if(i==z-2) printf("   %s", info);
		if(i!=z-1)printf("\n");
	}
	if(*st==1){
		if(az==1)	printf("   %s hat gewonnen\n", s1);
		else		printf("   %s hat gewonnen\n", s2);
		return 0;
	}
	if(*st==2){
		printf("    Unentschieden!   \n");
		return 0;
	}
	if(az==0) printf("   %s ist am Zug:", s1);
	else     printf("   %s ist am Zug:", s2);

	err=scanf("%d",&ein);
	if(err!=1) {
		pipe();
		ein=99;
	}

	return ein;
}	

//--------------------------------------------------------------------------------
// Funktion check
	
int check (char** m, int z0, int s0, int z, int s,int* h)
{	
	int zz=1;		//Siegzähler
	int i=1;		// Entfernung vom Nullpunkt (letzer Stein)

	while(s0+i<=s && m[z-z0][s0-1]==m[z-z0][s0-1+i])			// nach rechts
	{	zz++;
		i++;
		if(zz==4) return 1;
	}
	i=1;
	while(s0-i>=1 && (m[z-z0][s0-1]==m[z-z0][s0-1-i]))			// nach links
	{	zz++;
		i++;
		if(zz==4) return 1;
	}
	i=1;
	zz=1;
	while((s0+i<=s && z0+i<=z) && (m[z-z0][s0-1]==m[z-z0-i][s0-1+i]))		//rechts oben
	{	zz++;
		i++;
		if(zz==4) return 1;
	}
	i=1;
	while((s0-i>=1 && z0-i>=1) && (m[z-z0][s0-1]==m[z-z0+i][s0-1-i]))		//links unten
	{	zz++;
		i++;
		if(zz==4) return 1;
	}
	i=1;
	zz=1;
	while((s0-i>=1 && z0+i<=z) && (m[z-z0][s0-1]==m[z-z0-i][s0-1-i]))		//links oben	
	{	zz++;
		i++;
		if(zz==4) return 1;
	}
	i=1;
	while((s0+i<=s && z0-i>=1) && (m[z-z0][s0-1]==m[z-z0+i][s0-1+i]))	//rechts unten
	{	zz++;
		i++;
		if(zz==4) return 1;
	}
	i=1;
	zz=1;
	while(z0-i>=1 && (m[z-z0][s0-1]==m[z-z0+i][s0-1]))			//unten
	{	zz++;
		i++;
		if(zz==4) return 1;
	}

	for(int l=0; l<s; l++)			// ist noch Platz frei?
		if(h[l]<z) return 0;

	return 2; // Spielfeld voll!!!!!!!!!!!!!!
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
