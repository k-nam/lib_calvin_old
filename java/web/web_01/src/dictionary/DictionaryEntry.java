package dictionary;

public class DictionaryEntry implements Comparable<DictionaryEntry> {
	public String word;
	public int frequency; // n'th most frequent word

	public DictionaryEntry(String word, int frequency) {
		this.word = word;
		this.frequency = frequency;
	}

	@Override
	public int compareTo(DictionaryEntry entry) {
		return word.compareTo(entry.word);
	}

}
