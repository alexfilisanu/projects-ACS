public class Cont {
    private int numarCont, depozit;

    public Cont(int numarCont, int depozit) {
        this.numarCont = numarCont;
        this.depozit = depozit;
    }
    public void depune(int suma) {
        depozit += suma;
    }
    public void retrage(int suma) throws ExceptieFonduriInsuficiente{
        if (suma > depozit)
            throw new ExceptieFonduriInsuficiente();
        depozit -= suma;
    }
    public int numarCont() {
        return numarCont;
    }
    public int depozit() {
        return depozit;
    }

}
