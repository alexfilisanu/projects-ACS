public class PizzaFactory {
    private static PizzaFactory pizzaUnica;
    public static enum TipPizza {
        Diavola, Hawaii, Pepperoni
    }
    private PizzaFactory() { }
    public static PizzaFactory InstantaPizza() {
        if (pizzaUnica == null)
            pizzaUnica = new PizzaFactory();
        return pizzaUnica;
    }

    public Pizza creeazaPizza(TipPizza p, int dimensiune, int pret) {
        switch (p) {
            case Diavola:
                return new DiavolaPizza(dimensiune, pret);
            case Hawaii:
                return new HawaiiPizza(dimensiune, pret);
            case Pepperoni:
                return new PepperoniPizza(dimensiune, pret);
        }
        throw new IllegalArgumentException("Tipul de pizza " + p + " nu este cunoscut.");
    }

    public static void main(String[] args) {
        PizzaFactory f = InstantaPizza();
        Pizza d = f.creeazaPizza(TipPizza.Diavola, 25, 35);
        System.out.println(d.toString());
        Pizza h = f.creeazaPizza(TipPizza.Hawaii, 10, 5);
        System.out.println(h.toString());
        Pizza p = f.creeazaPizza(TipPizza.Pepperoni, 23, 30);
        System.out.println(p.toString());
    }


}
