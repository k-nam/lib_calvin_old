package euler

import scala.collection.mutable.HashMap

object prob_003 {
  
  def main(args:Array[String]): Unit = {
    //primeDecompose(6) 
    //println(findPrimes(1000))
    println(primeDecompose(600851475143L))
  }
  
  // Each key corresponds to a prime number, and the mapped value
  // is the power associated to that prime.
  // Ex) 12 = 2^2 * 3 , so function will return { 2:2, 3:1 }
  def primeDecompose(number: Long): HashMap[Int, Int] = {
    val map = new HashMap[Int, Int]
    val primes = findPrimes(10000000)
    
    def getDivisor(number: Long): Int = {
      for (prime <- primes) {
        if (number % prime == 0) {
          return prime
        }
      }
      if (number < 10000000) {
        return number.asInstanceOf[Int]
      } else {
        println("Found too big a prime number!! T.T")
        return number.asInstanceOf[Int]
      }
    }
    
    def primeDecompose(tempMap: HashMap[Int, Int], number: Long): HashMap[Int, Int] = {
      if (number == 1) {
        tempMap
      } else {
        val divisor = getDivisor(number)
        val power = tempMap.getOrElseUpdate(divisor, 0)
        tempMap += (divisor -> (power + 1))
        primeDecompose(tempMap, number / divisor)
      }
    }
    primeDecompose(map, number)    
  }
  
  // Find all primes up to 'maximum'
  def findPrimes(maximum: Int): List[Int] = {  
    val tempArray = new Array[Boolean](maximum + 1)
    var result = List[Int]()
    for (i <- 2 to maximum) {
      tempArray(i) = true
    }
    for (i <- 2 to maximum) {
      if (tempArray(i) == true) {
        result = i::result
        var j = i
        while (j <= maximum) {
          tempArray(j) = false
          j += i
        }
      }
    }
    result.reverse
  }
}
