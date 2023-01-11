import java.util.Random;
import java.util.concurrent.Semaphore;

public class Barbershop {
	private static Semaphore e = new Semaphore(1);
	private static final int K = 3, N = 20;
	private static Semaphore[] barber_a = new Semaphore[N];
	private static Semaphore[] chair_occ = new Semaphore[K];
	private static Semaphore[] door_open = new Semaphore[K];
	private static Semaphore[] customer_left = new Semaphore[K];
	private static int wiC = 0, riC = 0, wiB = 0, riB = 0, cntC = 0, cntB = 0;

	private static int[] pay = new int[K];

	static {
		for (int i = 0; i < K; i++) {
			door_open[i] = new Semaphore(0);
			chair_occ[i] = new Semaphore(0);
			customer_left[i] = new Semaphore(0);
		}
		for (int i = 0; i < N; i++) {
			barber_a[i] = new Semaphore(0);
		}
	}

	private static class Customer extends Thread {
		private int id = ID++;
		private static int ID = 0;

		private boolean take_haircut() throws InterruptedException {
			e.acquire();

			if (cntC == 20) {
				e.release();
				return false;
			}

			if (cntB == 0) {
				System.out.println("Customer " + id + " waiting for barber");
				cntC++;

				int ind = wiC++;
				wiC %= N;
				e.release();
				barber_a[ind].acquire();
				e.acquire();

				cntC--;
			}

			cntB--;

			int myB = riB++;
			riB %= K;

			System.out.println("Customer " + id + " sitting in chair of " + myB);

			e.release();

			chair_occ[myB].release();

			door_open[myB].acquire();

			int y = pay[myB];
			pay[myB] = 0;

			System.out.println("Customer " + id + " paying: " + y + " to " + myB);

			customer_left[myB].release();

			return true;
		}

		@Override
		public void run() {
			try {
				while (true) {
					take_haircut();
					synchronized (this) {
						wait(1000 + new Random().nextInt(2000));
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

		private int get_next_customer() throws InterruptedException {
			e.acquire();

			int myInd = wiB++;
			wiB %= K;

			System.out.println("Barber " + myInd + " available");

			cntB++;

			if (cntC != 0) {
				int ind = riC++;
				riC %= N;

				barber_a[ind].release();
			} else {
				e.release();
			}

			chair_occ[myInd].acquire();

			e.release();

			return myInd;
		}

		void finishHaircut(int myInd) throws InterruptedException {
			pay[myInd] = new Random().nextInt(200) + 600;

			System.out.println("Customer for " + myInd + " has to pay " + pay[myInd]);

			door_open[myInd].release();

			customer_left[myInd].acquire();

			e.release();
		}

		@Override
		public void run() {
			try {
				while (true) {
					// get next customer begin
					int myInd = get_next_customer();

					System.out.println("Barber " + id + " giving haircut to " + myInd);
					synchronized (this) {
						wait(1000 + new Random().nextInt(2000));
					}

					finishHaircut(myInd);
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
