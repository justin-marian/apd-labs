package oneProducerOneConsumer;

/**
 * DO NOT MODIFY
 */
public class Producer implements Runnable {
    private final Buffer buffer;

    public Producer(Buffer buffer) {
        this.buffer = buffer;
    }

    @Override
    public void run() {
        for (int i = 0; i < Main.N; i++) {
            try {
                buffer.put(i);
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }
    }

}
