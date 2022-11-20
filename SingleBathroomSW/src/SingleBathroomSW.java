import monitor.*;

public class SingleBathroomSW extends MonitorSW {
	int cnt = 0, cCh = 0, group = -1;
	static final int N = 5;
	Condition ok = new Condition(), leave = new Condition(), ch = new Condition();
	int ticket = 0;

	private void enterMW(int who) throws InterruptedException {
		if (ok.queue() || (group != -1 && group != who) || cnt == N) {
			ok.waitSW(ticket++ * 3 + who);
		}

		if (group == -1)
			group = who;
		cnt++;

		System.out.println("Using toilet.. " + (who == 0 ? " woman" : " man")); 

		if (leave.queue()) {
//			System.out.println("signal1");
			leave.signalSW();
		}else if (cnt < N) {
//			System.out.println("signal2");
			signalMWOrChild();
		}
	}

	private void signalMWOrChild() throws InterruptedException {
		boolean personWaiting = ok.queue() && (ok.minrank() % 3 == group);
//		System.out.println(personWaiting + " " + ok.queue() + " " + group + "!???!!" + ok.minrank() % 3);
		if (ch.queue() && (ok.empty() || ch.minrank() < ok.minrank())) {
			ch.signalSW();
		} else if (personWaiting) {
			ok.signalSW();
		}
	}

	void wEntry() throws InterruptedException {
		e.acquire();

		enterMW(0);

		e.release();
	}

	void mEntry() throws InterruptedException {
		e.acquire();

		enterMW(1);

		e.release();
	}

	void chEntry() throws InterruptedException {
		e.acquire();
		
		if (ch.queue() || leave.queue() || ok.queue() || group == -1 || group == 2 || cnt == N) {
//		System.out.println("Waiting.. child"); 
			ch.waitSW(ticket++ * 3);
		}
		cCh++;
		cnt++;

		System.out.println("Using toilet.. child"); 

		if (cnt < N)
			signalMWOrChild();
		
		e.release();
	}

	void mwLeave() throws InterruptedException {
		e.acquire();
		
		if (cCh != 0 && cnt - cCh == 1) {
//			System.out.println("wait leave...");
			leave.waitSW();
		}
		cnt--;
		
		System.out.println("Leaving toilet.. man or woman"); 
		
		if (cnt == 0) {
			group = -1;
			if (ok.queue())
				ok.signalSW();
		} else
			signalMWOrChild();
		
		e.release();
	}

	void chLeave() throws InterruptedException {
		e.acquire();
		
		cnt--;
		cCh--;
		
		System.out.println("Leaving toilet.. child"); 

		if (cCh == 0 && leave.queue())
			leave.signalSW();
		else {
			signalMWOrChild();
		}
		
		e.release();
	}

	void jEntry() throws InterruptedException {
		e.acquire();
		
//		System.out.println(ok.queue() + " " + group + "!!!!!!");
		if (ok.queue() || group != -1) {
			ok.waitSW(ticket++ * 3 + 2);
		}
		group = 2;
		cnt++;
		
		System.out.println("Using toilet.. janitor"); 
		
		e.release();
	}

	void jLeave() throws InterruptedException {
		e.acquire();
		
		group = -1;
		cnt--;
		
		System.out.println("Leaving toilet.. janitor"); 

		if (ok.queue())
			ok.signalSW();
		
		e.release();
	}

	private abstract class Person extends Thread {
		static private int ID = 0;
		private int id = ID++;

		@Override
		public String toString() {
			return Integer.toString(id);
		}

		protected void useToilet() throws InterruptedException {
			synchronized (this) {
				wait(1000);
			}
//			System.out.println("Exiting.. " + toString());
		}
	}

	private class Man extends Person {
		@Override
		public String toString() {
			return super.toString() + " man";
		}
		
		@Override
		public void run() {
			try {
				while (true) {
					mEntry();
					useToilet();
					mwLeave();
				}
			} catch (InterruptedException e) {
			}
		}
	}
	private class Woman extends Person {
		@Override
		public String toString() {
			return super.toString() + " woman";
		}
		
		@Override
		public void run() {
			try {
				while (true) {
					wEntry();
					useToilet();
					mwLeave();
				}
			} catch (InterruptedException e) {
			}
		}
	}
	private class Child extends Person {
		@Override
		public String toString() {
			return super.toString() + " child";
		}
		
		@Override
		public void run() {
			try {
				while (true) {
					chEntry();
					useToilet();
					chLeave();
				}
			} catch (InterruptedException e) {
			}
		}
	}
	private class Janitor extends Person {
		@Override
		public String toString() {
			return super.toString() + " janitor";
		}
		
		@Override
		public void run() {
			try {
				while (true) {
					jEntry();
					useToilet();
					jLeave();
				}
			} catch (InterruptedException e) {
			}
		}
	}
	
	public static void main(String[] args) {
		SingleBathroomSW sb = new SingleBathroomSW();
		for (int i = 0; i < N; i++) {
			sb.new Man().start();
			sb.new Man().start();
			sb.new Woman().start();
			sb.new Woman().start();
			sb.new Child().start();
			sb.new Child().start();
			sb.new Child().start();
			sb.new Janitor().start();
		}
	}

}
