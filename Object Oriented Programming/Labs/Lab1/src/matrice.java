import java.util.Scanner;

public class matrice {

    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);
        int n = s.nextInt();
        s.nextLine();
        int[][] mtx1 = new int[n][n];
        int[][] mtx2 = new int[n][n];

        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                mtx1[i][j] = s.nextInt();
            }
            s.nextLine();
        }

        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                mtx2[i][j] = s.nextInt();
            }
            s.nextLine();
        }

        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                System.out.print(mtx1[i][j] + mtx2[i][j] + " ");
            }
            System.out.println();
        }
    }

}
