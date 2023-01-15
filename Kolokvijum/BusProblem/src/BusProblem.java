import java.util.Random;

public class BusProblem {
	static class Station {
		int waiting = 0;
		Bus myBus = null;
	}

	private static int N = 12;
	private static int M = 4;
	private static int K = 3;
	private static Station[] stations = new Station[12];

	static {
		for (int i = 0; i < N; i++)
			stations[i] = new Station();
	}

	static class Bus extends Thread {
		int myStation = 0;
		int full = 0;
		int dir = 1;
		int leaving[] = new int[N];

		int id = ID++;
		private static int ID = 0;

		@Override
		public void run() {
			try {
				while (true) {
					Station st = stations[myStation];
					synchronized (st) {
						while (st.myBus != null)
							st.wait();
						st.myBus = this;
						st.notifyAll();
						
						while (leaving[myStation] != 0) 
							st.wait();

						while (st.waiting != 0 && full != K)
							st.wait();

						st.myBus = null;
						st.notifyAll();
					}
					if (myStation == N - 1 && dir == 1)
						dir = -1;
					if (myStation == 0 && dir == -1)
						dir = 1;
					myStation += dir;

					rideToNextStation();

				}
			} catch (InterruptedException e) {
			}
		}

		private void rideToNextStation() throws InterruptedException {
			System.out.println("Bus " + id + " going to station " + myStation);
			synchronized (this) {
				wait(2000);
			}
		}
	}

	static class Passenger extends Thread {
		int id = ID++;
		int dir = 1;
		private static int ID = 0;

		@Override
		public void run() {
			try {
				while (true) {
					int src = new Random().nextInt(N - 1);
					int dst = new Random().nextInt(src+1, N);
					if (dir == 1) {
						src = new Random().nextInt(N-1);
						dst = new Random().nextInt(src+1, N);
						dir = 0;
					} else {
						src = new Random().nextInt(1, N);
						dst = new Random().nextInt(src);
						dir = 1;
					}
					takeBus(src, dst);
				}
			} catch (InterruptedException e) {
			}
		}

		private void takeBus(int src, int dst) throws InterruptedException {
			Station s1 = stations[src], s2 = stations[dst];
			Bus onBus;
			synchronized (s1) {
				while (s1.myBus != null)
					s1.wait();
				s1.waiting++;

				while (s1.myBus == null || s1.myBus.full == K)
					s1.wait();

				onBus = s1.myBus;

				s1.myBus.leaving[dst]++;
				s1.myBus.full++;
				s1.waiting--;
				s1.notifyAll();
			}

			rideBus(onBus, src, dst);

			synchronized (s2) {
				while (s2.myBus != onBus)
					s2.wait();

				onBus.full--;
				onBus.leaving[dst]--;
				leaveBus(onBus);
				
				s2.notifyAll();
			}

		}

		private void leaveBus(BusProblem.Bus onBus) {
			System.out.println("Passenger " + id + " leaving bus " + onBus.id);
		}

		private void rideBus(BusProblem.Bus onBus, int src, int dst) {
			System.out.println("Passenger " + id + " riding bus " + onBus.id + " " + src + " " + dst);
		}
	}

	public static void main(String[] args) {
		for (int i = 0; i < M; i++) {
			new Bus().start();
			for (int j = 0; j < 10; j++) {
				new Passenger().start();
			}
		}
	}

}
