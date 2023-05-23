public class Student {
    long nrMatricol;
    String nume;
    String prenume;
    double medieFinala;
    Student() {
    }

    Student(long nrMatricol, String nume, String prenume, double medieFinala) {
        this.nrMatricol = nrMatricol;
        this.nume = nume;
        this.prenume = prenume;
        this.medieFinala = medieFinala;
    }

    public void setNrMatricol(long nrMatricol) { this.nrMatricol = nrMatricol; }
    public long getNrMatricol() { return nrMatricol; }

    public void setNume(String nume) { this.nume = nume; }
    public String getNume() { return nume; }

    public void setPrenume(String prenume) { this.prenume = prenume; }
    public String getPrenume() { return prenume; }

    public void setMedieFinala(double medieFinala) { this.medieFinala = medieFinala; }
    public double getMedieFinala() { return medieFinala; }

    String detaliiStudent() {
        return "[" + this.nrMatricol + "] " + this.prenume + " " + this.nume + " : " + this.medieFinala;
    }

    public static void main(String[] args){
        //Student student1 = new Student(1, "Ionescu", "Ionut", 9.8);
        //System.out.println(student1.detaliiStudent());

        Student[] semigrupa = new Student[3];

        for (int i = 0; i < 3; i++) {
            semigrupa[i] = new Student(1, "Ionescu", "Ionut", 9.8);
        }

        semigrupa[0].setMedieFinala(5.6);

        double suma = 0;
        for (int i = 0; i < 3; i++) {
            suma += semigrupa[i].getMedieFinala();
            System.out.println(semigrupa[i].detaliiStudent());
        }

        System.out.println("Media finala este " + suma / 3);
    }
}
