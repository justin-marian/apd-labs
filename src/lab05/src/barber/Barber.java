package barber;

import java.util.logging.Level;
import java.util.logging.Logger;

/** BARBER CUT HAIR & WAITING
 * barber() {
 *     while(true) {
 *        clients.acquire(); // se caută client; dacă există, el este chemat
 *        chairs.acquire(); // are client, un scaun este eliberat, modificăm freeChairs
 *        freeChairs++; // scaun eliberat
 *        barber_ready.release(); // bărbierul e gata să tundă
 *        chairs.release(); // freeChairs modificat
 *         // tunde bărbierul
 *     }
 * }
 */

public class Barber extends Thread {
    @Override
    public void run() {
        int servedClients = 0;

        do {
            try {
                // searches for a client; if found, the barber calls the client
                Main.barberReady.acquire();
                // has a client, a chair is released, modify freeChairs
                Main.chairsReady.acquire();
                // chair is released
                Main.chairs++;
                // the barber is ready to cut hair
                Main.chairsReady.release();
                // freeChairs is modified
                Main.clientsReady.release();

                try {
                    Thread.sleep(100);
                } catch (InterruptedException ignored) {}

                System.out.println("Barber served client " + servedClients);
                ++servedClients;
            } catch (InterruptedException e) {
                Logger.getLogger(barber.Barber.class.getName())
                        .log(Level.SEVERE, "Thread interrupted", e);
            }
        } while (checkIfThereAnyClients());
    }

    private boolean checkIfThereAnyClients() {
        int count = 0;
        for (var client: Main.leftClients) {
            if (client == 0) {
                count++;
            }
        }

        return count != 0;
    }
}
