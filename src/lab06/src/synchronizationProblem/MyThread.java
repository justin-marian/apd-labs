package synchronizationProblem;

public class MyThread implements Runnable {
	private final int id;
	public static int value;
	private static final Object lock = new Object(); // Shared lock object

	public MyThread(int id) {
		this.id = id;
	}

	@Override
	public void run() {
		if (id == 0) {
			for (int i = 0; i < Main.N; i++) {
				synchronized (lock) {
					value += 3;
				}
			}
		} else {
			for (int i = 0; i < Main.N; i++) {
				synchronized (lock) {
					value += 3;
				}
			}
		}
	}
}
