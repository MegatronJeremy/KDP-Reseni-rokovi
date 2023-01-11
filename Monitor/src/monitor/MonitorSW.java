package monitor;

import java.util.TreeMap;
import java.util.concurrent.Semaphore;

public class MonitorSW {
	protected static Semaphore e = new Semaphore(1);
	
	protected static class Condition {
		private Semaphore x = new Semaphore(0);
		private TreeMap<Integer, Semaphore> pq =  new TreeMap<>();
		private int cnt = 0;
		
		public Condition(){} 
		
		public void waitSW() throws InterruptedException {
			cnt++;
			e.release();
			x.acquire();
		}
		
		public void signalSW() throws InterruptedException {
			if (cnt == 0) return;
			cnt--;
			if (pq.isEmpty())
				x.release();
			else {
				Semaphore p = pq.pollFirstEntry().getValue();
				p.release();
			}
			e.acquire();
		}
		
		public boolean queue() {
			return cnt > 0;
		}
		
		public boolean empty() {
			return cnt == 0;
		}
		
		public void waitSW(int rank) throws InterruptedException {
			cnt++;
			Semaphore x = new Semaphore(0);
			pq.put(rank, x);
			e.release();
			x.acquire();
		}
		
		public int minrank() {
			if (pq.isEmpty()) return -1;
			return pq.firstKey();
		}
		
	}
	
}
