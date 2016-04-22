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