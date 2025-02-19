package readersWriters.readerPriority;

import java.util.concurrent.BrokenBarrierException;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Reader extends Thread {
    private final int id;

    public Reader(int id) {
        super();
        this.id = id;
    }

    @Override
    public void run() {
        try {
            Main.barrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            Logger.getLogger(readersWriters.readerPriority.
                    Reader.class.getName()).log(Level.SEVERE, "Thread interrupted", e);
        }

        do {
            // Acquire the mutexNumberOfReaders semaphore for letting readers to read first
            try {
                Main.mutexNumberOfReaders.acquire();
                Main.currentReaders++;
                // If it's the first reader, acquire the shared space
                if (Main.currentReaders == 1) {
                    Main.readWriteSem.acquire();
                }

                // Release the semaphore for other readers
                Main.mutexNumberOfReaders.release();

                System.out.println("Reader " + id + " is reading");
                Main.hasRead[id] = true;
                Thread.sleep(100);

                // Let again the readers to have priority over writers.
                Main.mutexNumberOfReaders.acquire();
                Main.currentReaders--;

                System.out.println("Reader " + id + " stopped reading");

                // If there are no readers anymore, let writers to write.
                if (Main.currentReaders == 0) {
                    Main.readWriteSem.release();
                }
                // Let again the readers to read.
                Main.mutexNumberOfReaders.release();
            } catch (InterruptedException e) {
                Logger.getLogger(readersWriters.readerPriority.
                        Reader.class.getName()).log(Level.SEVERE, "Thread interrupted", e);
            }
        } while (!Main.hasRead[id]);
    }
}
