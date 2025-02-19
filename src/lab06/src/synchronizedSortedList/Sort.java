package synchronizedSortedList;

import java.util.Collections;
import java.util.List;
import java.util.concurrent.CountDownLatch;

class Sort extends Thread {
    private final List<Integer> list;
    private final CountDownLatch latch;

    public Sort(List<Integer> list, CountDownLatch latch) {
        super();
        this.list = list;
        this.latch = latch;
    }

    @Override
    public void run() {
        try {
            latch.await(); // Wait for all reader threads to finish
            Collections.sort(list);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }
}