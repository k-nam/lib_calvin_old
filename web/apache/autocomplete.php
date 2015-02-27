
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
		$handle = fopen($fileName, "r");
		while ($line = fgets($handle, $this->maxLineLength)) {
			$rankAndWord = explode("\t", $line);
			$rank = intval($rankAndWord[0]);
			$word = $rankAndWord[1];
			$this->entries[$rank] = $word;
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
		$suggestions[] = $word;
	}
	return implode(" ", $suggestions);
}

if ($_SERVER['REQUEST_METHOD'] === 'GET') {
	$input = test_input($_GET["input"]);
	$numSuggestion = intval(test_input($_GET["num"]));
	//$response = "absolute"." "."beautiful"." "."commander";
	$response = getSuggestion($input, $numSuggestion);
	echo $response;
} else {
	echo "error";
}
?>
