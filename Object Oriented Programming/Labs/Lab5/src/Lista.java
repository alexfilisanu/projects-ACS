public class Lista {
    class Nod {
        private int val;
        private Nod urm;

        public Nod(int x) {
            val = x;
            urm = null;
        }

        public Nod() {
        }
    }
    private Nod varf;
    private Nod coada;
    private int contor;

    public Lista() {
        this.varf = null;
        this.coada = null;
        this.contor = 0;
    }

    public void adauga(int x) {
        Nod nod = new Nod(x);

        if (varf == null) {
            varf = nod;
            //coada = nod;
        } else {
            coada.urm = nod;
        }
        coada = nod;
        contor++;
    }

    public int dimensiune() {
        return contor;
    }

    public void adaugaPozitie(int index, int x) {
        if ((index > dimensiune()) || (index < 0)) {
            System.out.println("eroare");
        } else if (dimensiune() == 0) {
            adauga(x);
        } else {
            Nod nod = new Nod(x);
            Nod p = varf;
            for (int i = 1; i < index; i++) {
                p = p.urm;
            }
            nod.urm = p.urm;
            p.urm = nod;
            contor++;
        }
    }

    public int elementPozitie(int index) {
        if ((index > dimensiune()) || (index < 0) || (index == dimensiune())) {
            System.out.println("eroare");
            return -1;
        } else {
            Nod p = varf;
            for (int i = 0; i < index; i++) {
                p = p.urm;
            }
            return p.val;
        }
    }

    public void afisareLista() {
        Nod p = varf;
        while (p != null) {
            System.out.println(p.val + " ");
            p = p.urm;
        }
    }

    public int elimina(int index) {
        if ((index > dimensiune()) || (index < 0) || (index == dimensiune())) {
            System.out.println("eroare");
            return -1;
        } else {
            int val;
            if (varf == coada) {
                val = varf.val;
                varf = null;
                coada = null;
                contor = 0;
                return val;
            } else {
                Nod p = varf;
                Nod ant = null;
                for (int i = 0; i < index; i++) {
                    ant = p;
                    p = p.urm;
                }
                ant.urm = p.urm;
                return p.val;
            }
        }
    }

    public void seteaza(int index, int x) {
        if ((index > dimensiune()) || (index < 0) || (index == dimensiune())) {
            System.out.println("eroare");
        } else {
            Nod p = varf;
            for (int i = 0; i < index; i++) {
                p = p.urm;
            }
            p.val = x;
        }
    }

    public static void main(String[] args) {
        Lista lista = new Lista();
        lista.adauga(1);
        lista.adauga(2);
        lista.adauga(3);
        lista.afisareLista();

        System.out.println();
        System.out.println();

        lista.adaugaPozitie(2,5);
        lista.afisareLista();
        System.out.println();
        lista.adaugaPozitie(3, 7);
        lista.afisareLista();

        System.out.println();
        System.out.println();

        System.out.println(lista.elementPozitie(2));
        System.out.println();

        System.out.println(lista.elimina(2));
        System.out.println();
        lista.afisareLista();
        System.out.println();

        lista.seteaza(1,10);
        lista.afisareLista();
    }
}
