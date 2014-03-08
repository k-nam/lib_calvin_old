package euler

object ListAlgorithm {
  def quicksort(array: List[Int]): List[Int] = {
    array match {
      case head::tail =>
        val (leftSubArray, rightSubArray) = partition(_ <= head, tail)
        quicksort(leftSubArray):::head::quicksort(rightSubArray)
      case Nil =>
        Nil        
    }
  }
  
  def mergesort(array: List[Int]): List[Int] = {
    array match {
      case head::tail if array.length > 1=>
        val (leftArray, rightArray) = array splitAt array.length / 2
        merge(mergesort(leftArray), mergesort(rightArray))
      case _ => array
    }
  }
    
  def partition(predicate: Int => Boolean, array: List[Int]): (List[Int], List[Int]) = {
    array match {
      case head::tail if predicate(head.asInstanceOf[Int]) => 
        (head::(partition(predicate, tail)._1), partition(predicate, tail)._2)
      case head::tail =>
        (partition(predicate, tail)._1, head::(partition(predicate, tail)._2)) 
      case Nil =>
        (Nil, Nil)
    }
  }
  
  def merge(leftArray: List[Int], rightArray: List[Int]): List[Int] = {
    (leftArray, rightArray) match {
      case (Nil, _) => rightArray
      case (_, Nil) => leftArray
      case (leftHead::leftTail, rightHead::rightTail) =>
        if (leftHead <= rightHead) {
            leftHead::merge(leftTail, rightArray)
        } else {
            rightHead::merge(leftArray, rightTail)
        }
    }        
  }
  
  def main(args: Array[String]): Unit = {
    var list = List[Int](1, 100, 5, 10, 12, 13, 1, 2, -5, -3)
    var left = List[Int](1, 3, 5)
    var right = List[Int](2, 4, 6)
    val predicate = (x: Int) => x % 2 == 0
    println (partition(predicate, list))
    println (quicksort(list))
    println (mergesort(list))
    println (merge(left, right))
    
  }
}
