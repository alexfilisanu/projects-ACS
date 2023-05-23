import java.util.Arrays;
import java.util.Comparator;
import java.util.Scanner;

public class Autogara {
    MijlocTransport[] mijloaceT;

    public Autogara(int size) {
        this.mijloaceT = new MijlocTransport[size];

        Scanner s = new Scanner(System.in);

        for(int i = 0; i < size; i++) {
            String tip = s.next();
            String culoare = s.next();
            boolean functional = s.nextBoolean();
            int numarP = s.nextInt();
            int pretB = s.nextInt();

            s.nextLine();

            if(tip.equals("Autobuz")) {
                this.mijloaceT[i] = new Autobuz(culoare, functional, numarP, pretB);
            }
            else if(tip.equals("Microbuz")) {
                this.mijloaceT[i] = new Microbuz(culoare, functional, numarP, pretB);
            }
        }
        s.close();
    }

    public void afiseazaAutogara() {
        for(MijlocTransport i : this.mijloaceT)
            i.afiseaza();
    }

    public int profitTotal() {
        int total = 0;
        for(MijlocTransport i : this.mijloaceT)
            total += i.profit();

        return total;
    }

    public void sortare() {
        Arrays.sort(mijloaceT, new Comparator<>() {
            public int compare(MijlocTransport o1, MijlocTransport o2) {
                return o1.profit() - o2.profit();
            }
        });

    }

}


