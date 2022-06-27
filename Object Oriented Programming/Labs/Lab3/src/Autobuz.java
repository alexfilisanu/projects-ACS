public class Autobuz extends MijlocTransport {
    private int numarPasageri;
    private int pretBilet;

    public Autobuz(String culoare, boolean functional, int numarPasageri, int pretBilet) {
        super(culoare, functional);
        this.numarPasageri = numarPasageri;
        this.pretBilet = pretBilet;
    }

    public int incasare() {
        return this.numarPasageri * this.pretBilet;
    }

    public int profit() {
        return this.incasare() / 4;
    }

    public void afiseaza() {
        System.out.println("Autobuz(" + super.culoare() + "," + super.functional()
                + ") cu " + this.numarPasageri + " si " + this.pretBilet);
    }

}
