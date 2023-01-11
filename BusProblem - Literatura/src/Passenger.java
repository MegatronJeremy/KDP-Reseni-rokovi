
public class Passenger extends Thread {
	public static int ID = 0;
	int PID;
	Station start, end;
	Bus bus;

	public Passenger(Station start, Station end) {
		PID = ID++;
		this.start = start;
		this.end = end;
	}

	@Override
	public void run() {
		while (true) {
			bus = start.waitBus();
			// travel();
			bus.exitTheBus(end);
		}
	}

}
