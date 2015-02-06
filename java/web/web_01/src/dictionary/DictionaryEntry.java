package dictionary;

public class DictionaryEntry {
	private String word;
	private int frequency; // n'th most frequent word
	public DictionaryEntry(String word, int frequency) {
		word = word;
		frequency = frequency;
	}
}
