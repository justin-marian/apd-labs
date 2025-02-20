package oneProducerOneConsumer;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * DO NOT MODIFY
 */
public class Main {
    public static final int N = 1000000;

    public static void main(String[] args) {
        Buffer buffer = new Buffer();

        Thread[] threads = new Thread[2];
        threads[0] = new Thread(new Producer(buffer));
        threads[1] = new Thread(new Consumer(buffer));

        for (int i = 0; i < 2; i++) {
            threads[i].start();
        }
        for (int i = 0; i < 2; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                Logger.getLogger(oneProducerOneConsumer.
                        Main.class.getName()).log(Level.SEVERE, "Thread interrupted", e);
            }
        }
    }

}
/* DO NOT MODIFY */