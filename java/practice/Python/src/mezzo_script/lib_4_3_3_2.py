#!/usr/bin/env python
# -*- coding: utf_8 -*-

import sys, os
import math, time
import getopt
#sys.path.insert(0, './lib')
#sys.path.append(os.getenv('MEZZO_HOME', '/home1/irteam/minsu') + '/scripts/lib')
from mezzo_util import *
from time import strptime

def usage(progname):
    print >> sys.stderr, "\nUsage: " + progname + "\t[-d] [-n <# of days] [<date string>]\n"
    print >> sys.stderr, "\t\t-d\tDebug mode"
    print >> sys.stderr, "\t\t-n\tTime range of analysis (unit: day)"
    print >> sys.stderr, "\t\t<date string>\tIf omitted, the basic date is yesterday as default\n"
    

def nineteenth_1_new(progname, debug_mode=False, num_days=30):

    optlist = []
    args = sys.argv[1:]
    try:
        optlist, args = getopt.getopt(args, 'dn:')
    except getopt.GetoptError:
        usage(sys.argv[0])
        sys.exit(2)
    for o, a in optlist:
        if (o == "-d"):
            debug_mode = True
        elif (o == '-n'):
            num_days = int(a)

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
                
    # "엔돌핀_교환_완료_*" 또는 "아이템사용완료" 또는 "7포커/클라이언트/올인/올인팝업" 노드를 기준으로 split
    regex_pattern0 = '(<[^>]*edp_used="\d+"[^>]*p_mny_exchange="\d+"[^>]*sp=[^> ]*/엔돌핀_교환_완료_(웹|클라)[^>]*>|<[^>]*p_mny_bal="\d+"[^>]*p_mny_usegain="\d+"[^>]*sp=[^> ]*/아이템/아이템사용완료[^>]*>|<[^>]*sp=[^> ]*/7포커/클라이언트/올인/올인팝업[^>]*>)'
    regex_format0 = r'\1'

    # "엔돌핀_교환_완료_*" 또는 "아이템사용완료"로 시작하고 "올인팝업"으로 끝나는 노드의 연속만 추출
    regex_pattern1 = '<[^>]*sp=[^> ]*/(엔돌핀_교환_완료_(웹|클라)|아이템/아이템사용완료)[^>]*ts="(?P<start_ts>\d+)"[^>]*>.*?<[^>]*goodslist=(?P<goodslist>[^> ]*)[^>]*p_level="(?P<p_level>\d+)"[^>]*sp=[^>]*/7포커/클라이언트/올인/올인팝업[^>]*ts="(?P<end_ts>\d+)"[^>]*>'
    regex_format1 = 'goodslist=\g<goodslist> p_level=\g<p_level> \g<start_ts> \g<end_ts>'

    # 오늘 날짜의 ts를 가진 올인팝업을 추출
    end_day_6am_ts = time.mktime(end_datetime.replace(hour=06, minute=0, second=0, microsecond=0).timetuple())
    day_before_end_day_6am_ts = end_day_6am_ts - 3600 * 24
    awk_cmd1 = r'awk "BEGIN { FS=\" \"; OFS=\" \" } { if (' + str(day_before_end_day_6am_ts) + ' <= \$4 && \$4 < ' + str(end_day_6am_ts) + ') { print \$1, \$2, \\"duration=\\"\$4-\$3 } }"'

    aggr_key_pattern = '/goodslist="(?P<goodslist>[^> ]*)"[^>]*p_level=(?P<p_level>\d+)/'
    aggr_key_expr = 'membership:client_membership_type(goodslist),p_level:echo(p_level)'
    aggr_op_list = [ 'sum(1)', 'sum(/duration=(?P<duration>\d+)/)' ]

    re_aggr_key_pattern = '/membership=(?P<membership>[^> ]*)[^>]*p_level=(?P<p_level>\d+)/'
    re_aggr_op_list = [ 'sum(/value=(?P<value>\d+)/)', 'sum(/duration=(?P<duration>\d+)/)' ]
    
    draw_cmd = make_draw_cmd(start_datetime, end_datetime)
    args = []
    # 첫번째 regex는 split mode로 실행시켜야 recursion limit를 피할 수 있음
    args.extend([dbl_quote(regex_pattern0), dbl_quote(regex_format0)])
    regex_cmd0 = 'regex -s ' + ' '.join(args)
    regex_cmd1 = make_regex_cmd(regex_pattern1, regex_format1)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr)
    # 마지막 aggr은 tsv 포맷으로 출력해야 함
    re_aggr_cmd = make_aggr_cmd(re_aggr_key_pattern, re_aggr_op_list, None, True)
    
    hexec_cmd = make_hexec_cmd_month(start_datetime, end_datetime, [regex_cmd0, regex_cmd1, awk_cmd1, aggr_cmd],progname,True)
    os.system(hexec_cmd)

    cmd = 'zcat ' + progname + '.gz | ' + re_aggr_cmd
    
    if (debug_mode):
        print cmd

    hash_table = {}
    result = os.popen(cmd, 'rb')

    total_duration = {}
    total_num_events= {}

    for row in create_tsvreader(result):
        num_events = float(row['sum.value'])
        key = (row['k.membership'], row['k.p_level'])
        value = float(row['sum.duration']) / num_events
        hash_table[key] = value

        # 전체 회원제의 평균 올인 일수를 구하기 위한 Logic
        if ('__all__',row['k.p_level']) in total_duration.keys():
            total_duration['__all__',row['k.p_level']] = float(total_duration['__all__',row['k.p_level']]) + float(row['sum.duration'])
            total_num_events['__all__',row['k.p_level']] = float(total_num_events['__all__',row['k.p_level']]) + num_events
        else:
            total_duration['__all__',row['k.p_level']] = float(row['sum.duration'])
            total_num_events['__all__',row['k.p_level']] = num_events
 
        if (debug_mode):
            print key, value

    for temp_keys in total_duration.keys():
        hash_table[temp_keys] = float(total_duration[temp_keys]) / float(total_num_events[temp_keys])

    return hash_table

if __name__ == '__main__':
    nineteenth_1_new(False)
