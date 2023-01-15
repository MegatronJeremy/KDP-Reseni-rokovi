import java.util.concurrent.Semaphore;

public abstract class Person extends Thread {
	static private int N = 10;
	static private int ID = 0;

	static Semaphore toilet = new Semaphore(N);
	static Semaphore mutex = new Semaphore(1);
	static Semaphore entry = new Semaphore(1);
	static Semaphore entryCH = new Semaphore(0);

	private int id;
	

	public Person() {
		id = ID++;
	}

	@Override
	public String toString() {
		return Integer.toString(id);
	}

	public void useToilet() throws InterruptedException {
		System.out.println("Using toilet.. " + toString());
		synchronized (this) {
			wait(1000);
		}
		System.out.println("Exiting.. " + toString());
	}

	public static void main(String[] args) {
		for (int i = 0; i < 5; i++) {
			new Male().start();
		}
		for (int i = 0; i < 5; i++) {
			new Female().start();
		}
		for (int i = 0; i < 5; i++) {
			new Child().start();
		}
		for (int i = 0; i < 3; i++) {
			new Janitor().start();
		}
	}
}

class Male extends Person {
	private static Semaphore mutexM = new Semaphore(1);
	private static int cntM = 0;

	@Override
	public String toString() {
		return "male " + super.toString();
	}

	@Override
	public void run() {
		while (true) {
			try {
				entry.acquire();
				mutexM.acquire();
				cntM++;
				if (cntM == 1) {
					mutex.acquire();
					entryCH.release();
				}
				mutexM.release();
				entry.release();
				
				toilet.acquire();
				useToilet();
				toilet.release();
				
				mutexM.acquire();
				cntM--;
				if (cntM == 0) {
					entryCH.acquire();
					mutex.release();
				}
				mutexM.release();

			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

	}

}

class Female extends Person {
	private static Semaphore mutexF = new Semaphore(1);
	private static int cntF = 0;

	@Override
	public String toString() {
		return "female " + super.toString();
	}

	@Override
	public void run() {
		while (true) {
			try {
				entry.acquire();
				mutexF.acquire();
				cntF++;
				if (cntF == 1) {
					mutex.acquire();
					entryCH.release();
				}
				mutexF.release();
				entry.release();
				
				toilet.acquire();
				useToilet();
				toilet.release();
				
				mutexF.acquire();
				cntF--;
				if (cntF == 0) {
					entryCH.acquire();
					mutex.release();
				}
				mutexF.release();

			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

	}

}

class Janitor extends Person {
	@Override
	public String toString() {
		return "janitor " + super.toString();
	}

	@Override
	public void run() {
		while (true) {
			try {
				entry.acquire();
				mutex.acquire();
				entry.release();
				toilet.acquire();
				useToilet();
				toilet.release();
				mutex.release();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
}

class Child extends Person {
	private static Semaphore mutexC = new Semaphore(1);

	private static int cntC = 0;
	
	@Override
	public String toString() {
		return "child " + super.toString();
	}

	@Override
	public void run() {
		while (true) {
			try {
				entry.acquire();
				entry.release();
				
				mutexC.acquire();
				if (cntC == 0)
					entryCH.acquire();
				cntC++;
				mutexC.release();
				
				toilet.acquire();
				useToilet();
				toilet.release();

				mutexC.acquire();
				if (cntC == 1)
					entryCH.release();
				cntC--;
				mutexC.release();
				
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
}