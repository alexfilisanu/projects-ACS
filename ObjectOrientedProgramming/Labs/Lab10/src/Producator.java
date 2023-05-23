import java.util.Random;

public class Producator implements Runnable{
    Tablou tablou;
    public Producator(Tablou tablou) { this.tablou = tablou; }

    @Override
    public void run() {
        Random random = new Random();
        for (int i = 0; i < Tablou.getCapacitate(); i++) {
            try {
                System.out.print("Produc: ");
                tablou.scrie(random.nextInt(100));
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
