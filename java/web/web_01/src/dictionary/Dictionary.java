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
	private String fileName1 = "5000_ansi.txt";
	private String fileName2 = "100k_ansi.txt";
	private static Dictionary theDictionary = null;

	private Dictionary() {
		loadDictionary();
	}

	private void loadDictionary() {
		// load from file
		BufferedReader reader1 = null;
		BufferedReader reader2 = null;
		final int noFrequency = 10000000;
		reader1 = new BufferedReader(new InputStreamReader(this.getClass()
				.getClassLoader().getResourceAsStream(fileName1)));
		reader2 = new BufferedReader(new InputStreamReader(this.getClass()
				.getClassLoader().getResourceAsStream(fileName2)));

		String line = null;
		String word = null;
		Integer frequency = null;
		Map<String, Integer> entryMap = new HashMap<String, Integer>();
		while (true) {
			try {
				line = reader1.readLine();
			} catch (IOException e) {
				e.printStackTrace();
			}
			if (line == null) {
				break;
			} else {
				frequency = Integer.valueOf(line.split("\t")[0]);
				word = line.split("\t")[1];
				// System.out.println("freq: " + frequency + " word:" + word);
				if (entryMap.containsKey(word) == false) {
					entryMap.put(word, frequency);
				}
			}
		}
		
		while (true) {
			try {
				line = reader2.readLine();
			} catch (IOException e) {
				e.printStackTrace();
			}
			if (line == null) {
				break;
			} else {
				word = line; 
				if (entryMap.containsKey(word) == false) {
					// big dictionary's words has lower frequency.
					entryMap.put(word, noFrequency);
				}
			}
		}
		entries = new ArrayList<DictionaryEntry>();
		for (Map.Entry<String, Integer> entry : entryMap.entrySet()) {
			entries.add(new DictionaryEntry(entry.getKey(), entry.getValue()));
		}
		entries.sort(null); // lexicographically
	}

	public static Dictionary getDictionary() {
		if (theDictionary == null) {
			theDictionary = new Dictionary();
		}
		return theDictionary;
	}

	// return N most frequent words with given prefix
	public static List<String> getWordsWithPrefix(String prefix, int num) {
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
}
