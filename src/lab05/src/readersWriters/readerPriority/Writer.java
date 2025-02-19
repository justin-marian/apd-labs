package readersWriters.readerPriority;

import java.util.concurrent.BrokenBarrierException;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Writer extends Thread {
    private final int id;

    public Writer(int id) {
        super();
        this.id = id;
    }

    @Override
    public void run() {
        try {
            Main.barrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            Logger.getLogger(readersWriters.readerPriority.
                    Writer.class.getName()).log(Level.SEVERE, "Thread interrupted", e);
        }

        do {
            try {
                // shared space is locked
                Main.readWriteSem.acquire();

                System.out.println("Writer " + id + " is writing");
                Main.hasWritten[id] = true;

                Main.readWriteSem.release();

                Thread.sleep(100);
                System.out.println("Writer " + id + " stopped writing");

            } catch (InterruptedException e) {
                Logger.getLogger(readersWriters.readerPriority.
                        Writer.class.getName()).log(Level.SEVERE, "Thread interrupted", e);
            }
        } while (!Main.hasWritten[id]);
    }
}
