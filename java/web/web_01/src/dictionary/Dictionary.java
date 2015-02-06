package dictionary;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class Dictionary {
	private List<DictionaryEntry> entries;

	public Dictionary(List<DictionaryEntry> entries) {
		entries.sort(null); // lexicographically sort
		this.entries = entries;
	}
	
	// return N most frequent words with given prefix
	public List<String> getWordsWithPrefix(String prefix, int num) {
		List<DictionaryEntry> prefixMatches = new ArrayList<DictionaryEntry>();
		for (DictionaryEntry entry : this.entries) {
			if (entry.word.startsWith(prefix)){
				prefixMatches.add(entry);
			}
		}
		// This time, sort by frequency
		prefixMatches.sort(new Comparator<DictionaryEntry> () {
			@Override
			public int compare(DictionaryEntry entry1, DictionaryEntry entry2) {
				return entry1.frequency - entry2.frequency;
			}
		});
		List<DictionaryEntry> mostFrequent = prefixMatches.subList(0, num);
		// Sort again to make result readable
		mostFrequent.sort(null);
		List<String> result = new ArrayList<String>();
		for (DictionaryEntry entry : mostFrequent) {
			result.add(entry.word);
		}
		return result;
	}
}
