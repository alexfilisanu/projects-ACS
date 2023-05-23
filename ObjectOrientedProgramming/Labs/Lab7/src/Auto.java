import java.io.BufferedReader;
import java.io.IOException;

public class Auto {
    private String model;
    private int an;
    private int km;
    private double pret;

    public Auto() { }
    public Auto(String model, int an, int km, double pret) {
        this.model = model;
        this.an = an;
        this.km = km;
        this.pret = pret;
    }

    public String toString() {
        return model + " " + Integer.toString(an) + " " + Integer.toString(km) +
                " " + Double.toString(pret);
    }

    public String getModel() { return model; }
    public void setModel(String model) { this.model = model; }
    public int getAn() { return an; }
    public void setAn(int an) { this.an = an; }
    public int getKm() { return km; }
    public void setKm(int km) { this.km = km; }
    public double getPret() { return pret; }
    public void setPret(double pret) { this.pret = pret; }

    public boolean identic(Auto a) {
        if (a.model != this.model)
            return false;
        if (a.an != this.an)
            return false;
        if (a.km != this.km)
            return false;
        if (a.pret != this.pret)
            return false;
        return true;
    }

    public static Auto citeste (BufferedReader br) throws IOException {
        String nume = br.readLine();
        if (nume != null) {
            Auto auto = new Auto(nume, Integer.parseInt(br.readLine()),
                    Integer.parseInt(br.readLine()), Double.parseDouble(br.readLine()));
            return auto;
        }
        return null;
    }





}
