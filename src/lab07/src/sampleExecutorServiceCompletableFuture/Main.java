package sampleExecutorServiceCompletableFuture;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {

	public static void main(String[] args) {
		ExecutorService tpe = Executors.newFixedThreadPool(4);
		CompletableFuture<String> completableFuture = new CompletableFuture<>();
		AtomicInteger counter = new AtomicInteger(0);

		counter.incrementAndGet();
		tpe.submit(new MyRunnable(tpe, "files", "file1.txt", counter, completableFuture));

		completableFuture.thenAccept(result -> {
			if (result != null) {
				System.out.println("File was found at this path: " + result);
			} else {
				System.out.println("File was not found");
			}
		});
	}
}