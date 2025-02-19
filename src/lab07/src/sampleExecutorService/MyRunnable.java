package sampleExecutorService;

import java.io.File;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

public class MyRunnable implements Runnable {
	private final String path;
	private final ExecutorService tpe;
	private final AtomicInteger inQueue;

	public MyRunnable(String path, ExecutorService tpe, AtomicInteger inQueue) {
		this.path = path;
		this.tpe = tpe;
		this.inQueue = inQueue;
	}

	@Override
	public void run() {
		try {
			File file = new File(path);
			if (file.isFile()) {
				System.out.println("File path: " + file.getPath());
			} else if (file.isDirectory()) {
				File[] files = file.listFiles();

				if (files != null) {
					for (File f : files) {
						inQueue.incrementAndGet();
						tpe.submit(new MyRunnable(f.getPath(), tpe, inQueue));
					}
				}
			}
		} finally {
			int left = inQueue.decrementAndGet();
			if (left == 0) {
				tpe.shutdown();
			}
		}
	}
}
