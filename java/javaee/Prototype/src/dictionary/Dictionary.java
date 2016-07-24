package dictionary;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.lang.Math;

public class Dictionary {
	private List<DictionaryEntry> entries;
	private static Dictionary theDictionary = null;

	private Dictionary() {

	}

	public static Dictionary getDictionary() {
		if (theDictionary == null) {
			theDictionary = new Dictionary();
		}
		return theDictionary;
	}

	public static List<String> getAutocompleteSuggestions(String prefix, int num) {
		List<String> result = getAutocompleteSuggestionsFromDb(prefix, num);
		for (String a : result) {
			System.out.println(a);
		}
		return result;
		// return getAutocompleteSuggestionsFromFile(prefix, num);
	}

	// return N most frequent words with given prefix
	public static List<String> getAutocompleteSuggestionsFromFile(String prefix,
			int num) {
		List<DictionaryEntry> prefixMatches = new ArrayList<DictionaryEntry>();
		for (DictionaryEntry entry : getDictionary().entries) {
			if (entry.word.startsWith(prefix)) {
				prefixMatches.add(entry);
			}
		}
		// This time, sort by frequency
		prefixMatches.sort(new Comparator<DictionaryEntry>() {
			@Override
			public int compare(DictionaryEntry entry1, DictionaryEntry entry2) {
				return entry1.frequency - entry2.frequency;
			}
		});
		List<DictionaryEntry> mostFrequent = prefixMatches.subList(0,
				Math.min(num, prefixMatches.size()));

		List<String> result = new ArrayList<String>();
		for (DictionaryEntry entry : mostFrequent) {
			result.add(entry.word);
		}
		return result;
	}

	public static List<String> getAutocompleteSuggestionsFromDb(String prefix,
			int num) {
		return db.DictionaryDao.getAutocompleteSuggestions(prefix, num);
	}

}
