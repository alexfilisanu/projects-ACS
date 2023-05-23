import java.util.Scanner;

public class lab1 {
    public static void main(String[] args){
        Scanner scanner  = new Scanner(System.in);
        int n = scanner.nextInt();
        scanner.nextLine();
        int[][] m1 = new int[n][n];
        int[][] m2 = new int[n][n];

        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++) {
                m1[i][j] = scanner.nextInt();
            }
            scanner.nextLine();
        }

        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++) {
                m2[i][j] = scanner.nextInt();
            }
            scanner.nextLine();
        }

        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                System.out.print(m1[i][j] + m2[i][j] + " ");
            }
            System.out.println();
        }
        System.out.println();


    }
}
