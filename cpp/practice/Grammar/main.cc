#include "Shape.h"
#include "predicate.h"
#include "Constructor.h"

void predicateTest() {
	using namespace calvin;
	Shape *shapes = new Shape[5];
	Shape const *shapes2 = shapes;
	foreach(shapes, shapes + 5, memfun(&Shape::draw)); 
	foreach(shapes, shapes + 5, memfun(&Shape::draw2)); // const function
	foreach(shapes2, shapes2 + 5, memfun(&Shape::draw2)); // const object + const function

	foreach(shapes, shapes + 5, bind2nd(memfun(&Shape::draw_arg), 5));
	foreach(shapes, shapes + 5, bind2nd(memfun(&Shape::draw2_arg), 6)); // const function
	// const object + const function
	foreach(shapes2, shapes2 + 5, bind2nd(memfun(&Shape::draw2_arg), 6)); 
}

void ctorTest() {
	Test a(1, 2);
	Test b { 1, 2 };
	Test c = { 1, 2 };
	Test d(a);
	Test e = a;
	Test f { b };
	Test g = { c };
}

int main()
{
	ctorTest();
	return 0;
}
