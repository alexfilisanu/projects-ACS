public class Imagine {
    private int lungime;
    private int latime;
    private int nivelLuminozitate;
//    private Boolean primaComanda;


    public Imagine(int lungime, int latime, int nivelLuminozitate) {
        this.lungime = lungime;
        this.latime = latime;
        this.nivelLuminozitate = nivelLuminozitate;
    }

    public int getLungime() { return lungime; }
    public void setLungime(int lungime) { this.lungime = lungime; }
    public int getLatime() { return latime; }
    public void setLatime(int latime) { this.latime = latime; }
    public int getNivelLuminozitate() { return nivelLuminozitate; }
    public void setNivelLuminozitate(int nivelLuminozitate) { this.nivelLuminozitate = nivelLuminozitate; }
//    public Boolean getPrimaComanda() { return primaComanda; }
//    public void setPrimaComanda(Boolean primaComanda) { this.primaComanda = primaComanda; }


    @Override
    public String toString() {
        return "Imagine{" +
                "lungime=" + lungime +
                ", latime=" + latime +
                ", nivelLuminozitate=" + nivelLuminozitate +
                '}';
    }
}
