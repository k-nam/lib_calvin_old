

package sortingTest;

import java.util.*;
public class SortingAlgorithm1 {
	
	
	
	
	// 0: shell,  1:  merge, 2: quick, 3 heap

	
	public long[] callingCount;
	public long[] comparingCount;
	public long[] movingCount;
	
	
	
	// 0: shell,  1:  merge, 2: quick, 3 heap
	public SortingAlgorithm1(){
		callingCount	= new long[ 10];
		comparingCount	= new long [10];
		movingCount	= new long [10];
		for ( int i= 0 ; i <10 ; i++){
			callingCount[i]=0;
			comparingCount[i]=0;
			movingCount[i]=0;
		}
		
	}
		
		
	
	/**
	 * This sorting algorithm is designed to be 'stable'
	 * Even in this simple algorithm, there is many ways to improve the performance:
	 *  1. do not just do the routine - check whether any of the element has been swapped
	 *     or not. If none has been swapped in an iteration, then the sort is ended.
	 *     this will improve the performance when the input array is almost sorted. 
	 *  
	 *  2. refer to the bubbleSortImproved...
	 * @param inputArray
	 * @return
	 */
	public int[] bubbleSort (int[] inputArray){
		int[] temp;
		temp=new int[inputArray.length];
		for (int i=0 ; i< inputArray.length ; i++){
			temp[i]=inputArray[i];
		}
		
		
		int length=inputArray.length;
		int bottom=0;
		int top=length-1;
		
		boolean swap=true;
		
		while (swap==true){
			swap=false; 
			for ( int i = bottom ; i < top ;  i++){
				if ( temp[i] > temp[i+1]){
					Func.swap(temp, i, i+1);
					swap=true;
				}
			}
			top--;
		}
		
		return(temp);
	}
	
	
	/**
	 * Bubble sort Improved Version (called 'Cocktail sort')
	 * it does not go one way; it moves in both direction, narrowing the scope each time
	 */
	
	public int[] bubbleSortImproved(int[] inputArray){
		int[] temp;
		temp=new int[inputArray.length];
		for (int i=0 ; i< inputArray.length ; i++){
			temp[i]=inputArray[i];
		}
		
		int length=inputArray.length;
		int bottom=0;
		int top=length-1;
		
		boolean swap=true;
		
		while (swap==true){
			swap=false; 
			for ( int i = bottom ; i < top ;  i++){
				if ( temp[i] > temp[i+1]){
					Func.swap(temp, i, i+1);
					swap=true;
				}
			}
			top--;
			for ( int i=top ; i > bottom ; i--){
				if ( temp [i]< temp[i-1]){
					Func.swap(temp, i,i-1);
					swap=true;
				}
			}
			bottom++;
		}
		
		return(temp);
		
		
	}
	
	
	/**
	 * Insertion sort, implemented to be stable.
	 */
	public  int[] insertionSort ( int[] inputArray){
	
		
		int length=inputArray.length;
		int[] temp1;
		temp1=new int[length];
		for ( int i=0 ; i < length ; i++){
			temp1[i]=0;
		}
		
		temp1[0]=inputArray[0];
		
		
		for ( int i = 1 ; i < length ; i++){
			boolean swap=false;
			for (int j= i-1 ; j >=0 ;  j-- ){
				if ( temp1[j]<=inputArray[i]){
					Func.insertIntoArray(temp1, 1, j+1,i+1, inputArray[i]);
					swap=true;
					break;
				}
			}
			if (swap==false)
			Func.insertIntoArray(temp1, 1, 0, i+1, inputArray[i]);
		}
		return (temp1);
	}
	
	/**
	 * More general version of insertionSort.
	 * This is needed as a component of Shell sort, which is an improved insertion sort
	 * h is interval.
	 */
	
	public  int[] insertionSort_1 (int[] inputArray, int h){
		int[] temp;
		temp=new int[inputArray.length];
		for (int i=0 ; i< inputArray.length ; i++){
			temp[i]=inputArray[i];
		}
		
		for ( int i = 0 ; i < h ; i++){
			// sort the subarrays [i, i+h, i+2h....] by insertion sort
			for  (int j = i ;j<temp.length  ; j+=h){
				boolean swap=false;
				for (int k = j-h ; k >=0 ; k-=h){
					if (temp[k] <=temp[j]){
						Func.insertIntoArray(temp, h, k+h, j+1, temp[j]);
						swap=true;
						break;
					}					
				}
				if(swap==false){
					Func.insertIntoArray(temp, h, i , j+1, temp[j]);
				}
			}
		}
		
		return (temp);
		
	}
	
	/**
	 * shellsort uses insertionSort_1 as base
	 * start with h=length/2  and decrease h gradually
	 * typically, reducing by half but it is said that 1/2.2 is better (reason is unexplainable)
	 * really interesting Sort algorithm!!!
	 * @param inputArray
	 * @return
	 */
	public  int[] shellSort (int[] inputArray){
		int[] temp;
		temp=new int[inputArray.length];
		for (int i=0 ; i< inputArray.length ; i++){
			temp[i]=inputArray[i];
		}
		int length=temp.length;
		// start with h=length/2 and reduce by factor 1/2.2
		for ( int h = length/2 ; h >0 ; h=( (h==2)? 1 :new Double(h/2.2).intValue() )){
			temp=this.insertionSort_1(temp, h);
		}
		return (temp);
		
	}
	
	/**
	 * shellSort revised version
	 * 
	 */
	
	
	//h=2.2
	public int[] shellSort_2 ( int[] inputArray){
		int[] temp;
		temp=new int[inputArray.length];
		for (int i=0 ; i< inputArray.length ; i++){
			temp[i]=inputArray[i];
		}
		int length=temp.length;
		for ( int h = length/2 ;  h >0 ; h=( (h==2)? 1 :new Double(h/2.2).intValue() ) ){
			//this.comparingCount[0]+=2;
			
			for (  int j = h ; j < length ; j++){
				
				
				for (int k = j ; (k >=h)&&(temp[k-h]>temp[k]) ; k = (k-h) ){
				
					
					int t=temp[k-h];
					temp[k-h]=temp[k];
					temp[k]=t;
				}
			}
			
		}
		
		return (temp);
			
	}
	
	//h=2
	public int[] shellSort_1 ( int[] inputArray){
		int[] temp;
		temp=new int[inputArray.length];
		for (int i=0 ; i< inputArray.length ; i++){
			temp[i]=inputArray[i];
		}
		int length=temp.length;
		for ( int h = length/2 ;  h >0 ; h=( (h==2)? 1 :new Double(h/2).intValue() ) ){
			//this.comparingCount[0]+=2;
			
			for (  int j = h ; j < length ; j++){
				
				
				for (int k = j ; (k >=h)&&(temp[k-h]>temp[k]) ; k = (k-h) ){
				
					
					int t=temp[k-h];
					temp[k-h]=temp[k];
					temp[k]=t;
				}
			}
			
		}
		
		return (temp);
			
	}
	
	
	
	public int[] shellSort_3 (int[]  inputArray){
		int[] temp;
		temp=new int[inputArray.length];
		for (int i=0 ; i< inputArray.length ; i++){
			temp[i]=inputArray[i];
		}
		int length=temp.length;
		
		for ( int h = length/2 ;  h >0 ; h=( (h==2)? 1 :new Double(h/2.2).intValue() ) ){
			//this.comparingCount[0]+=2;
			
			for (  int j = h ; j < length ; j++){
				
				int insertIndex=this.binarySearch(inputArray, j%h, j-h, h, temp[j]);
				int a=temp[j];
				for ( int k = j-h ; k  >=insertIndex ; k-=h){
					temp[k+h]=temp[k];
				}
				temp[insertIndex]=a;
			
			}
		}			
				
		return (temp);
	}
	
	
	
	/**
	 * This function will return the appropriate 'index' of the inputArray to insert number in order.
	 * The scopoe of the search is limited to 'index start - index end-1'
	 * ie., start, start+h, start+2h,  ...  , end-h, end
	 * it is assumed that end-start is a multiple of h. critical !!!
	 * 
	 */
	public int binarySearch (int[] inputArray, int start, int end, int h, int number) {
		if (number>=inputArray[end]) {
			return(end+h);
		}
		if (number<=inputArray[start]){
			return(start);
		}
		
		int width=(end-start)/h;
		if (width==1 ){
			return(start);
		}
		
		int mid=start+h*(width/2);
		if (inputArray[mid]==number){
			return(mid);
		}
		
		if (number> inputArray[mid]){
			return ( this.binarySearch(inputArray,	 mid, end, h, number));
		}	
		if (number< inputArray[mid]){
			return ( this.binarySearch(inputArray,	 start, mid, h, number));
		}
		
		return(0);	
	}
		
	
	
	
	
	
	public  int [] mergeSort (int[] inputArray) {
		int[] temp;
		temp=new int[inputArray.length];
		for (int i=0 ; i< inputArray.length ; i++){
			temp[i]=inputArray[i];
		}
		int length=inputArray.length;
		this.mergeSort(temp, 0, length);
		return(temp);
	}
	
	
	//sorts from inputArray[startIndex]  to inputArray[endIndex-1]
	public  void mergeSort (int[] inputArray, int startIndex, int endIndex){
		int size=endIndex-startIndex;
		
		
		if (size==1){
			
			return ;
		}
		
		int middle=(startIndex+endIndex)/2;
		this.mergeSort(inputArray, startIndex, middle);
		this.mergeSort(inputArray, middle, endIndex);
		this.merge(inputArray, startIndex, middle, endIndex);
		return ;
		
	}
	
	
	public void merge (int[] inputArray, int start, int middle, int end){
		int[] temp;
		
		
		temp=new int[end-start];
		int count_1, count_2;
		int count;   // for temp
		count_1=start; count_2=middle;
	    count=0;
	    
		while ((count_1<middle) &&( count_2<end)){
			
			
			if ( inputArray[count_1]<=inputArray[count_2]){
				
				temp[count]=inputArray[count_1];
				count_1++;
				count++;
				continue;
			}
			
			
			
			temp[count]=inputArray[count_2];
			count_2++;
			count++;
		}
		
		
		if (count_1==middle){
			
			for ( int i = count_2 ; i < end ;i ++){
			
			
				temp[count]=inputArray[count_2];
				count_2++;
				count++;
			}
		}
		else{
			for(int i= count_1 ; i < middle ; i++){
				
			
				temp[count]=inputArray[count_1];
				count_1++;
				count++;			
			}
		}
		for (int i=start ; i<end; i++){
			
	
			inputArray[i]=temp[i-start];
		}
		
	}
	
	
	
	
	
	
	
	
	
	
	public  int[] quickSort(int[] inputArray){
		int[] temp;
		temp=new int[inputArray.length];
		for (int i=0 ; i< inputArray.length ; i++){
			temp[i]=inputArray[i];
		}
	
		this.quickSort(temp, 0, temp.length);
		return(temp);
	}
	
	// sort the array form start to end-1
	public  void quickSort ( int[] inputArray, int start, int end){
		if (start>end){
			System.out.println("Fuck");
		}
		if( ( end-start)==0 || (end-start)==1){
			return;
		}
		
		// pivot selecting
		
	   int pivot=inputArray[start];
			
			
	
		
		// partitioning...
		
	int largeStarting=start+1;   // starting point of the larger than pivot elements
		
		for (int i = start+1 ; i <end ; i++){
			if ( inputArray[i]<pivot){
			
				Func.swap(inputArray, i, largeStarting);
				largeStarting++;
			}
		}
		
		Func.swap(inputArray, start, largeStarting-1);

		this.quickSort(inputArray, start, largeStarting-1);
		this.quickSort(inputArray, largeStarting, end);
		
	}
	
	
	public int[] quickSort_1 (int[] inputArray){
		int[] temp;
		temp=new int[inputArray.length];
		for (int i=0 ; i< inputArray.length ; i++){
			temp[i]=inputArray[i];
		}
	
		this.quickSort_1(temp, 0, temp.length);
		return(temp);
	}
	
	// improve quicksort
	// uses randomized method for pivot selecting,
	// and uses Hoare algorithm for pivoting...
	//	 sort the array form start to end-1
	public void quickSort_1 (int[] inputArray, int start, int end){
		this.callingCount[0]++;
		if (start>end){
			System.out.println("Fuck");
		}
		if( ( end-start)==0 || (end-start)==1){
			return;
		}
		
		// pivot selecting
		
		int p=(int)(0.0*(end-start-1));
		int pivot=inputArray[start+p];
		// move the pivot value to the leftmost position.
		Func.swap(inputArray, start, start+p);
		
		// partitioning...
		
	int largeStarting=start+1;   // starting point of the larger than pivot elements
		
		for (int i = start+1 ; i <end ; i++){
			if ( inputArray[i]<pivot){
				//if(i==largeStarting){
				//	largeStarting++;
				//	continue;
				//}
				
				Func.swap(inputArray, i, largeStarting);
				largeStarting++;
			}
		}
		
		Func.swap(inputArray, start, largeStarting-1);

		if(start+1<largeStarting){
			this.quickSort_1(inputArray, start, largeStarting-1);
		}
		if (largeStarting<end-1)
		this.quickSort_1(inputArray, largeStarting, end);
		
		
		
	}
	
	
	public int[] heapSort ( int[] inputArray) {
		int[] temp;
		temp=new int[inputArray.length];
		for (int i=0 ; i< inputArray.length ; i++){
			temp[i]=inputArray[i];
		}
		
		int[] temp1;
		temp1=new int[inputArray.length];
		
	   // first	, construct a heap from the inputArray
		int length=inputArray.length;
		for ( int i= ( length-1) /2 ; i>=0 ; i--) {
		
			this.percolateDown(temp, i, length);
		}
		
		// then pop elements...
		int heapSize=temp.length;
		for ( int i=0 ; i < temp.length ; i++){
			temp1[i]=temp[0];
	
			temp[0]=temp[heapSize-1];
			heapSize--;
			this.percolateDown(temp, 0,heapSize);
		}
		
		return(temp1);
		
	}
	
	
	// this heap is min heap
	public void percolateDown (int[] inputArray, int index, int heapSize){
		if (( index*2+1) > heapSize-1){
	
			return;
		}
		if (( index*2+2) > heapSize-1){
		
			if( inputArray[(index*2+1)] <inputArray[index] ){
				
				Func.swap(inputArray, index, index*2+1);							
			}
			return;
		}
		if  (  ( inputArray[index*2+1]<inputArray[index])  &&
				(inputArray[index*2+1]<=inputArray[index*2+2] ) ) {
		
			
	
			Func.swap(inputArray, index, index*2+1);
			
	
			this.percolateDown(inputArray, index*2+1, heapSize);	
			return;
		}
		if  (  ( inputArray[index*2+2]<inputArray[index])  &&
				(inputArray[index*2+2]<=inputArray[index*2+1] ) ) {
		
			
	
			Func.swap(inputArray, index, index*2+2);
			
		
			this.percolateDown(inputArray, index*2+2, heapSize);	
		}
		
	}
	
	
	public int[] radixSort (int[] inputArray){
		int[] temp;
		temp=new int[inputArray.length];
		for (int i=0 ; i< inputArray.length ; i++){
			temp[i]=inputArray[i];
		}
		int length=temp.length;
		
		// i is the digit
		for (int i = 0 ; i < 10 ; i++){
			Queue[] a;
			a=new Queue[10];  // a[0] - a[9] contains corresponding digit.
			for ( int b=0 ;b<10 ;b++){
				a[b]=new Queue();
			}
			for (int j = 0 ; j< length ; j++){
				int c=Math.abs( ( temp[j]/(int)(Math.pow(10, i)) )%10 );
				a[c].enQueue(temp[j]);
			}
			for (int j= 1 ; j<10 ; j++){
				a[0].enQueue(a[j]);
			}
			for (int j = 0 ; j< length ; j++){
				temp[j]=a[0].deQueue();
				
			}
		}
		
		Queue[] a;
		a= new Queue[2];
		a[0]=new Queue();
		a[1]=new Queue();
		for (int j = 0 ; j< length ; j++){
			if(temp[j]>=0){
				a[1].enQueue(temp[j]);
			}			
		}
		
		for (int j = length-1 ; j>=0 ; j--){
			if (temp[j]<0){
				a[0].enQueue(temp[j]);
			}
			
		}
		
		a[0].enQueue(a[1]);
		for (int j = 0 ; j< length ; j++){
			temp[j]=a[0].deQueue();
			
		}
		
		return(temp);		
		
	}
	
	
	// 8 bit * 4 times...\
	// h : the width of each process (ex: 4 bits)
	//
	public int[] radixSort_1 (int [] inputArray){
		int[] temp;
		temp=new int[inputArray.length];
		for (int i=0 ; i< inputArray.length ; i++){
			temp[i]=inputArray[i];
		}
		
		int h=8;
		int k=(int)Math.pow(2, h);
		Queue[] a;
		a=new Queue[k];
		
		
		int mask=k;
		mask=mask-1;
		
		// i is the starting bit for the process
		// i, i+1, i+2, i+3, ... , i+7
		for (int i=0 ; i<32 ; i+=h){
			for(int j=0 ; j<k ; j++){
				a[j]=new Queue();
			}
			
			for (int j = 0 ; j < temp.length; j++){
				int tmp;
				tmp=temp[j];
				tmp=tmp>>i;
				tmp=(tmp&mask);
				a[tmp].enQueue(temp[j]);
				
			}
			for (int j= 1 ; j<k ; j++){
				a[0].enQueue(a[j]);
			}
			for (int j = 0 ; j< temp.length ; j++){
				temp[j]=a[0].deQueue();
			}
		}		
		
		
		return(temp);
	}
	
	
	// I am making yet again attempt to build a faster algorithm than the QuickSort
	// The point is not to use external operation space. Operate in the limited area of memory
	// This is basically the same to the radixSort_1, but using the Counting Sort algorithm, not bucket sorting
	public int[] radixSort_2 (int[] inputArray){
		int[] temp;
		temp=new int[inputArray.length];
		for (int i=0 ; i< inputArray.length ; i++){
			temp[i]=inputArray[i];
		}
		
		
		int h=8;  // num of bits operated in paralell in one process
		int k=(int)Math.pow(2, h);
	
		
		int mask=k;
		mask=mask-1;
		
		// i is the starting bit for the process
		// i, i+1, i+2, i+3, ... , i+7
		for (int i=0 ; i<32 ; i+=h){
			
			for (int j = 0 ; j < temp.length; j++){
				int tmp;
				tmp=temp[j];
				tmp=tmp>>i;
				tmp=(tmp&mask);
			
			}
		
		}		
		
		
		
		
		return( temp);
	}
	
	
	
	public int[] bucketSort (int[] inputArray){
		int[] temp;
		temp=new int[inputArray.length];
		for (int i=0 ; i< inputArray.length ; i++){
			temp[i]=inputArray[i];
		}
		
		Queue[] a;
		a=new Queue[500000];
		
		for(int j=0 ; j<500000 ; j++){
			a[j]=new Queue();
		}
		/*
		for (int i=0 ; i < temp.length ; i++){
			int tmp=inputArray[i]/10000+250000;
			a[tmp].enQueue(temp[i]);
		}
		for ( int i=1 ; i < 500000; i++){
			a[0].enQueue(a[i]);
		}
		for (int j = 0 ; j< temp.length ; j++){
			temp[j]=a[0].deQueue();
		}
		*/
		
		
		for(int i = 0 ; i<temp.length ; i++){
			int tmp=temp[i]/100000;
		}
		
		return(temp);
	}
}



























