package readersWriters.writerPriority;

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
            Logger.getLogger(readersWriters.writerPriority.
                    Writer.class.getName()).log(Level.SEVERE, "Thread interrupted", e);
        }

        do {
            // TODO
            try {
                Main.enter.acquire();
                if (Main.currentReaders > 0 || Main.currentWriters > 0) {
                    Main.waitingWriters++;
                    Main.enter.release();
                    Main.sem_writer.acquire();
                }
                Main.currentWriters++;
                Main.enter.release();

                //Thread.sleep(100);
                System.out.println("Writer " + id + " is writing");
                Main.hasWritten[id] = true;

                // TODO
                Main.enter.acquire();

                System.out.println("Writer " + id + " stopped writing");

                if (Main.waitingReaders > 0 && Main.waitingWriters == 0) {
                    Main.waitingReaders--;
                    Main.sem_reader.release();
                } else if (Main.waitingWriters > 0) {
                    Main.waitingWriters--;
                    Main.sem_writer.release();
                } else if (Main.waitingReaders == 0 && Main.waitingWriters == 0) {
                    Main.enter.release();
                }

            } catch (InterruptedException e) {
                Logger.getLogger(readersWriters.writerPriority.
                        Writer.class.getName()).log(Level.SEVERE, "Thread interrupted", e);
            }
        } while (!Main.hasWritten[id]);
    }
}
