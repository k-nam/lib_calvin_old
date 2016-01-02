<?php
require $_SERVER["DOCUMENT_ROOT"]."/lib/mylib.php";
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
	private function loadDictionary() {
		$handle = fopen($_SERVER["DOCUMENT_ROOT"].$this->fileName, "r");
		while ($line = fgets($handle, $this->maxLineLength)) {
			$word = "";
			$wordAndRank = explode("\t", $line);
			if (count($wordAndRank) === 2) {
				$word = $wordAndRank[0];
				$rank = intval($wordAndRank[1]);
			} else { // error
				echo "FUCK<br>";
			}
			$this->entries[$word] = $rank;
		}
		fclose($handle);
	}
	protected function __construct() { 
		$this->loadDictionary();
	}
	private $entries = array();
	private $fileName = "/data/5k_ansi.txt";	
	private $maxLineLength = 100;
	private function __clone() { }
	private function __wakeup() { }
}

function getSuggestion($input, $numSuggestion) {
	//return getSuggestionFromDb($input, $numSuggestion);
	return getSuggestionFromFile($input, $numSuggestion);
}

function getSuggestionFromFile($input, $num) {
	$dictionary = Dictionary::getInstance();
	$matches = array();
	foreach ($dictionary->getEntries() as $word => $rank) {
		if (strpos($word, $input) === 0) {
			$matches[$word] = $rank;
		}
	}
	asort($matches); // sort by rank
	$matches = array_slice($matches, 0, min($num, sizeof($matches)));
	$suggestions = array();
	foreach ($matches as $word => $rank) {
		$suggestions[] = $word;
	}
	return implode(" ", $suggestions);
}

function getSuggestionFromDb($input, $num) {
	//debugLog("start");
	$conn = getDbConnection();
	$dropQuery = "DROP TABLE IF EXISTS web.temp;";
	$createQuery = "CREATE TEMPORARY TABLE web.temp AS (
				SELECT word, rank FROM web.eng_100k
				WHERE word LIKE '".$input."%' ORDER BY  (CASE WHEN rank IS NULL THEN 1000000 END), rank ASC LIMIT ".$num.");";
	$selectQuery = "SELECT word, rank FROM web.temp ORDER BY word;";
			
	// this attempt of using only one sql query is difficult. so just omit last sorting phase.
	$sqlQuery2 = "SELECT word, rank FROM web.eng_100k
				WHERE word LIKE '".$input."%' ORDER BY (CASE WHEN rank IS NULL THEN 100000 END), rank ASC LIMIT ".$num.";";
	
	// for mssql
	$sqlQuery3 = "SELECT TOP ".$num." word FROM Web.Wordlist.Eng_100K
				WHERE word LIKE '".$input."%' ORDER BY (CASE WHEN rank IS NULL THEN 100000 END), rank ASC;";

	//debugLog("query:<".$sqlQuery.">");
	//$conn->query($dropQuery);
	//$conn->query($createQuery);
	//$result = $conn->query($selectQuery);
	$suggestions = array();

/*
	while($row = $result->fetch_assoc()) {
		$suggestions[] = $row["word"];
	}
*/
	$stmt = $conn->prepare($sqlQuery3);
	$stmt->execute();
	foreach($stmt->fetchAll() as &$row) {
		$suggestions[] = $row["word"];
	}
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



