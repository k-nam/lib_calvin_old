package sortingTest;

import java.util.*;
import java.io.*;


public class Func {
	
	public static void swap(int[] inputArray, int index_0, int index_1){
		int temp;
		temp=inputArray[index_0];
		inputArray[index_0]=inputArray[index_1];
		inputArray[index_1]=temp;
	}
	
	
	
	/**
	 * This method inserts a given integer inputInt to index's position of the array
	 * and shift the next elements to the right. The rightmost element gets lost.
	 * @param index
	 * @param inputInt
	 * 2007 May 1st, a little bit modified (to include an aditional argument interval)
	 * This will shift the elements along the certain interval only, not caring irrelevant elements
	 * unsorted means the index of the first element in the unsorted (don't care) part of the array.
	 * Should not modify any element whose index is equal to or greater than 'unsorted'!!!
	 */
	public static void insertIntoArray(int[] inputArray, int interval, int index, int unsorted, int inputInt){
		
		int numOfElements=(unsorted-1-index)/interval;
		
		for (int i = numOfElements ; i>0 ; i-- ){
			inputArray[interval*i+index]=inputArray[interval*(i-1)+index];
		}
		inputArray[index]=inputInt;		
	}
	
	
	
	//if false, the array contains 5% of unordered elements
	public static int[] randomMaker ( boolean random, int num, int min	, int max){
		int[] temp;
		temp= new int [num];
		
		Random rand;
		rand=new Random();
		
		if ( random==true){
			for ( int i= 0 ; i < num ; i ++){
				temp[i]=( (rand.nextInt()%(max-min+1)) +min);
			}
			return(temp);
		}
		else{
			for ( int i= 0 ; i < num ; i ++){
				if (Math.random()>0.95){
					temp[i]=rand.nextInt();
				}
				else{
					temp[i]=i;
				}
			}
			return(temp);
		}
		
	}
	
	public static int[] randomMaker ( boolean random, int num){
		int[] temp;
		temp= new int [num];
		
		Random rand;
		rand=new Random();
		
		if ( random==true){
			for ( int i= 0 ; i < num ; i ++){
				temp[i]=(rand.nextInt());
			}
			return(temp);
		}
		else{
			for ( int i= 0 ; i < num ; i ++){
				if (Math.random()>0.95){
					temp[i]=rand.nextInt();
				}
				else{
					temp[i]=i;
				}
			}
			return(temp);
		}
		
	}
	// returns the n'th digit of the input. 
	// n starts from 0
	// integer in Java is 32 bit so there is 10 digits. we ignore sign here.
	// 0<= n <= 9
	public static int getDigitNumber (int input, int n){
		return (( input / (int)(Math.pow(10,n)) ) %10);
	}
	
	
	
	
	
	public static void starter (int[] inputArray, String fileName, boolean random) throws FileNotFoundException {
		
		
		  SortingAlgorithm1 a;
		 a= new SortingAlgorithm1();
		  
		  int[] in1;
		  in1=inputArray;
		  int[] out1, out2, out3, out4, out5, out6, out7, out8, out9;
		
		 
			long time1start=System.currentTimeMillis();
			out1=a.bubbleSort(in1);
			long time1end=System.currentTimeMillis();
			long time1=time1end-time1start;
										
			long time3start=System.currentTimeMillis();
			out2=a.bubbleSortImproved(in1);
			long time3end=System.currentTimeMillis();
			long time3=time3end-time3start;
										
			long time5start=System.currentTimeMillis();
			out3=a.insertionSort (in1);
			long time5end=System.currentTimeMillis();
			long time5=time5end-time5start;
			
			long time7start=System.currentTimeMillis();
			out4=a.shellSort_2(in1);  //h=2
			long time7end=System.currentTimeMillis();
			long time7=time7end-time7start;
			
			long time8start=System.currentTimeMillis();
			out5=a.shellSort_1(in1);  //h=2.2
			long time8end=System.currentTimeMillis();
			long time8=time8end-time8start;
			
			long time9start=System.currentTimeMillis();
			out6=a.mergeSort(in1);
			long time9end=System.currentTimeMillis();
			long time9=time9end-time9start;
			
			long time11start=System.currentTimeMillis();
			out7=a.quickSort(in1);
			long time11end=System.currentTimeMillis();
			long time11=time11end-time11start;
			
			long time12start=System.currentTimeMillis();
			out8=a.heapSort(in1);
			long time12end=System.currentTimeMillis();
			long time12=time12end-time12start;
			
			long time13start=System.currentTimeMillis();
			out9=a.radixSort(in1);
			long time13end=System.currentTimeMillis();
			long time13=time13end-time13start;
			
		
			PrintWriter c;
			c=new PrintWriter (new FileOutputStream ( fileName, true ));
			
			String tmp;
			if ( random==true){
				tmp="r";
			}
			else{
				tmp="";
			}
			
			String size;
			size=new String(inputArray.length+"");
			c.println(size+"  "+tmp);
			
		    c.println("BubbleSort:    "+time1+" ms "+Func.print(out1));
		
			c.println("CocktailSort:  "+time3+" ms "+Func.print(out2));
		
			c.println("InsertinoSort: "+time5+" ms "+Func.print(out3));
	
			c.println("ShellSort:     "+time7+" ms "+Func.print(out4));
		
			c.println("ShellSort+:    "+time8+" ms "+Func.print(out5));
		
			c.println("MergeSort:     "+time9+" ms "+Func.print(out6));
		
			c.println("QuickSort:     "+time11+" ms" +Func.print(out7));
		
			c.println("HeapSort:      "+time12+" ms "+Func.print(out8));

			c.println("RadixSort:     "+time13+" ms "+Func.print(out9));
			
			c.println("");
	
			c.close();
	  }
		
	
	public static String print (int[] inputArray){
		 StringBuffer out;
		 out=new StringBuffer();
		 int length=inputArray.length;
		 for (int i=0 ; i < length ; i++){
			 out.append(inputArray[i]+" ");
		 }
		 String a;
		 a=new String(out);
		 return(a);
		
		
	}
	
	
	
	

}























