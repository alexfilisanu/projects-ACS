public class Persoana {
    private static int nrPersoane;
    private Cont cont;
    private String nume, cnp;

    public void seteazaDate(String nume, String cnp, Cont cont) throws ExceptieCnpInvalid {
        if (cnp.charAt(0) != '1' && cnp.charAt(0) != '2')
            throw new ExceptieCnpInvalid();

        this.nume = nume;
        this.cnp = cnp;
        this.cont = cont;
    }
    public Persoana(String nume, String cnp, Cont cont) {
        try {
            seteazaDate(nume, cnp, cont);
            nrPersoane++;
        }
        catch (ExceptieCnpInvalid e) {
            System.out.println(e.getMessage());
        }
    }
    public Cont cont() {
        return cont;
    }
    public void afiseazaInformatii() {
        System.out.println(nume + " " + cnp + " " + cont);
    }
}
