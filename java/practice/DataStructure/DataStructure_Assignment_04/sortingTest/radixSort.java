package sortingTest;

public class radixSort{
	static void radixSort(int[] inputArray) {
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
