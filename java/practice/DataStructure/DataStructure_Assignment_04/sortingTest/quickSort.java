package sortingTest;

public class quickSort {
	
	public static void quickSort(int[] inputArray) {
		sort(inputArray, 1, inputArray.length);
	}

	public static void sort(int[] inputArray, int low, int high) {
		int i = low;
		int j = high;
		int pivot = (int) (Math.random() * (high - low)) + low;
		
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
			sort(inputArray, low, j);
		}
		if (i < high) {
			sort(inputArray, i, high);
		}

	}
}