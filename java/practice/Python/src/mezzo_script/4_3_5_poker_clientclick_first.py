#!/usr/bin/env python
# -*- coding: utf_8 -*-
"""
포커클라이언트로그기록분석_20080521_박민수
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
    if x > 0: progname = progname[:x]

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
#    outfile = open(os.path.join(target_date_path, '4_3_5_poker_clientclick_first.tsv'), 'wb')
    outfile = open(os.path.join('./4_3_5_poker_clientclick_first.tsv'),'wb')
    
    execute_start = time.ctime()

    print '대기실 최초클릭 처리중...'
    firstclick_waitingroom = cntFirstClickWaitingroom( start_time, end_time, progname )
    print '경기장 최초클릭 처리중...'
    firstclick_gameroom = cntFirstClickGameroom( start_time, end_time, progname )

    hdrs = ['날짜','경기장구분','섹션구분','경기장','등급','영역','PV']

    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow(dict(zip(hdrs,hdrs)))

    for row in firstclick_waitingroom:
        new_row = {}
        new_row['날짜'] = start_time.strftime('%Y-%m-%d')
        new_row['경기장구분'] = row['k.cls']
        new_row['섹션구분'] = row['k.section_cls']
        new_row['경기장'] = row['k.ground']
        new_row['등급'] = row['k.p_level']
        new_row['영역'] = row['k.sp']
        new_row['PV'] = row['sum.pv']

        writer.writerow(new_row)
        
    for row in firstclick_gameroom:
        new_row = {}
        new_row['날짜'] = start_time.strftime('%Y-%m-%d')
        new_row['경기장구분'] = row['k.cls']
        new_row['섹션구분'] = row['k.section_cls']
        new_row['경기장'] = row['k.ground']
        new_row['등급'] = row['k.p_level']
        new_row['영역'] = row['k.sp']
        new_row['PV'] = row['sum.pv']

        writer.writerow(new_row)

    execute_end = time.ctime()

    print 'START TIME : ' + execute_start + '\t' + 'END TIME : ' + execute_end + '\t' + 'File creation successed : 4_3_5_poker_clientclick_first.tsv\n'
    

# 대기실 최초 클릭 PV
def cntFirstClickWaitingroom( start_time, end_time, progname ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/웹/홈\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/웹/홈"[^>]*>.*?<[^>]*p_(hl|7p|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|맞포커|로우바둑이)/클라이언트/경기장대기실"[^>]*><[^>]*(?P<sp>sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/대기실/[^> ]*")[^>]*>'

    regex_format = '\g<ground> \g<p_level> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" ground="(?P<ground>[^> ]*)" p_level="(?P<p_level>\d*)" sp="한게임/포커게임/(?P<cls>7포커|하이로우|맞포커|로우바둑이)/클라이언트/(?P<sp>[^> ]*)"/'
    aggr_key_expr = 'id:echo(id),ground:echo(ground),p_level:echo(p_level),cls:echo(cls),sp:echo(sp)'
    aggr_op_list = ['sum(1)']


    aggr_key_pattern1 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) ground=(?P<ground>[^> ]*) id=[^> ]* p_level=(?P<p_level>\d*) sp=(?P<sp>[^> ]*)>/'
    aggr_key_expr1 = 'cls:echo(cls),section_cls:echo(대기실),ground:echo(ground),p_level:echo(p_level),sp:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd1 = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1, 'rb') )


# 게임방 최초 클릭 PV
def cntFirstClickGameroom( start_time, end_time, progname ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/웹/홈\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/웹/홈"[^>]*>.*?<[^>]*p_(hl|7p|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*>.*?<[^>]*(?P<sp>sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/[^> ]*")[^>]*>'

    regex_format = '\g<ground> \g<p_level> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" ground="(?P<ground>[^> ]*)" p_level="(?P<p_level>\d*)" sp="한게임/포커게임/(?P<cls>7포커|하이로우|로우바둑이|맞포커)/클라이언트/(?P<sp>[^> ]*)"/'
    aggr_key_expr = 'id:echo(id),ground:echo(ground),p_level:echo(p_level),cls:echo(cls),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) ground=(?P<ground>[^> ]*) id=[^> ]* p_level=(?P<p_level>\d*) sp=(?P<sp>[^> ]*)>/'
    aggr_key_expr1 = 'cls:echo(cls),section_cls:echo(게임방),ground:echo(ground),p_level:echo(p_level),sp:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd1 = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


if __name__ == '__main__':
    main()
