#!/usr/bin/env python
# -*- coding: utf_8 -*-

"""
4_3_2 7포커 올인시 회원행동패턴 분석 (신규 PLUS회원제를 가입한 내역에 대한 분석)
"""

import sys, os
import getopt
import time
#sys.path.insert(0, os.path.join(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib'))
from mezzo_util import *

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

    # 올인팝업부터 회원가입완료까지 longest match
    sevenpoker_allin_popup_p = '(<[^>]*goodslist=(?P<goodslist>[^> ]*)[^>]*p_ispu=(?P<p_ispu>[^> ]*)[^>]*p_level=(?P<p_level>"\d+")[^>]*sp=[^> ]*/7포커/클라이언트/올인/올인팝업[^>]*>)'

    regex_pattern = '(<[^>]*sp=[^> ]*/7포커/클라이언트/올인/올인팝업[^>]*>|<[^>]*sp=[^> ]*/7포커/클라이언트/방_입장[^>]*>)'
    regex_format = r'\1'

    regex_pattern1 = '(?P<after_allin>' + sevenpoker_allin_popup_p + '.*)'
    regex_format1 = r'\g<after_allin>'

    # 상품정보 추출
    regex_pattern2 = '<[^>]*goodslist=(?P<join>"(\[((PM0001)|(PM0002)|(PM0003)|(PM0004)|(PM0005)|(PO0001)|(PO0002)|(PO0003)|(PO0004)|(PO0005)|(PO0006)|(PO0007)|(PO0008)|(PO0009)),[^>\]]*\])+")[^>]*sp=[^> ]*회원가입시작[^>]*>(?!<[^>]*회원가입시작[^>]*>).*?<[^>]*sp=[^> ]*회원가입완료[^>]*>'
    regex_format2 = r'goodslist_new=\g<join>'
    regex_key_pattern2 = '(<[^>]*goodslist=(?P<goodslist>[^> ]*)[^>]*p_ispu=(?P<p_ispu>[^> ]*)[^>]*p_level=(?P<p_level>"\d+")[^>]*sp=[^> ]*/7포커/클라이언트/올인/올인팝업[^>]*>)'
    regex_key_format2 = r'goodslist=\g<goodslist> p_ispu=\g<p_ispu> p_level=\g<p_level> '

    regex_pattern3 = 'goodslist_new=.*(?P<goodslist_new>(PM|PO)[^">\[\]]+),'
    regex_format3 = r'goodslist_new=\g<goodslist_new>'
    regex_key_pattern3 = 'goodslist=(?P<goodslist>[^> ]*)[^>]*p_ispu=(?P<p_ispu>[^> ]*)[^>]*p_level=(?P<p_level>"\d+")'
    regex_key_format3 = r'goodslist=\g<goodslist> p_ispu=\g<p_ispu> p_level=\g<p_level> '

    aggr_key_pattern = '/goodslist="(?P<goodslist>[^> ]*)"[^>]*p_ispu="(?P<p_ispu>[^> ]*)"[^>]*p_level="(?P<p_level>\d+)"[^>]*goodslist_new=(?P<goodslist_new>[^> ]*)/'
    aggr_key_expr = 'membership:client_membership_type(goodslist),p_ispu:echo(p_ispu),p_level:echo(p_level),goodslist_new:echo(goodslist_new)'
    aggr_op_list = [ 'sum(1)' ]

    re_aggr_key_pattern = '/goodslist_new=(?P<goodslist_new>[^> ]*) membership=(?P<membership>[^> ]*) p_ispu=(?P<p_ispu>[^> ]*) p_level=(?P<p_level>[^> ]*)/'
    re_aggr_op_list = ['sum(/value=(?P<value>\d+)/)']

    if (debug_mode):
        target_date_path = os.path.join(target_path + ".old", start_time.strftime('%Y/%m/%d'))
    else:
        target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
    os.system('mkdir -p ' + target_date_path)

#    outfile = open(os.path.join(target_date_path, '4_3_2_p_7p_allin_user_behavior_pattern.2.tsv'), 'wb')
    outfile = open(os.path.join('./4_3_2_p_7p_allin_user_behavior_pattern.2.tsv'), 'wb')

    hdrs = ['date', 'p_level', 'p_ispu', 'mshiptype', 'mshiptype_new', 'num_actions']
    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect = 'excel-tab')
    writer.writerow(dict(zip(hdrs, hdrs)))

    draw_cmd = make_draw_cmd(start_time, end_time)
    args = []
    args.extend([dbl_quote(regex_pattern), dbl_quote(regex_format)])
    regex_cmd = 'regex -s ' + ' '.join(args)
    regex_cmd1 = make_regex_cmd(regex_pattern1, regex_format1)
    regex_cmd2 = make_regex_cmd(regex_pattern2, regex_format2, regex_key_pattern2, regex_key_format2)
    regex_cmd3 = make_regex_cmd(regex_pattern3, regex_format3, regex_key_pattern3, regex_key_format3)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr)
    re_aggr_cmd = make_aggr_cmd(re_aggr_key_pattern, re_aggr_op_list, None, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, regex_cmd2, regex_cmd3, aggr_cmd],progname)
    
    os.system(hexec_cmd)
    cmd = 'zcat ' + progname + '.gz | ' + re_aggr_cmd

    result = os.popen(cmd, 'rb')

    hash_table = {}
    for row in create_tsvreader(result):
        key = (row['k.p_level'], row['k.p_ispu'], row['k.membership'], row['k.goodslist_new'])
        value = row['sum.value']
        hash_table[key] = value
        
        new_row = {'date': start_time.strftime('%Y-%m-%d'),  'p_level': key[0], 'p_ispu': key[1], 'mshiptype': key[2], 'mshiptype_new': key[3], 'num_actions': value}
        writer.writerow(new_row)

        if (debug_mode):
            print new_row
    
    execute_end = time.ctime()

    print execute_start + '\t' + execute_end + '\t' + 'File creation successed : 4_3_2_p_7p_allin_behavior_pattern.2.tsv\n'
            
if __name__ == '__main__':
    main()
