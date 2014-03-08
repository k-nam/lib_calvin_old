package euler

object prob_010 {
  
  
  def main(args: Array[String]): Unit = {
    val max: Int = 2000000
    val primeArray = prob_003.findPrimes(max)
    var sum: Long = 0
    for (prime <- primeArray) {
      sum += prime
      //println(prime)     
    }
    println (sum)
    sum = 0
    
    
 
    

  }
		
}

