function Card(id, type) {
	this.id = id;
	this.type = type;
}

Card.prototype.toString = function () {
	return this.month().toString() + this.type;
}

Card.prototype.month = function () {
	return Math.floor(this.id / 4) + 1;
}

function getCard(id) {
	return getAllCards()[id];
}

function sortCards(cards) {
	var result = {};
	//alert(cards.length);
	for (var i = 0; i < cards.length; i++) {
		var thisMonth = getCard(cards[i]).month().toString();
		//alert(thisMonth);
		if (result[thisMonth]) {
			//alert('u');
			result[thisMonth].push(cards[i]);
			//alert(result[thisMonth].length);
		} else {
			//alert('v');
			result[thisMonth] = [cards[i]];
		}
		

	}
	//alert('size: ' + Object.keys(result).length);
	return result;
}

function splitCardsForDisplay(cards) {
	function getGeneralTypeFromType(cardType) {
		if (cardType == '광' || cardType == '비광') {
			return '광';
		} else if (cardType == '고도리' || cardType == '열끗') {
			return '열끗';
		} else if (cardType == '홍단' || cardType == '청단' ||
								cardType == '초단' || cardType == '다섯끗') {
			return '다섯끗';
		} else {
			return '피';
		}
	}
	//alert('예');
	var result = { 광:[], 열끗:[], 다섯끗:[], 피:[] };
	for (var i = 0; i < cards.length; i++) {
		var thisType = getGeneralTypeFromType(getCard(cards[i]).type);
		result[thisType].push(i);
	}
	//alert(result['광']);
	return result;
}

function splitShellsForDisplay(shells) {
	// shells are arranged in 3 lines
	// first and second line should contain at most 5 shells (double shell counts as 2)
	var result = [[], [], []];
	var doubles = [];
	var normals = [];
	for (var i = 0; i < shells.length; i++) {
		//alert(getCard(shells[i]).type);
		if (getCard(shells[i]).type == '피') {
			normals.push(shells[i]);
		} else {
			doubles.push(shells[i]);
		}
	}
	var totalShellCount = 0;
	for (i = 0; i < doubles.length; i++) {
		if (totalShellCount < 5) {
			result[0].push(doubles[i]);
		} else if (totalShellCount < 10) {
			result[1].push(doubles[i]);
		} else {
			result[2].push(doubles[i]);
		}
		totalShellCount += 2;
	}
	
	for (i = 0; i < normals.length; i++) {
		if (totalShellCount < 5) {
			result[0].push(normals[i]);
		} else if (totalShellCount < 10) {
			result[1].push(normals[i]);
		} else {
			result[2].push(normals[i]);
		}
		totalShellCount += 1;
	}
	return result;
}

function getAllCards() {
	return [new Card(0, '광'), // 1
					new Card(1, '홍단'),
					new Card(2, '피'),
					new Card(3, '피'),

					new Card(4, '고도리'), // 2
					new Card(5, '홍단'),
					new Card(6, '피'),
					new Card(7, '피'),

					new Card(8, '광'), // 3
					new Card(9, '홍단'),
					new Card(10, '피'),
					new Card(11, '피'),

					new Card(12, '고도리'), // 4
					new Card(13, '초단'),
					new Card(14, '피'),
					new Card(15, '피'),

					new Card(16, '열끗'), // 5
					new Card(17, '초단'),
					new Card(18, '피'),
					new Card(19, '피'),

					new Card(20, '열끗'), //6
					new Card(21, '청단'),
					new Card(22, '피'),
					new Card(23, '피'),

					new Card(24, '열끗'), //7
					new Card(25, '초단'),
					new Card(26, '피'),
					new Card(27, '피'),

					new Card(28, '광'), // 8
					new Card(29, '고도리'),
					new Card(30, '피'),
					new Card(31, '피'),

					new Card(32, '열끗'), // 9
					new Card(33, '청단'),
					new Card(34, '피'),
					new Card(35, '피'),

					new Card(36, '열끗'), // 10
					new Card(37, '청단'),
					new Card(38, '피'),
					new Card(39, '피'),

					new Card(40, '광'), // 11 똥
					new Card(41, '피'),
					new Card(42, '피'),
					new Card(43, '쌍피'),

					new Card(44, '비광'), // 12 비
					new Card(45, '열끗'),
					new Card(46, '다섯끗'),
					new Card(47, '쌍피')];


}

function printAllCards() {
	for (i = 0 ; i < 48; i++) {
		document.write(getAllCards()[i]);
		document.write('<br>');
	}
}

function drawGameRoom(gameStatus) {
 
}