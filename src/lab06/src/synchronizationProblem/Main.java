package synchronizationProblem;

import java.util.logging.Level;
import java.util.logging.Logger;

public class Main {
	public static final int N = 100000;
	private final static int N_ITERATIONS = 100;

	public static void main(String[] args) {
		Thread[] threads = new Thread[2];
		boolean sw = true;
		System.out.println("Synchronization problem. Do not solve me with locks");		
		
		for (int j = 0; j < N_ITERATIONS; j++) {
			MyThread.value = 0;
			for (int i = 0; i < 2; i++) {
				threads[i] = new Thread(new MyThread(i));
				threads[i].start();
			}
			for (int i = 0; i < 2; i++) {
				try {
					threads[i].join();
				} catch (InterruptedException e) {
					Logger.getLogger(synchronizationProblem.
							Main.class.getName()).log(Level.SEVERE, "Thread interrupted", e);
				}
			}
			if (MyThread.value != 2 * 3 * N) {
				System.out.println("i was different than " + 2 * 3 * N + ", it is " + MyThread.value);
				sw = false;
			}
		}
		if (sw)
			System.out.println("Correct");
	}
}
