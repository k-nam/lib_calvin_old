'''
Created on 2009. 6. 7.

@author: Administrator
'''
import time
import random

arraySize = 1000000
array = []
for i in range(arraySize):
    array.append(random.randint(0,100000000))

time1 = time.clock()
array.sort()
time2 = time.clock()
duration = time2 - time1

print "sorting throughput: %e per sec\n" % (arraySize / duration)

