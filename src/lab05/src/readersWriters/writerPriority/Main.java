package readersWriters.writerPriority;


import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.Semaphore;

/**
 *  // readers that read from the memory area
 *  int readers = 0;
 *  // writers that write to the memory area
 *  // (there will be only one, there can't be multiple writers writing simultaneously)
 *  int writers = 0;
 *  int waiting_readers = 0; // readers waiting to enter memory area
 *  int waiting_writers = 0; // writers waiting to enter the memory area
 *  // semaphore used to put writers on hold, if we have a writer
 *  // or one or more readers in the memory area (critical area)
 *  semaphore sem_writer(0);
 *  // semaphore used to put readers on hold if we have a writer writing to the memory area
 *  // or if we have writers waiting (since they take precedence over readers)
 *  semaphore sem_reader(0);
 *  // semaphore used as a mutex to protect the memory area (critical area)
 *  semaphore enter(1);
 *-
 *  reader (int id) {
 *      while(true) {
 *          enter.acquire();
 *          // if we have at least one writer writing to the shared resource
 *          // or if we have a writer waiting, the reader is waiting
 *          if (writers > 0 || waiting_writers > 0) {
 *              waiting_readers++;
 *              enter.release();
 *              sem_reader.acquire();
 *          }
 *-
 *          readers++;
 *          if (waiting_readers > 0) {
 *              // another reader came into the shared resource,
 *              // exiting the wait state
 *              waiting_readers--;
 *              sem_reader.release();
 *          } else if (waiting_readers == 0) {
 *              enter.release();
 *          }
 *-
 *           // read from shared area
 *          enter.acquire();
 *          readers--;
 *          if (readers == 0 && waiting_writers > 0) {
 *              waiting_writers--;
 *              sem_writer.release();
 *          } else if (readers > 0 || waiting_writers == 0) {
 *              enter.release();
 *          }
 *      }
 * }
 *-
 * writer (int id) {
 *      while(true) {
 *          enter.acquire();
 *          if (readers > 0 || writers > 0) {
 *              waiting_writers++;
 *              enter.release();
 *              sem_writer.acquire();
 *          }
 *          writers++;
 *          enter.release();
 *-
 *          // write to the shared area
 *          enter.acquire();
 *          writers--;
 *          if (waiting_readers > 0 && waiting_writers == 0) {
 *              waiting_readers--;
 *              sem_reader.release();
 *          } else if (waiting_writers > 0) {
 *              waiting_writers--;
 *              sem_writer.release();
 *          } else if (waiting_readers == 0 && waiting_writers == 0) {
 *              enter.release();
 *          }
 *      }
 * }
 */

public class Main {
    public static final int WRITERS = 6;
    public static final int READERS = 8;

    public static int currentReaders = 0;
    public static int currentWriters = 0;
    public static int waitingReaders = 0;
    public static int waitingWriters = 0;

    public static final boolean[] hasRead = new boolean[READERS];
    public static final boolean[] hasWritten = new boolean[WRITERS];

    // used to make sure threads try to go in the same to the shared zone
    public static CyclicBarrier barrier = new CyclicBarrier(READERS + WRITERS);

    // TODO: add semaphores
    public static Semaphore sem_writer = new Semaphore(0);
    public static Semaphore sem_reader = new Semaphore(0);
    public static Semaphore enter = new Semaphore(1);

    public static void main(String[] args) throws InterruptedException {
        Thread[] readers = new Reader[READERS];
        Thread[] writers = new Writer[WRITERS];

        for (int i = 0; i < READERS; i++) {
            readers[i] = new Reader(i);
        }

        for (int i = 0; i < WRITERS; i++) {
            writers[i] = new Writer(i);
        }

        for (var reader: readers) {
            reader.start();
        }

        for (var writer: writers) {
            writer.start();
        }

        for (var reader: readers) {
            reader.join();
        }

        for (var writer: writers) {
            writer.join();
        }
    }
}
