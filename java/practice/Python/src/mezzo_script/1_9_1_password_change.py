#!/usr/bin/env python
# -*- coding: utf_8 -*-
"""
비밀번호보안성강화캠페인_20080721_박민수
"""

import sys, os
import time, datetime
from time import strptime
import getopt
#sys.path.insert(0, os.path.join(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib'))
from mezzo_util import *
import time, datetime
import csv


def usage(program):
    print >> sys.stderr, "\nUsage: " + program + "\t[-d] [<date string>]"
    print >> sys.stderr, "\t\t-d\tDebug mode\n\n"


def main():
    optlist = []
    args = sys.argv[1:]

    progname = os.path.basename(sys.argv[0])
    x = progname.rfind('.')
    if x > 0: progname = progname[:x]

    start_time, end_time = parse_timerange(args)

#    target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
#    os.system('mkdir -p ' + target_date_path)
#    outfile = open(os.path.join(target_date_path,'1_9_1_password_change.tsv'), 'wb')
    outfile = open(os.path.join('1_9_1_password_change.tsv'), 'wb')

    try_draw_cmd = make_draw_cmd( start_time, end_time )
    
    start = time.ctime()
    print 'Start To: ', start

    try_chgpass = cntPopupPassChgPage( start_time, end_time, progname )

    hdrs = ['날짜','노출ID']

    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow(dict(zip(hdrs,hdrs)))


    for row in try_chgpass:
        temp = {}
        temp['날짜'] = start_time.strftime('%Y%m%d')
        temp['노출ID'] = row['sum.value']
        writer.writerow(temp)

    end = time.ctime()
    print 'End To: ', end
    

# Count of Page which is changed password
def cntPopupPassChgPage( start_time, end_time, progname ):

    regex_pattern = '<[^>]*(?P<sp>sp="한게임/한게임회원/비밀번호변경캠페인/변경팝업")[^>]*>'
    regex_format = '\g<sp>'
    regex_key_pattern = '<[^>]*(?P<id>li="[^> ]*")[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" sp="(?P<sp>한게임/한게임회원/비밀번호변경캠페인/변경팝업)"/'
    aggr_key_expr = 'id:echo(id),sp:echo(sp)'

    aggr_key_pattern1 = '/<k id=(?P<id>[^> ]*) sp=(?P<sp>한게임/한게임회원/비밀번호변경캠페인/변경팝업)>/'
    aggr_key_expr1 = 'sp:echo(sp)'
    aggr_op_list1 = ['sum(1)']
    
    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, '', aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)
    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, aggr_cmd], progname, True)
    os.system( hexec_cmd )

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1, 'rb') )


if __name__ == '__main__':
    main()
