package queue;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;

public class Factory {
	
	public Factory() { 
	}
	
	public boolean hasNext() {		
		if (isFinished == true && queue.isEmpty()) {
			return false;
		} else {
			return true;
		}
	}

	public Object getNext() {
		try {
			return queue.poll(10, TimeUnit.MILLISECONDS);
		} catch (InterruptedException e) {
			return null;
		}
	}
	
	public boolean pushBack(Object newItem) {
		try {
			queue.put(newItem);
			return true;
		} catch (InterruptedException e) {
			e.printStackTrace();
			return false;
		}
	}
	
	public void close() {
		this.isFinished = true;
	}
	
	private boolean isFinished = false;
	private final int queueSize = 1000;
	private BlockingQueue<Object> queue = new LinkedBlockingQueue<Object>(queueSize);

}
