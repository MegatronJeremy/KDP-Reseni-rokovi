import java.util.Arrays;
import java.util.Random;

import monitor.MonitorSC;

public class Raskrsnica extends MonitorSC {
	private static int segment[] = new int[2];
	private static Condition c[] = { new Condition(), new Condition() };
	private static Condition cLeave = new Condition();
	private static final int N = 10;

	private static void signalSegment(int i) {
		if (i == 0 && segment[i] == 0 && cLeave.queue()) {
			cLeave.signalSC();
		}

		if (segment[i] == 0 && (i == 0 || segment[0] == 0) && c[i].queue()) {
			c[i].signalSC();
		}
	}

	public static void enter(int i) throws InterruptedException {
		e.acquire();

		segment[i]++;
		System.out.println("Enter main " + Arrays.toString(segment));


		e.release();
	}

	public static void exit(int i) throws InterruptedException {
		e.acquire();

		segment[i]--;
		System.out.println("Exit main " + Arrays.toString(segment));

		signalSegment(i);


		e.release();
	}

	public static void enterSide(int i) throws InterruptedException {
		e.acquire();

		while (i == 0 && segment[i] != 0 || i == 1 && segment[i] != 0 && segment[0] != 0) {
			c[i].waitSC();
		}

		segment[i]++;
		System.out.println("Enter side " + Arrays.toString(segment));

		signalSegment(i);
		

		e.release();
	}

	public static void exitSide(int i) throws InterruptedException {
		e.acquire();

		while (i == 1 && segment[0] > 0) {
			c[0].waitSC();
		}

		segment[i]--;
		System.out.println("Exit side " + Arrays.toString(segment));

		signalSegment(i);


		e.release();
	}
	
	public static class Car extends Thread {
		private int id = ID++;
		private static int ID = 0;
		
		@Override
		public void run() {
			try {
			while (true) {
				int side = new Random().nextInt(2);
				int i = new Random().nextInt(2);
				
				System.out.println("Car " + id + " entering" + (side == 0 ? " main " : " side ") + i);
				
				if (side == 0) {
					enter(i);
				} else {
					enterSide(i);
				}
				
				System.out.println("Car " + id + " driving");
				
				synchronized(this) {
					wait(100 + new Random().nextInt(200));
				}
				
				int exitSide = side == 1 ? 0 : new Random().nextInt(2);

				
				if (exitSide == 0) {
					exit(i);
				} else {
					exitSide(i);
				}

				System.out.println("Car " + id + " exiting" + (side == 0 ? " main " : " side ") + i);
				
			}
			} catch (InterruptedException e) {
			}
		}
		
	}
	
	public static void main(String[] args) {
		for (int i = 0; i < N; i++) {
			new Car().start();
		}
	}
}
