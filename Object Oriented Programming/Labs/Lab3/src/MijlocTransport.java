public abstract class MijlocTransport {
    private String culoare;
    private  boolean functional;

    public MijlocTransport() {
        this.culoare = "alb";
        this.functional = false;
    }

    public MijlocTransport(String culoare, boolean functional) {
        this.culoare = culoare;
        this.functional = functional;
    }

    public String culoare() { return culoare; }

    public boolean functional() { return functional; }

    public void seteazaCuloare(String culoare) { this.culoare = culoare; }

    public void seteazaFunctional(boolean functional) { this.functional = functional; }

    public abstract int incasare ();

    public abstract int profit ();

    public abstract  void afiseaza ();




}
