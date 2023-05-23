import java.util.Scanner;

public class tablouri {

    public static void main(String[] args){

        Scanner s = new Scanner(System.in);
        int n = s.nextInt();
        s.nextLine();
        String[] nume = new String[n];
        double[] note = new double[n];

        for (int i = 0; i < n; i++){
            nume[i] = s.nextLine();
            note[i] = s.nextDouble();
            s.nextLine();
        }

        for (int i = 0; i < n; i++){
            System.out.print(nume[i] + " ");
        }
        System.out.println();

        for (int i = 0; i < n; i++) {
            System.out.print(note[i] + " ");
        }
        System.out.println();

    }

}
