import java.util.Random;

class Consumator implements Runnable{
    Tablou tablou;
    public Consumator(Tablou tablou) {
        this.tablou = tablou;
    }

    @Override
    public void run() {
        for (int i = Tablou.getCapacitate(); i > 0; i--) {
            try {
                System.out.print("Conusm: ");
                System.out.println(tablou.citeste());
                Thread.sleep(4000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
