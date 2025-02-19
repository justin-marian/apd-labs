package synchronizedSortedList;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Main {
    private final static int N_ITERATIONS = 100;
    private final static int NUMBER_OF_THREADS = 4;

    public static void main(String[] args) {
        Thread[] threads = new Thread[NUMBER_OF_THREADS];
        boolean sw = true;

        for (int i = 0; i < N_ITERATIONS; i++) {
            List<Integer> list = Collections.synchronizedList(new ArrayList<>());
            CountDownLatch latch = new CountDownLatch(NUMBER_OF_THREADS - 1); // 3 reader threads

            threads[0] = new Reader("elemente1.txt", list, latch);
            threads[1] = new Reader("elemente2.txt", list, latch);
            threads[2] = new Reader("elemente3.txt", list, latch);
            threads[3] = new Sort(list, latch);

            for (int j = 0; j < NUMBER_OF_THREADS; j++) {
                threads[j].start();
            }

            for (int j = 0; j < NUMBER_OF_THREADS; j++) {
                try {
                    threads[j].join();
                } catch (InterruptedException e) {
                    Logger.getLogger(synchronizedSortedList.
                            Main.class.getName()).log(Level.SEVERE, "Thread interrupted", e);
                }
            }

            for (int j = 0; j < list.size() - 1; j++) {
                if (list.get(j) > list.get(j + 1)) {
                    sw = false;
                    System.out.println("Incorrect result: " + list.get(j) + " > " +  list.get(j + 1));
                    break;
                }
            }
        }

        if (sw) {
            System.out.println("Correct");
        }
    }
}