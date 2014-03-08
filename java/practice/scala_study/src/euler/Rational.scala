package euler


class Rational(n: Int, d: Int) {
  require(d != 0)
  
  private val g = gcd(n.abs, d.abs)
  val numerator = n / g
  val denominator = d / g
  
  def this(n: Int) = this(n, 1)
  
  def + (that: Rational): Rational =
    new Rational(numerator * that.denominator + that.numerator * denominator,
                 denominator * that.denominator)
  def + (i: Int): Rational = 
    new Rational(numerator + i * denominator, denominator)
  
  def print(): Unit = { println (numerator);
                        println (denominator);
                        println (numerator.asInstanceOf[Double] / denominator) } 
  
  private def gcd(a: Int, b: Int): Int = 
    if (b == 0) a else gcd(b, a % b)
}



