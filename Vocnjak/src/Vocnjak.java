import java.util.Random;

public class Vocnjak {
	static private int gajbice = 0, tiket = 0, next = 0;
	static private final int prikolica_kap = 20, N = 5;

	private static class Traktor extends Thread {
		private int id = ID++;
		private static int ID = 0;
		@Override
		public void run() {
			try {
				synchronized (Vocnjak.class) {
					while (true) {
						spreman_da_pokupim(id);
						synchronized (this) {
							System.out.println(id + "...spavam");
							wait(3000);
						}
					}
				}
			} catch (Exception e) {
			}
		}
	}

	private static class Nosac extends Thread {
		private int nosivost = new Random().nextInt(1, 11);

		@Override
		public void run() {
			try {
				while (true) {
					ostavljam_gajbice(nosivost);
					synchronized (this) {
//						System.out.println("...berem");
						wait(5000);
					}
				}
			} catch (Exception e) {
			}
		}
	}

	private static synchronized void spreman_da_pokupim(int id) throws InterruptedException {
		int red = tiket++;
		System.out.println(id + "...je spreman");
		while (red != next || gajbice < prikolica_kap)
			Vocnjak.class.wait();
		System.out.println(id + "...nosi gajbice");
		gajbice -= prikolica_kap;
		next++;
		if (gajbice >= prikolica_kap)
			Vocnjak.class.notifyAll();
	}

	private static synchronized void ostavljam_gajbice(int nosivost) {
//		System.out.println("...ostavljam " + nosivost + " gajbica");
		gajbice += nosivost;
		if (gajbice >= prikolica_kap)
			Vocnjak.class.notifyAll();
	}
	
	public static void main(String[] args) {
		for (int i = 0; i < N; i++) {
			new Traktor().start();
			new Nosac().start();
		}
	}

}
