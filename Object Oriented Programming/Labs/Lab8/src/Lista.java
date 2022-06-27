public class Lista<E> {
    public E[] tablou;
    public int nr;

    public Lista(int dim) {
        if (dim > 0) {
            tablou = (E[]) new Object[dim];
        } else
            throw new IllegalArgumentException();
    }

    public void adauga(E x) {
        if (tablou.length == nr) {
            E[] tablouNou;
            tablouNou = (E[]) new Object[tablou.length * 2];
            for (int i = 0; i < nr; i++) {
                tablouNou[i] = tablou[i];
            }
            tablou = tablouNou;
        }
        tablou[nr] = x;
        nr++;
    }

    public void afiseaza() {
        for (int i = 0; i < nr; i++) {
            System.out.println(tablou[i]);
        }
    }

    public boolean cauta(E x) {
        for (int i = 0; i < nr; i++) {
            if (tablou[i].equals(x)) {
                return true;
            }
        }
        return false;
    }




}
