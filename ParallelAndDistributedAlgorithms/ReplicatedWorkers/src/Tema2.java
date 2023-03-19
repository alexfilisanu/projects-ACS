import java.io.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

public class Tema2 {
	public static void main(String[] args) throws IOException {
		// verific ca programul sa fie rulat cu un numar corect de parametri
		if (args.length < 2) {
			System.out.println("Usage: java Tema2 <folder_input> <nr_max_threads>");
			return;
		}

		// creez si salvez numele fisierelor de input
		String folderInput = args[0];
		String ordersInput = folderInput.concat("/orders.txt");
		String orderProductsInput = folderInput.concat("/order_products.txt");

		// numarul P de thread uri
		int nrThreads = Integer.parseInt(args[1]);

		// folosesc pentru citirea din fisier BufferedReader pentru ca este thread-safe si nu
		// mai este nevoie sa folosesc synchronized
		BufferedReader bufferedReaderOrders = new BufferedReader(new FileReader(ordersInput));

		// folosesc pentru scriereea in fisier BufferedWriter impreuna cu optiunea flush()
		// pentru ca esste thread-safe
		BufferedWriter bufferedWriterOrders = new BufferedWriter(
				new FileWriter("orders_out.txt"));
		BufferedWriter bufferedWriterProducts = new BufferedWriter(
				new FileWriter("order_products_out.txt"));

		// folosesc ExecutorService pe modelul din laboratorul 7 pentru ca problema descrisa
		// in enunt este una de tipul Replicated Workers:
		// - folosesc un ExecutorService tpeProducts de maxim P thread uri pentru a simula
		// angajatii care se ocupa de prelucararea produselor din comenzi
		// - foloasesc un ExecutorService tpeOrders de maxim P thread uri opentru a simula
		// angajatii care citesc comenzile si numarul de produse
		AtomicInteger inQueueProducts = new AtomicInteger(0);
		ExecutorService tpeProducts = Executors.newFixedThreadPool(nrThreads);

		AtomicInteger inQueueOrders = new AtomicInteger(0);
		ExecutorService tpeOrders = Executors.newFixedThreadPool(nrThreads);
		inQueueOrders.incrementAndGet();
		tpeOrders.submit(new MyRunnableOrders(bufferedReaderOrders, orderProductsInput, bufferedWriterOrders,
				bufferedWriterProducts, tpeOrders, inQueueOrders, tpeProducts, inQueueProducts));
	}
}