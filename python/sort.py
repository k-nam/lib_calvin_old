import random
import time

def myfunc(a):
    b = a + 5
    return b

def mergesort(inArray):
    temp = inArray[0:len(inArray)]
    mergesort_R(temp, inArray, 0, len(inArray))
    return

def mergesort_R(inArray, outArray, startIndex, endIndex):
    if endIndex - startIndex < 25:
        insertionsort_(outArray, startIndex, endIndex)
        return
    middleIndex = startIndex + (endIndex - startIndex) / 2
    mergesort_R(outArray, inArray, startIndex, middleIndex)
    mergesort_R(outArray, inArray, middleIndex, endIndex)
    merge(inArray, outArray, startIndex, middleIndex, endIndex)
    return

def merge(inArray, outArray, startIndex, middleIndex, endIndex):
    leftIndex = startIndex
    rightIndex = middleIndex
    targetIndex = startIndex
    while 2 > 1:
        if inArray[leftIndex] > inArray[rightIndex]:
            outArray[targetIndex] = inArray[rightIndex]
            rightIndex += 1
            targetIndex += 1
            if rightIndex >= endIndex:
                while leftIndex < middleIndex:
                    outArray[targetIndex] = inArray[leftIndex]
                    leftIndex += 1
                    targetIndex += 1
                return
                
        else:
            outArray[targetIndex] = inArray[leftIndex]
            leftIndex = leftIndex + 1
            targetIndex = targetIndex + 1
            if leftIndex >= middleIndex:
                while rightIndex < endIndex:
                    outArray[targetIndex] = inArray[rightIndex]
                    rightIndex += 1
                    targetIndex += 1
                return

def insertionsort(inArray):
    insertionsort_(inArray, 0, len(inArray))
    return
                  
                  
def insertionsort_(inArray, startIndex, endIndex):
    p = startIndex + 1
    while p < endIndex:
        store = inArray[p]
        k = p - 1
        while k>= startIndex:
            if inArray[k] > store:
                inArray[k + 1] = inArray[k]
                k -= 1
            else:
                break
        inArray[k + 1] = store    
        p += 1
    return
    
if __name__ == "__main__":
    print "Hello"
    print myfunc(3)
    a = [] 
    count = 1000000
    while count > 0:
        a.append(count)
        count = count - 1
    random.shuffle(a)
        
    b = a[0:len(a)]
    start = time.clock()
    b.sort()
    end = time.clock()
    print "tim sort took: "
    print end - start

    b = a[0:len(a)]
    start = time.clock()
    mergesort(b)
    end = time.clock()
    print "mergesort took: "
    print end - start



    temp1 = [1,2,3,4,5,6,7,8,10]
    random.shuffle(temp1)
    mergesort(temp1)
    insertionsort(temp1)
    print temp1
    
