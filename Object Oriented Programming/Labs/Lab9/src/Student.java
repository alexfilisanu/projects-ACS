public class Student {
    private String nume;
    private float medie;

    public Student(String nume, float medie) {
        this.nume = nume;
        this.medie = medie;
    }

    public String getNume() { return nume; }
    public void setNume(String nume) { this.nume = nume; }
    public float getMedie() { return medie; }
    public void setMedie(float medie) { this.medie = medie; }

    @Override
    public String toString() {
        return "Student{" +  "nume = " + nume + ", medie= " + medie + "}";
    }


}
