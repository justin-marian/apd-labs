package bug5;

/**
 * Solve the deadlock.
 * Rewrite the code such that MyThreadA and MyThreadB to
 * execute additions (on different variables) in parallel.
 * The results in valueA and valueB should be deterministic at the end.
 */
public class MyThreadB implements Runnable {

    @Override
    public void run() {
        synchronized (Main.lockB) {
            for (int i = 0; i < Main.N; i++) {
                Main.valueB++;
            }
            // THREAD B IS WAITING THREAD A!
            /*\
            synchronized (Main.lockA) {
                for (int i = 0; i < Main.N; i++) {
                    Main.valueA++;
                }
            }
            \*/
        }
    }
}
