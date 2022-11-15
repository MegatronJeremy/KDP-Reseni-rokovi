import java.util.ArrayList;
import java.util.concurrent.Semaphore;

public class BumperCars {
	private static final int N = 10;
	private static final int M = 50;

	private static Semaphore e = new Semaphore(1);
	private static Semaphore[] dp = new Semaphore[M];
	private static Semaphore[] dc = new Semaphore[N];

	private static ArrayList<Integer> listp = new ArrayList<>();
	private static ArrayList<Integer> listc = new ArrayList<>();

	public BumperCars() {
		for (int i = 0; i < M; i++)
			dp[i] = new Semaphore(0);
		for (int i = 0; i < N; i++)
			dc[i] = new Semaphore(0);
	}

	private class Person extends Thread {
		private int id;

		public Person(int id) {
			this.id = id;
		}

		public void run() {
			try {
				while (true) {
					e.acquire();

					if (listc.size() == 0 || listp.size() > 0) {
						listp.add(id);
						e.release();
						dp[id].acquire();
					}

					int k = listc.remove(0);

					if (listc.size() > 0 && listp.size() > 0) {
						int j = listp.remove(0);
						dp[j].release();
					} else
						e.release();

					useCar(k);

					dc[k].release();
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

		private void useCar(int k) throws InterruptedException {
			System.out.println("Person " + id + " using car " + k);
			synchronized (this) {
				wait(1000);
			}
			System.out.println("Person " + id + " done with car " + k);
		}
	}

	private class Car extends Thread {
		private int id;

		public Car(int id) {
			this.id = id;
		}

		@Override
		public void run() {
			try {
				while (true) {
					e.acquire();

					listc.add(id);

					if (listp.size() > 0) {
						int j = listp.remove(0);
						dp[j].release();
					} else
						e.release();

					dc[id].acquire();
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

	public static void main(String[] args) {
		BumperCars bc = new BumperCars();

		for (int i = 0; i < N; i++) {
			bc.new Car(i).start();
		}

		for (int i = 0; i < M; i++) {
			bc.new Person(i).start();
		}
	}

}
