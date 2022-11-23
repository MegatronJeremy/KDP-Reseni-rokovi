import java.util.Random;

import monitor.MonitorSC;

public class Barbershop extends MonitorSC {
	static int wi[] = new int[2], ri[] = new int[2];
	static Condition[][] barber_a = new Condition[2][10];
	static Condition[] chair_occ = { new Condition(), new Condition() };
	static Condition[] customer_left = { new Condition(), new Condition() };
	static Condition[] door_open = { new Condition(), new Condition() };
	static boolean[] barber = new boolean[2];
	static boolean[] door = new boolean[2];
	static int cnt[] = new int[2];
	static int price[] = new int[2];
	static final int N = 10;
	static int idA[] = new int[2];

	static {
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 10; i++) {
				barber_a[j][i] = new Condition();
			}
		}
	}

	public static boolean getHaircut(int ko, int id) throws InterruptedException {
		e.acquire();

		if (cnt[0] + cnt[1] == 10) {
			e.release();
			return false;
		}
		if (ko == 2)
			ko = cnt[0] > cnt[1] ? 1 : 0;

		if (barber[ko] == false || cnt[ko] > 0) {
			System.out.println("Musterija " + id + " sedi i ceka " + ko);
			cnt[ko]++;
			int ind = wi[ko]++;
			wi[ko] %= N;
			barber_a[ko][ind].waitSC();
			cnt[ko]--;
		}

		idA[ko] = id;
		barber[ko] = false;

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

		barber[ko] = true;

		if (cnt[ko] > 0) {
			int ind = ri[ko]++;
			ri[ko] %= N;
			barber_a[ko][ind].signalSC();
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
						wait(1000 + new Random().nextInt(1000));
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
