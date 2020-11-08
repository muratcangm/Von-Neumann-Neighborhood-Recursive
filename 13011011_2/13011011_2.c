#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

/*
*Yukarýdaki iki satýr Visual Studio'nýn scanf de
*hata vermemesi için gerekmektedir baþka IDE'lerde
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
* main fonksiyon üzerinde kullanýcýdan n saysýný
* girmesi istenir bu sayeden Von Neumann komþuluk
* kuralýna göre çizilecek olan matrisin boyutu
* kullanýcýdan alýnýr
*
* daha sonrasýnda ilgili formüller aracýlýðý ile
* orta nokta , matrixin boyutu gibi deðerler hesaplanýr
*
* ardýndan recursive bir fonksiyon olan recursiveCellFormatter
* çaðrýlarak geri kalan iþlemi onun yapmasý saðlanýr en sonda
* girilen n deðere göre olmasý gereken matrix ekrana yazdýrýlýr
*/
int main()
{
	int n = 0; //kullanýcýdan alýnacak n deðeri
	int row = 0; //recursive fonksiyonun kontrol ettiði satýr deðeri
	int col = 0; //recursive fonksiyonun kontrol ettiði sütun deðeri
	int middlePoint = 0; //Von Neumann kuralýný uygulayabilmek için gereken orta nokta deðeri
	int matrixSize = 0; //çizilecek kare matrixin boyutu
	int* array; //ekrana yazdýrýlacak olan matrixin pointer'ý
	int* blackCellCountArr; //her satýrdaki toplam siyah hücre sayýsýný tutan dizinin pointer'ý
	int totalBlackCellCount = 0;

	/*Kullanýcýdan çizilmesini istediði matrixin
	boyutu burada alýnmaktadýr*/
	printf("Enter number n : ");
	scanf("%d", &n);

	/*Matrixin saðýnda , solunda , üstünde ve altýnda birer boþluk (yani 0 deðeri) olmasý
	istendiði için +3 ile toplama yapýlýyor bu sayede kullanýcýnýn girdiði n deðerine göre
	çizilecek matrixin tüm köþelerinde birer boþluk olmasý saðlanmýþ oluyor*/
	matrixSize = (2 * n) + 3;

	/*Von Neumann komþuluk kuralýna göre formül |x-x0|+|y-y0|<=n dir
	dolayýsýyla her bir satýr ve sütun deðerinden orta nokta deðeri
	çýkarýlarak o ilgili hücrenin 0 veya 1 olduðu (beyaz veya siyah)
	belirlenir. buradaki middle point deðiþkeni x0 ve y0 ýn hangi noktada
	olduðunu ifade eder*/
	middlePoint = matrixSize / 2;

	/*Matrix üzerinde bulunmasý gereken toplam 1 sayýsý yani siyah hücre
	sayýsý burada bulunmaktadýr*/
	totalBlackCellCount = ((2 * n) * (n + 1)) + 1;

	/*Alýnan boyuta göre memory allocate ediliyor
	malloc yerine calloc kullanýlmasýnýn sebebi ilgili adreslerin
	deðerlerinin otomatik olarak 0 lanmasý bu sayede recursive
	fonksiyona gönderilmeden önce matrixin tüm elemanlarýnýn 0 yani
	beyaz olduðu kesinlik kazanmaktadýr*/
	array = (int*)calloc(matrixSize * matrixSize, sizeof(int));

	/*Her satýrda bulunan 1 sayýný, yani siyah ile boyalý hücre sayýsýný
	tutmasý gereken dizi için memory allocate edilmektedir*/
	blackCellCountArr = (int*)calloc(matrixSize, sizeof(int));

	/*eðer ilgili memorylerden herhangi biri allocate edilemez
	ise hata veriliyor*/
	if (array == NULL || blackCellCountArr == NULL) {
		printf("Memory not allocated.\n");
		exit(0);
	}

	/*Hücrelerin Von Neumann komþuluk kuralýna göre 1 veya 0 olmasýný
	belirleyecek olan recursive fonksiyon burada çaðýrýlýyor*/
	recursiveCellFormatter(array, row, col, middlePoint, n, matrixSize, blackCellCountArr);

	/*Hücrelerinin kurala uygun olarak 1 veya 0 olacaðý kararlaþtýrýlmýþ olan
	matrixin deðerleri ekrana uygun formatta yazdýrýlmaktadýr*/
	for (row = 0; row < matrixSize; row++) {
		printf("\n");
		for (col = 0; col < matrixSize; col++) {
			printf("  %d  ", *(array + row * matrixSize + col));
		}
	}

	//Matrixin toplam kaç adet 1 yani siyah hücre içerdiði ekrana yazdýrýlmaktadýr
	printf("\n\nThis matrix includes total %d black cell(s)\n\n", totalBlackCellCount);

	/*Her satýrýn içerdiði 1 sayýsý yani siyah hücre sayýsý burada
	ekrana uygun formatta yazdýrýlmaktadýr*/
	for (row = 0; row < matrixSize; row++) {
		printf("%d th row include  %d black cell(s)\n", row, *(blackCellCountArr + row));
	}

	return 0;
}

/*
* @arr   siyaha boyalý veya beyaz olarak býrakma iþleminin yapýldýðý matrixin adresi
* @row   matrixin satýr deðeri
* @col   matrixin sütun deðeri
* @middlePoint   formüldeki x0 ve y0 deðerlerini tutan orta nokta deðeri
* @n   kullanýcýnýn girdiði, matrix boyutunu belirleyen ilgili deðer
* @matrixSize   kullanýcýnýn girdiði n deðerine göre belirlenen kare matrixin bir köþesinin boyutu
* @blackCellCountArr   her satýrda bulunan siyah hücre sayýsýný tutan dizinin adresi
*
* Matrixin Von Neumann komþuluk kuralýna uygun olarak ilgili gözünün siyaha boyanmasý ( 1 yapýlmasý )
* veya beyaz býrakýlmasý ( 0 olarak býrakýlmasý) iþlemini gerçekleþtiren recursive fonksiyon
*/
void recursiveCellFormatter(int* arr, int row, int col, int middlePoint, int n, int matrixSize, int* blackCellCountArr) {

	/*Von Neumann komþuluk kuralýna göre |x-x0|+|y-y0|<=n kuralý saðlanýrsa
	ilgili hücre siyaha boyanýr. Bu if kontrolü ile bu durum kontrol edilmekte
	eðer kural saðlanýyorsa ilgili hücre 1 yapýlýr ayný zamanda satýrda bulunan
	toplam 1 sayýsýný tutan dizinin ilgili gözü bir arttýrýlýr*/
	if ((abs(row - middlePoint) + abs(col - middlePoint)) <= n) {
		*(arr + row * matrixSize + col) = 1;
		*(blackCellCountArr + row) += 1;
	}

	/*Eðer col<matrixSize saðlanýrsa ilgili satýrda henüz son sütuna gelinmemiþ demektir. Bu
	sebeple satýrýn diðer sütunlarýna da bakmak gerektiði için fonksiyon her seferinde col + 1
	olarak tekrar kendini çaðýrýr. Eðer col < matrixSize saðlanmaz ise bu durum ilgili satýrdaki
	son sütuna da bakma iþleminin tamamlandýðý anlamýna gelir. Bu aþamadan sonra bir aþaðýdaki
	else if kýsmýna geçiþ yapýlýr*/
	if (col < matrixSize) {
		recursiveCellFormatter(arr, row, col + 1, middlePoint, n, matrixSize, blackCellCountArr);
	}

	/*Buraya gelinmesinin koþulu yukarýda bulunan col<matrixSize ýn saðlanmamasýdýr. Bu da ilgili
	satýrda son sütuna bakýldýðý anlamýna gelmektedir. Bu sebeple sütun deðeri 0 lanýr. Bu sayede
	bir sonraki satýrda tekrar ilk sütundan baþlanmasý saðlanmýþ olur. Ayný zamanda fonksiyon kendini
	row + 1 þeklinde çaðýrýr bu þekilde bir sonraki satýra geçiþ iþlemi tamamlanmýþ olur. Bu recursive
	call dan sonra bir üstteki if gerçekleþir bu sayede ilgili satýrýnda tüm sütunlarýna bakýlmasý saðlanýr
	Burada else yerine else if seçilmesinin sebebi hem row hem column deðerinin <matrixSize koþulunu
	saðlamamasý ile birlikte recursive call iþleminin sona erdirilebilmesinin saðlanmasý amaçlanarak
	gerçekleþtirilmiþtir*/
	else if (row < matrixSize) {
		col = 0;
		recursiveCellFormatter(arr, row + 1, col, middlePoint, n, matrixSize, blackCellCountArr);
	}
}