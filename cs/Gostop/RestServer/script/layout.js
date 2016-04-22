var interval = 32;

var cardWidth = 54;
var cardHeight = 81;

var lightStartPoint = [5, 100];
var lightEndPoint = [50, 100];

var tenStartPoint = [115, 5];
var tenEndPoint = [260, 5];

var fiveStartPoint = [115, 100];
var fiveEndPoint = [260, 102];

var shellStartPoint1 = [330, 100];
var shellEndPoint1 = [440, 102];

var shellStartPoint2 = [330, 60];
var shellEndPoint2 = [440, 60];

var shellStartPoint3 = [330, 5];
var shellEndPoint3 = [440, 5];


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
}

function drawAtCoordinate(card, coordinate, screen) {
	var img = document.createElement('img');
	screen.appendChild(img);
	img.src = '/image/' + card + '.png';
	//alert(coordinate); 
	img.id = 'card-' + card;
	img.style.position = 'absolute';
	img.style.width = cardWidth + 'px';
	img.style.height = cardHeight + 'px';
	img.style.left = coordinate[0] + 'px';
	img.style.top = coordinate[1] + 'px';
	
	img.onmouseenter = function () {
		img.style.left = (coordinate[0]) + 'px';
	}
	img.onmouseleave = function () {
		img.style.left = coordinate[0] + 'px';
	}
	//img.style['z-index'] = '-1';
}

function draw(cards, screen) {
	
	var splitedCards = splitCardsForDisplay(cards);
	
	var lights = splitedCards['광'];
	var tens = splitedCards['열끗'];
	var fives = splitedCards['다섯끗'];
	var shells = splitedCards['피'];
	var shellByLines = splitShellsForDisplay(shells);
	//alert(shellByLines[0]);

	var lightCoordinates = getCoordinates(lightStartPoint, lightEndPoint, interval, lights.length);
	var tenCoordinates = getCoordinates(tenStartPoint, tenEndPoint, interval, tens.length);
	var fiveCoordinates = getCoordinates(fiveStartPoint, fiveEndPoint, interval, fives.length);

	drawAtCoordinates(lights, lightCoordinates, screen);
	drawAtCoordinates(tens, tenCoordinates, screen);
	drawAtCoordinates(fives, fiveCoordinates, screen);

	var shellCoordinates1 =
		getCoordinates(shellStartPoint1, shellEndPoint1, interval, shellByLines[0].length);
	var shellCoordinates2 =
		getCoordinates(shellStartPoint2, shellEndPoint2, interval, shellByLines[1].length);
	var shellCoordinates3 =
		getCoordinates(shellStartPoint3, shellEndPoint3, interval, shellByLines[2].length);

	drawAtCoordinates(shellByLines[0], shellCoordinates1, screen);
	drawAtCoordinates(shellByLines[1], shellCoordinates2, screen);
	drawAtCoordinates(shellByLines[2], shellCoordinates3, screen);
}

