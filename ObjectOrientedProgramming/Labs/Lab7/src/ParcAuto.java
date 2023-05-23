import java.io.*;

public class ParcAuto {
    private String numeFisier;

    public ParcAuto(String numeFisier) { this.numeFisier = numeFisier; }

    public String getNumeFisier() { return numeFisier; }
    public void setNumeFisier(String numeFisier) { this.numeFisier = numeFisier; }

    public int numaraMasiniNoi() throws IOException {
        FileReader buff = new FileReader(numeFisier);
        BufferedReader br = new BufferedReader(buff);
        int count = 0;
        while (true) {
            Auto auto = Auto.citeste(br);
            if (auto != null) {
                if (auto.getKm() == 0) {
                    count++;
                }
            } else {
                return count;
            }
        }
    }

    public Auto celMaiScumpAuto() throws IOException {
        Auto celMaiScumpAuto = new Auto();
        FileReader buff = new FileReader(numeFisier);
        BufferedReader br = new BufferedReader(buff);
        while (true) {
            Auto auto = Auto.citeste(br);
            if (auto != null) {
                if (auto.getPret() > celMaiScumpAuto.getPret()) {
                    celMaiScumpAuto = auto;
                }
            } else {
                return celMaiScumpAuto;
            }
        }
    }

    public void adaugaAuto(Auto a) throws IOException {
        BufferedWriter br = new BufferedWriter(new FileWriter(numeFisier, true));
        br.write(a.getModel() + "\n");
        br.write(a.getAn() + "\n");
        br.write(a.getKm() + "\n");
        br.write(a.getPret() + "\n");
    }

    public boolean cauta(Auto a) throws IOException {
        FileReader buff = new FileReader(numeFisier);
        BufferedReader br = new BufferedReader(buff);
        while (true) {
            Auto auto = Auto.citeste(br);
            if (auto != null) {
                if (auto.identic(a)) {
                    return true;
                }
            } else {
                return false;
            }
        }
    }

    public void afiseazaParcAuto() throws IOException {
        FileReader buff = new FileReader(numeFisier);
        BufferedReader br = new BufferedReader(buff);
        while (true) {
            Auto auto = Auto.citeste(br);
            if (auto != null) {
                System.out.println(auto.toString());
            } else {
                return;
            }
        }
    }


    public static void main(String[] args) throws IOException {
        ParcAuto p = new ParcAuto("date.txt");
        System.out.println("Masini cu kilometraj 0: " + p.numaraMasiniNoi() + "\n");
        System.out.println("Cea mai scumpa masina: " + p.celMaiScumpAuto().toString() + "\n");
        Auto autoSuzuki = new Auto("Suzuki", 2015, 0, 19000.0);
        System.out.println("Exista Suzuki? " + p.cauta(autoSuzuki) + "\n");
        System.out.println("Parc auto: \n");
        p.afiseazaParcAuto();

    }










}
