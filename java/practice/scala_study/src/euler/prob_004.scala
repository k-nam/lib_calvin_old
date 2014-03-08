package euler

object prob_004 {
  def isPalandrome(number: Int): Boolean = {
    val word = number.toString
    //println(word)
    val reversedWord = word.reverse.toString
    //println(reversedWord)
    if (word == reversedWord) true
    else false
  }
  
  def isProductOfThreeDigitNumbers(number: Int): Boolean = {
    for (i <- 100 to 999) {
      if (number % i == 0 && number / i >= 100 && number / i <= 999) {
        println(number + " is producdt of " + i + " and " + number / i)
        return true
      }
    }
    return false
  }  
  	
  
  def main(args: Array[String]): Unit = {
    var i = 1000000
    while (true) {
      if (isPalandrome(i) && isProductOfThreeDigitNumbers(i)) {
        return i
      } else {
        i -= 1
      } 
    }
		  
  }
}
