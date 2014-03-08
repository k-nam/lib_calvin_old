#! /usr/bin/env python
# -*- coding: utf_8 -*-

"""
2_2_2 고스톱 엔돌핀 교환 경로분석
"""


import sys,os
import time
#sys.path.insert(0, '/home1/irteam/mezzo/lib')
#sys.path.append(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib')
from mezzo_util import *


def main():
    progname = os.path.basename(sys.argv[0])
    x = progname.rfind('.')
    if x > 0: progname = progname[:x]

    start_time, end_time = parse_timerange( sys.argv[1:] )

    execute_start = time.ctime()

    idcount = find_idcount(start_time, end_time, progname)

    if False:
        outfile = sys.stdout
    else:
        target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
        os.system('mkdir -p ' + target_date_path)
#        outfile = open(os.path.join(target_date_path, '2_2_2_g_edp_exchange_path.2.tsv'), 'wb')
        outfile = open(os.path.join('./', '2_2_2_g_edp_exchange_path.2.tsv'), 'wb')

    hdrs = ['date', 'mshiptype', 'g_level', 'g_mny_class', 'web', 'docref', 'num_ids']

    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow( dict( zip(hdrs, hdrs) ) )

    for row in idcount:
       new_row = {
           'date': start_time.strftime('%Y-%m-%d'),
           'mshiptype': row['k.membership'],
           'g_level': row['k.level'],
           'g_mny_class': row['k.mny'],
           'web': row['k.web'],
           'docref': row['k.docref'],
           'num_ids': row['sum.value']
           }
       writer.writerow( new_row )

    execute_end = time.ctime()

    print execute_start + '\t' + execute_end + '\t' + 'File creation successed : 2_2_g_edp_exchange_path.2.tsv\n'


def find_idcount(start_time, end_time, progname):
    re_cmds = []
    re_cmds.append( (
        '(?P<level><[^>]*g_level=[^>]*>)' +
        '((?!<[^>]*g_level=[^>]*>)<[^>]*>){0,100}?' +
        '(?P<edp><[^>]*g_mny_exchange=[^>]*sp=[^> ]*엔돌핀/엔돌핀_교환_완료_[^>]*>)',
        r'\g<level>\g<edp>',

        '^(<[^>]*li=[^>]*>)',
        r'\1'
        ) )

    aggr_key_pattern = '/<[^>]*li="(?P<id>[^> ]*)"[^>]*>' \
                       '<[^>]*g_level="(?P<g_level>[^> ]*)"[^>]*>' + \
                       '<[^>]*abstype="(?P<abstype>[^> ]*)"[^>]*' + \
                       'docref="(?P<docref>[^">]*)"[^>]*' + \
                       'g_mny_bal="(?P<mny>[^> ]*)"[^>]*' + \
                       'g_mny_exchange="(?P<exchange>[^> ]*)"[^>]*' \
                       'pluscode="(?P<pluscode>[^> ]*)"[^>]*' + \
                       'sp="[^> ]*엔돌핀_교환_완료_(?P<web>[^> ]*)"[^>]*>/'
    aggr_key_expr = 'id:echo(id),docref:echo(docref),exchange:echo(exchange),' + \
                    'g_level:echo(g_level),mny:endolphin(mny),membership:web_membership(g,abstype,pluscode),web:echo(web)'
    aggr_op_list = [ 'sum(1)' ]

    cmd_str_list = []
    cmd_str_list.append( make_regex_cmd( *re_cmds[0] ) )
    cmd_str_list.append( make_aggr_cmd( aggr_key_pattern, aggr_op_list, aggr_key_expr ) )

    gexec_cmd_str = make_hexec_cmd( start_time, end_time, cmd_str_list, progname, True )
    os.system(gexec_cmd_str) 

    import tempfile
    temp_file = tempfile.NamedTemporaryFile()
    os.system ( 'zcat ' + progname + '.gz > ' + temp_file.name )

    reaggr_key_pattern = '/<[^>]* docref=(?P<docref>[^>]*) [^>]*' \
                         'exchange=(?P<exchange>[^> ]*)[^>]*' \
                         'g_level=(?P<g_level>[^> ]*)[^>]*' \
                         'id=(?P<id>[^> ]*)[^>]*' \
                         'membership=(?P<membership>[^> ]*)[^>]*' \
                         'mny=(?P<mny>[^> ]*)[^>]*' \
                         'web=(?P<web>[^> ]*)[^>]*>/'
    reaggr_key_expr_list = ['id:echo(id)',
                            'docref:echo(docref)',
                            'level:echo(g_level)',
                            'membership:echo(membership)',
                            'mny:sub_divide_floor(mny,exchange,5000000000)',
                            'web:echo(web)']
    reaggr_op_list = [ 'sum(1)' ]

    reaggr_key_pattern2 = '/<[^>]*docref=(?P<docref>[^> ]*)[^>]*' \
                          'level=(?P<level>[^> ]*)[^>]*' \
                          'membership=(?P<membership>[^> ]*)[^>]*' \
                          'mny=(?P<mny>[^> ]*)[^>]*' \
                          'web=(?P<web>[^> ]*)[^>]*>/'
    reaggr_op_list2 = [ 'sum(1)' ]

    # 엔돌핀 교환완료 내역중에서 각 컬럼별(mshiptype, g_level, g_mny_class, web, docref, num_ids)로 전체라는 내용을 포함한 데이터를 추출

    gen_list = []
    for i in combination(reaggr_key_expr_list[1:], ignore):
        cmds = [ make_aggr_cmd( reaggr_key_pattern, reaggr_op_list, ','.join([reaggr_key_expr_list[0]] + i), tsv=False ) + ' < ' + temp_file.name,
                 make_aggr_cmd( reaggr_key_pattern2, reaggr_op_list2, tsv=True ) ]
        gen_list.append( create_tsvreader( os.popen( 'PATH=/home1/irteam/mezzo/bin:$PATH; ' + ' | '.join(cmds), 'rb' ) ) )

    import itertools
    return itertools.chain(*gen_list)


if __name__ == '__main__':
    main()
