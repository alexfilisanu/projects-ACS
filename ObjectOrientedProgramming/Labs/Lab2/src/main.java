import java.util.Scanner;

public class main {
    public static void main(String[] args){
        Scanner s = new Scanner(System.in);

        Pacient[] pacienti = new Pacient[3];
        pacienti[0] = new Pacient("aaa");
        pacienti[1] = new Pacient("bbb");
        pacienti[2] = new Pacient("ccc");

        for (Pacient i : pacienti){
            System.out.println(i.afisare());
        }

        String numeD = s.nextLine();
        Integer ratingD = Integer.parseInt(s.nextLine());

        Doctor doctor = new Doctor(numeD, ratingD);

        doctor.setPacienti(pacienti);
        doctor.afisare();


    }
}
