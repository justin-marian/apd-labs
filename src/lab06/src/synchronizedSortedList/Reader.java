package synchronizedSortedList;

import java.util.Scanner;
import java.util.List;

import java.io.File;
import java.io.FileNotFoundException;

import java.util.concurrent.CountDownLatch;
import java.util.logging.Level;
import java.util.logging.Logger;

class Reader extends Thread {
    private final String filename;
    private final List<Integer> list;
    private final CountDownLatch latch;

    public Reader(String filename, List<Integer> list, CountDownLatch latch) {
        super();
        this.filename = filename;
        this.list = list;
        this.latch = latch;
    }

    @Override
    public void run() {
        try {
            Scanner scanner = new Scanner(new File(filename));
            while (scanner.hasNextInt()) {
                list.add(scanner.nextInt());
            }
        } catch (FileNotFoundException e) {
            Logger.getLogger(synchronizedSortedList.
                    Main.class.getName()).log(Level.SEVERE, "Thread interrupted", e);
        } finally {
            latch.countDown();
        }
    }
}