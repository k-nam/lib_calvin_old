#include <iostream>

namespace calvin
{

class Shape {
public:
	void draw2() const { std::cout << "Shape::draw() const\n"; }
	void draw() { std::cout << "Shape::draw()\n"; }
	void draw_arg(int a) { std::cout << "Shape::draw(" << a << ")\n"; }
	void draw2_arg(int a) const { std::cout << "Shape::draw(" << a << ") const\n"; }
};


}
