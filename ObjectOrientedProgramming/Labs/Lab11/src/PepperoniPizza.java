public class PepperoniPizza extends Pizza{
    public PepperoniPizza(int dimensiune, int pret) {
        this.dimensiune = dimensiune;
        this.pret = pret;
    }

    @Override
    public String toString() {
        return "PepperoniPizza{" +
                "dimensiune=" + dimensiune +
                ", pret=" + pret +
                '}';
    }
}
