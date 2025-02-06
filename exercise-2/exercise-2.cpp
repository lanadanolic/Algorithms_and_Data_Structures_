/*Napisati program koji generira niz od 20 slučajnih brojeva u opsegu od 100-120.
Brojeve je potrebno spremiti u u vezanu listu. 
Način formiranja liste nije bitan, ali se elementi ne smiju ponavljati. Ispisati
listu.
Napraviti "novu" vezanu listu koja sadrži sve elemente kao i prethodna, 
ali u obrnutom redoslijedu. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 20 //broj elemenata koji se generiraju
#define MIN_VAL 100 //minimalna vrijednost generiranog broja
#define MAX_VAL  120 //maksimalna vrijednost generiranog broja

typedef struct Head
{
	int data;  //polje za pohranu vrijednosti elementa
	struct Head* next; //pokazivac na sljedeci element u listi
}Head;

Head* insertUnique(Head* head, int data); //umece vrijednost u listu samo ako element nije vec prisutan u listi
void printList(Head* head); //ispisuje sve elemente liste
int contains(Head* head, int data); //provjera postoji li neka vrijed vec u listi
Head* newHead(int data); //stvara novi element liste s danom vrijednoscu
void freeList(Head* head); //oslobada memoriju zauzetu listom
Head* reverseList(Head* head); //stvara novu listu s elementima obrnutog redoslijeda

int main()
{
	srand((unsigned)time(NULL)); //inicijalizira generator slucajnih brojeva

	Head* list = NULL; //inicijalizira pokazivac na pocetak originalne liste na NULL (lista je prazna)

	for (int i = 0; i < ARRAY_SIZE; i++) //petlja koja se vrti 20 puta
	{
		int num = MIN_VAL + rand() % (MAX_VAL - MIN_VAL + 1); //generira nasumicni broj izmedu
		list = insertUnique(list, num); //umece broj u listu samo ako vec nije pristuan
	}

	//ispis originalne liste
	printf("originalna lista: \n"); //ispis zaglavlja
	printList(list); //poziva se fja koja ispisuje lemente liste

	//kreira novu listu s elementima obrnutog redoslijeda
	Head* reversed = reverseList(list); //gradi novu listu s obrnutim redoslijedom
	printf("lista u obrnutom redoslijedu: \n");
	printList(reversed); //ispis nove, obrnute liste

	//oslobadanje zauzete memorije
	freeList(list); //oslobada memoriju orginalne liste
	freeList(reversed); //oslobada memoriju obrnute liste

	return 0;
}

Head* insertUnique(Head* head, int data) //umece vrijednost u listu samo ako element nije vec prisutan u listi
{
	if (contains(head, data)) //provjera postoji li vec element s ovom vrijednoscu
	{
		return head; //ako postoji, lista se ne mijenja i vraca se postojeci pocetak
	}

	Head* n = newHead(data); //ako ne postoji, stvara se novi element s danom vrijednoscu
	n->next = head; //novi element se umece na pocetak liste

	return n; //novi element postaje pocetak liste
}

int contains(Head* head, int data)
{
	while (head) //dokle god element postoji
	{
		if (head->data == data) //ako trenutni element ima trazenu vrijednost
		{
			return 1; //vrijednost je pronadena
		}
		head = head->next; //inicijalizira se pokazivac na sljedeci element
	}
	return 0; //ako vrijednost nije pronadena, vraca 0 tj false
}

Head* newHead(int data) //stvara novi element liste s danom vrijednoscu
{
	Head* n = (Head*)malloc(sizeof(Head)); //alocira memoriju za novi element
	if (!n) //provjera jel alokacija uspjela
	{
		printf("error, alokacija neuspjesna \n");
		exit(EXIT_FAILURE);
	}

	n->data = data; //postavlja vrijednost elementa na proslijedeni podatak
	n->next = NULL; //inic pokazivac na sljedeci element na NULL (nema sljedeceg)

	return n;
}

void printList(Head* head) //ispisuje sve elemente liste
{
	while (head)  //dokle god element postoji u listi
	{
		printf("%d", head->data); //ispisuje se vrijednost trenutnog elementa
		head = head->next; //prelazi se na sljedeci element u listi
	}
	printf("\n");
}

void freeList(Head* head) //oslobada memoriju zauzetu listom
{
	Head* temp; //privremena varijabla za cuvanje trenutnog elementa
	while (head) //dokle god postoji element u listi
	{
		temp = head; //sprema se trenutni element
		head = head->next;
		free(temp); //prelazi se na iduci element
	}
}

Head* reverseList(Head* head) //stvara novu listu s elementima obrnutog redoslijeda
{
	Head *rev = NULL;  //inicijalizira novu listu kao praznu
	while (head) //dokle god postoji element u originalnoj listi
	{
		Head* n = newHead(head->data); //stvara se novi element s istom vrijednoscu kao trenutni element originalne liste
		n->next = rev; //novi element se umece na poc nove liste (time se obrce redosl)
		rev = n; //azurira se pocetak nove liste
		head = head->next; //prelazi se na sljed element u origin listi
	}
	return rev; //vraca se pokazivac na novu listu s obrnutim redoslijedom
}
