#!/usr/bin/env python
# -*- coding: utf_8 -*-
"""
네이버 시작 페이지 변경 분석_20080804_박민수
"""

import sys, os
import time, datetime
from time import strptime
import getopt
#sys.path.insert(0,os.path.join(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib'))
from mezzo_util import *
import time, datetime
import csv


def usage(program):
    print >> sys.stderr, "\nUsage: " + program + "\t[-d] [<date string>]"
    print >> sys.stderr, "\t\t-d\tDebug mode\n\n"


def main():
    progname = os.path.basename(sys.argv[0])
    x = progname.rfind('.')
    if x > 0 : progname = progname[:x]
    
    optlist = []
    args = sys.argv[1:]

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
#    target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
#    os.system('mkdir -p ' + target_date_path)
#    outfile = open(os.path.join(target_date_path, '9_4_1_change_to_naver.tsv'), 'wb')
   
    outfile = open(os.path.join('9_4_1_change_to_naver.tsv'),'wb')
    
    start = time.ctime()
    print 'Start To: ', start

    start_launcher = cntStartLauncher( start_time, end_time, progname )
    print '런처 실행...'
 #   chg_to_naver = cntChgToNaver( start_time, end_time, progname )
    print '네이버 시작페이지 변경...'
 #   chg_other_to_naver = cntChgOtherToNaver( start_time, end_time, progname )
    print '다른 페이지에서 네이버로 변경...'

    hdrs = ['날짜','프로그램','실행수','네이버 변경','타사이트 시작']

    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow(dict(zip(hdrs,hdrs)))

    temp = {}

    for row in start_launcher:
        temp[row['k.sp']] = [row['sum.value'],'0','0']

    for row in chg_to_naver:
        temp[row['k.sp']][1] = row['sum.value']

    for row in chg_other_to_naver:
        try:
            temp[row['k.sp']][2] = row['sum.value']
        except:
            continue

    for key in temp.keys():
        result = dict()
        result['날짜'] = start_time.strftime('%Y-%m-%d')
        result['프로그램'] = key
        result['실행수'] = temp[key][0]
        result['네이버 변경'] = temp[key][1]
        result['타사이트 시작'] = temp[key][2]
        
        writer.writerow(result)

    end = time.ctime()
    print 'End To: ', end
    

# Count of Starting Launcher HANSTART, PUBLE and SINGLE
def cntStartLauncher( start_time, end_time, progname ):

    regex_pattern = '<[^>]*(?P<mac_addr>mac_addr=[^> ]*)[^>]*(?P<sp>sp="한게임/(한스타트|퍼블런처|싱글런처)/실행")[^>]*>'
    regex_format = '\g<mac_addr> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>li="[^> ]*")[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="[^> ]*" mac_addr="(?P<mac_addr>[^> ]*)" sp="(?P<sp>한게임/(한스타트|퍼블런처|싱글런처)/실행)"/'
    aggr_key_expr = 'mac_addr:echo(mac_addr),sp:echo(sp)'

    aggr_key_pattern1 = '/<k mac_addr=(?P<mac_addr>[^> ]*) sp=(?P<sp>한게임/(한스타트|퍼블런처|싱글런처)/실행)>/'
    aggr_key_expr1 = 'mac_addr:echo(mac_addr),sp:echo(sp)'

    aggr_key_pattern2 = '/<k mac_addr=[^> ]* sp=한게임/(?P<sp>한스타트|퍼블런처|싱글런처)/실행>/'
    aggr_key_expr2 = 'sp:echo(sp)'
    aggr_op_list2 = ['sum(1)']
    
    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, '', aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, '', aggr_key_expr1, False)
    aggr_cmd2 = make_aggr_cmd(aggr_key_pattern2, aggr_op_list2, aggr_key_expr2, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, aggr_cmd], progname, True)
    print hexec_cmd
   # os.system(hexec_cmd)
 
    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1 + ' | ' + aggr_cmd2,'rb') )
    
    
# Count of Changing Page To Naver whatever before page was
def cntChgToNaver( start_time, end_time, progname ):

    regex_pattern = '<[^>]*(?P<mac_addr>mac_addr=[^> ]*)[^>]*(?P<sp>sp="한게임/(한스타트|퍼블런처|싱글런처)/네이버_시작페이지")[^>]*>'
    regex_format = '\g<mac_addr> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>li="[^> ]*")[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="[^> ]*" mac_addr="(?P<mac_addr>[^> ]*)" sp="(?P<sp>한게임/(한스타트|퍼블런처|싱글런처)/네이버_시작페이지)"/'
    aggr_key_expr = 'mac_addr:echo(mac_addr),sp:echo(sp)'

    aggr_key_pattern1 = '/mac_addr=(?P<mac_addr>[^> ]*) sp=(?P<sp>한게임/(한스타트|퍼블런처|싱글런처)/네이버_시작페이지)/'
    aggr_key_expr1 = 'mac_addr:echo(mac_addr),sp:echo(sp)'

    aggr_key_pattern2 = '/<k mac_addr=[^> ]* sp=한게임/(?P<sp>한스타트|퍼블런처|싱글런처)/네이버_시작페이지>/'
    aggr_key_expr2 = 'sp:echo(sp)'
    aggr_op_list2 = ['sum(1)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, '', aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, '', aggr_key_expr1, False)
    aggr_cmd2 = make_aggr_cmd(aggr_key_pattern2, aggr_op_list2, aggr_key_expr2, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1 + ' | ' + aggr_cmd2,'rb') )
    

# Count of Changing Page that wasn't Naver To Naver
def cntChgOtherToNaver( start_time, end_time, progname ):

    regex_pattern = '<[^>]*first_page="(?![^> ]*naver)[^> ]*" (?P<mac_addr>mac_addr=[^> ]*)[^>]*(?P<sp>sp="한게임/(한스타트|퍼블런처|싱글런처)/네이버_시작페이지")[^>]*>'
    regex_format = '\g<mac_addr> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>li="[^> ]*")[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="[^> ]*" mac_addr="(?P<mac_addr>[^> ]*)" sp="(?P<sp>한게임/(한스타트|퍼블런처|싱글런처)/네이버_시작페이지)"/'
    aggr_key_expr = 'mac_addr:echo(mac_addr),sp:echo(sp)'

    aggr_key_pattern1 = '/mac_addr=(?P<mac_addr>[^> ]*) sp=(?P<sp>한게임/(한스타트|퍼블런처|싱글런처)/네이버_시작페이지)/'
    aggr_key_expr1 = 'mac_addr:echo(mac_addr),sp:echo(sp)'

    aggr_key_pattern2 = '/<k mac_addr=[^> ]* sp=한게임/(?P<sp>한스타트|퍼블런처|싱글런처)/네이버_시작페이지>/'
    aggr_key_expr2 = 'sp:echo(sp)'
    aggr_op_list2 = ['sum(1)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, '', aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, '', aggr_key_expr1, False)
    aggr_cmd2 = make_aggr_cmd(aggr_key_pattern2, aggr_op_list2, aggr_key_expr2, True)

    hexec_cmd = make_hexec_cmd( start_time, end_time, [regex_cmd, aggr_cmd], progname, True)
    os.system(hexec_cmd)
    
    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1 + ' | ' + aggr_cmd2,'rb') )


if __name__ == '__main__':
    main()
