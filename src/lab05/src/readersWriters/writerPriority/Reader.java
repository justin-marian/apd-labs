package readersWriters.writerPriority;

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
            Logger.getLogger(readersWriters.writerPriority.
                    Reader.class.getName()).log(Level.SEVERE, "Thread interrupted", e);
        }

        do {
            // TODO
            try {
                Main.enter.acquire();
                if (Main.currentWriters > 0 || Main.waitingWriters > 0) {
                    Main.waitingReaders++;
                    Main.enter.release();
                    Main.sem_reader.acquire();
                }
                Main.currentReaders++;
                // TODO
                if (Main.waitingReaders > 0) {
                    Main.waitingReaders--;
                    Main.sem_reader.release();
                } else {
                    Main.enter.release();
                }

                Thread.sleep(100);
                System.out.println("Reader " + id + " is reading");
                Main.hasRead[id] = true;

                // TODO
                Main.enter.acquire();
                Main.currentReaders--;

                System.out.println("Reader " + id + " stopped reading");

                // TODO
                if (Main.currentReaders == 0 && Main.waitingWriters > 0) {
                    Main.waitingWriters--;
                    Main.sem_writer.release();
                } else if (Main.currentReaders > 0 || Main.waitingWriters > 0) {
                    Main.enter.release();
                }

            } catch (InterruptedException e) {
                Logger.getLogger(readersWriters.writerPriority.
                        Reader.class.getName()).log(Level.SEVERE, "Thread interrupted", e);
            }
        } while (!Main.hasRead[id]);
    }
}
