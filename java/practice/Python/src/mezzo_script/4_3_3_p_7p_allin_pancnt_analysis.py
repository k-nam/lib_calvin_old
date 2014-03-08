#!/usr/bin/env python
# -*- coding: utf_8 -*-

"""
4-3-3- 7포커 올인판수 분석
"""

import sys, os
import math, time
import getopt
#sys.path.insert(0, './lib')
#sys.path.append(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib')
from mezzo_util import *
from time import strptime
from lib_4_3_3_1 import *
from lib_4_3_3_2 import *

def usage(progname):
    print >> sys.stderr, "\nUsage: " + progname + "\t[-d] [-n <# of days] [<date string>]\n"
    print >> sys.stderr, "\t\t-d\tDebug mode"
    print >> sys.stderr, "\t\t-n\tTime range of analysis (unit: day)"
    print >> sys.stderr, "\t\t<date string>\tIf omitted, the basic date is yesterday as default\n"

def main():
    progname = os.path.basename(sys.argv[0])
    x = progname.rfind('.')
    if x > 0: progname = progname[:x]

    optlist = []
    args = sys.argv[1:]
    try:
        optlist, args = getopt.getopt(args, 'dn:')
    except getopt.GetoptError:
        usage(sys.argv[0])
        sys.exit(2)
    debug_mode = False
    num_days = 30         #30일은 넘긴 데이터들은 통계에 반영하지 않는다.
#    num_days = 3
    for o, a in optlist:
        if (o == "-d"):
            debug_mode = True
        elif (o == '-n'):
            num_days = int(a)

    execute_start = time.ctime()

    # 날짜 구간을 정하기 위한 option 및 argument 처리
    if (len(args) == 0):
        # 오늘 5시
        end_datetime = (datetime.datetime.now()).replace(hour=05, minute=0, second=0, microsecond=0)
        end_time_str = end_datetime.strftime("%Y-%m-%d %H:%M:%S")
    elif (len(args) == 1):
        # 기준일 5시
        end_datetime = (datetime.datetime(*strptime(args[0], '%Y%m%d_%H')[0:6])).replace(hour=05, minute=0, second=0, microsecond=0)
        end_datetime += datetime.timedelta(hours=24)
        end_time_str = mezzo_strptime(args[0])
    else:
        usage(sys.argv[0])
        sys.exit(2)
    start_datetime = end_datetime - datetime.timedelta(hours=num_days*24-1)
    start_time_str = start_datetime.strftime("%Y-%m-%d %H:%M:%S")
    the_day_before_end_datetime = end_datetime - datetime.timedelta(hours=24)
                
    # 오늘 날짜의 ts를 가진 올인팝업을 추출
    end_day_6am_ts = time.mktime(end_datetime.replace(hour=06, minute=0, second=0, microsecond=0).timetuple())
    day_before_end_day_6am_ts = end_day_6am_ts - 3600 * 24
    awk_cmd1 = r'awk "BEGIN { IFS=\" \" } { if (' + str(day_before_end_day_6am_ts) + ' <= \$1 && \$1 < ' + str(end_day_6am_ts) + ') { print \$0 } }"'
    #print awk_cmd

#    outfile = open(os.path.join(target_date_path, '4_3_3_p_7p_allin_pancnt_analysis.tsv'), 'wb')
    outfile = open(os.path.join('./4_3_3_p_7p_allin_pancnt_analysis.tsv'), 'wb')
    hdrs = ['date', 'mshiptype', 'p_level', 'avg_buy_p_7p_pancount', 'avg_buy_allin_duration']
    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow(dict(zip(hdrs, hdrs)))

    hash_table1 = nineteenth_0_new(progname, debug_mode, num_days)
    hash_table2 = nineteenth_1_new(progname, debug_mode, num_days)

    for key in hash_table1.keys():
        value1 = hash_table1[key]
        if (hash_table2.has_key(key)):
            value2 = hash_table2[key]
        else:
            value2 = 0
        new_row = { 'date': the_day_before_end_datetime.strftime('%Y-%m-%d'), 'mshiptype': key[0], 'p_level': key[1], 'avg_buy_p_7p_pancount': value1, 'avg_buy_allin_duration': value2 }
        writer.writerow(new_row)
        if (debug_mode):
            print new_row

    execute_end = time.ctime()

    print execute_start + '\t' + execute_end + '\t' + 'File creation successed : 4_3_3_p_7p_allin_panct_analysis.tsv\n'
            
if __name__ == '__main__':
    main()
