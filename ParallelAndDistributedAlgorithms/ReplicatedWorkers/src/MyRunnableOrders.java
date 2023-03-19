import java.io.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

public class MyRunnableOrders implements Runnable {
	private final BufferedReader bufferedReaderOrders;
	private final String orderProductsInput;
	private final BufferedWriter bufferedWriterOrders;
	private final BufferedWriter bufferedWriterProducts;
	private final ExecutorService tpeOrders;
	private final AtomicInteger inQueueOrders;
	private final ExecutorService tpeProducts;
	private final AtomicInteger inQueueProducts;
	public MyRunnableOrders(BufferedReader bufferedReaderOrders, String orderProductsInput,
							BufferedWriter bufferedWriterOrders, BufferedWriter bufferedWriterProducts,
							ExecutorService tpeOrders, AtomicInteger inQueueOrders, ExecutorService tpeProducts,
							AtomicInteger inQueueProducts) {
		this.bufferedReaderOrders = bufferedReaderOrders;
		this.orderProductsInput = orderProductsInput;
		this.bufferedWriterOrders = bufferedWriterOrders;
		this.bufferedWriterProducts = bufferedWriterProducts;
		this.tpeOrders = tpeOrders;
		this.inQueueOrders = inQueueOrders;
		this.tpeProducts = tpeProducts;
		this.inQueueProducts = inQueueProducts;
	}

	@Override
	public void run() {
		try {
			String line = bufferedReaderOrders.readLine();
			if (line != null) {
				// preiau din linia citita id ul comenzii si numarul de produse al comenzii
				String[] orderLine = line.split(",");
				String orderId = orderLine[0];
				Integer orderNumberProducts = Integer.parseInt(orderLine[1]);

				// daca numarul de produse din comanda este diferit de 0 trec la nivelul 2 de
				// paralizare(tot Executor Service pe modelul din lab7)
				if (orderNumberProducts != 0) {
					BufferedReader bufferedReaderProducts = new BufferedReader(new FileReader(orderProductsInput));
					inQueueProducts.incrementAndGet();
					tpeProducts.submit(new MyRunnableProducts(bufferedReaderProducts, bufferedWriterProducts,
							tpeProducts, inQueueProducts, orderId));
					// cand se ajunge in acest punct toate produsele unei comenzi au fost livrate
					// si afisez in oreders_out acest lucru
					try {
						bufferedWriterOrders.write(line + ",shipped\n");
						bufferedWriterOrders.flush();
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
				inQueueOrders.incrementAndGet();
				tpeOrders.submit(new MyRunnableOrders(bufferedReaderOrders, orderProductsInput, bufferedWriterOrders,
						bufferedWriterProducts, tpeOrders, inQueueOrders, tpeProducts, inQueueProducts));
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		// toate comenzile au fost procesata si se incheie primul ExecutorService
		int left = inQueueOrders.decrementAndGet();
		if (left == 0) {
			tpeOrders.shutdown();
		}
	}
}