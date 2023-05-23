/* Filisanu Mihai-Alexandru -- 314CB */

    Punctaj local: 135p.
    
    Fisierul functiiLG.c contine operatiile folosite pentru listele simplu inlantuite
generice. InsLG insereaza in lista elementul dat pe pozitia specificata(descrescator 
dupa rating; egalitate -> crescator dupa nume), functia cmpRating fiind folosita pentru
determinarea pozitiei. InsTOP insereaza elementul dat pe pozitia ceruta si pastreaza 
maxim 10 elemente in lista. AfisareLG afiseaza lista, DistrugeLG elibereaza memoria 
si LungimeLG returneaza numarul de elemente;

    Fisierul functiiStiva.c contine operatiile folosite pentru stiva implementata cu ajutorul
listelor simplu inlantuite generice. InitS initilizeaza stiva, Push pune un element in varful 
stivei si Pop il extrage. VidaS verifica daca stiva e vida, NrES returneaz numarul de elemente,
DimES dimensiunea unui element. Functia ResetS transforma stiva in una vida si DistrS elibereaza
memoria ocupata de stiva. PrelS prelucreaza elementele stivei in ordinea varf->baza apeland 
pentru fiecare o functie specifica. Rastoarna suprapune inversul sursei la destinatie.
ExtrageS extrage un anumit element din stiva, la final restul elementelor din 
stiva pastrandu-si ordinea. AfiS aiseaza stiva.

    Fisierul functiiCoada.c contine operatiile folosite pentru coada implementata cu ajutorul
listelor simplu inlantuite generice. InitQ initilizeaza stiva, IntrQ pune un element la sfarsitul  
cozii si ExtrQ il extrage de la inceputul ei. VidaQ verifica daca coada e vida, NrEQ returneaz 
numarul de elemente, DimEQ dimensiunea unui element. Functia ResetQ transforma coada in 
una vida si DistrQ elibereaza memoria ocupata de coada. PrelQ prelucreaza elementele cozii
in ordinea iceput->sfarsit apeland pentru fiecare o functie specifica. ConcatQ muta 
toate elementele din sursa la destinatie. ExtrageQ extrage un anumit element din coada,
la final restul elementelor din coada pastrandu-si ordinea. AfiQ aiseaza coada.

    Functiile AddSezLG/AddSezS/AddSezQ adauga un sezon nou citit din fisier unui element 
din lista/stiva/coada si afiseaza mesajul specificat. 

    Fisierul tema2.c contine strucurile folosite pentru implemetarea cerintelor (TSerial,
TSezon, TEpisod). Functia de comparare cmpRating stabileste ordinea elementelor, iar cmpNume 
verifica daca doua elemente sunt identice. CitireSerial returneaza o structura TSerial
cu datele citite, iar CitireSezon adauga cate o strucura TSezon in coada TSerial. AfiSerial
este folosita pentru a afisa o structura TSerial in formatul cerut.
    Parcurg pe rand fiecare comanda si in functie de aceasta apelez functiile definite
anterior. Pentru "add" citesc o structura TSerial si o adaug unei liste folosind InsLG. Pentru
"add_sez" adaug o structura TSezon in structura TSerial dupa ce caut in liste/stiva Watch/
coada Later serialul respectiv. Pentru "add_top" apelez functia InsTOP. Pentru "later" extrag
un element din una dintre liste si il introduc in coada Later. Pentru "watch" caut elementul 
in liste/stiva Watch/coada Later, il extrag si daca a fost vizionat in intregime il
adaug stivei History, daca nu stivei Watch. Pentru "show" afisez structura de date ceruta.
