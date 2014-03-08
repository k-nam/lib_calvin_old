package euler


object prob_002 { 
  
  def main(args: Array[String]) {
    println(" answer is " + getSum(0, 1, 0, 4000000))
  }    
 
 
  // getSum(fibo[k], fibo[k+1], tempSum, max): sum of l = fibo[k], for 2|l
  //   and l <= max plus tempSum
  def getSum(fibo1: Int, fibo2: Int, sum: Int, max: Int): Int = {
    if (fibo2 > max) {
      sum
    } else if (fibo2 % 2 == 0) {
      getSum(fibo2, fibo1 + fibo2, sum + fibo2, max)
    } else {
      getSum(fibo2, fibo1 + fibo2, sum, max) 
    }
  }
}
