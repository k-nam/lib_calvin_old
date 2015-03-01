<?php
require 'mylib.php';

class Dictionary {
	public static function getInstance() {
		static $instance = null;
		if ($instance === null) {
			$instance = new static();
		}
		return $instance;
	}
	public function getEntries() {
		return $this->entries;
	}
	private function loadDictionary($fileName) {
		$infinity = 10000000; // words without frequency data 
		$handle = fopen($fileName, "r");
		while ($line = fgets($handle, $this->maxLineLength)) {
			$rank = $infinity;
			$word = "";
			$rankAndWord = explode("\t", $line);
			if (count($rankAndWord) === 1) {
				$word = removeNewline($rankAndWord[0]);
			} else if (count($rankAndWord) === 2) {
				$rank = intval($rankAndWord[0]);
				$word = removeNewline($rankAndWord[1]);
			} else { // error
				continue;
			}
			$this->entries[$rank] = $word;
			//debugLog("".$rank."=>".$word);
		}
		$handle.fclose($handle);
	}
	protected function __construct() { 
		$this->loadDictionary($this->fileName);
	}
	private $entries = array();
	private $fileName = "data/5000_ansi.txt";
	private $maxLineLength = 100;
	private function __clone() { }
	private function __wakeup() { }
}

function getSuggestion($input, $numSuggestion) {
	$dictionary = Dictionary::getInstance();
	$matches = array();
	foreach ($dictionary->getEntries() as $rank => $word) {
		if (strpos($word, $input) === 0) {
			$matches[$rank] = $word;
		}
	}
	ksort($matches);
	$matches = array_slice($matches, 0, min($numSuggestion, sizeof($matches)));
	$suggestions = array();
	foreach ($matches as $rank => $word) {
		//debugLog("Adding word: ".$word);
		if ($word === "" || $word === " ") {
			return "error fuck";
		}
		$suggestions[] = $word;
	}
	//debugLog("Result: ".$suggestions);
	return implode(" ", $suggestions);
}

if ($_SERVER['REQUEST_METHOD'] === 'GET') {
	$input = test_input($_GET["input"]);
	$numSuggestion = intval(test_input($_GET["num"]));
	//$response = "absolute"." "."beautiful"." "."commander"."cc"."\n"."ddd";
	$response = getSuggestion($input, $numSuggestion);
	debugLog("Response: <".$response.">");
	echo $response;
} else {
	echo "error";
}
?>
