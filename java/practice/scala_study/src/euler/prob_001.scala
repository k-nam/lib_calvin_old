package euler



object prob_001 {  
  // getSum(max): sigma of k for 3|k or 5|k, k <= max
  def getSum(max: Int): Int = {
    if (max == 0) {
      0
    } else if (max % 3 == 0 || max % 5 == 0) {
      max + getSum(max - 1)
    } else {
      getSum(max - 1) 
    }
  }

  
  def main(args: Array[String]) {
    println("answer is " + getSum(999))
    5
  }
    
}
