#Filisanu Mihai-Alexandru#
#324CBa#

	Am definit clasele Book, Language, Author, EditorialGroup, PublishingBrand, 
PublishingRetailer, Country si interfata IPublishingArtifact(contine metoda Publish()),
conform cerintei. In plus am creat clasa Main(pentru citirea din fisiere si initializare),
Administration (implemntarea metodelor din cerinta), Test(testarea functiilor implementate).
	Clasa Book are definite atributele ID, name, subtitle, ISBN, pageCount, keywords,
languageID, createdOn, authors + constructor + getter & setter + metoda Publish().
	Clasa Language are definite atributele ID, code, name + construct + getter & 
setter.
	Clasa Author are definite atributele ID, firstName, lastName + constructor + 
+ construct + getter & setter.
	Clasa EditorialGroup are definite atributele ID, name, books + constructor + getter
& setter + metoda Publish().	
	Clasa PublishingBrand are definite atributele ID, name, books + constructor + getter
& setter + metoda Publish().
	Clasa PublishingRetailer are definite atributele ID, name, books, countries + constructor 
+ getter & setter.
	Clasa Country are definite atributele ID, countryCode + constructor + getter & setter.
	
	Metodele Publish() returneaza cate un String care respecta formatul indicat in enunt.

	Metodele din clasa Administration:
	- getBooksForPublishingRetailerID(int publishingRetailerID) - intoarce o lista de carti
unice ale unui anumit PublishingRetailer;
	- getLanguagesForPublishingRetailerID(int publishingReatilerID) – intoarce o lista de 
limbi ale unui anumit PublishingRetailer;
	- getCountriesForBookID(int bookID) – intoarce o listă de tari unice ale unei anumite carti;
	- getCommonBooksForRetailerIDs(int retailerID1, int retailerID2) – intoarce o lista de carti 
comune intre retaileri;
	- getAllBooksForRetailerIDs (int retailerID1, int retailerID2) - intoarce o lista de carti 
intre retaileri (uniunea celor doua liste);

	Metoda din clasa Test scrie intr-un fisier "tests_output.txt" output urile celor 5 teste ale
fiecarei metoda mentionata mai sus.

