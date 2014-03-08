#!/usr/bin/env python
# -*- coding: utf_8 -*-
"""
4_3_1 포커 상품구분별 구매전환율 분석
""" 
 

import sys, os
import getopt
import time
#sys.path.insert(0, os.path.join(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib'))
from mezzo_util import *
from lib_4_3_1_1 import *
from lib_4_3_1_2 import *

def usage(progname):
    print >> sys.stderr, "\nUsage: " + progname + "\t[-d] [<date string>]\n"
    print >> sys.stderr, "\t\t-d\tDebug mode\n\n"
            
def main():
    progname = os.path.basename(sys.argv[0])
    x = progname.rfind('.')
    if x > 0: progname = progname[:x]

    optlist = []
    args = sys.argv[1:]
    try:
        optlist, args = getopt.getopt(args, 'd')
    except getopt.GetoptError:
        usage(sys.argv[0])
        sys.exit(2)
    debug_mode = False
    for o, a in optlist:
        if (o == "-d"):
            debug_mode = True

    start_time, end_time = parse_timerange(args)

    execute_start = time.ctime()

    hash_table1 = seventeenth_new_0(debug_mode, progname)
    hash_table2 = seventeenth_new_1(debug_mode, progname)

    if (debug_mode):
        target_date_path = os.path.join(target_path + ".old", start_time.strftime('%Y/%m/%d'))
    else:
        target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
    os.system('mkdir -p ' + target_date_path)
#    outfile = open(os.path.join(target_date_path, '4_3_1_p_all_buy_transition_classified_by_goods.tsv'), 'wb')
    outfile = open(os.path.join('./', '4_3_1_p_all_buy_transition_classified_by_goods.tsv'), 'wb')

    hdrs = ['date', 'p_level', 'mshiptype', 'goodstype', 'num_goods_popup', 'num_buy_done', 'sum_goodsamt']
    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow(dict(zip(hdrs, hdrs)))
    
    for key in hash_table1.keys():
        value1 = hash_table1[key]
        if (hash_table2.has_key(key)):
            value2 = hash_table2[key]
        else:
            value2 = (0, 0)
        new_row = { 'date': key[0], 'p_level': key[1], 'mshiptype': key[2], 'goodstype': key[3], 'num_goods_popup': value1, 'num_buy_done': value2[0], 'sum_goodsamt': value2[1] }
        writer.writerow(new_row)

    execute_end = time.ctime()

    print execute_start + '\t' + execute_end + '\t' + 'File creation successed : 4_3_1_p_all_buy_transition_classified_by_goods.tsv\n'
        
if __name__ == '__main__':
    main()
            
