import java.util.Random;
import java.util.concurrent.Semaphore;

public class Parking {
	private static final int N = 10;
	private static final int M = 20;
	private static final int K = 3;

	private static int ID = 0;
	private static int empty = N, brU = 0, brI = 0, uzU = K, uzI = 0;
	private static int[] wi = new int[2], ri = new int[2];
	private static boolean rampaZauzeta = false;

	private static Semaphore sem[] = new Semaphore[2 * N + 1];

	public Parking() {
		for (int i = 0; i < 2 * N; i++) {
			sem[i] = new Semaphore(0);
		}
		sem[2 * N] = new Semaphore(1);
	}

	private class Car extends Thread {
		private int id = ID++;

		public void run() {
			try {
				while (true) {
					requestEntry();
					synchronized(this) {
						wait(new Random().nextInt(100) + 300);
					}
					signalEntry();
					synchronized (this) {
						wait(new Random().nextInt(1000) + 2000);
					}
					requestExit();
					synchronized(this) {
						wait(new Random().nextInt(100) + 300);
					}
					signalExit();

				}
			} catch (Exception e) {
				System.out.println("Car " + id + " looking for another parking lot...");
			}
		}

		private void signalExit() throws InterruptedException {
			sem[2 * N].acquire();

			System.out.println("Car " + id + " exiting...");

			uzI++;
			brI--;
			empty++;

			if (wi[0] != ri[0] && uzI >= K) {
				uzI = 0;

				int ind = ri[0]++;
				ri[0] %= N;
				sem[ind].release();
			} else if (wi[1] != ri[1]) {
				int ind = ri[1]++ + N;
				ri[1] %= N;

				sem[ind].release();
			} else {
				rampaZauzeta = false;
				sem[2 * N].release();
			}
		}

		private void requestExit() throws InterruptedException {
			sem[2 * N].acquire();

			System.out.println("Car " + id + " requesting exit...");

			brI++;

			if (rampaZauzeta) {
				int ind = wi[1]++ + N;
				wi[1] %= N;
				
				sem[2 * N].release();
				sem[ind].acquire();
			} else
				rampaZauzeta = true;

			sem[2 * N].release();
		}

		private void signalEntry() throws InterruptedException {
			sem[2 * N].acquire();

			System.out.println("Car " + id + " entered...");

			uzU++;
			brU--;
			empty--;

			if (wi[1] != ri[1] && uzU >= K) {
				uzU = 0;

				int ind = ri[1]++ + N;
				ri[1] %= N;

				sem[ind].release();
			} else if (empty != 0 && wi[0] != ri[0]) {
				int ind = ri[0]++;
				ri[0] %= N;

				sem[ind].release();
			} else {
				rampaZauzeta = false;
				sem[2 * N].release();
			}
		}

		private void requestEntry() throws InterruptedException {
			sem[2 * N].acquire();

			System.out.println("Car " + id + " requesting entry...");

			if (empty + brI - brU == 0) {
				sem[2 * N].release();
				throw new InterruptedException();
			}

			brU++;

			if (empty == 0 || rampaZauzeta) {
				int ind = wi[0]++;
				wi[0] %= N;
				
				sem[2 * N].release();
				sem[ind].acquire();
			} else
				rampaZauzeta = true;


			sem[2 * N].release();
		}

	}

	public static void main(String[] args) {
		Parking p = new Parking();

		for (int i = 0; i < M; i++) {
			p.new Car().start();
		}

	}

}
