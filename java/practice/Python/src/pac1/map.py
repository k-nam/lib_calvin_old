'''
Created on 2009. 6. 7.

@author: Administrator
'''
import time
import random

mapSize = 5000000
testSize = 10000
map = {}
keys = []
testKeys = []
values = []
testValues = []
for i in range(mapSize):
    key = random.randint(0, 100000000)
    value = random.randint(0,100000000)
    keys.append(key)
    values.append(value)
    
for i in range(testSize):
    key = random.randint(0, 100000000)
    value = random.randint(0,100000000)
    testKeys.append(key)
    testValues.append(value)


time1 = time.clock()
for i in range(mapSize):
    map[keys[i]] = values[i]
time2 = time.clock()
duration = time2 - time1
print "building throughput: %e items per sec\n" % (mapSize / duration)

time1 = time.clock()
for i in range(testSize):
    #if not map.has_key(testKeys[i]):
        map[testKeys[i]] = testValues[i]
time2 = time.clock()
duration = time2 - time1
print "insert throughput: %e items per sec\n" % (testSize / duration)

time1 = time.clock()
for i in range(testSize):
    map.has_key(testKeys[i])
time2 = time.clock()
duration = time2 - time1
print "search throughput: %e items per sec\n" % (testSize / duration)

time1 = time.clock()
for i in range(testSize):
    if map.has_key(testKeys[i]):
        del map[testKeys[i]]
time2 = time.clock()
duration = time2 - time1
print "delete throughput: %e items per sec\n" % (testSize / duration)