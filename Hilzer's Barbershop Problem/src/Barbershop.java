import java.util.LinkedList;
import java.util.concurrent.Semaphore;

public class Barbershop {
	private static Semaphore entry = new Semaphore(1);
	private static LinkedList<Semaphore> cust = new LinkedList<>();
	private static LinkedList<Semaphore> barb = new LinkedList<>();
	private static int waitingBarbers = 0, waitingCustomers = 0, chair = 3;

	private static class Customer extends Thread {
		private int id = ID++;
		private static int ID = 0;

		private Semaphore x, b;

		boolean request_entry() throws InterruptedException {
			entry.acquire();

			if (cust.size() >= 20) {
				entry.release();
				return false;
			}

			x = new Semaphore(0);
			cust.add(x);

			if (cust.size() > 1 || barb.isEmpty()) {
				// wait for queue or a barber
				entry.release();
				x.acquire();
			}
			
			return true;
		}

		void enter() {
			// sit on sofa
			b = barb.remove();
			// a barber must exist
			b.release();
		}

		void takeHaircut() throws InterruptedException {
			// wait for haircut to be done
			x.acquire();
		}

		void pay() throws InterruptedException {
			// paid
			b.release();
		}

		@Override
		public void run() {
			try {
				while (true) {
					// enter barbershop
					if (!request_entry())
						break;

					System.out.println("Customer " + id + " entering barbershop");
					synchronized (this) {
						wait(100);
					}

					enter();

					System.out.println("Customer " + id + " taking haircut");
					synchronized (this) {
						wait(100);
					}

					takeHaircut();

					System.out.println("Customer " + id + " paying");
					synchronized (this) {
						wait(100);
					}

					pay();

					System.out.println("Customer " + id + " leaving");
					synchronized (this) {
						wait(100);
					}

				}
			} catch (InterruptedException e) {
				System.out.println("No free space...");
			}
		}
	}

	private static class Barber extends Thread {
		private int id = ID++;
		private static int ID = 0;

		private Semaphore x, b;

		private void get_next_customer() throws InterruptedException {
			entry.acquire();

			b = new Semaphore(0);
			barb.add(b);

			// wait for next barber or for a customer
			System.out.println("Barber " + id + " sleeping...");
			
			if (cust.size() > 0) {
				cust.peek().release();
			} else {
				entry.release();
			}
			b.acquire();

			x = cust.remove();

		}

		private void got_customer() {
			// acquired customer - now signal someone who can continue
			if (cust.size() > 0 && barb.size() > 0) {
				cust.peek().release();
			} else
				entry.release();
		}

		void finishHaircut() {
			// haircut done
			x.release();
		}

		void receivePay() throws InterruptedException {
			// wait for pay
			b.acquire();
		}

		@Override
		public void run() {
			try {
				while (true) {
					// get next customer begin
					get_next_customer();
					// get next customer end
					got_customer();

					System.out.println("Barber " + id + " giving haircut");
					synchronized (this) {
						wait(3000);
					}

					finishHaircut();

					System.out.println("Barber " + id + " receiving pay");
					synchronized (this) {
						wait(100);
					}

					receivePay();
				}
			} catch (InterruptedException e) {
			}
		}
	}

	public static void main(String[] args) {
		for (int i = 0; i < 3; i++) {
			new Barber().start();
			new Customer().start();
		}

		for (int i = 0; i < 25; i++) {
			new Customer().start();
		}
	}

}
