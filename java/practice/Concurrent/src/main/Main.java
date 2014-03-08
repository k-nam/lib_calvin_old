package main;

import queue.*;



public  class Main {

	static final int NumConsumer = 1;
	static final int NumProducer = 1;
	static final int NumItemPerProducer = 100000000;
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Factory factory = new Factory();
		Thread[] consumers = new Thread[NumConsumer];
		Thread[] producers = new Thread[NumProducer];
		
		for (int i = 0; i < NumProducer; ++i) {
			producers[i] = new Thread(new Producer(factory, NumItemPerProducer));
		}
		for (int i = 0; i < NumConsumer; ++i) {
			consumers[i] = new Thread(new Consumer(factory));
		}

		
		for (Thread producer : producers) {
			producer.start();
		}
		for (Thread consumer : consumers) {
			consumer.start();
		}

		
		for (Thread producer : producers) {
			try {
				producer.join();
				System.out.println("Producer all finished\n");
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		// Close factory
		factory.close();
		
		for (Thread consumer : consumers) {
			try {
				consumer.join();
				System.out.println("Consumer all finished\n");
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		

	}

}
