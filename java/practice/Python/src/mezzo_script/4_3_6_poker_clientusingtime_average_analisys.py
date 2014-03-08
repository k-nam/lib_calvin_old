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


hdrs = ['날짜','유형구분','게임구분','경기장','등급','시간','인원']
start_time, end_time = parse_timerange(sys.argv[1:])
#target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
#os.system('mkdir -p ' + target_date_path)
#outfile = open(os.path.join(target_date_path,'4_3_6_poker_client_usingtime_average.1.tsv'), 'wb')

outfile = open(os.path.join('./4_3_6_poker_clientusingtime_average_analisys.tsv'),'wb')

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

    draw_cmd = make_draw_cmd( start_time, end_time )
    
    execute_start = time.ctime()
    
    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow(dict(zip(hdrs,hdrs)))

    print '포커 평균 클라이언트 이용시간 처리중...'
    clsClientUsingTime( start_time, end_time, progname, 'cls' )
    clsClientUsingTime( start_time, end_time, progname, 'ground' )
    clsClientUsingTime( start_time, end_time, progname, 'p_level' )
    clsClientUsingTime( start_time, end_time, progname, 'all' )
    print '포커 평균 게임이용시간 처리중...'
    clsPlayTime( start_time, end_time, progname, 'cls' )
    clsPlayTime( start_time, end_time, progname, 'ground' )
    clsPlayTime( start_time, end_time, progname, 'p_level' )
    clsPlayTime( start_time, end_time, progname, 'all' )
    print '포커 평균 게임이용시간 처리중...'
    clsWaitingTime( start_time, end_time, progname, 'cls' )
    clsWaitingTime( start_time, end_time, progname, 'ground' )
    clsWaitingTime( start_time, end_time, progname, 'p_level' )
    clsWaitingTime( start_time, end_time, progname, 'all' )
    print '포커 관전 시간 처리중...'
    clsWatchingTime( start_time, end_time, progname, 'cls' )
    clsWatchingTime( start_time, end_time, progname, 'ground' )
    clsWatchingTime( start_time, end_time, progname, 'p_level' )
    clsWatchingTime( start_time, end_time, progname, 'all' )
 
    execute_end = time.ctime()
    print 'START TIME : ' + execute_start + '\t' + 'END TIME : ' + execute_end + '\t' + 'File creation successed : 4_3_6_poker_client_usingtime_average.1.tsv'

def clsClientUsingTime( start_time, end_time, progname, _cls ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장_입장\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장_입장"[^>]*ts=(?P<ts1>"\d*")[^>]*><[^>]*p_(7p|hl|ba|du)_(?P<ground>ground=[^> ]*)[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(?P<cls>7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*>.*?<[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장_퇴장"[^>]*ts=(?P<ts2>"\d*")[^>]*>'

    regex_format = 'cls=\g<cls> \g<ground> \g<p_level> ts1=\g<ts1> ts2=\g<ts2>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    if _cls == 'cls':
        aggr_key_pattern = '/li="(?P<id>[^> ]*)" cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground="(?P<ground>[^> ]*)" p_level="(?P<p_level>\d*)" ts1="(?P<ts1>\d+)" ts2="(?P<ts2>\d+)"/'
        aggr_key_expr = 'id:echo(id),cls:echo(cls),ts:sub(ts2,ts1)'
        aggr_op_list = ['sum(1)']

        aggr_key_pattern1 = '/<k cls=(?P<cls>[^> ]*) id=(?P<id>[^> ]*) ts=(?P<ts>[^> ]*)><sum value=(?P<count>[^> ]*)>/'
        aggr_key_expr1 = 'id:echo(id),cls:echo(cls),ts:multiple(ts,count)'
        aggr_op_list1 = ['sum(1)']

        aggr_key_pattern2 = '/<k cls=(?P<cls>[^> ]*) id=(?P<id>[^> ]*)[^>]*>/'
        aggr_key_expr2 = 'id:echo(id),cls:echo(cls)'
        aggr_op_list2 = ['sum(/ts=(?P<ts>\d+)/)']

        aggr_key_pattern3 = '/<k cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커)[^>]*>/'
        aggr_key_expr3 = 'cls:echo(cls),type_cls:echo(클라이용시간),ground:echo(all),p_level:echo(all)'
    elif _cls == 'ground':
        aggr_key_pattern = '/li="(?P<id>[^> ]*)" cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground="(?P<ground>[^> ]*)" p_level="(?P<p_level>\d*)" ts1="(?P<ts1>\d+)" ts2="(?P<ts2>\d+)"/'
        aggr_key_expr = 'id:echo(id),cls:echo(cls),ground:echo(ground),ts:sub(ts2,ts1)'
        aggr_op_list = ['sum(1)']

        aggr_key_pattern1 = '/<k cls=(?P<cls>[^> ]*) ground=(?P<ground>[^> ]*) id=(?P<id>[^> ]*) ts=(?P<ts>[^> ]*)><sum value=(?P<count>[^> ]*)>/'
        aggr_key_expr1 = 'id:echo(id),cls:echo(cls),ground:echo(ground),ts:multiple(ts,count)'
        aggr_op_list1 = ['sum(1)']

        aggr_key_pattern2 = '/<k cls=(?P<cls>[^> ]*) ground=(?P<ground>[^> ]*) id=(?P<id>[^> ]*)[^>]*>/'
        aggr_key_expr2 = 'id:echo(id),cls:echo(cls),ground:echo(ground)'
        aggr_op_list2 = ['sum(/ts=(?P<ts>\d+)/)']

        aggr_key_pattern3 = '/<k cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground=(?P<ground>[^> ]*)[^>]*>/'
        aggr_key_expr3 = 'cls:echo(cls),type_cls:echo(클라이용시간),ground:echo(ground),p_level:echo(all)'
    elif _cls == 'p_level':
        aggr_key_pattern = '/li="(?P<id>[^> ]*)" cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground="(?P<ground>[^> ]*)" p_level="(?P<p_level>\d*)" ts1="(?P<ts1>\d+)" ts2="(?P<ts2>\d+)"/'
        aggr_key_expr = 'id:echo(id),cls:echo(cls),p_level:echo(p_level),ts:sub(ts2,ts1)'
        aggr_op_list = ['sum(1)']

        aggr_key_pattern1 = '/<k cls=(?P<cls>[^> ]*) id=(?P<id>[^> ]*) p_level=(?P<p_level>[^> ]*) ts=(?P<ts>[^> ]*)><sum value=(?P<count>[^> ]*)>/'
        aggr_key_expr1 = 'id:echo(id),cls:echo(cls),p_level:echo(p_level),ts:multiple(ts,count)'
        aggr_op_list1 = ['sum(1)']

        aggr_key_pattern2 = '/<k cls=(?P<cls>[^> ]*) id=(?P<id>[^> ]*) p_level=(?P<p_level>[^> ]*)[^>]*>/'
        aggr_key_expr2 = 'id:echo(id),cls:echo(cls),p_level:echo(p_level)'
        aggr_op_list2 = ['sum(/ts=(?P<ts>\d+)/)']

        aggr_key_pattern3 = '/<k cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) [^>]*p_level=(?P<p_level>[^> ]*)[^>]*>/'
        aggr_key_expr3 = 'cls:echo(cls),type_cls:echo(클라이용시간),ground:echo(all),p_level:echo(p_level)'
    elif _cls == 'all':
        aggr_key_pattern = '/li="(?P<id>[^> ]*)" cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground="(?P<ground>[^> ]*)" p_level="(?P<p_level>\d*)" ts1="(?P<ts1>\d+)" ts2="(?P<ts2>\d+)"/'
        aggr_key_expr = 'id:echo(id),cls:echo(cls),ground:echo(ground),p_level:echo(p_level),ts:sub(ts2,ts1)'
        aggr_op_list = ['sum(1)']

        aggr_key_pattern1 = '/<k cls=(?P<cls>[^> ]*) ground=(?P<ground>[^> ]*) id=(?P<id>[^> ]*) p_level=(?P<p_level>[^> ]*) ts=(?P<ts>[^> ]*)><sum value=(?P<count>[^> ]*)>/'
        aggr_key_expr1 = 'id:echo(id),cls:echo(cls),ground:echo(ground),p_level:echo(p_level),ts:multiple(ts,count)'
        aggr_op_list1 = ['sum(1)']

        aggr_key_pattern2 = '/<k cls=(?P<cls>[^> ]*) ground=(?P<ground>[^> ]*) id=(?P<id>[^> ]*) p_level=(?P<p_level>[^> ]*)[^>]*>/'
        aggr_key_expr2 = 'id:echo(id),cls:echo(cls),ground:echo(ground),p_level:echo(p_level)'
        aggr_op_list2 = ['sum(/ts=(?P<ts>\d+)/)']

        aggr_key_pattern3 = '/<k cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground=(?P<ground>[^> ]*)[^>]*p_level=(?P<p_level>[^> ]*)[^>]*>/'
        aggr_key_expr3 = 'cls:echo(cls),type_cls:echo(클라이용시간),ground:echo(ground),p_level:echo(p_level)'
    aggr_op_list3 = ['sum(1)', 'sum(/ts=(?P<ts>\d+)/)']

    regex_cmd1 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, False)
    aggr_cmd2 = make_aggr_cmd(aggr_key_pattern2, aggr_op_list2, aggr_key_expr2, False)
    aggr_cmd3 = make_aggr_cmd(aggr_key_pattern3, aggr_op_list3, aggr_key_expr3, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, aggr_cmd, aggr_cmd1, aggr_cmd2], progname, True)
    os.system(hexec_cmd)

    write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd3 )

    
#  7포커 포커 게임을 즐긴 시간
def clsPlayTime( start_time, end_time, progname, _cls ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*p_(7p|hl|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*>.*?<[^>]*sp="한게임/포커게임/(?P<cls>7포커|하이로우|로우바둑이|맞포커)/클라이언트/방_입장"[^>]*ts=(?P<ts1>"\d+")[^>]*>.*?<[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/(게임방/상단나가기|게임방/하단나가기|방_퇴장|경기장대기실)"[^>]*ts=(?P<ts2>"\d+")[^>]*>'
    regex_format = 'cls=\g<cls> \g<ground> \g<p_level> ts1=\g<ts1> ts2=\g<ts2>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    if _cls == 'cls':
        aggr_key_pattern = '/li="(?P<id>[^> ]*)" cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground="(?P<ground>[^> ]*)" p_level="(?P<p_level>\d*)" ts1="(?P<ts1>\d+)" ts2="(?P<ts2>\d+)"/'
        aggr_key_expr = 'id:echo(id),cls:echo(cls),ts:sub(ts2,ts1)'
        aggr_op_list = ['sum(1)']

        aggr_key_pattern1 = '/<k cls=(?P<cls>[^> ]*) id=(?P<id>[^> ]*) ts=(?P<ts>[^> ]*)><sum value=(?P<count>[^> ]*)>/'
        aggr_key_expr1 = 'id:echo(id),cls:echo(cls),ts:multiple(ts,count)'
        aggr_op_list1 = ['sum(1)']
        
        aggr_key_pattern2 = '/<k cls=(?P<cls>[^> ]*) id=(?P<id>[^> ]*)[^>]*>/'
        aggr_key_expr2 = 'id:echo(id),cls:echo(cls)'
        aggr_op_list2 = ['sum(/ts=(?P<ts>\d+)/)']

        aggr_key_pattern3 = '/<k cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커)[^>]*>/'
        aggr_key_expr3 = 'cls:echo(cls),type_cls:echo(게임이용시간),ground:echo(all),p_level:echo(all)'
    elif _cls == 'ground':
        aggr_key_pattern = '/li="(?P<id>[^> ]*)" cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground="(?P<ground>[^> ]*)" p_level="(?P<p_level>\d*)" ts1="(?P<ts1>\d+)" ts2="(?P<ts2>\d+)"/'
        aggr_key_expr = 'id:echo(id),cls:echo(cls),ground:echo(ground),ts:sub(ts2,ts1)'
        aggr_op_list = ['sum(1)']

        aggr_key_pattern1 = '/<k cls=(?P<cls>[^> ]*) ground=(?P<ground>[^> ]*) id=(?P<id>[^> ]*) ts=(?P<ts>[^> ]*)><sum value=(?P<count>[^> ]*)>/'
        aggr_key_expr1 = 'id:echo(id),cls:echo(cls),ground:echo(ground),ts:multiple(ts,count)'
        aggr_op_list1 = ['sum(1)']

        aggr_key_pattern2 = '/<k cls=(?P<cls>[^> ]*) ground=(?P<ground>[^> ]*) id=(?P<id>[^> ]*)[^>]*>/'
        aggr_key_expr2 = 'id:echo(id),cls:echo(cls),ground:echo(ground)'
        aggr_op_list2 = ['sum(/ts=(?P<ts>\d+)/)']

        aggr_key_pattern3 = '/<k cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground=(?P<ground>[^> ]*)[^>]*>/'
        aggr_key_expr3 = 'cls:echo(cls),type_cls:echo(게임이용시간),ground:echo(ground),p_level:echo(all)'
    elif _cls == 'p_level':
        aggr_key_pattern = '/li="(?P<id>[^> ]*)" cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground="(?P<ground>[^> ]*)" p_level="(?P<p_level>\d*)" ts1="(?P<ts1>\d+)" ts2="(?P<ts2>\d+)"/'
        aggr_key_expr = 'id:echo(id),cls:echo(cls),p_level:echo(p_level),ts:sub(ts2,ts1)'
        aggr_op_list = ['sum(1)']

        aggr_key_pattern1 = '/<k cls=(?P<cls>[^> ]*) id=(?P<id>[^> ]*) p_level=(?P<p_level>[^> ]*) ts=(?P<ts>[^> ]*)><sum value=(?P<count>[^> ]*)>/'
        aggr_key_expr1 = 'id:echo(id),cls:echo(cls),p_level:echo(p_level),ts:multiple(ts,count)'
        aggr_op_list1 = ['sum(1)']

        aggr_key_pattern2 = '/<k cls=(?P<cls>[^> ]*) id=(?P<id>[^> ]*) p_level=(?P<p_level>[^> ]*)[^>]*>/'
        aggr_key_expr2 = 'id:echo(id),cls:echo(cls),p_level:echo(p_level)'
        aggr_op_list2 = ['sum(/ts=(?P<ts>\d+)/)']

        aggr_key_pattern3 = '/<k cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) [^>]*p_level=(?P<p_level>[^> ]*)[^>]*>/'
        aggr_key_expr3 = 'cls:echo(cls),type_cls:echo(게임이용시간),ground:echo(all),p_level:echo(p_level)'
    elif _cls == 'all':
        aggr_key_pattern = '/li="(?P<id>[^> ]*)" cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground="(?P<ground>[^> ]*)" p_level="(?P<p_level>\d*)" ts1="(?P<ts1>\d+)" ts2="(?P<ts2>\d+)"/'
        aggr_key_expr = 'id:echo(id),cls:echo(cls),ground:echo(ground),p_level:echo(p_level),ts:sub(ts2,ts1)'
        aggr_op_list = ['sum(1)']

        aggr_key_pattern1 = '/<k cls=(?P<cls>[^> ]*) ground=(?P<ground>[^> ]*) id=(?P<id>[^> ]*) p_level=(?P<p_level>[^> ]*) ts=(?P<ts>[^> ]*)><sum value=(?P<count>[^> ]*)>/'
        aggr_key_expr1 = 'id:echo(id),cls:echo(cls),ground:echo(ground),p_level:echo(p_level),ts:multiple(ts,count)'
        aggr_op_list1 = ['sum(1)']

        aggr_key_pattern2 = '/<k cls=(?P<cls>[^> ]*) ground=(?P<ground>[^> ]*) id=(?P<id>[^> ]*) p_level=(?P<p_level>[^> ]*)[^>]*>/'
        aggr_key_expr2 = 'id:echo(id),cls:echo(cls),ground:echo(ground),p_level:echo(p_level)'
        aggr_op_list2 = ['sum(/ts=(?P<ts>\d+)/)']

        aggr_key_pattern3 = '/<k cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground=(?P<ground>[^> ]*)[^>]*p_level=(?P<p_level>[^> ]*)[^>]*>/'
        aggr_key_expr3 = 'cls:echo(cls),type_cls:echo(게임이용시간),ground:echo(ground),p_level:echo(p_level)'
    aggr_op_list3 = ['sum(1)', 'sum(/ts=(?P<ts>\d+)/)']
  
    regex_cmd1 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format) 
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, False)
    aggr_cmd2 = make_aggr_cmd(aggr_key_pattern2, aggr_op_list2, aggr_key_expr2, False)
    aggr_cmd3 = make_aggr_cmd(aggr_key_pattern3, aggr_op_list3, aggr_key_expr3, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, aggr_cmd, aggr_cmd1, aggr_cmd2], progname, True)
    os.system(hexec_cmd)

    write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd3 )


# 7포커 최초 경기장 대기실 입장 후 대기실에서 대기한 시간
def clsWaitingTime( start_time, end_time, progname, _cls ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장_입장\"[^>]*><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장_입장"[^>]*><[^>]*p_(7p|hl|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*ts=(?P<ts1>"\d*")[^>]*>.*?<[^>]*sp="한게임/포커게임/(?P<cls>7포커|하이로우|로우바둑이|맞포커)/클라이언트/(방입장|방_입장|경기장_퇴장)"[^>]*ts=(?P<ts2>"\d+")[^>]*>'
    regex_format = 'cls=\g<cls> \g<ground> \g<p_level> ts1=\g<ts1> ts2=\g<ts2>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    if _cls == 'cls':
        aggr_key_pattern = '/li="(?P<id>[^> ]*)" cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground="(?P<ground>[^> ]*)" p_level="(?P<p_level>\d*)" ts1="(?P<ts1>\d+)" ts2="(?P<ts2>\d+)"/'
        aggr_key_expr = 'id:echo(id),cls:echo(cls),ts:sub(ts2,ts1)'
        aggr_op_list = ['sum(1)']

        aggr_key_pattern1 = '/<k cls=(?P<cls>[^> ]*) id=(?P<id>[^> ]*) ts=(?P<ts>[^> ]*)><sum value=(?P<count>[^> ]*)>/'
        aggr_key_expr1 = 'id:echo(id),cls:echo(cls),ts:multiple(ts,count)'
        aggr_op_list1 = ['sum(1)']

        aggr_key_pattern2 = '/<k cls=(?P<cls>[^> ]*) id=(?P<id>[^> ]*)[^>]*>/'
        aggr_key_expr2 = 'id:echo(id),cls:echo(cls)'
        aggr_op_list2 = ['sum(/ts=(?P<ts>\d+)/)']

        aggr_key_pattern3 = '/<k cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커)[^>]*>/'
        aggr_key_expr3 = 'cls:echo(cls),type_cls:echo(대기시간),ground:echo(all),p_level:echo(all)'
    elif _cls == 'ground':
        aggr_key_pattern = '/li="(?P<id>[^> ]*)" cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground="(?P<ground>[^> ]*)" p_level="(?P<p_level>\d*)" ts1="(?P<ts1>\d+)" ts2="(?P<ts2>\d+)"/'
        aggr_key_expr = 'id:echo(id),cls:echo(cls),ground:echo(ground),ts:sub(ts2,ts1)'
        aggr_op_list = ['sum(1)']

        aggr_key_pattern1 = '/<k cls=(?P<cls>[^> ]*) ground=(?P<ground>[^> ]*) id=(?P<id>[^> ]*) ts=(?P<ts>[^> ]*)><sum value=(?P<count>[^> ]*)>/'
        aggr_key_expr1 = 'id:echo(id),cls:echo(cls),ground:echo(ground),ts:multiple(ts,count)'
        aggr_op_list1 = ['sum(1)']

        aggr_key_pattern2 = '/<k cls=(?P<cls>[^> ]*) ground=(?P<ground>[^> ]*) id=(?P<id>[^> ]*)[^>]*>/'
        aggr_key_expr2 = 'id:echo(id),cls:echo(cls),ground:echo(ground)'
        aggr_op_list2 = ['sum(/ts=(?P<ts>\d+)/)']

        aggr_key_pattern3 = '/<k cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground=(?P<ground>[^> ]*)[^>]*>/'
        aggr_key_expr3 = 'cls:echo(cls),type_cls:echo(대기시간),ground:echo(ground),p_level:echo(all)'
    elif _cls == 'p_level':
        aggr_key_pattern = '/li="(?P<id>[^> ]*)" cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground="(?P<ground>[^> ]*)" p_level="(?P<p_level>\d*)" ts1="(?P<ts1>\d+)" ts2="(?P<ts2>\d+)"/'
        aggr_key_expr = 'id:echo(id),cls:echo(cls),p_level:echo(p_level),ts:sub(ts2,ts1)'
        aggr_op_list = ['sum(1)']

        aggr_key_pattern1 = '/<k cls=(?P<cls>[^> ]*) id=(?P<id>[^> ]*) p_level=(?P<p_level>[^> ]*) ts=(?P<ts>[^> ]*)><sum value=(?P<count>[^> ]*)>/'
        aggr_key_expr1 = 'id:echo(id),cls:echo(cls),p_level:echo(p_level),ts:multiple(ts,count)'
        aggr_op_list1 = ['sum(1)']

        aggr_key_pattern2 = '/<k cls=(?P<cls>[^> ]*) id=(?P<id>[^> ]*) p_level=(?P<p_level>[^> ]*)[^>]*>/'
        aggr_key_expr2 = 'id:echo(id),cls:echo(cls),p_level:echo(p_level)'
        aggr_op_list2 = ['sum(/ts=(?P<ts>\d+)/)']

        aggr_key_pattern3 = '/<k cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) [^>]*p_level=(?P<p_level>[^> ]*)[^>]*>/'
        aggr_key_expr3 = 'cls:echo(cls),type_cls:echo(대기시간),ground:echo(all),p_level:echo(p_level)'
    elif _cls == 'all':
        aggr_key_pattern = '/li="(?P<id>[^> ]*)" cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground="(?P<ground>[^> ]*)" p_level="(?P<p_level>\d*)" ts1="(?P<ts1>\d+)" ts2="(?P<ts2>\d+)"/'
        aggr_key_expr = 'id:echo(id),cls:echo(cls),ground:echo(ground),p_level:echo(p_level),ts:sub(ts2,ts1)'
        aggr_op_list = ['sum(1)']

        aggr_key_pattern1 = '/<k cls=(?P<cls>[^> ]*) ground=(?P<ground>[^> ]*) id=(?P<id>[^> ]*) p_level=(?P<p_level>[^> ]*) ts=(?P<ts>[^> ]*)><sum value=(?P<count>[^> ]*)>/'
        aggr_key_expr1 = 'id:echo(id),cls:echo(cls),ground:echo(ground),p_level:echo(p_level),ts:multiple(ts,count)'
        aggr_op_list1 = ['sum(1)']

        aggr_key_pattern2 = '/<k cls=(?P<cls>[^> ]*) ground=(?P<ground>[^> ]*) id=(?P<id>[^> ]*) p_level=(?P<p_level>[^> ]*)[^>]*>/'
        aggr_key_expr2 = 'id:echo(id),cls:echo(cls),ground:echo(ground),p_level:echo(p_level)'
        aggr_op_list2 = ['sum(/ts=(?P<ts>\d+)/)']

        aggr_key_pattern3 = '/<k cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground=(?P<ground>[^> ]*)[^>]*p_level=(?P<p_level>[^> ]*)[^>]*>/'
        aggr_key_expr3 = 'cls:echo(cls),type_cls:echo(대기시간),ground:echo(ground),p_level:echo(p_level)'
    aggr_op_list3 = ['sum(1)', 'sum(/ts=(?P<ts>\d+)/)']

    regex_cmd1 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, False)
    aggr_cmd2 = make_aggr_cmd(aggr_key_pattern2, aggr_op_list2, aggr_key_expr2, False)
    aggr_cmd3 = make_aggr_cmd(aggr_key_pattern3, aggr_op_list3, aggr_key_expr3, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, aggr_cmd, aggr_cmd1, aggr_cmd2], progname, True)
    os.system(hexec_cmd)
   
    write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd3 )


# 관전 시간
def clsWatchingTime( start_time, end_time, progname, _cls ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*p_(hl|7p|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*>.*?<[^>]*sp="한게임/포커게임/(?P<cls>(7포커|하이로우|로우바둑이|맞포커))/클라이언트/대기실/관전"[^>]*ts=(?P<ts1>"\d+")[^>]*>.*?<[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/(경기장대기실|방입장|방_입장)"[^>]*ts=(?P<ts2>"\d*")[^>]*>'
    regex_format = 'cls=\g<cls> \g<ground> \g<p_level> ts1=\g<ts1> ts2=\g<ts2>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    if _cls == 'cls':
        aggr_key_pattern = '/li="(?P<id>[^> ]*)" cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground="(?P<ground>[^> ]*)" p_level="(?P<p_level>\d*)" ts1="(?P<ts1>\d+)" ts2="(?P<ts2>\d+)"/'
        aggr_key_expr = 'id:echo(id),cls:echo(cls),ts:sub(ts2,ts1)'
        aggr_op_list = ['sum(1)']

        aggr_key_pattern1 = '/<k cls=(?P<cls>[^> ]*) id=(?P<id>[^> ]*) ts=(?P<ts>[^> ]*)><sum value=(?P<count>[^> ]*)>/'
        aggr_key_expr1 = 'id:echo(id),cls:echo(cls),ts:multiple(ts,count)'
        aggr_op_list1 = ['sum(1)']

        aggr_key_pattern2 = '/<k cls=(?P<cls>[^> ]*) id=(?P<id>[^> ]*)[^>]*>/'
        aggr_key_expr2 = 'id:echo(id),cls:echo(cls)'
        aggr_op_list2 = ['sum(/ts=(?P<ts>\d+)/)']

        aggr_key_pattern3 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커))[^>]*>/'
        aggr_key_expr3 = 'cls:echo(cls),type_cls:echo(관전시간),ground:echo(all),p_level:echo(all)'
    elif _cls == 'ground':
        aggr_key_pattern = '/li="(?P<id>[^> ]*)" cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground="(?P<ground>[^> ]*)" p_level="(?P<p_level>\d*)" ts1="(?P<ts1>\d+)" ts2="(?P<ts2>\d+)"/'
        aggr_key_expr = 'id:echo(id),cls:echo(cls),ground:echo(ground),ts:sub(ts2,ts1)'
        aggr_op_list = ['sum(1)']

        aggr_key_pattern1 = '/<k cls=(?P<cls>[^> ]*) ground=(?P<ground>[^> ]*) id=(?P<id>[^> ]*) ts=(?P<ts>[^> ]*)><sum value=(?P<count>[^> ]*)>/'
        aggr_key_expr1 = 'id:echo(id),cls:echo(cls),ground:echo(ground),ts:multiple(ts,count)'
        aggr_op_list1 = ['sum(1)']

        aggr_key_pattern2 = '/<k cls=(?P<cls>[^> ]*) ground=(?P<ground>[^> ]*) id=(?P<id>[^> ]*)[^>]*>/'
        aggr_key_expr2 = 'id:echo(id),cls:echo(cls),ground:echo(ground)'
        aggr_op_list2 = ['sum(/ts=(?P<ts>\d+)/)']

        aggr_key_pattern3 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) ground=(?P<ground>[^> ]*)[^>]*>/'
        aggr_key_expr3 = 'cls:echo(cls),type_cls:echo(관전시간),ground:echo(ground),p_level:echo(all)'
    elif _cls == 'p_level':
        aggr_key_pattern = '/li="(?P<id>[^> ]*)" cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground="(?P<ground>[^> ]*)" p_level="(?P<p_level>\d*)" ts1="(?P<ts1>\d+)" ts2="(?P<ts2>\d+)"/'
        aggr_key_expr = 'id:echo(id),cls:echo(cls),p_level:echo(p_level),ts:sub(ts2,ts1)'
        aggr_op_list = ['sum(1)']

        aggr_key_pattern1 = '/<k cls=(?P<cls>[^> ]*) id=(?P<id>[^> ]*) p_level=(?P<p_level>[^> ]*) ts=(?P<ts>[^> ]*)><sum value=(?P<count>[^> ]*)>/'
        aggr_key_expr1 = 'id:echo(id),cls:echo(cls),p_level:echo(p_level),ts:multiple(ts,count)'
        aggr_op_list1 = ['sum(1)']

        aggr_key_pattern2 = '/<k cls=(?P<cls>[^> ]*) id=(?P<id>[^> ]*) p_level=(?P<p_level>[^> ]*)[^>]*>/'
        aggr_key_expr2 = 'id:echo(id),cls:echo(cls),p_level:echo(p_level)'
        aggr_op_list2 = ['sum(/ts=(?P<ts>\d+)/)']

        aggr_key_pattern3 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) [^>]*p_level=(?P<p_level>[^> ]*)[^>]*>/'
        aggr_key_expr3 = 'cls:echo(cls),type_cls:echo(관전시간),ground:echo(all),p_level:echo(p_level)'
    elif _cls == 'all':
        aggr_key_pattern = '/li="(?P<id>[^> ]*)" cls=(?P<cls>7포커|하이로우|로우바둑이|맞포커) ground="(?P<ground>[^> ]*)" p_level="(?P<p_level>\d*)" ts1="(?P<ts1>\d+)" ts2="(?P<ts2>\d+)"/'
        aggr_key_expr = 'id:echo(id),cls:echo(cls),ground:echo(ground),p_level:echo(p_level),ts:sub(ts2,ts1)'
        aggr_op_list = ['sum(1)']

        aggr_key_pattern1 = '/<k cls=(?P<cls>[^> ]*) ground=(?P<ground>[^> ]*) id=(?P<id>[^> ]*) p_level=(?P<p_level>[^> ]*) ts=(?P<ts>[^> ]*)><sum value=(?P<count>[^> ]*)>/'
        aggr_key_expr1 = 'id:echo(id),cls:echo(cls),ground:echo(ground),p_level:echo(p_level),ts:multiple(ts,count)'
        aggr_op_list1 = ['sum(1)']

        aggr_key_pattern2 = '/<k cls=(?P<cls>[^> ]*) ground=(?P<ground>[^> ]*) id=(?P<id>[^> ]*) p_level=(?P<p_level>[^> ]*)[^>]*>/'
        aggr_key_expr2 = 'id:echo(id),cls:echo(cls),ground:echo(ground),p_level:echo(p_level)'
        aggr_op_list2 = ['sum(/ts=(?P<ts>\d+)/)']

        aggr_key_pattern3 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) ground=(?P<ground>[^> ]*)[^>]*p_level=(?P<p_level>[^> ]*)[^>]*>/'
        aggr_key_expr3 = 'cls:echo(cls),type_cls:echo(관전시간),ground:echo(ground),p_level:echo(p_level)'
    aggr_op_list3 = ['sum(1)', 'sum(/ts=(?P<ts>\d+)/)']

    regex_cmd1 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format) 
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, False)
    aggr_cmd2 = make_aggr_cmd(aggr_key_pattern2, aggr_op_list2, aggr_key_expr2, False)
    aggr_cmd3 = make_aggr_cmd(aggr_key_pattern3, aggr_op_list3, aggr_key_expr3, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, aggr_cmd, aggr_cmd1, aggr_cmd2], progname, True)
    os.system(hexec_cmd)
    
    write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd3 )


def write_to_file( _gexec_cmd ):

    optlist = []
    args = sys.argv[1:]
    oplist, args = getopt.getopt( args, 'd' )
    start_time, end_time = parse_timerange(args)

    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')

    for row in create_tsvreader(os.popen(_gexec_cmd,'rb')):
        new_row = {}
        new_row['날짜'] = start_time.strftime('%Y-%m-%d')
        new_row['유형구분'] = row['k.type_cls']
        new_row['게임구분'] = row['k.cls']
        new_row['경기장'] = row['k.ground']
        new_row['등급'] = row['k.p_level']
        new_row['시간'] = long(row['sum.ts'])
        new_row['인원'] = long(row['sum.value'])

        writer.writerow(new_row)


if __name__ == '__main__':
    main()
