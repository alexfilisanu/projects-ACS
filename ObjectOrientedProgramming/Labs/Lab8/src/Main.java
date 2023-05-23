public class Main {
    public static void main(String[] args) {
        Lista<Integer> lista1 = new Lista<>(3);
        Lista<Double> lista2 = new Lista<>(3);
        Lista<String> lista3 = new Lista<>(3);

        lista1.adauga(1);
        lista1.adauga(2);
        lista1.adauga(3);
        lista1.adauga(4);
        lista1.afiseaza();
        System.out.println(lista1.cauta(2));
        System.out.println();

        lista2.adauga(1.2);
        lista2.afiseaza();
        System.out.println(lista2.cauta(2.3));
        System.out.println();

        lista3.adauga("AAA");
        lista3.adauga("BBB");
        lista3.adauga("CCC");
        lista3.adauga("DDD");
        lista3.adauga("FFF");
        lista3.afiseaza();
        System.out.println(lista3.cauta("AAA"));
        System.out.println();

    }
}
