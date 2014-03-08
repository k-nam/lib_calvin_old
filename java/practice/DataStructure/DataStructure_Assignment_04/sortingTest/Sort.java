package sortingTest;

import java.io.*;




public class Sort {
	final static int SIZE=3000000 ;
	public static void main (String[] args) throws IOException{
		
		
		
		System.out.println("asf");
		
		SortingAlgorithm1 a;
		a=new SortingAlgorithm1();
		//SortingAlgorithm2 b;
		//b=new SortingAlgorithm2();
		int[] in1;
		int[] in2;
		in1=Func.randomMaker(true, SIZE);
		in2=Func.randomMaker(false, SIZE);
		int[] out1;
		int[] out2;
		
	
	
		
	/*
		long time1start=System.currentTimeMillis();
		out1=a.bubbleSort(in1);
		long time1end=System.currentTimeMillis();
		long time1=time1end-time1start;
		
		long time2start=System.currentTimeMillis();
		out1=a.bubbleSort(in2);
		long time2end=System.currentTimeMillis();
		long time2=time2end-time2start;
		
		long time3start=System.currentTimeMillis();
		out1=a.bubbleSortImproved(in1);
		long time3end=System.currentTimeMillis();
		long time3=time3end-time3start;
		
		long time4start=System.currentTimeMillis();
		out1=a.bubbleSortImproved(in2);
		long time4end=System.currentTimeMillis();
		long time4=time4end-time4start;
	
		long time5start=System.currentTimeMillis();
		out1=a.insertionSort (in1);
		long time5end=System.currentTimeMillis();
		long time5=time5end-time5start;
		
		long time6start=System.currentTimeMillis();
		out1=a.insertionSort(in2);
		long time6end=System.currentTimeMillis();
		long time6=time6end-time6start;
		
		long time7start=System.currentTimeMillis();
		out1=a.shellSort_1(in1);
		long time7end=System.currentTimeMillis();
		long time7=time7end-time7start;
		
		long time7astart=System.currentTimeMillis();
		out1=a.shellSort_1(in2);
		long time7aend=System.currentTimeMillis();
		long time7a=time7aend-time7astart;
		*/
		long time8start=System.currentTimeMillis();
		out2=a.shellSort_2(in1);
		long time8end=System.currentTimeMillis();
		long time8=time8end-time8start;
		
		long time8astart=System.currentTimeMillis();
		out1=a.shellSort_2(in2);
		long time8aend=System.currentTimeMillis();
		long time8a=time8aend-time8astart;
		
		long time9start=System.currentTimeMillis();
		out1=a.mergeSort(in1);
		long time9end=System.currentTimeMillis();
		long time9=time9end-time9start;
		
		long time10start=System.currentTimeMillis();
		out1=a.mergeSort(in1);
		long time10end=System.currentTimeMillis();
		long time10=time10end-time10start;

		long time11start=System.currentTimeMillis();
		out1=a.quickSort_1(in1);
		long time11end=System.currentTimeMillis();
		long time11=time11end-time11start;
		

		
		
		
		long time12start=System.currentTimeMillis();
		out1=a.heapSort(in1);
		long time12end=System.currentTimeMillis();
		long time12=time12end-time12start;
		
		long time12astart=System.currentTimeMillis();
		out1=a.heapSort(in2);
		long time12aend=System.currentTimeMillis();
		long time12a=time12aend-time12astart;
		
	/*
		long time13start=System.currentTimeMillis();
		out1=a.radixSort(in1);
		long time13end=System.currentTimeMillis();
		long time13=time13end-time13start;
		
		long time13astart=System.currentTimeMillis();
		out1=a.radixSort(in2);
		long time13aend=System.currentTimeMillis();
		long time13a=time13aend-time13astart;
	
		long time13bstart=System.currentTimeMillis();
		out1=a.radixSort_1(in1);
		long time13bend=System.currentTimeMillis();
		long time13b=time13bend-time13bstart;
		
		long time13cstart=System.currentTimeMillis();
		out2=a.radixSort_1(in2);
		long time13cend=System.currentTimeMillis();
		long time13c=time13cend-time13cstart;
		*/
		long time14start=System.currentTimeMillis();
		out1=a.bucketSort(in1);
		long time14end=System.currentTimeMillis();
		long time14=time14end-time14start;
		
		long time14cstart=System.currentTimeMillis();
		out1=a.bucketSort(in2);
		long time14cend=System.currentTimeMillis();
		long time14c=time14cend-time14cstart;
		
		
	/*
		long time15start=System.currentTimeMillis();
		out1=b.quickSort(in1);
		long time15end=System.currentTimeMillis();
		long time15=time15end-time15start;
		
		long time15cstart=System.currentTimeMillis();
		out2=b.quickSort(in2);
		long time15cend=System.currentTimeMillis();
		long time15c=time15cend-time15cstart;
	
		long time16start=System.currentTimeMillis();
		b.radixSort(in2);
		long time16end=System.currentTimeMillis();
		long time16=time16end-time16start;
		*/
		

		
		
		
		//System.out.println("BubbleSort random:"+time1);
		//System.out.println("BubbleSort not random:"+time2);
		//System.out.println("BubbleSortImproved random:"+time3);
		//System.out.println("BubbleSortImproved not random:"+time4);
		//System.out.println("InsertinoSort random:"+time5);
		//System.out.println("InsertionSort not random:"+time6);
		
		//System.out.println("ShellSort random:"+time7);
		//System.out.println("ShellSort not random:"+time7a);
		System.out.println("ShellSort+ random:"+time8);
		System.out.println("ShellSort+ not random:"+time8a);
 		System.out.println("MergeSort random:"+time9);
 	    System.out.println("MergeSort not random:"+time10);
 	     System.out.println("QuickSort random:"+time11);

		
		System.out.println("HeapSort random:"+time12);
		System.out.println("HeapSort not random:"+time12a);
		//System.out.println("RadixSort random:"+time13);
		//System.out.println("RadixSort not random:"+time13a);
		//System.out.println("RadixSort+ random:"+time13b);
	 	//System.out.println("RadixSort+ not random:"+time13c);
	    //stem.out.println("BucketSort random:"+time14);
		//stem.out.println("BucketSort not random:"+time14c);
		//below is by Sung-Hyun
	 	//System.out.println("QuickSort* random:"+time15);
		//System.out.println("QuickSort* not random:"+time15c);
		
		
	/*
		for (int i = 0; i < out1.length; i++) {
			System.out.println(in2[i]);
			
		}
	*/
		
		/*
	
		System.out.println("ShellSort_1 "+a.callingCount[0]+" "+a.comparingCount[0]+" "
				+a.movingCount[0]);
		System.out.println("MergeSort "+a.callingCount[1]+" "+a.comparingCount[1]+" "
				+a.movingCount[1]);
				
		System.out.println("QuickSort "+a.callingCount[0]+" "+a.comparingCount[2]+" "
				+a.movingCount[2]);
		System.out.println("RadixSort(bin) "+b.callingCount[0]+" "+a.comparingCount[3]+" "
				+a.movingCount[3]);
	 */
 
		/*
		// file input processing....    
	    File file1;
	    // if the argument in not specified, end the program
	    try{		    
	    	file1=new File (args[0]);
	    }catch(ArrayIndexOutOfBoundsException e){
	    	System.out.println("You must input a file name");
	    	return;
	    }
	 
	    
	    FileReader fileReader1 = null;

        try {
			fileReader1=new FileReader(file1);
		} catch (FileNotFoundException e) {
			System.out.println("File name "+args[0]+" does not exist.");
		
			return;
		}
	
	    BufferedReader bufferedReader1;
	    bufferedReader1=new BufferedReader(fileReader1);
	    
	    FileWriter outFile=new FileWriter(args[0]+".out");
	    PrintWriter f=new PrintWriter(outFile);
	   */
	     /*
	   	String temp;
		  while (( temp=bufferedReader1.readLine () )!=null ){
			
			
			
			if(temp.equalsIgnoreCase("")){
				continue;
			}
			String[] a;
			a=temp.split("\\s+");
			if (a[0].equalsIgnoreCase("r")){
				int[] b;
				b=Func.randomMaker(true, Integer.parseInt(a[1]), Integer.parseInt( a[2])
						, Integer.parseInt(a[3]));
				Func.starter(b, args[0]+".out", true);
				
				continue;
			}
			
			
		    int length=a.length;
		    int[] b;
		    b=new int [length];
		    for(int i=0; i<length ; i++){
		    	b[i]=Integer.parseInt(a[i]);
		    }
			Func.starter(b, args[0]+".out", false);
			
 
		  }*/
		  
		  
	
	}
	
		  
		  
	  
	
	

}


















