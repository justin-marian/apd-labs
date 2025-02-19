package barber;

import java.util.logging.Level;
import java.util.logging.Logger;

/** CLIENT IS SERVED & WAIT IN CHAIR
 * client(int id) {
 *      while(true) {
 *          chairs.acquire(); // a customer comes and looks for a free seat
 *          if (freeChairs > 0) {
 *              freeChairs--; // client found seat
 *              clients.release(); // the barber knows that a customer has taken a seat
 *              chairs.release(); // modified freeChairs
 *              barber_ready.acquire(); // the customer waits for his turn to be cut
 *          } else {
 *              // there are no free seats
 *              chairs.release();
 *              // client leaves untrimmed
 *          }
 *      }
 * }
 */

public class Client extends Thread {
    private final int id;

    public Client(int id) {
        super();
        this.id = id;
    }

    @Override
    public void run() {
        // TODO
        try {
            // a client comes in and looks for an available seat
            Main.chairsReady.acquire();

            if (Main.chairs > 0) {
                // client occupies a seat, if it exists
                Main.chairs--;
                // the barber knows a client took a seat
                Main.chairsReady.release();
                // the client waits for their turn to get a haircut
                Main.barberReady.release();

                Main.clientsReady.acquire();

                System.out.println("Client " + id + " is waiting for haircut");
                System.out.println("Available seats: " + Main.chairs);
                System.out.println("Client " + id + " is served by the barber");

                Main.leftClients[id] = Main.SERVED_CLIENT;
            } else {
                // freeChairs is modified
                Main.chairsReady.release();

                System.out.println("Client " + id + " left unserved");
                Main.leftClients[id] = Main.UNSERVED_CLIENT;
            }
        } catch (InterruptedException e) {
            Logger.getLogger(barber.Client.class.getName())
                    .log(Level.SEVERE, "Thread interrupted", e);
        }
    }
}
