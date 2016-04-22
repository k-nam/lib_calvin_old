var interval = 15;

var cardWidth = 58;
var cardHeight = 87;

var lightStartPoint = [10, 102];
var lightEndPoint = [60, 102];

var tenStartPoint = [130, 10];
var tenEndPoint = [210, 10];

var fiveStartPoint = [130, 102];
var fiveEndPoint = [210, 102];

var shellStartPoint1 = [280, 102];
var shellEndPoint1 = [380, 102];

var shellStartPoint2 = [280, 60];
var shellEndPoint2 = [380, 60];

var shellStartPoint3 = [280, 10];
var shellEndPoint3 = [380, 10];



function getCoordinates(startPoint, endPoint, interval, numCards) {
	
	var result = [];
	if (startPoint[0] + interval * (numCards - 1) < endPoint[0]) {		
	} else {
		interval = (endPoint[0] - startPoint[0]) / (numCards - 1);
	}
	for (var i = 0; i < numCards; i++) {
		result.push([startPoint[0] + interval * i, startPoint[1]]);
	}
	
	return result;
}

function drawAtCoordinates(cards, coordinates, screen) {
	for (var i = 0; i < cards.length; i++) {
		drawAtCoordinate(cards[i], coordinates[i], screen);
	}
	function drawAtCoordinate(card, coordinate) {
		
		var img = document.createElement('img');
		screen.appendChild(img);
		img.src = '/image/' + card + '.png';
		//alert(coordinate);
		img.style.position = 'absolute';
		img.style.width = cardWidth + 'px';
		img.style.height = cardHeight + 'px';
		img.style.left = coordinate[0] + 'px';
		img.style.top = coordinate[1] + 'px';
		//img.style['z-index'] = 1;
	}
}

function draw(cards, screen) {
	
	var splitedCards = splitCardsForDisplay(cards);
	
	var lights = splitedCards['광'];

	var tens = splitedCards['열끗'];
	var fives = splitedCards['다섯끗'];
	var shells = splitedCards['피'];

	var lightCoordinates = getCoordinates(lightStartPoint, lightEndPoint, interval, lights.length);
	var tenCoordinates = getCoordinates(tenStartPoint, tenEndPoint, interval, tens.length);
	var fiveCoordinates = getCoordinates(fiveStartPoint, fiveEndPoint, interval, fives.length);
	var shellCoordinates = getCoordinates(shellStartPoint1, shellEndPoint1, interval, shells.length);
	drawAtCoordinates(lights, lightCoordinates, screen);
	drawAtCoordinates(tens, tenCoordinates, screen);
	drawAtCoordinates(fives, fiveCoordinates, screen);
	drawAtCoordinates(shells, shellCoordinates, screen);
}

