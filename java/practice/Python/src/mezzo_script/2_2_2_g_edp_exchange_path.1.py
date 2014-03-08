#! /usr/bin/env python
# -*- coding: utf_8 -*-
### khsim 070827 - apply pluscode

"""
2_2_2 고스톱 엔돌핀 교환 경로분석
"""


import sys,os
#sys.path.insert(0, '/home1/irteam/mezzo/lib')
#sys.path.append(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib')
import time
from mezzo_util import *


def main():
    progname = os.path.basename(sys.argv[0])
    x = progname.rfind('.')
    if x > 0: progname = progname[:x]

    start_time, end_time = parse_timerange( sys.argv[1:] )
    
    execute_start = time.ctime()

    results = find_edpexchange(start_time, end_time, progname)

    if False:
        outfile = sys.stdout
    else:
        target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
        os.system('mkdir -p ' + target_date_path)
#        outfile = open(os.path.join(target_date_path, '2_2_2_g_edp_exchange_path.1.tsv'), 'wb')
        outfile = open(os.path.join('./', '2_2_2_g_edp_exchange_path.1.tsv'), 'wb')

    hdrs = ['date', 'mshiptype', 'g_level', 'g_mny_class', 'web', 'docref', 'num_edp', 'edp_used', 'g_mny_exchange']

    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow( dict( zip(hdrs, hdrs) ) )

    for row in results:
       new_row = {
           'date': start_time.strftime('%Y-%m-%d'),
           'mshiptype': row['k.membership'],
           'g_level': row['k.level'],
           'g_mny_class': row['k.mny'],
           'web': row['k.web'],
           'docref': row['k.docref'],
           'num_edp': row['sum.num_edp'],
           'edp_used': row['sum.edp'],
           'g_mny_exchange': row['sum.exchange']
           }
       writer.writerow( new_row )

    execute_end = time.ctime()

    print execute_start + '\t' + execute_end + '\t' + 'File creation successed : 2_2_2_g_exchange_path.1.tsv\n'

def find_edpexchange(start_time, end_time, progname):
    
    # 서비스 페이지가 엔돌핀 교환 완료인 노드들을 찾아낸다.
    re_cmds = []
    re_cmds.append( (
        '(?P<level><[^>]*g_level=[^>]*>)' +
        '((?!<[^>]*g_level=[^>]*>)<[^>]*>){0,100}?' +
        '(?P<edp><[^>]*g_mny_exchange="[^> ]*"[^>]*sp=[^> ]*엔돌핀/엔돌핀_교환_완료_[^>]*>)',
        r'\g<level>\g<edp>',
        '(<[^>]*li=[^>]*>)',
        r'\1'
        ) )

    # 서비스 페이지가 엔돌핀 교환 완료인 노드에서 등급, 회원제정보, docref정보, 보유고스톱머니 정보,
    # 웹에서의 교환인지 클라이언트에서의 교환인지에 대한 정보를 추출

    aggr_key_pattern = '/<[^>]*g_level="(?P<g_level>[^> ]*)"[^>]*>' + \
                       '<[^>]*abstype="(?P<abstype>[^> ]*)"[^>]*' + \
                       'docref="(?P<docref>[^>"]*)"[^>]*' + \
                       'g_mny_bal="(?P<mny>[^> ]*)"[^>]*' + \
                       'g_mny_exchange="(?P<exchange>[^> ]*)"[^>]*' \
                       'pluscode="(?P<pluscode>[^> ]*)"[^>]*' \
                       'sp="[^> ]*엔돌핀_교환_완료_(?P<web>[^> ]*)"[^>]*>/'
    aggr_key_expr = 'docref:echo(docref),exchange:echo(exchange),' + \
                    'g_level:echo(g_level),membership:web_membership(g,abstype,pluscode),mny:endolphin(mny),web:echo(web)'
    aggr_op_list = [ 'sum(1)', 'sum(/ edp_used="(?P<edp>[^> ]*)"[^>]* g_mny_exchange="(?P<exchange>[^> ]*)"/)' ]

    cmd_str_list = []
    cmd_str_list.append( make_regex_cmd( *re_cmds[0] ) )
    cmd_str_list.append( make_aggr_cmd( aggr_key_pattern, aggr_op_list, aggr_key_expr ) )

    hexec_cmd_str = make_hexec_cmd( start_time, end_time, cmd_str_list ,progname,True)
    os.system(hexec_cmd_str)

    reaggr_key_pattern = '/<[^>]*docref=(?P<docref>[^>]*) [^>]*' \
                         'exchange=(?P<exchange>[^> ]*)[^>]*' \
                         'g_level=(?P<g_level>[^> ]*)[^>]*' \
                         'membership=(?P<membership>[^> ]*)[^>]*' \
                         'mny=(?P<mny>[^> ]*)[^>]*' \
                         'web=(?P<web>[^> ]*)[^>]*>/'
    reaggr_key_expr = 'docref:echo(docref),level:echo(g_level),' \
                      'mny:sub_divide_floor(mny,exchange,5000000000),membership:echo(membership),web:echo(web)'
    reaggr_op_list = [ 'sum(/<[^>]*value=(?P<num_edp>[^> ]*)[^>]*><[^>]*edp=(?P<edp>[^> ]*)[^>]*exchange=(?P<exchange>[^> ]*)[^>]*>/)' ]

    cmd = 'zcat ' + progname + '.gz | ' + make_aggr_cmd( reaggr_key_pattern, reaggr_op_list, reaggr_key_expr, tsv=True )
        
    return create_tsvreader( os.popen( cmd, 'rb' ) )


if __name__ == '__main__':
    main()
