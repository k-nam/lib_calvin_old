package euler


object Test {
    def main(args:Array[String]): Unit = {
	    val list = List(1, 2, 3)
	    list.foreach(println _)
	    val sum1 = (x: Int, y: Int) => x + y
	    val sum2 = sum1(_: Int, 5)
	    list.foreach(x => println((sum1(_: Int, 5))(x)))
	    val list2 = list.filter(_ > 1)
	    println (list2)
	    val a = new Rational(2, 3)
	    val b = new Rational(2, 3)
	    val c = a + b
	    a.print()
	    b.print()
	    c.print()
    }
}
