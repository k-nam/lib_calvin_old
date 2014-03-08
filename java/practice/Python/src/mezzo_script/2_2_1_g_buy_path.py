#! /usr/bin/env python
# -*- coding: utf_8 -*-
"""
2_2_1 고스톱 구매경로별 매출 분석
"""

import sys,os
import time
#sys.path.insert(0, '/home1/irteam/mezzo/lib')
#sys.path.append(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib')
from mezzo_util import *


allin_node_re = '<[^>]*sp=[^> ]*/고스톱게임/신맞고/클라이언트/올인/올인팝업[^>]*>'


before_allin_re_cmd = ( 
                    '<[^>]*li=[^>]*>(?P<path>.+)' + allin_node_re
                    ,r'\g<path>'
                    ,'(<[^>]*li=[^>]*>)'
                    ,r'\1'
                    )


after_allin_re_cmd = ( '<[^>]*li=[^>]*>.+(' + allin_node_re + '(?P<path>.+)\\n)', 
                       r'\g<path>',
                       '(<[^>]*li=[^>]*>)',
                       r'\1'
                       )


goodstypes = [ 'REDAVT', 'GOSTOPITEM', 'GOSTOPAVT', 'EDPAVT', 'COMITEM', 'COMAVT', 'GOSTOPVPLUSAVT', 'GOSTOPWPLUSVAT' ]

# 구매시도건수
find_buy_avataritem_ask_cmd = {
    're_cmds' : [ ( '(?P<ask><[^>]*goodstype="((GOSTOPITEM)|(GOSTOPAVT)|(EDPAVT)|(COMITEM)|(COMAVT)|(GOSTOPVPLUSAVT)|(GOSTOPWPLUSVAT))"[^>]*'
                    'pluscode=[^>]*'
                    'sp=[^> ]*((아바타)|(아이템))/구매시작팝업[^>]*>)',
                    r'\g<ask>',
                    '(<[^>]*li=[^>]*>)',
                    r'\1'
                    )
                  ],

    'aggr_cmd' : ( '/(<[^>]*abstype="(?P<abstype>[^>"]*)"[^>]*' \
                   'docref="(?P<docref>[^>"]*)"[^>]*' \
                   'g_ispu="(?P<ispu>[^>"]*)"[^>]*' \
                   'g_level="(?P<level>[^>"]*)"[^>]*' \
                   'g_mny_bal="(?P<mny>[^>"]*)"[^>]*' \
                   'pluscode="(?P<pluscode>[^>"]*)"[^>]*>)/',
                   [ 'sum(1)' ],
                   { 'docref':'echo(docref)',
                     'ispu':'echo(ispu)',
                     'level':'echo(level)',
                     'mny':'divide_floor(5000000000,mny)',
                     'membership':'web_membership(g,abstype,pluscode)' }
                   )
    }

# 구매완료건수
find_buy_avataritem_done_cmd = {
    're_cmds' : [ ( '(?P<ask><[^>]*'
                    'abstype=[^>]*docref=[^>]*'
                    'g_ispu=[^>]*g_level=[^>]*g_mny_bal=[^>]*'
                    '(?P<goodstype>goodstype="((GOSTOPITEM)|(GOSTOPAVT)|(EDPAVT)|(COMITEM)|(COMAVT)|(GOSTOPVPLUSAVT)|(GOSTOPWPLUSVAT))")[^>]*pluscode=[^>]*sp=[^> ]*((아바타)|(아이템))/구매시작팝업[^>]*>)'
                    '(?!<[^>]*sp=[^> ]*구매시작팝업[^>]*>).*?'
                    '(?P<done><[^>]*goodstype="((GOSTOPITEM)|(GOSTOPAVT)|(EDPAVT)|(COMITEM)|(COMAVT)|(GOSTOPVPLUSAVT)|(GOSTOPWPLUSVAT))"[^>]*sp=[^> ]*((아바타)|(아이템))/구매완료팝업[^>]*>)',
                    r'\g<ask>\g<done>',
                    '(<[^>]*li=[^>]*>)',
                    r'\1'
                    )
                  ],

    'aggr_cmd' : ( '/(<[^>]*abstype="(?P<abstype>[^> ]*)"[^>]*' \
                   'docref="(?P<docref>[^> ]*)"[^>]*' \
                   'g_ispu="(?P<ispu>[^> ]*)"[^>]*' \
                   'g_level="(?P<level>[^> ]*)"[^>]*' \
                   'g_mny_bal="(?P<mny>[^> ]*)"[^>]*' \
                   'pluscode="(?P<pluscode>[^> ]*)"[^>]*sp=[^> ]*(아바타|아이템)/구매시작팝업[^>]*>)/',

                   [ 'sum(1)', 'sum(/<[^>]*><[^>]*goodsamt="(?P<goodsamt>[^> ]*)"[^>]*>/)' ],

                   { 'docref':'echo(docref)',
                     'ispu':'echo(ispu)',
                     'membership':'web_membership(g,abstype,pluscode)',
                     'level':'echo(level)',
                     'mny':'divide_floor(5000000000,mny)'
                     }
                   )
    }


def main():
    progname = os.path.basename(sys.argv[0])
    x = progname.rfind('.')
    if x > 0: progname = progname[:x]

    start_time, end_time = parse_timerange( sys.argv[1:] )

    execute_start = time.ctime()

    results = []
    results.append( find_buy_avataritem(start_time, end_time, progname) )
    results.append( find_buy_mship(start_time, end_time,progname) )

    target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
    os.system('mkdir -p ' + target_date_path)

    #outfile = sys.stdout
#    outfile = open(os.path.join(target_date_path, '2_2_1_g_buy_path.tsv'), 'wb')
    outfile = open(os.path.join('./2_2_1_g_buy_path.tsv'), 'wb')

    hdrs = ['date', 'g_ispu', 'mshiptype', 'g_level', 'g_mny_class', 'g_allin_after', 'docref', 'num_goods_popup', 'num_buy_done', 'sum_goodsamt']

    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow( dict( zip(hdrs, hdrs) ) )

    import itertools
    for k, v in itertools.chain( *[r.items() for r in results] ):
       new_row = { 'date': start_time.strftime('%Y-%m-%d') }
       new_row.update( dict(zip(hdrs[1:len(k)+1], k)) )
       new_row.update( dict(zip(hdrs[len(k)+1:], v)) )

       writer.writerow( new_row )

    execute_end = time.ctime()

    print execute_start + '\t' + execute_end + '\t' + '2_2_1_g_buy_path.py\n'

def find_buy_avataritem(start_time, end_time, progname):

    print '고스톱관련 아바타/아이템 구매내역 처리...'

    ask_result = find_beforeafter_allin(start_time, end_time, find_buy_avataritem_ask_cmd, progname, True)
    done_result = find_beforeafter_allin(start_time, end_time, find_buy_avataritem_done_cmd, progname, True)

    key_names = ['k.ispu', 'k.membership', 'k.level', 'k.mny', 'k.g_allin_after', 'k.docref']

    result = dict()
        
    for row in ask_result:
        key = tuple( get_values_from_dict(row, key_names) )
        if key not in result: result[key] = [ '0', '0', '0' ]
        result[key][0] = row['sum.value']
    
    for row in done_result:
        key = tuple( get_values_from_dict(row, key_names) )
        if key not in result: result[key] = [ '0', '0', '0' ]
        result[key][1] = row['sum.value']
        result[key][2] = row['sum.goodsamt']
    return result


def find_buy_mship(start_time, end_time, progname): 

    print '고스톱관련 회원제 가입내역 처리...'

    import tempfile
    tempfile = tempfile.NamedTemporaryFile()

    # Plus이용약관, 고스톱_W-Plus, 고스톱_V_Plus, 고스톱_R_Plus, 고스톱_M_Plus, 고스톱_A_Plus, 추천결과, 업그레이드_고스톱Plus 경로에서 회원가입한 경우
    re_cmds = []

    re_cmds.append( (
         '(?P<ask><[^>]*docref=[^> ]*((Plus_이용약관)|(고스톱_W-Plus)|(고스톱_V-Plus)|(고스톱_R-Plus)|(고스톱_M-Plus)|(고스톱_A-Plus)|(추천결과)|(업그레이드_고스톱Plus))[^>]*g_level=[^>]*sp=[^> ]*회원가입시작[^>]*>)' \
         '.*?(?P<done><[^>]*goodslist=(?P<goodslist>[^> ]*)[^>]*sp=[^> ]*회원가입완료[^>]*>)?'
        ,r'\g<ask>\g<done>'
        ,'(<[^>]*li=[^>]*>)'
        ))

    aggr_key_pattern = '/<[^>]*abstype="(?P<abstype>[^> ]*)"[^>]*docref="(?P<docref>[^>"]*)"[^>]*g_ispu="(?P<ispu>[^> ]*)"[^>]*g_level="(?P<level>[^> ]*)"[^>]*g_mny_bal="(?P<mny>[^> ]*)"[^>]*goodslist="(?P<goodslist>[^> ]*)"[^>]*pluscode="(?P<pluscode>[^ >]*)"[^>]*sp=[^> ]*회원가입시작[^>]*>' \
                       '(<[^>]*sp=[^> ]*회원가입(?P<done>완료)[^>]*>)?/'

    aggr_key_expr_dict = {
        'docref':'echo(docref)',
        'membership':'web_membership(g,abstype,pluscode)',
        'ispu':'echo(ispu)',
        'level':'echo(level)',
        'mny':'divide_floor(5000000000,mny)',
        'price':'price_membership(goodslist)',
        'done':'echo(done)'
        }

    aggr_op_list = [ 'sum(1)' ]

    notetc_cmd = { 're_cmds': re_cmds,
                   'aggr_cmd': (aggr_key_pattern, aggr_op_list, aggr_key_expr_dict)
                   }
    result_notetc = find_beforeafter_allin(start_time, end_time, notetc_cmd, progname,True)

    re_cmds[0] = ((
                '(?!<N[^>]*docref=[^> ]*((Plus_이용약관)|(고스톱_W-Plus)|(고스톱_V-Plus)|(고스톱_R-Plus)|(고스톱_M-Plus)|(고스톱_A-Plus)|(추천결과)|(업그레>이드_고스톱Plus))[^>]*g_level=[^>]*sp=[^> ]*회원가입시작[^>]*>)'
                '(?P<ask><[^>]*goodslist="(\[((GM0001)|(GM0002)|(GM0003)|(GM0004)|(GM0005)|(GO0001)|(GO0002)|(GO0003)|(GO0004)|(GO0005)|(GO0006)|(GO0007)|(GO0008)|(GO0009)),[^>\]]*\])+"[^>]*sp=[^>]*회원가입시작[^>]*>)' \
         '(?P<done><[^>]*sp=[^> ]*회원가입완료[^>]*>)?'
         ,r'\g<ask>\g<done>'
         ,'(<[^>]*li=[^>]*>)'
         ))
        
    aggr_key_pattern = '/<[^>]*abstype="(?P<abstype>[^> ]*)"[^>]*g_ispu="(?P<ispu>[^> ]*)"[^>]*g_level="(?P<level>[^> ]*)"[^>]*' \
                       'g_mny_bal="(?P<mny>[^> ]*)"[^>]*' \
                       'goodslist="(?P<goodslist>[^> ]*)"[^>]*pluscode="(?P<pluscode>[^> ]*)"[^>]*>' \
                       '(<[^>]*sp=[^> ]*회원가입(?P<done>완료)[^>]*>)?/'
    aggr_key_expr_dict['docref'] = 'echo(__etc__)'

    etc_cmd = { 're_cmds': re_cmds,
                'aggr_cmd': (aggr_key_pattern, aggr_op_list, aggr_key_expr_dict)
                }

    result_etc = find_beforeafter_allin(start_time, end_time, etc_cmd, progname, True)
    result = result_notetc  + result_etc


    key_names = ['k.ispu', 'k.membership', 'k.level', 'k.mny', 'k.g_allin_after', 'k.docref']

    result2 = dict()

    for row in result:
        key = tuple( get_values_from_dict(row, key_names) )

        if key not in result2: result2[ key ] = [ 0, 0, 0, 0 ]

        if 'k.done' in row and row[ 'k.done' ] == '완료':
            result2[key][1] = long(row['sum.value'])
            result2[key][2] = long(row['sum.goodsamt'])

        result2[key][0] += long(row['sum.value'])
        result2[key][3] += 1
    return result2 


def find_beforeafter_allin(start_time, end_time, cmd, progname, key_change = False):

    find_before_allin_cmd = cmd.copy()
    find_before_allin_cmd['re_cmds'] = [ before_allin_re_cmd ] + find_before_allin_cmd['re_cmds'][:]
    result_before_allin = list( exec_mana_cmd(start_time, end_time, find_before_allin_cmd, progname, key_change) )
    find_after_allin_cmd = cmd.copy()
    find_after_allin_cmd['re_cmds'] = [ after_allin_re_cmd ] + find_after_allin_cmd['re_cmds'][:]
    result_after_allin = list( exec_mana_cmd(start_time, end_time, find_after_allin_cmd, progname, key_change) )


    # 올인전 노드 = 전체 노드 - 올인 후 노드
    for r in result_before_allin:
        r['k.g_allin_after'] = 'N'
        for i in result_after_allin:
            if (r['k.ispu'],r['k.membership'],r['k.level'],r['k.mny'],r['k.docref']) == (i['k.ispu'],i['k.membership'],i['k.level'],i['k.mny'],i['k.docref']):
                r['sum.value'] = str(long(r['sum.value']) - long(i['sum.value']))
                if long(r['sum.value']) < 0:
                    r['sum.value'] = '0'
                if 'sum.goodsamt' in i.keys():
                    r['sum.goodsamt'] = str(long(r['sum.goodsamt']) - long(i['sum.goodsamt']))
                    if long(r['sum.goodsamt']) < 0:
                        r['sum.goodsamt'] = '0'
            if 'k.price' in i.keys():
                r['sum.goodsamt'] = str(long(r['sum.value'])*long(r['k.price']))

    for r in result_after_allin:
        r['k.g_allin_after'] = 'Y'
        if 'k.price' in r.keys():
            r['sum.goodsamt'] = str(long(r['sum.value'])*long(r['k.price']))

    return result_before_allin + result_after_allin 


def exec_mana_cmd(start_time, end_time, cmd, progname, key_change):
    re_cmds = cmd['re_cmds']
    aggr_cmd = cmd['aggr_cmd']
    
    cmd = make_hexec_cmd2(key_change, start_time, end_time, progname, re_cmds, *aggr_cmd)
    print cmd
    result = os.popen(cmd, 'rb')

    return create_tsvreader( result )


def get_values_from_dict(dictionary, keys):
    def append_value(l, k):
        if not l:
            l = []
        l.append( dictionary[k] )
        return l 

    return reduce(append_value, keys, list()) 


if __name__ == '__main__':
    main()

