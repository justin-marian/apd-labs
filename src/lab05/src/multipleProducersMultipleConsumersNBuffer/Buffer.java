package multipleProducersMultipleConsumersNBuffer;

import java.util.Queue;
import java.util.concurrent.Semaphore;

public class Buffer {
    private final Queue<Integer> queue;
    private final Semaphore empty;
    private final Semaphore full;

    public Buffer(int size) {
        queue = new LimitedQueue<>(size);
        empty = new Semaphore(size);
        full = new Semaphore(0);
    }

	public void put(int value) throws InterruptedException {
        empty.acquire();
        synchronized (queue) {
            queue.add(value);
        }
        full.release();
    }

    public int get() throws InterruptedException {
        Integer value;

        full.acquire();
        synchronized (queue) {
            value = queue.poll();
        }
        empty.release();

        if (value == null) {
            throw new NullPointerException("Value retrieved is null");
        }
        return value;
    }
}
