#!/usr/bin/env python
# -*- coding: utf_8 -*-

"""
7포커 올인시 회원행동패턴 분석 (포커아바타 구매, 포커아이템 구매, 아바타 구매)
"""

import sys, os
import getopt
import time
#sys.path.insert(0, os.path.join(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib'))
from mezzo_util import *
from lib_4_3_2_1 import *

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


    hash_table = eighteenth_0(debug_mode, progname)

    if (debug_mode):
        target_date_path = os.path.join(target_path + ".old", start_time.strftime('%Y/%m/%d'))
    else:
        target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
    os.system('mkdir -p ' + target_date_path)

#    outfile = open(os.path.join(target_date_path, '4_3_2_p_7p_allin_user_behavior_pattern.1.tsv'), 'wb')
    outfile = open(os.path.join('./4_3_2_p_7p_allin_user_behavior_pattern.1.tsv'), 'wb')

    hdrs = ['date', 'p_level', 'p_ispu', 'mshiptype', 'goodstype', 'goodsid', 'num_actions']
    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow( dict( zip(hdrs, hdrs) ) )

    for key in hash_table.keys():
        value = hash_table[key]
        new_row = { 'date': start_time.strftime('%Y-%m-%d'), 'p_level': key[0], 'p_ispu': key[1], 'mshiptype': key[2], 'goodstype': key[3], 'goodsid': key[4], 'num_actions': value}
        writer.writerow(new_row)
        if (debug_mode):
            print new_row

    execute_end = time.ctime()

    print execute_start + '\t' + execute_end + '\t' + 'File creation successed : 4_3_2_p_7p_allin_user_behavior_pattern.1.tsv\n'

if __name__ == '__main__':
    main()
