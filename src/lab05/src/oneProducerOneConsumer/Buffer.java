package oneProducerOneConsumer;

import java.util.LinkedList;
import java.util.concurrent.Semaphore;

public class Buffer {
    final LinkedList<Integer> a = new LinkedList<>();
    Semaphore empty = new Semaphore(10);
    Semaphore full = new Semaphore(0);

    void put(int value) throws InterruptedException {
        empty.acquire();
        synchronized (a) {
            a.add(value);
        }
        full.release();
    }

    int get() throws InterruptedException {
        full.acquire();
        empty.release();
        synchronized (a) {
            return a.pop();
        }
    }
}
