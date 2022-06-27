import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Main {
    public static void main(String[] args) {
        ExecutorService executor = Executors.newFixedThreadPool(2);
        Tablou tablou = new Tablou();
        Producator producator = new Producator(tablou);
        Consumator consumator = new Consumator(tablou);
        executor.execute(producator);
        executor.execute(consumator);

        executor.shutdown();
    }
}