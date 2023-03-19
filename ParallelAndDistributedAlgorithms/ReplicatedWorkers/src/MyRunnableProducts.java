import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

public class MyRunnableProducts implements  Runnable{
	private final BufferedReader bufferedReaderProducts;
	private final BufferedWriter bufferedWriterProducts;
	private final ExecutorService tpeProducts;
	private final AtomicInteger inQueueProducts;
	private final String orderId;

	public MyRunnableProducts(BufferedReader bufferedReaderProducts, BufferedWriter bufferedWriterProducts,
							  ExecutorService tpeProducts, AtomicInteger inQueueProducts, String orderId) {
		this.bufferedReaderProducts = bufferedReaderProducts;
		this.bufferedWriterProducts = bufferedWriterProducts;
		this.tpeProducts = tpeProducts;
		this.inQueueProducts = inQueueProducts;
		this.orderId = orderId;
	}

	@Override
	public void run() {
		try {
			String line = bufferedReaderProducts.readLine();
			if (line != null) {
				// preiau din linia produsului id ul comenzii(productsLine[0])
				String[] productsLine = line.split(",");

				// daca id ul comenzii citite este identic cu cel expected,
				// produsul este livrat si afisez acest lucru in order_products_out
				if (productsLine[0].equals(orderId)) {
					try {
						bufferedWriterProducts.write(line + ",shipped\n");
						bufferedWriterProducts.flush();
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
				inQueueProducts.incrementAndGet();
				tpeProducts.submit(new MyRunnableProducts(bufferedReaderProducts, bufferedWriterProducts,
						tpeProducts, inQueueProducts, orderId));
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		// toate comenzile au fost procesate si se incheie al 2lea ExecutorService
		int left = inQueueProducts.decrementAndGet();
		if (left == 0) {
			tpeProducts.shutdown();
		}
	}
}