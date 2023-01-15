import java.util.Random;

import monitor.MonitorSC;

public class Barbershop extends MonitorSC {
	static int ticket = 0;
	static Condition[] barber_a = new Condition[3];
	static Condition[] chair_occ = { new Condition(), new Condition() };
	static Condition[] customer_left = { new Condition(), new Condition() };
	static Condition[] door_open = { new Condition(), new Condition() };
	static boolean[] barber = new boolean[3];
	static int cnt[] = new int[3];
	static int price[] = new int[2];
	static final int N = 10;
	static int idA[] = new int[2];
	static int idB[] = new int[12];
	static int ri = 0, wi = 0;

	static {
		for (int j = 0; j < 3; j++) {
			barber_a[j] = new Condition();
		}
		for (int i = 0; i < 10; i++)
			idB[i] = -1;
	}

	public static boolean getHaircut(int ko, int id) throws InterruptedException {
		e.acquire();

		if (ko == 2 && barber[0] == true)
			ko = 0;
		else if (ko == 2 && barber[1] == true)
			ko = 1;

		if (cnt[0] + cnt[1] + cnt[2] == 10 && !barber[ko]) {
			e.release();
			return false;
		}

		int myInd = -1;

		if (barber[ko] == false || cnt[ko] > 0 || cnt[2] > 0) {
			System.out.println("Musterija " + id + " sedi i ceka " + ko);
			cnt[ko]++;
			if (ko == 2) {
				myInd = wi++;
				System.out.println(myInd + " " + id);
				wi %= (N + 2);
				barber_a[ko].waitSC(ticket++ * 12 + myInd);
			} else
				barber_a[ko].waitSC(ticket++ * 12);
		}

		if (ko == 2) {
				System.out.println(myInd + " " + id);
			ko = idB[myInd];
			idB[myInd] = -1;
		}

		if (barber[ko])
			barber[ko] = false;

		idA[ko] = id;

		System.out.println("Musterija " + id + " sedi u stolici kod " + ko);

		chair_occ[ko].signalSC();

		door_open[ko].waitSC();

		System.out.println("Placa " + price[ko] + " " + id);
		price[ko] = 0;

		customer_left[ko].signalSC();

		e.release();
		return true;
	}

	public static void getNextCustomer(int ko) throws InterruptedException {
		e.acquire();

		System.out.println("Barber " + ko + " je dostupan");

		if (cnt[2] > 0 && barber_a[2].minrank() < barber_a[ko].minrank()) {
			cnt[2]--;
			int wi = barber_a[2].minrank() % 12;
			idB[wi] = ko;
			System.out.println(wi + " "+ ko);
			barber_a[2].signalSC();
		} else if (cnt[ko] > 0) {
			cnt[ko]--;
			barber_a[ko].signalSC();
		} else {
			barber[ko] = true;
		}

		chair_occ[ko].waitSC();

		e.release();
	}

	public static void finishCut(int pr, int ko) throws InterruptedException {
		e.acquire();

		price[ko] = pr;

		System.out.println("Musterija " + idA[ko] + " placa " + pr);

		door_open[ko].signalSC();

		customer_left[ko].waitSC();

		e.release();
	}

	public static class Customer extends Thread {
		private int id = ID++;
		private static int ID = 0;

		@Override
		public void run() {
			int ko = new Random().nextInt(3);

			try {
				while (true) {
					if (getHaircut(ko, id) == false) {
						System.out.println("Nema mesta " + id);
						break;
					} else
						System.out.println("Musterija " + id + " odlazi");
				}
			} catch (InterruptedException e) {
			}
		}

	}

	public static class Barber extends Thread {
		private int id = ID++;
		private static int ID = 0;

		@Override
		public void run() {
			try {
				while (true) {
					getNextCustomer(id);
					System.out.println("Sledeca musterija " + id + " je " + idA[id]);
					synchronized (this) {
						wait(1000 + new Random().nextInt(2000));
					}
					finishCut(new Random().nextInt(600) + 200, id);
				}
			} catch (InterruptedException e) {

			}
		}
	}

	public static void main(String[] args) {
		new Customer().start();
		new Barber().start();
		new Barber().start();

		for (int i = 0; i < 15; i++) {
			new Customer().start();
		}
	}

}
