public class HawaiiPizza extends Pizza{
    public HawaiiPizza(int dimensiune, int pret) {
        this.dimensiune = dimensiune;
        this.pret = pret;
    }

    @Override
    public String toString() {
        return "HawaiiPizza{" +
                "dimensiune=" + dimensiune +
                ", pret=" + pret +
                '}';
    }
}
