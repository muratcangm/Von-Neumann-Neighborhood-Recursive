#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

/*
*Yukar�daki iki sat�r Visual Studio'n�n scanf de
*hata vermemesi i�in gerekmektedir ba�ka IDE'lerde
*gerekmeyebilir.
*/

#include <stdio.h>
#include <stdlib.h>

/*
*Recursive fonksiyonun prototipi
*/
void
recursiveCellFormatter(int* arr, int row, int col, int middlePoint, int n, int matrixSize, int* blackCellCountArr);

/*
* main fonksiyon �zerinde kullan�c�dan n says�n�
* girmesi istenir bu sayeden Von Neumann kom�uluk
* kural�na g�re �izilecek olan matrisin boyutu
* kullan�c�dan al�n�r
*
* daha sonras�nda ilgili form�ller arac�l��� ile
* orta nokta , matrixin boyutu gibi de�erler hesaplan�r
*
* ard�ndan recursive bir fonksiyon olan recursiveCellFormatter
* �a�r�larak geri kalan i�lemi onun yapmas� sa�lan�r en sonda
* girilen n de�ere g�re olmas� gereken matrix ekrana yazd�r�l�r
*/
int main()
{
	int n = 0; //kullan�c�dan al�nacak n de�eri
	int row = 0; //recursive fonksiyonun kontrol etti�i sat�r de�eri
	int col = 0; //recursive fonksiyonun kontrol etti�i s�tun de�eri
	int middlePoint = 0; //Von Neumann kural�n� uygulayabilmek i�in gereken orta nokta de�eri
	int matrixSize = 0; //�izilecek kare matrixin boyutu
	int* array; //ekrana yazd�r�lacak olan matrixin pointer'�
	int* blackCellCountArr; //her sat�rdaki toplam siyah h�cre say�s�n� tutan dizinin pointer'�
	int totalBlackCellCount = 0;

	/*Kullan�c�dan �izilmesini istedi�i matrixin
	boyutu burada al�nmaktad�r*/
	printf("Enter number n : ");
	scanf("%d", &n);

	/*Matrixin sa��nda , solunda , �st�nde ve alt�nda birer bo�luk (yani 0 de�eri) olmas�
	istendi�i i�in +3 ile toplama yap�l�yor bu sayede kullan�c�n�n girdi�i n de�erine g�re
	�izilecek matrixin t�m k��elerinde birer bo�luk olmas� sa�lanm�� oluyor*/
	matrixSize = (2 * n) + 3;

	/*Von Neumann kom�uluk kural�na g�re form�l |x-x0|+|y-y0|<=n dir
	dolay�s�yla her bir sat�r ve s�tun de�erinden orta nokta de�eri
	��kar�larak o ilgili h�crenin 0 veya 1 oldu�u (beyaz veya siyah)
	belirlenir. buradaki middle point de�i�keni x0 ve y0 �n hangi noktada
	oldu�unu ifade eder*/
	middlePoint = matrixSize / 2;

	/*Matrix �zerinde bulunmas� gereken toplam 1 say�s� yani siyah h�cre
	say�s� burada bulunmaktad�r*/
	totalBlackCellCount = ((2 * n) * (n + 1)) + 1;

	/*Al�nan boyuta g�re memory allocate ediliyor
	malloc yerine calloc kullan�lmas�n�n sebebi ilgili adreslerin
	de�erlerinin otomatik olarak 0 lanmas� bu sayede recursive
	fonksiyona g�nderilmeden �nce matrixin t�m elemanlar�n�n 0 yani
	beyaz oldu�u kesinlik kazanmaktad�r*/
	array = (int*)calloc(matrixSize * matrixSize, sizeof(int));

	/*Her sat�rda bulunan 1 say�n�, yani siyah ile boyal� h�cre say�s�n�
	tutmas� gereken dizi i�in memory allocate edilmektedir*/
	blackCellCountArr = (int*)calloc(matrixSize, sizeof(int));

	/*e�er ilgili memorylerden herhangi biri allocate edilemez
	ise hata veriliyor*/
	if (array == NULL || blackCellCountArr == NULL) {
		printf("Memory not allocated.\n");
		exit(0);
	}

	/*H�crelerin Von Neumann kom�uluk kural�na g�re 1 veya 0 olmas�n�
	belirleyecek olan recursive fonksiyon burada �a��r�l�yor*/
	recursiveCellFormatter(array, row, col, middlePoint, n, matrixSize, blackCellCountArr);

	/*H�crelerinin kurala uygun olarak 1 veya 0 olaca�� kararla�t�r�lm�� olan
	matrixin de�erleri ekrana uygun formatta yazd�r�lmaktad�r*/
	for (row = 0; row < matrixSize; row++) {
		printf("\n");
		for (col = 0; col < matrixSize; col++) {
			printf("  %d  ", *(array + row * matrixSize + col));
		}
	}

	//Matrixin toplam ka� adet 1 yani siyah h�cre i�erdi�i ekrana yazd�r�lmaktad�r
	printf("\n\nThis matrix includes total %d black cell(s)\n\n", totalBlackCellCount);

	/*Her sat�r�n i�erdi�i 1 say�s� yani siyah h�cre say�s� burada
	ekrana uygun formatta yazd�r�lmaktad�r*/
	for (row = 0; row < matrixSize; row++) {
		printf("%d th row include  %d black cell(s)\n", row, *(blackCellCountArr + row));
	}

	return 0;
}

/*
* @arr   siyaha boyal� veya beyaz olarak b�rakma i�leminin yap�ld��� matrixin adresi
* @row   matrixin sat�r de�eri
* @col   matrixin s�tun de�eri
* @middlePoint   form�ldeki x0 ve y0 de�erlerini tutan orta nokta de�eri
* @n   kullan�c�n�n girdi�i, matrix boyutunu belirleyen ilgili de�er
* @matrixSize   kullan�c�n�n girdi�i n de�erine g�re belirlenen kare matrixin bir k��esinin boyutu
* @blackCellCountArr   her sat�rda bulunan siyah h�cre say�s�n� tutan dizinin adresi
*
* Matrixin Von Neumann kom�uluk kural�na uygun olarak ilgili g�z�n�n siyaha boyanmas� ( 1 yap�lmas� )
* veya beyaz b�rak�lmas� ( 0 olarak b�rak�lmas�) i�lemini ger�ekle�tiren recursive fonksiyon
*/
void recursiveCellFormatter(int* arr, int row, int col, int middlePoint, int n, int matrixSize, int* blackCellCountArr) {

	/*Von Neumann kom�uluk kural�na g�re |x-x0|+|y-y0|<=n kural� sa�lan�rsa
	ilgili h�cre siyaha boyan�r. Bu if kontrol� ile bu durum kontrol edilmekte
	e�er kural sa�lan�yorsa ilgili h�cre 1 yap�l�r ayn� zamanda sat�rda bulunan
	toplam 1 say�s�n� tutan dizinin ilgili g�z� bir artt�r�l�r*/
	if ((abs(row - middlePoint) + abs(col - middlePoint)) <= n) {
		*(arr + row * matrixSize + col) = 1;
		*(blackCellCountArr + row) += 1;
	}

	/*E�er col<matrixSize sa�lan�rsa ilgili sat�rda hen�z son s�tuna gelinmemi� demektir. Bu
	sebeple sat�r�n di�er s�tunlar�na da bakmak gerekti�i i�in fonksiyon her seferinde col + 1
	olarak tekrar kendini �a��r�r. E�er col < matrixSize sa�lanmaz ise bu durum ilgili sat�rdaki
	son s�tuna da bakma i�leminin tamamland��� anlam�na gelir. Bu a�amadan sonra bir a�a��daki
	else if k�sm�na ge�i� yap�l�r*/
	if (col < matrixSize) {
		recursiveCellFormatter(arr, row, col + 1, middlePoint, n, matrixSize, blackCellCountArr);
	}

	/*Buraya gelinmesinin ko�ulu yukar�da bulunan col<matrixSize �n sa�lanmamas�d�r. Bu da ilgili
	sat�rda son s�tuna bak�ld��� anlam�na gelmektedir. Bu sebeple s�tun de�eri 0 lan�r. Bu sayede
	bir sonraki sat�rda tekrar ilk s�tundan ba�lanmas� sa�lanm�� olur. Ayn� zamanda fonksiyon kendini
	row + 1 �eklinde �a��r�r bu �ekilde bir sonraki sat�ra ge�i� i�lemi tamamlanm�� olur. Bu recursive
	call dan sonra bir �stteki if ger�ekle�ir bu sayede ilgili sat�r�nda t�m s�tunlar�na bak�lmas� sa�lan�r
	Burada else yerine else if se�ilmesinin sebebi hem row hem column de�erinin <matrixSize ko�ulunu
	sa�lamamas� ile birlikte recursive call i�leminin sona erdirilebilmesinin sa�lanmas� ama�lanarak
	ger�ekle�tirilmi�tir*/
	else if (row < matrixSize) {
		col = 0;
		recursiveCellFormatter(arr, row + 1, col, middlePoint, n, matrixSize, blackCellCountArr);
	}
}