#!/usr/bin/env python
# -*- coding: utf_8 -*-
"""
금칙어KPI_금칙어_ID그룹별_사용현황_박민수
"""

import sys, os
import time, datetime
from time import strptime
import getopt
#sys.path.insert(0,os.path.join(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib'))
from mezzo_util import *
import time, datetime
import csv


game_category = ['전체','고스톱류','포커류']


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
#    target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
#    os.system('mkdir -p ' + target_date_path)
#    outfile = open(os.path.join(target_date_path, '9_4_3_using_group.tsv'), 'wb')
    outfile = open(os.path.join('9_4_3_using_group.tsv'),'wb')

    start = time.ctime()
    print 'Start To: ', start

    abusegroup_tot = cntAbusingGroupTot(start_time, end_time, progname)                 # 금칙어 출현한 건수 - 전체 기준
    abusegroup_group = cntAbusingGroupGamegroup(start_time, end_time, progname)         # 금칙어 출현한 건수 - 게임그룹 기준

    hdrs = ['날짜','게임군','사용횟수','ID건수']

    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow(dict(zip(hdrs,hdrs)))

    temp = {}
    result = {}

    for i in range(0,3):
        for j in range(1,11):
            temp[(game_category[i],str(j))] = '0'

    # 기록
    for row  in abusegroup_tot:
        group = row['k.game_group']
        cnt_time = row['k.using_cnt']
        cnt_id = row['sum.value']
        temp[(group,cnt_time)] = cnt_id

    for row in abusegroup_group:
        group = row['k.game_group']
        cnt_time = row['k.using_cnt']
        cnt_id = row['sum.value']
        temp[(group,cnt_time)] = cnt_id

    for key_group, key_time in temp:
        result['날짜'] = start_time.strftime('%Y-%m-%d')
        result['게임군'] = key_group
        result['사용횟수'] = key_time
        result['ID건수'] = temp[(key_group,key_time)]
        writer.writerow(result)

    end = time.ctime()
    print 'End To: ', end


# 금칙어를 사용한 횟수를 기준으로 분리
def cntAbusingGroupTot( start_time, end_time, progname ):

    regex_pattern = '<[^>]*(?P<sp>sp="한게임/채팅룸/금칙어")[^>]*>'
    regex_format = '\g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" sp="(?P<sp>한게임/채팅룸/금칙어)"/'
    aggr_key_expr = 'id:echo(id),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k id=[^> ]* sp=한게임/채팅룸/금칙어><sum value=(?P<value>\d+)>/'
    aggr_key_expr1 = 'using_cnt:abuse_using_slice(value),game_group:echo(전체)'
    aggr_op_list1 = ['sum(1)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd], progname)
    print hexec_cmd
    os.system(hexec_cmd)

    sys.exit()
    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1, 'rb') )

    
def cntAbusingGroupGamegroup( start_time, end_time, progname ):
    regex_pattern = '<[^>]*game_cls="(?P<game_cls>[^> ]*)" sp="한게임/채팅룸/금칙어"[^>]*>'
    regex_format = 'game=\m<game>'
    regex_map_pattern = 'game=changing_gamelist(game_cls)'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    regex_pattern1 = r'(?P<game>li=[^> ]* game=[^>/ ]*/[^>]*)\n'
    regex_format1 = '\g<game>'

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" game=(?P<game_group>[^>/ ]*)/[^>]*/'
    aggr_key_expr = 'id:echo(id),game_group:echo(game_group)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k game_group=(?P<game_group>[^> ]*) id=[^> ]*><sum value=(?P<value>\d+)>/'
    aggr_key_expr1 = 'using_cnt:abuse_using_slice(value),game_group:echo(game_group)'
    aggr_op_list1 = ['sum(1)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format, regex_map_pattern)
    regex_cmd1 = make_regex_cmd(regex_pattern1, regex_format1)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


if __name__ == '__main__':
    main()
