public class Editor {
    int index = 0;
    public Comanda[] listaComenzi = new Comanda[10];
    public void adaugaComanda(Comanda comanda) {
        listaComenzi[index] = comanda;
        index++;
    }

    public void executaComenzi(Imagine imagine) {
        for (int i = 0; i < index; i++) {
            listaComenzi[i].executa(imagine);
        }
    }

    public void reexecuta(Imagine imagine) {
        listaComenzi[index - 1].executa(imagine);
    }

    public void anuleaza(Imagine imagine) {
        listaComenzi[index - 1].anuleaza(imagine);
    }

}
