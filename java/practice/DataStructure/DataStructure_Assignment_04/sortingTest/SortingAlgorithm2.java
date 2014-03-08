
/**
 * 
 * @author Calvin Caulfield
 *
 * April 30th, 2007 starting
 * 
 *  Implementing Bubble sort,  Insertion sort, Quick sort, Merge sort,
 *  					Radix sort, Shell sort, Heap sort
 */

package sortingTest;

public class SortingAlgorithm2 {
	
	public long[] callingCount;
	public long[] comparingCount;
	public long[] movingCount;
	
	
	
	// 0: shell,  1:  merge, 2: quick, 3 heap
	public SortingAlgorithm2(){
		callingCount	= new long[ 10];
		comparingCount	= new long [10];
		movingCount	= new long [10];
		for ( int i= 0 ; i <10 ; i++){
			callingCount[i]=0;
			comparingCount[i]=0;
			movingCount[i]=0;
		}
	}
	
	
	public int[] quickSort(int[] inputArray) {
		int[] temp;
		temp=new int[inputArray.length];
		for (int i=0 ; i< inputArray.length ; i++){
			temp[i]=inputArray[i];
		}
		
		this.quickSort(temp, 0, temp.length-1);
		return(temp);
	}

	public  void quickSort(int[] inputArray, int low, int high) {
		this.callingCount[0]++;
		int i = low;
		int j = high;
		int a = (int) (1* (high - low)) + low;
		int pivot=inputArray[a];
		
		while (i <= j) {
			while (inputArray[i] < pivot && i < high) {
				i++;
			}
			while (pivot < inputArray[j] && j > low) {
				j--;
			}
			if (i <= j) {
				int temp = inputArray[i];
				inputArray[i] = inputArray[j];
				inputArray[j] = temp;
				i++;
				j--;
			}
		}

		if (low < j) {
			this.quickSort(inputArray, low, j);
		}
		if (i < high) {
			this.quickSort(inputArray, i, high);
		}

	}
	
	
	
	
	
	
	
	
	public void radixSort(int[] inputArray) {
		int[] source = inputArray;
		int min = inputArray[0];

		for (int i = 0; i < inputArray.length; i++) {
			if (min > inputArray[i])
				min = inputArray[i];
		}

		for (int i = 0; i < inputArray.length; i++) {
			inputArray[i] = inputArray[i] - min;
		}

		int[] destination = new int[inputArray.length];
		int[] indexes = new int[257];

		for (int j = 0; j < 10; j++) {
			for (int i = 0; i < indexes.length; i++)
				indexes[i] = 0;
			for (int i = 0; i < inputArray.length; i++) {
				int index = (source[i] >> j * 8) & 0xFF;
				indexes[index + 1]++;
			}
			for (int i = 1; i < indexes.length; i++)
				indexes[i] += indexes[i - 1];
			for (int i = 0; i < inputArray.length; i++) {
				int index = (source[i] >> j * 8) & 0xFF;
				destination[indexes[index]++] = source[i];
			}
			int[] temp = source;
			source = destination;
			destination = temp;
		}

		for (int i = 0; i < inputArray.length; i++) {
			inputArray[i] = inputArray[i] + min;
		}
	}
	
	
	
		
		
}
























