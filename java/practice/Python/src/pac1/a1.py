import re
import timeit
import random
import datetime
import time

romanNumeralPattern = re.compile(r'(XL|XC|L?X?X?X?)(IV|IX|V?I?I?I?)')
result = romanNumeralPattern.search('LXVII').groups()

pattern= re.compile(r'7[^a]+77777[^5]+5')
textLength = 1000000




if __name__ == "__main__":
    print 'this is main'
    string = ''
    for i in range(textLength):
        string = string + str(random.randint(0, 9))
        
    
    time1 = time.clock()
    #for match in pattern.findall(string):
        #print match
    numMatch = len(pattern.findall(string))
    time2 = time.clock()
    diff = time2 - time1
    print "match is: %d \nregex throughput is: %e" % (numMatch, (textLength / diff))
    print result

        



