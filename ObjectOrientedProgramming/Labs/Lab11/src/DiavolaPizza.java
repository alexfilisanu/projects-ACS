public class DiavolaPizza extends Pizza{

    public DiavolaPizza(int dimensiune, int pret) {
        this.dimensiune = dimensiune;
        this.pret = pret;
    }

    @Override
    public String toString() {
        return "DiavolaPizza{" +
                "dimensiune=" + dimensiune +
                ", pret=" + pret +
                '}';
    }
}
