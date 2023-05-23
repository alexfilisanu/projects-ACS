public class Doctor {
    private final String nume;
    private Integer rating;
    private Pacient[] pacienti;

    public Doctor() {
        this.nume = "doctor";
    }

    public Doctor(String nume, Integer rating) {
        this.nume = nume;
        this.rating = rating;
    }

    public String getNume() {
        return nume;
    }

    public void setRating(Integer rating) {
        this.rating = rating;
    }

    public void setPacienti(Pacient[] pacienti) {
        this.pacienti = pacienti;
    }

    public Integer getRating() {
        return rating;
    }

    public Pacient[] getPacienti() {
        return pacienti;
    }

    public void afisare(){
        System.out.println(this.nume + " " + this.rating);
        for (Pacient i : this.pacienti){
            System.out.println(i.afisare());
        }
    }
}
