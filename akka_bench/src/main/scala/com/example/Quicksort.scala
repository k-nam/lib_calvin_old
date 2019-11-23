package com.example

import akka.actor.{ Actor, ActorLogging, ActorRef, ActorSystem, Props }
import scala.util

object Quicksort {
	import Master._

	def props(array: Array[ElemType], beginIndex: Int, endIndex: Int): Props = 
		Props(new Quicksort(array, beginIndex, endIndex))

	def swap(array: Array[ElemType], index1: Int, index2: Int): Unit = {
		val temp: ElemType = array(index1)
		array(index1) = array(index2)
		array(index2) = temp
	}

	def swapIfNeeded(array: Array[ElemType], index1: Int, index2: Int): Boolean = {
		if (array(index1) > array(index2)) {
			swap(array, index1, index2)
			return true
		} else {
			return false
		}
	}

	def insertionSort(array: Array[ElemType], beginIndex: Int, endIndex: Int): Unit = {
		var right = beginIndex + 1
		while (right < endIndex) {
			var temp = right
			while (temp > beginIndex && swapIfNeeded(array, temp - 1, temp)) {
				temp -= 1
			}
			right += 1
		}
	}

	def quicksort(array: Array[Master.ElemType], beginIndex: Int, endIndex: Int): Unit = {
		val len: Int = endIndex - beginIndex
		if (len < 20) {
			insertionSort(array, beginIndex, endIndex)
			return
		}
		
		val pivotIndex = partition(array, beginIndex, endIndex)
		quicksort(array, beginIndex, pivotIndex)
		quicksort(array, pivotIndex + 1, endIndex)
	}

	def partition(array: Array[Master.ElemType], beginIndex: Int, endIndex: Int): Int = {
		var leftIndex: Int = beginIndex
		var rightIndex: Int = endIndex - 1
		val midIndex: Int = beginIndex + (endIndex - beginIndex) / 2

		// Median of three. beginIndex and endIndex will act as guards
		swapIfNeeded(array, leftIndex, midIndex)
		swapIfNeeded(array, midIndex, rightIndex)
		swapIfNeeded(array, leftIndex, midIndex)

		val pivot: ElemType = array(midIndex)
		swap(array, beginIndex, midIndex)
		leftIndex += 1
	
		while (leftIndex < endIndex && array(leftIndex) < pivot) {
			leftIndex += 1
		}
		while (rightIndex >= beginIndex && pivot < array(rightIndex)) {
			rightIndex -= 1
		}

		while (leftIndex < rightIndex) {
			swap(array, leftIndex, rightIndex)
			leftIndex += 1
			rightIndex -= 1
			while (array(leftIndex) < pivot) {
				leftIndex += 1
			}
			while (pivot < array(rightIndex)) {
				rightIndex -= 1
			}
		}

		swap(array, beginIndex, leftIndex - 1)
		return leftIndex - 1
	}

	case object SortNow
	case object ChildFinished
} 


class Quicksort(array: Array[Master.ElemType], beginIndex: Int, endIndex: Int) extends Actor with ActorLogging {
	import Quicksort._
	import Master._

	var numFinishedChildred = 0
	var parent: ActorRef = null

	def receive = {		
		case SortNow=> {
			parent = sender
			val len: Int = endIndex - beginIndex
			if (len < 100000) {
				//util.Sorting.stableSort(array)				
				//util.Sorting.quickSort(array)
				//java.util.Arrays.sort(array)

				quicksort(array, beginIndex, endIndex)			
				finished
			} else {
				val pivotIndex = Quicksort.partition(array, beginIndex, endIndex)
				val leftActor: ActorRef = TestQuicksort.system.actorOf(Quicksort.props(array, beginIndex, pivotIndex))
				val rightActor: ActorRef = TestQuicksort.system.actorOf(Quicksort.props(array, pivotIndex + 1, endIndex))

				leftActor ! SortNow
				rightActor ! SortNow
			}			
		}

		case ChildFinished => {
			numFinishedChildred += 1
			if (numFinishedChildred == 2) {
				finished
			}
		}

		case _ => log.error("Wrong message to Quicksort actor")
	} 

	def finished = {
		parent ! ChildFinished		
	}
}

object Master {
	def props: Props = {
		Props(new Master)
	}
	
	type ElemType = Long
	case object Start
}

class Master extends Actor with ActorLogging {
	import Quicksort._
	import Master._

	
	val gen = scala.util.Random
	val arraySize = 1000 * 1000 * 10
	//val array: Array[ElemType] = Array.fill(arraySize){1} 
	val array: Array[ElemType] = Array.fill(arraySize){ Math.abs(gen.nextInt)} 
	val copy: Array[ElemType] = array.clone()

	java.util.Arrays.sort(copy)

	val t0 = System.currentTimeMillis()

	val sorter = TestQuicksort.system.actorOf(Quicksort.props(array, 0, arraySize))

	def receive = {
		case Start => {
			log.info("Master started")
			sorter ! SortNow
		}

		case ChildFinished => {
			val t1 = System.currentTimeMillis()
			val work = arraySize * Math.log(arraySize) 
			val time = ((t1 - t0)) / 1000.0
			val mega = 1000 * 1000
			log.info("Sorting work: " + work.toInt + " time: " + time +  " speed was: " + work / time / mega  + "M/s")

			var sortingCorrect = true
			for (index <- (0 until arraySize - 1)) {
				if (array(index) > array(index + 1)) {
					sortingCorrect = false
				}
			}
			if (array.deep != copy.deep) {
				sortingCorrect = false
			}

			if (sortingCorrect) {
				log.info("Sorting correct")
			} else {
				log.info("Sorting failed")
			}

			//log.info(array.mkString(", "))
		}

		case _ => log.error("Wrong message to Master actor")
	}
}

object TestQuicksort extends App {
	import Master._

	val system: ActorSystem = ActorSystem("Test_Quicksort_Actor_System")

	val master = system.actorOf(Master.props)
	master ! Start
}
   
   