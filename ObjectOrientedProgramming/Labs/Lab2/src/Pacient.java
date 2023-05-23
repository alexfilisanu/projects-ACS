public class Pacient {
    private String nume;
    private Integer id;
    private static Integer gen = 0;

    public Pacient(){}

    public Pacient(String nume) {
        this.nume = nume;
        this.id = ++gen;
    }


    public String getNume() {
        return nume;
    }
    public void setNume(String nume) {
        this.nume = nume;
    }

    public int getId() {
        return id;
    }
    public void setId(int id) {
        this.id = id;
    }

    public String afisare(){
        return this.getNume() + " " +this.getId();
    }

}
