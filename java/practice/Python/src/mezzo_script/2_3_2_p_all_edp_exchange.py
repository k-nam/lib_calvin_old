#! /usr/bin/env python
# -*- coding: utf_8 -*-

import sys,os
import csv 
#sys.path.insert(0, os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib')
from mezzo_util import *
import time

def main():
    progname = os.path.basename(sys.argv[0])
    x = progname.rfind('.')
    if x > 0: progname = progname[:x]

    start_time, end_time = parse_timerange( sys.argv[1:] )

    execute_start = time.ctime()    

    regex_ptn = (
        r'(<[^>]* docref="[^"]*"[^>]* p_mny_exchange=[^>]* sp=[^ >]*엔돌핀_교환_완료_웹[^>]*>)')
    regex_fmt = r"\1"
    aggr_op_lst =["sum(1)",  "sum(/edp_used=\"(?P<edp_used>[^\"]+)\"[^>]* p_mny_exchange=\"(?P<p_mny_exchange>[^\"]+)\"/)"]
    aggr_key = r'/docref=(?P<docref>"[^"]*")/'

    target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
    if not os.path.exists(target_date_path):
        os.makedirs(target_date_path, 0750)

#    outfile = open(os.path.join(target_date_path, progname + '.tsv'), 'wb')
    outfile = open('./2_3_2_p_all_edp_exchange.tsv','wb')

    hdrs = ['date', 'docref', 'num_edp_popup', 'edp_used', 'p_mny_exchange']
    date = start_time.strftime('%Y-%m-%d')

    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow( dict( zip(hdrs, hdrs) ) )
   
    for row in mana_exec2(False, start_time, end_time, progname, [(regex_ptn, regex_fmt)], aggr_key, aggr_op_lst):
        new_row = {
            'date': date,
            'docref': row['k.docref'],
            'num_edp_popup': row['sum.value'],
            'edp_used': row['sum.edp_used'],
            'p_mny_exchange': row['sum.p_mny_exchange']
            }
        writer.writerow( new_row )

    execute_end = time.ctime()

    print execute_start + '\t' + execute_end + '\t' + 'File creation successed : 2_3_2_p_all_edp_exchange.tsv\n'


if __name__ == '__main__':
    main()
