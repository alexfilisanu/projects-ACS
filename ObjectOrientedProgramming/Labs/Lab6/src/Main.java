public class Main {
    public static void main(String[] args) {

        Persoana[] persoane = new Persoana[4];
        persoane[0] = new Persoana("AAA", "2839730398", new Cont(1,100));
        persoane[1] = new Persoana("BBB", "3465346352", new Cont(2,1000));
        persoane[0] = new Persoana("CCC", "1213141234", new Cont(3,5000));
        persoane[0] = new Persoana("DDD", "4653643577", new Cont(4,7000));


        for(Persoana pers : persoane) {

                pers.afiseazaInformatii();
            }

    }
}
