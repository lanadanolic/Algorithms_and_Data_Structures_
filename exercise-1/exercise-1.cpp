/*Kreirati dvije datoteke i u njih upisati 10 slučajno generiranih brojeva iz opsega 
od 20-35. Svaku od datoteka pročitati i spremiti u vezanu listu (kreiraju se dvije
vezane liste, npr. L1 i L2). Ispisati ih.
a) Nakon toga potrebno je kreirati novu vezanu listu koja sadrži presjek članova 
listi L1 i L2, ali u listi ne smije biti ponavljanja vrijednosti. 
Ispisati sve tri liste.
b) Kod kreiranja presjeka se ne smije koristiti malloc, već uzeti postojeće 
elemente iz listi L1 i L2 i preusmjeriti pokazivače i vrijednosti moraju bit složene
uzlaznim redoslijedom.
Napomena: niti jedna lista se ne smije sortirati, već se koristi sortirani unos. */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define NUM_COUNT 10  //broj generiranih brojeva u datoteci
#define MIN_NUM 20 //donja granica generiranih brojeva
#define MAX_NUM 35 //gornja granica generir brojeva


typedef struct Head
{
	int data; //polje za spremanje cijelog broja
	struct Head* next; //pokazivac na sljedeci head u listi
}Head;

typedef struct //sadrži tri pokazivača na head(za L1, L2 i L3) – koristi se za povratak rezultata presjeka
{
	Head* L1;
	Head* L2;
	Head* L3;
}Triple;

void createFile(const char* filename); //stvara datoteku s nasumicnim brojevima
Head* loadlistfromfile(const char* filename); //ucitava brojeve iz datoteke i stvara sortiranu listu
Head* newHead(int val); //stvara novi head s danom vrijednoscu
Head* sortedinsert(Head* head, Head* new_head); //umece novi head u sortiranom redoslijedu u listu
void printlist(Head* head); //ispisuje sve elemente u listi
Triple createintersection(Head* L1, Head* L2); //stvara presjek 2 listi i vraca azurir liste
void freelist(Head* head); //fja oslobada memoriju zauzetu listom

int main()
{
	//1.kreiraj 2 datoteke s 10 slucajnih brojeva

	srand((unsigned)time(NULL)); // Inicijalizacija generatora nasumičnih brojeva pomoću trenutnog vremena

	createFile("dat1.txt"); //kreiranje prve datoteke dat1.txt s 10 nasumicnih brojeva
	createFile("dat2.txt"); //kreiranje druge datoteke dat2.txt s 10 nasumicnih brojeva

	//2.ucitaj podatke iz datoteka u 2 vezane liste koristeci sortirani unos

	Head* L1 = loadlistfromfile("dat1.txt"); //ucitavanje podataka iz "dat1.txt" u sortirau listu L1
	Head* L2 = loadlistfromfile("dat2.txt"); //ucitavanje podataka iz "dat2.txt" u sortiranu listu L2

	printf("Lista L1: \n");  //ispis zaglavlja za listu L1
	printlist(L1);           //ispis sadrzaja liste L2
	printf("Lista L2: \n");  //ispis zaglavlja za listu L2
	printlist(L2);           //ispis sadrzaja liste L2

	//3.kreiraj novu listu L3 koja sadrzi presjek L1 i L2 (bez dupliciranja)

	Triple t = createintersection(L1, L2); //kreiranje presjeka izmedu L1 i L2. rezultat azurirane L1 L2 i presjeka L3 sprema se u strukturu t 
	//azurirane liste
	L1 = t.L1; //postavljanje L1 na azurirani pocetak prve liste (bez zajednickih elemenata)
	L2 = t.L2; //postavljanje L2 na azurirani pocetak druge liste (bez zajed elem)
	Head* L3 = t.L3; //postavljanje L3 na pocetak liste presjeka

	printf("nakon kreiranja presjeka \n"); //ispis zaglavlja nakon kreiranja presjeka
	printf("Lista L1 (bez zajednickih elemenata \n"); //ispis opisa Liste L1 nakon uklanjanja zajednickih elemenata
	printlist(L1);
	printf("Lista L2 (bez zajeenickih elemenata) \n");
	printlist(L2);
	printf("presjek L3: \n");
	printlist(L3); //ispis sadrzaja liste L3

    //oslobodenje memorije
	freelist(L1); //oslobodenje memorije zauzete listom L1
	freelist(L2);
	freelist(L3);


	return 0;
}


void createFile(const char* filename)
{
	FILE* fp = fopen(filename, "w"); //otvara se datoteka za pisanje
	if (!fp) //provjera uspjesnosti otvaranja datoteke
	{
		printf("datoteka se ne moze otvoriti \n"); //ako se datoteka ne otvori, ispisuje se greska
		exit(EXIT_FAILURE); //i izlazi iz prgograma s kodom greske
	}

	for (int i = 0; i < NUM_COUNT; i++) //petlja koja se izvrsava 10 puta
	{
		int num = MIN_NUM + rand() % (MAX_NUM + MIN_NUM + 1); //generira se nasumicni broj u zadanom rasponu
		fprintf(fp, "%d", num); //upisuje se broj u datoteku, nakon cega slijedi razmak
	}

	fclose(fp); //zatvara se datoteka
}

Head* loadlistfromfile(char* filename)  //ucitava brojeve iz datoteke i stvara sortiranu listu
{
	FILE* fp = fopen(filename, "r"); //otvara se datoteka za citanje
	if (!fp) //otvara se datoteka za citanje
	{
		printf("file se ne moze otvoriti \n"); //ako otvaranje ne uspije, ispisuje se greska
		exit(EXIT_FAILURE); //i izlazi izprograma s kodom greske
	}

	Head* head = NULL; //inicijalizira se pocetak liste na NULL (prazna lista)
	int num; //varijabla za privremeno spremanje procitanog broja
	while (fscanf(fp, "%d", &num) == 1)
	{
		Head* n = newHead(num); //stvara se novi head s procitanom vrijednoscu
		head = sortedinsert(head, n); //umece se novi head u listu na nacind a lista ostane sortirana
	}
	
	fclose(fp); //zatvara se datoteka
	return head; //vraca se pocetak stvorene liste
}


Head* newHead(int val) //stvara novi head s danom vrijednoscu
{
	Head* n = (Head*)malloc(sizeof(Head));  //alocira se memorija za novi head
	if (!n) //provjera jel alokacija uspjela
	{
		printf("malloc nije uspio \n");
		exit(EXIT_FAILURE);
	}

	n->data = val; //postavlja se vrijednost heada na zadanu vrijednost
	n->next = NULL; //inicijalno se postavlja pokazuvac na sljedeci head na NULL

	return n;  //vraca se pokazivac na novi head
} 

Head* sortedinsert(Head* head, Head* new_head) //umece novi head u sortiranom redoslijedu u listu
{
	if (head == NULL || head->data >= new_head->data) //ako je lista prazna ili novi head ide na pocetak
	{
		new_head->next = head; //povezuje se novi head na pocetak (ili ispred postojeceg
		return new_head;  //novi head postaje pocetak liste
	}
	else
	{
		Head* current = head; //inicijalizira se pomocni pokazivac na pocetak liste
		while (current->next && current->next->data < new_head->data)
		{
			current = current->next; //prolazak kroz listu dok ne nade odgovarajucu poziciju
		}
		new_head->next = current->next; //povezuje se novi head s ostatkom liste
		current->next = new_head; //umece se novi head u listu

		return head; //vraca se pocetak liste (koji se nije promijenio)
	}
}


void printlist(Head* head)
{
	while (head) //dok ne dode do kraja liste
	{
		printf("%d", head->data); //ispisuje se vrijednost trenutnog heada
		head = head->next; //prelazi se na sljedeci head u listi
	}

	printf("\n"); //nakon ispisa svih headova prelazi se na novi element
}


Triple createintersection(Head* L1, Head* L2) //stvara presjek 2 listi i vraca azurir liste
{
    Triple result; //deklarira se struktura za spremanje rezultata presjeka
	result.L3 = NULL; //inicijalizira se lista presjeka na NULL

	//deklariraju se 2 dummy elementa
	Head dummy1;
	Head dummy2;
	dummy1.next = L1; //dummy head za L1 pokazuje na pocetak liste L1
	dummy2.next = L2; //dummy head za L2 pokazuje na pocetak liste L2

	Head* p1 = &dummy1; //inicijal se pokazivaci p1 i p2 na dummy head elem
	Head* p2 = &dummy2;
	Head* last = NULL; //pokazivac last ce pratiti zadnji head u listi presjeka

	while (p1->next && p2->next) //petlja se vrti sve dok oba pokaz imaju sljed elem
	{
		if (p1->next->data < p2->next->data) //ako je vrijednost sljed heada u L1 veca
		{
			p1 = p1->next; //pomakni se u listi na sljedeci head
		}
		else if (p1->next->data > p2->next->data) //ako vrijed sljed heada od p1 veca
		{
			p2 = p2->next; //pomakni se u listi L2 na sljedeci head
		}
		else //ako su vrijednosti jednake
		{
			int common = p1->next->data; //sprema se zajecnicka vrijednost u varijablu common
			Head* intersectHead = NULL; //inicijalizira se pokazivac na head koji ce se koristiti za presjek
			//uklanjanje svih head-ova s vrijednoscu common iz prve liste osim prvog
			while (p1->next && p1->next->data == common) //dok god je sljedeci head u L1 jednak common
			{
				Head* temp = p1->next; //sprema se trenutni head u privremenu varijablu
				p1->next = temp->next; //preskace se head tako sto se p1 preusmjerava na sljedeci head
				if (!intersectHead) //ako jos nismo sacuvali prvi head s ovom vrijednoscu
				{
					temp->next = NULL; //prekida se veza s ostakom liste
					intersectHead = temp; //sacuva se prvi head koji ce se dodati u presjek
				}
				else
				{
					free(temp); //ako je head visak, oslobada se memorija zauzeta tim head-om
				}
			}

			/* Uklanjanje svih head-ova s vrijednošću 'common' iz druge liste */
			while (p2->next && p2->next->data == common) { // Dok god je sljedeći head u L2 jednak common
				Head* temp = p2->next;  // Sprema se trenutni head u privremenu varijablu
				p2->next = temp->next;  // Preskače se head tako što se p2 preusmjerava na sljedeći head
				free(temp);             // Oslobađa se memorija zauzeta tim head-om
			}
			/* Dodavanje head-a u L3 */
			if (intersectHead) {        // Ako je pronađen zajednički element (presjek head)
				if (result.L3 == NULL) {  // Ako je lista presjeka L3 trenutno prazna
					result.L3 = intersectHead; // Prvi zajednički head postaje početak L3
					last = intersectHead;      // Zadnji head u L3 postaje ovaj head
				}
				else {
					last->next = intersectHead; // Dodaje se head na kraj L3
					last = intersectHead;         // Zadnji head se ažurira
				}
			}
		}
	}
	/* Ažuriramo početke L1 i L2 */
	result.L1 = dummy1.next;       // Početak ažurirane L1 preuzima se iz dummy head-a
	result.L2 = dummy2.next;       // Početak ažurirane L2 preuzima se iz dummy head-a
	return result;                 // Vraća se struktura s ažuriranim listama L1, L2 i presjekom L3
}


void freelist(Head* head) //fja oslobada memoriju zauzetu listom
{
	Head* temp; //varijabla koja priv cuva adresu tren elementa
	while (head) //petlja dok postoji head u listi
	{
		temp = head; //sprema se trenutni head
		head = head->next; //pomak na sljedeci head
		free(temp); //oslobada se memorija zauzeta trenutnim headom
	}
}
