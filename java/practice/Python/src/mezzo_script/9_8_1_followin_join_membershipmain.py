#!/usr/bin/env python
# -*- coding: utf_8 -*-
"""
회원가입프로세스분석_도메인별분석_20080425_박민수
"""

import sys, os
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
    if x > 0 : progname = progname[:x]

    try :
        optlist, args = getopt.getopt( args, 'd' )
    except getopt.GetoptError:
        usage(sys.argv[0])
        sys.exit(1)

    debug_mode = False

    for opt, arg in optlist:
        if opt == '-d':
            debug_mode = True

    start_time, end_time = parse_timerange(args)
    
    target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))

    start = time.ctime()
    print 'Start To: ', start

    cnt_followin = cntFollowInMemmain( start_time, end_time, progname )
#    outfile = open(os.path.join(target_date_path,'9_8_1_followin_join_membershipmain.tsv'), 'wb')
    outfile = open(os.path.join('9_8_1_followin_join_membershipmain.tsv'),'wb')
    hdrs = ['date','docref','pv','uv']

    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow(dict(zip(hdrs,hdrs)))

    followin_row = []

    # 개별 pv,uv를 구하면서 전체 합을 구한다.
    for row in cnt_followin:
        followin_row.append({
            'date':start_time.strftime('%Y-%m-%d'),
            'docref':row['k.docref'],
            'pv':row['sum.pv'],
            'uv':row['sum.value']})

    for row in followin_row:
        writer.writerow(row)
    
    end = time.ctime()
    print 'End To: ', end
    

# 유입경로별 한게임 메인페이지 접속
def cntFollowInMemmain( start_time, end_time, progname ):

    regex_pattern = '<[^>]*(?P<docref>docref="?[^>\"]*"?)[^>]*(?P<sp>sp="한게임/한게임회원/회원가입메인페이지")[^>]*>'
    regex_format = '\g<docref> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc="[^> ]*")[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc="(?P<id>[^> ]*)" docref="?(?P<docref>[^>\"]*)"? sp="(?P<sp>한게임/한게임회원/회원가입메인페이지)"/'
    aggr_key_expr = 'id:echo(id),docref:remain_domain(docref),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k docref=(?P<docref>[^> ]*)[^>]*sp=한게임/한게임회원/회원가입메인페이지>/'
    aggr_key_expr1 = 'docref:echo(docref)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd( start_time, end_time, [regex_cmd,aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )

if __name__ == '__main__':
    main()
