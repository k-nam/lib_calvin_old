var Card = {};

Card.GostopCard = function(id, type) {
	this.id = id;
	this.type = type;
}

Card.GostopCard.prototype.toString = function() {
	return this.month().toString() + this.type;
}

Card.GostopCard.prototype.month = function() {
	return Math.floor(this.id / 4) + 1;
}

Card.getCard = function(id) {
	return Card.getAllCards()[id];
}

Card.sortCardsForFloorDisplay = function(cards) {
	var result = {};
	for (var i = 0; i < cards.length; i++) {
		var thisMonth = Card.getCard(cards[i]).month();
		if (result[thisMonth]) {
			result[thisMonth].push(cards[i]);
		} else {
			result[thisMonth] = [cards[i]];
		}
	}
	for (var month in result) {
		result[month].sort(function(a, b) { return b - a; });
	}
	return result;
}

Card.splitCardsForDisplay = function(cards) {
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
		var a = Card.getCard(cards[i]).type;
		var thisType = getGeneralTypeFromType(a);
		result[thisType].push(i);
		
	}
	//alert(result['광']);
	return result;
}

Card.splitShellsForDisplay = function(shells) {
	// shells are arranged in 3 lines
	// first and second line should contain at most 5 shells (double shell counts as 2)
	var result = [[], [], []];
	var doubles = [];
	var normals = [];
	for (var i = 0; i < shells.length; i++) {
		//alert(Card.getCard(shells[i]).type);
		if (Card.getCard(shells[i]).type == '피') {
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

Card.getAllCards = function() {
	return [new Card.GostopCard(0, '광'), // 1
					new Card.GostopCard(1, '홍단'),
					new Card.GostopCard(2, '피'),
					new Card.GostopCard(3, '피'),

					new Card.GostopCard(4, '고도리'), // 2
					new Card.GostopCard(5, '홍단'),
					new Card.GostopCard(6, '피'),
					new Card.GostopCard(7, '피'),

					new Card.GostopCard(8, '광'), // 3
					new Card.GostopCard(9, '홍단'),
					new Card.GostopCard(10, '피'),
					new Card.GostopCard(11, '피'),

					new Card.GostopCard(12, '고도리'), // 4
					new Card.GostopCard(13, '초단'),
					new Card.GostopCard(14, '피'),
					new Card.GostopCard(15, '피'),

					new Card.GostopCard(16, '열끗'), // 5
					new Card.GostopCard(17, '초단'),
					new Card.GostopCard(18, '피'),
					new Card.GostopCard(19, '피'),

					new Card.GostopCard(20, '열끗'), //6
					new Card.GostopCard(21, '청단'),
					new Card.GostopCard(22, '피'),
					new Card.GostopCard(23, '피'),

					new Card.GostopCard(24, '열끗'), //7
					new Card.GostopCard(25, '초단'),
					new Card.GostopCard(26, '피'),
					new Card.GostopCard(27, '피'),

					new Card.GostopCard(28, '광'), // 8
					new Card.GostopCard(29, '고도리'),
					new Card.GostopCard(30, '피'),
					new Card.GostopCard(31, '피'),

					new Card.GostopCard(32, '열끗'), // 9
					new Card.GostopCard(33, '청단'),
					new Card.GostopCard(34, '피'),
					new Card.GostopCard(35, '피'),

					new Card.GostopCard(36, '열끗'), // 10
					new Card.GostopCard(37, '청단'),
					new Card.GostopCard(38, '피'),
					new Card.GostopCard(39, '피'),

					new Card.GostopCard(40, '광'), // 11 똥
					new Card.GostopCard(41, '피'),
					new Card.GostopCard(42, '피'),
					new Card.GostopCard(43, '쌍피'),

					new Card.GostopCard(44, '비광'), // 12 비
					new Card.GostopCard(45, '열끗'),
					new Card.GostopCard(46, '다섯끗'),
					new Card.GostopCard(47, '쌍피')];


}

Card.printAllCards = function() {
	for (i = 0 ; i < 48; i++) {
		document.write(Card.getAllCards()[i]);
		document.write('<br>');
	}
}
