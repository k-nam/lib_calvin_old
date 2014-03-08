#! /usr/bin/env python
# -*- coding: utf_8 -*-
"""
2_2_3 고스톱 세부 상품 구매경로 분석
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

    results = []
    results.append( find_buy_mship(start_time, end_time, progname) )
 

    target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
    os.system('mkdir -p ' + target_date_path)

    #outfile = sys.stdout
#    outfile = open(os.path.join(target_date_path, './2_2_3_g_buy_path.tsv'), 'wb')
    outfile = open('./2_2_3_g_buy_path.tsv', 'wb')

    hdrs = ['date', 'g_ispu', 'mshiptype', 'g_level', 'g_mny_class', 'goodstype', 'goodslist', 'docref', 'num_buy_ask', 'num_buy_done']

    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow( dict( zip(hdrs, hdrs) ) )

    import itertools
    for k, v in itertools.chain( *[r.items() for r in results] ):
       new_row = { 'date': start_time.strftime('%Y-%m-%d') }
       new_row.update( dict(zip(hdrs[1:8], k)) )
       new_row.update( dict(zip(hdrs[8:], v)) )

       writer.writerow( new_row )

    execute_end = time.ctime()

    print execute_start + '\t' + execute_end + '\t' + 'File creation successed : 2_2_3_g_buy_path.tsv\n'

# 아바타를 구매한 내역을 조회
def find_buy_avatar(start_time, end_time, progname):
    ask_result = find_buy_avatar_ask(start_time, end_time,progname)
    done_result = find_buy_avatar_done(start_time, end_time, progname)

    key_names = ['k.ispu', 'k.membership', 'k.level', 'k.mny', 'k.goodstype', 'k.goodsid', 'k.docref']

    result = dict()

    for row in ask_result:
        key = tuple( get_values_from_dict(row, key_names) )
        if key not in result: result[key] = [ 0, 0 ]
        result[key][0] = long( row['sum.value'] )

    for row in done_result:
        key = tuple( get_values_from_dict(row, key_names) )
        if key not in result: result[key] = [ 0, 0 ]
        result[key][1] = long( row['sum.value'] )

    return result


# 아바타를 구매하기 위해 시도한 건수
def find_buy_avatar_ask(start_time, end_time, progname):
    re_cmds = []

    re_cmds.append( (
        '(?P<ask><[^>]*'
        'abstype=[^>]*docref=[^>]*'
        'g_ispu=[^>]*g_level=[^>]*g_mny_bal[^>]*'
        'goodsid=(?P<goodsid>[^> ]*)[^>]*'
        'goodstype="(GOSTOPITEM|COMITEM|GOSTOPAVT|EDPAVT|COMAVT|GOSTOPVPLUSAVT|GOSTOPWPLUSAVT)"[^>]*'
        'pluscode=[^>]*'
        'sp=[^> ]*아바타/구매시작팝업[^>]*>)',
        r'\g<ask>',
        '(<[^>]*li=[^>]*>)',
        r'\1'
        ) )

    re_cmds.append( (
        '(goodsid="|@)(?P<goods>\w+)',
        r'<f goodsid=\g<goods>>',
        '<[^>]*li=[^>]*>(?P<ask><[^>]*>)',
        r'\g<ask>'
        ) )

    aggr_key_pattern = '/<[^>]* abstype="(?P<abstype>[^> ]*)"[^>]*' \
                       ' docref="(?P<docref>[^> ]*)"[^>]*' \
                       ' g_ispu="(?P<ispu>[^> ]*)"[^>]*' \
                       ' g_level="(?P<level>[^> ]*)"[^>]*' \
                       ' g_mny_bal="(?P<mny>[^> ]*)"[^>]*' \
                       ' goodstype="(?P<goodstype>[^> ]*)"[^>]*' \
                       ' pluscode="(?P<pluscode>[^> ]*)"[^>]*>' \
                       '<f goodsid=(?P<goodsid>[^> ]*)>/'

    aggr_key_expr_dict = {
        'docref':'echo(docref)',
        'ispu':'echo(ispu)',
        'membership':'web_membership(g,abstype,pluscode)',
        'level':'echo(level)',
        'mny':'divide_floor(5000000000,mny)',
        'goodstype':'echo(goodstype)',
        'goodsid':'echo(goodsid)'
        }
    aggr_op_list = [ 'sum(1)' ]

    cmd = make_hexec_cmd2(True, start_time, end_time, progname, re_cmds, aggr_key_pattern, aggr_op_list, aggr_key_expr_dict)
    result = os.popen(cmd, 'rb' )

    return create_tsvreader( result )


# 아바타 구매를 완료한 건수
def find_buy_avatar_done(start_time, end_time, progname):
    re_cmds = []

    re_cmds.append( (
        '(?P<ask><[^>]*'
        'abstype=[^>]*docref=[^>]*'
        'g_ispu=[^>]*g_level=[^>]*g_mny_bal[^>]*'
        '(?P<goodstype>goodstype="(GOSTOPITEM|COMITEM|GOSTOPAVT|EDPAVT|COMAVT|GOSTOPVPLUSAVT|GOSTOPWPLUSAVT)")[^>]*'
        'pluscode=(?P<pluscode>[^> ]*)[^>]*'
        'sp=[^> ]*아바타/구매시작팝업[^>]*>)'
        '(?!<[^>]*구매시작팝업[^>]*>).*?'
        '(?P<done><[^>]*goodsid=(?P<goodsid>[^> ]*)[^>]*sp=[^> ]*아바타/구매완료팝업[^>]*>)',
        r'\g<ask>',
        '(<[^>]*li=[^>]*>)',
        r'\1'
        ) )

    re_cmds.append( (
        '(goodsid="|@)(?P<goods>[^@>" ]+)',
        r'<f goodsid=\g<goods>>',
        '<[^>]*li=[^>]*>(?P<ask><[^>]*>)',
        r'\g<ask>'
        ) )

    aggr_key_pattern = '/<[^>]* abstype="(?P<abstype>[^> ]*)"[^>]*' \
                       'docref="(?P<docref>[^> ]*)"[^>]*' \
                       'g_ispu="(?P<ispu>[^> ]*)"[^>]*' \
                       'g_level="(?P<level>[^> ]*)"[^>]*' \
                       'g_mny_bal="(?P<mny>[^> ]*)"[^>]*' \
                       'goodstype="(?P<goodstype>[^> ]*)"[^>]*' \
                       'pluscode="(?P<pluscode>[^> ]*)"[^>]*>' \
                       '<f goodsid=(?P<goodsid>[^> ]*)>/'

    aggr_key_expr_dict = {
        'docref':'echo(docref)',
        'ispu':'echo(ispu)',
        'membership':'web_membership(g,abstype,pluscode)',
        'level':'echo(level)',
        'mny':'divide_floor(5000000000,mny)',
        'goodstype':'echo(goodstype)',
        'goodsid':'echo(goodsid)'
        }
    aggr_op_list = [ 'sum(1)' ]

    result_hexec = make_hexec_cmd2(True, start_time, end_time,  progname, re_cmds, aggr_key_pattern, aggr_op_list, aggr_key_expr_dict)

    result = os.popen(result_hexec, 'rb' )

    return create_tsvreader( result )


# 아이템 구매
def find_buy_item(start_time, end_time,progname):
    ask_result = find_buy_item_ask(start_time, end_time, progname)
    done_result = find_buy_item_done(start_time, end_time, progname)

    key_names = ['k.ispu', 'k.membership', 'k.level', 'k.mny', 'k.goodstype', 'k.goodsid', 'k.docref']

    result = dict()


    for row in ask_result:
        key = tuple( get_values_from_dict(row, key_names) )
        if key not in result: result[key] = [ 0, 0 ]
        result[key][0] = long( row['sum.value'] )

    for row in done_result:
        key = tuple( get_values_from_dict(row, key_names) )
        if key not in result: result[key] = [ 0, 0 ]
        result[key][1] = long( row['sum.value'] )

    return result


# 아이템 구매를 시도한 건수
def find_buy_item_ask(start_time, end_time, progname):
    re_cmds = []

    re_cmds.append( (
        '(?P<ask><[^>]*'
        'abstype=[^>]*docref=[^>]*'
        'g_ispu=[^>]*g_level=[^>]*g_mny_bal[^>]*'
        'goodsid=(?P<goodsid>[^> ]*)[^>]*'
        'goodstype="(GOSTOPITEM|COMITEM|GOSTOPAVT|EDPAVT|COMAVT|GOSTOPVPLUSAVT|GOSTOPWPLUSAVT)"[^>]*'
        'pluscode=[^>]*'
        'sp=[^> ]*아이템/구매시작팝업[^>]*>)',
        r'\g<ask>',
        '(<K[^>]*>)',
        r'\1'
        ) )
        
    re_cmds.append( (
        '(goodsid="|@)(?P<goods>[^@>" ]+)',
        r'<f goodsid=\g<goods>>',
        '<[^>]*li=[^>]*>(?P<ask><[^>]*>)',
        r'\g<ask>'
        ) )

    aggr_key_pattern = '/<[^>]* abstype="(?P<abstype>[^> ]*)"[^>]*' \
                       'docref="(?P<docref>[^> ]*)"[^>]*' \
                       'g_ispu="(?P<ispu>[^> ]*)"[^>]*' \
                       'g_level="(?P<level>[^> ]*)"[^>]*' \
                       'g_mny_bal="(?P<mny>[^> ]*)"[^>]*' \
                       'goodstype="(?P<goodstype>[^> ]*)"[^>]*' \
                       'pluscode="(?P<pluscode>[^> ]*)"[^>]*>' \
                       '<f goodsid=(?P<goodsid>[^> ]*)>/'
    aggr_key_expr_dict = {
        'docref':'echo(docref)',
        'ispu':'echo(ispu)',
        'membership':'web_membership(g,abstype,pluscode)',
        'level':'echo(level)',
        'mny':'divide_floor(5000000000,mny)',
        'goodstype':'echo(goodstype)',
        'goodsid':'echo(goodsid)'
        }
    aggr_op_list = [ 'sum(1)' ]

    result_hexec = make_hexec_cmd2(True,start_time, end_time, progname, re_cmds,aggr_key_pattern, aggr_op_list, aggr_key_expr_dict)
    result =  os.popen(result_hexec, 'rb')

    return create_tsvreader( result )


# 아이템 구매를 완료한 건수
def find_buy_item_done(start_time, end_time, progname):
    re_cmds = []

    re_cmds.append( (
        '(?P<ask><[^>]*'
        'abstype=[^>]*docref=[^>]*'
        'g_ispu=[^>]*g_level=[^>]*g_mny_bal[^>]*'
        '(?P<goodstype>goodstype="(GOSTOPITEM|COMITEM|GOSTOPAVT|EDPAVT|COMAVT|GOSTOPVPLUSAVT|GOSTOPWPLUSAVT)")[^>]*'
        'pluscode=(?P<pluscode>[^> ]*)[^>]*'
        'sp=[^> ]*아이템/구매시작팝업[^>]*>)'
        '(?!<[^>]*구매시작팝업[^>]*>).*?'
        '(?P<done><C[^>]*(?P<goodscnt>goodscnt="\d*")[^>]*goodsid=(?P<goodsid>[^> ]*)[^>]*sp=[^> ]*아이템/구매완료팝업[^>]*>)',
        r'\g<ask>\g<goodscnt>',
        '(<[^>]*li=[^>]*>)',
        r'\1'
        ) )
        
    re_cmds.append( (
        '(goodsid="|@)(?P<goods>[^@>" ]+)',
        r'<f goodsid=\g<goods>>',
        '<[^>]*li=[^>]*>(?P<ask><[^>]*>)(?P<goodscnt>goodscnt="\d*")',
        r'\g<ask>\g<goodscnt>'
        ) )

    aggr_key_pattern = '/<[^>]* abstype="(?P<abstype>[^> ]*)"[^>]*' \
                       'docref="(?P<docref>[^> ]*)"[^>]*' \
                       'g_ispu="(?P<ispu>[^> ]*)"[^>]*' \
                       'g_level="(?P<level>[^> ]*)"[^>]*' \
                       'g_mny_bal="(?P<mny>[^> ]*)"[^>]*' \
                       'goodstype="(?P<goodstype>[^> ]*)"[^>]*' \
                       'pluscode="(?P<pluscode>[^> ]*)"[^>]*>' \
                       'goodscnt="(?P<goodscnt>\d*)"[^>]*'\
                       '<f goodsid=(?P<goodsid>[^> ]*)>/'

    aggr_key_expr_dict = {
        'docref':'echo(docref)',
        'ispu':'echo(ispu)',
        'membership':'web_membership(g,abstype,pluscode)',
        'level':'echo(level)',
        'mny':'divide_floor(5000000000,mny)',
        'goodstype':'echo(goodstype)',
        'goodsid':'echo(goodsid)'
        }
    aggr_op_list = [ 'sum(/goodscnt="(?P<value>\d*)"/)' ]
    
    result_hexec = make_hexec_cmd2(True, start_time, end_time, progname, re_cmds,aggr_key_pattern, aggr_op_list, aggr_key_expr_dict)
    result = os.popen(result_hexec, 'rb')
    return create_tsvreader( result )


# 신규 회원제에 가입한 건수
def find_buy_mship(start_time, end_time, progname):
    import tempfile
    tempfile = tempfile.NamedTemporaryFile()

    re_cmds = []

    re_cmds.append( (
         '(?P<ask><[^>]*docref=[^>]*((Plus_이용약관)|(고스톱_W-Plus)|(고스톱_V-Plus)|(고스톱_R-Plus)|(고스톱_M-Plus)|(고스톱_A-Plus)|(추천결과)|(업그레이드_고스톱Plus))[^>]*' \
         'g_level=[^>]*(?P<goodsid>goodslist="(\[((GM0001)|(GM0002)|(GM0003)|(GM0004)|(GM0005)|(GO0001)|(GO0002)|(GO0003)|(GO0004)|(GO0005)|(GO0006)|(GO0007)|(GO0008)|(GO0009)),[^>\]]*\])+")[^>]*sp=[^> ]*회원가입시작[^>]*>)' \
         '(?!<[^>]*회원가입시작[^>]*>).*?' \
         '(<[^>]*(?P<done>sp=[^> ]*회원가입완료[^>]*)>)?'
        ,r'\g<ask>\g<done>'
        ,'(<[^>]*li=[^>]*>)'
        ) )

    re_cmds.append( (
         '(?P<goodslist>(GM|GO)[^>\[\]]+),'
        ,'<goodsid=\g<goodslist>>'
        ,'(?P<ask><[^>]*>)((?P<done>sp=[^> ]*회원가입완료")|)'
        ,'\g<ask>\g<done>'
        ) )

    aggr_key_pattern = '/<[^>]*abstype="(?P<abstype>[^> ]*)"[^>]*docref="(?P<docref>[^> ]*)"[^>]*g_ispu="(?P<ispu>[^> ]*)"[^>]*g_level="(?P<level>[^> ]*)"[^>]*g_mny_bal="(?P<mny>[^> ]*)"[^>]*pluscode="(?P<pluscode>[^> ]*)"[^>]*>' \
                       '(sp="[^> ]*회원가입(?P<done>완료)")?<goodsid=(?P<goodsid>[^> ]*)[^>]*>/'

    aggr_key_expr_dict = {
        'docref':'echo(docref)',
        'membership':'web_membership(g,abstype,pluscode)',
        'ispu':'echo(ispu)',
        'level':'echo(level)',
        'mny':'divide_floor(5000000000,mny)',
        'goodsid':'echo(goodsid)',
        'goodstype':'echo(MSHIP)',
        'done':'echo(done)'
        }
    aggr_op_list = [ 'sum(1)' ]

    cmd = make_hexec_cmd(
          start_time, end_time,
         [ make_regex_cmd(*re_cmds[0]),
           make_regex_cmd(*re_cmds[1]),
           make_aggr_cmd(aggr_key_pattern, aggr_op_list, ','.join([k + ':' + v for k, v in aggr_key_expr_dict.items()]))
          ], progname, True)

    os.system(cmd)
    os.system('zcat ' + progname + '.gz | cut -d " " -f 2- > ' + tempfile.name )

    re_cmds[0] = ((
         '(?!<[^>]*docref=[^> ]*((Plus_이용약관)|(고스톱_W-Plus)|(고스톱_V-Plus)|(고스톱_R-Plus)|(고스톱_M-Plus)|(고스톱_A-Plus)|(추천결과)|(업그레이드_고스톱Plus))[^>]*g_level=[^>]*sp=[^> ]*회원가입시작[^>]*>)'
         '(?P<ask><[^>]*(?P<goodsid>goodslist="(\[((GM0001)|(GM0002)|(GM0003)|(GM0004)|(GM0005)|(GO0001)|(GO0002)|(GO0003)|(GO0004)|(GO0005)|(GO0006)|(GO0007)|(GO0008)|(GO0009)),[^>\]]*\])+")[^>]*sp="[^>]*회원가입시작[^>]*>)'
         '(?!<[^>]*회원가입시작[^>]*>).*?'
         '(<[^>]*(?P<done>sp=[^> ]*회원가입완료[^>]*)>)?'
        ,r'\g<ask>\g<done>'
        ,'(<[^>]*li=[^>]*>)'
        ) )

    re_cmds[1] = ( (
         '(?P<goodslist>(GM|GO)[^>\[\]]+),'
        ,'<goodsid=\g<goodslist>>'
        ,'(?P<ask><[^>]*>)((?!<goodslist=[^>]*>)(?P<done>sp=[^> ]*회원가입완료)|)'
        ,'\g<ask>\g<done>'
        ) )

    aggr_key_pattern = '/<[^>]*abstype="(?P<abstype>[^> ]*)"[^>]*g_ispu="(?P<ispu>[^> ]*)"[^>]*g_level="(?P<level>[^> ]*)"[^>]*' \
                       'g_mny_bal="(?P<mny>[^> ]*)"[^>]*' \
                       'goodslist="(?P<goodslist>[^> ]*)"[^>]*pluscode="(?P<pluscode>[^> ]*)"[^>]*>' \
                       '(sp="[^> ]*회원가입(?P<done>완료)[^>]*)?<goodsid=(?P<goodsid>[^> ]*)[^>]*>/'
    
    aggr_key_expr_dict['docref'] = 'echo(__etc__)'

    ##by hwangpd
    cmd = make_hexec_cmd(
            start_time, end_time, 
            [ make_regex_cmd(*re_cmds[0]),
            make_regex_cmd(*re_cmds[1]),
            make_aggr_cmd(aggr_key_pattern, aggr_op_list, ','.join([k + ':' + v for k, v in aggr_key_expr_dict.items()]))
            ], progname, True)
    os.system(cmd)
    os.system('zcat ' + progname + '.gz | cut -d " " -f 2- >> ' + tempfile.name )

    aggr_key_pattern = '/[^>]*docref=(?P<docref>[^> ]*)[^>]*' \
                       'done=(?P<done>[^> ]*)[^>]*' \
                       'goodsid=(?P<goodsid>[^> ]*)[^>]*' \
                       'goodstype=(?P<goodstype>[^> ]*)[^>]*' \
                       'ispu=(?P<ispu>[^> ]*)[^>]*' \
                       'level=(?P<level>[^> ]*)[^>]*' \
                       'membership=(?P<membership>[^> ]*)[^>]*' \
                       'mny=(?P<mny>[^> ]*)[^>]*>/'
    
    aggr_key_expr_dict['docref'] = 'echo(docref)'
    aggr_key_expr_dict['goodstype'] = 'echo(goodstype)'
    aggr_key_expr_dict['membership'] = 'echo(membership)'
    aggr_key_expr_dict['mny'] = 'echo(mny)'
    aggr_key_expr = ','.join([k + ':' + v for k, v in aggr_key_expr_dict.items()])
    aggr_op_list = [ 'sum(/ value=(?P<value>[^> ]*)/)' ]

    result = create_tsvreader(
        os.popen( 'PATH=/home1/irteam/mezzo/bin:$PATH; ' +
                  make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, tsv=True) + ' < ' + tempfile.name, 'rb' ) )

    key_names = ['k.ispu', 'k.membership', 'k.level', 'k.mny', 'k.goodstype', 'k.goodsid', 'k.docref']

    result2 = dict()

    for row in result:
        key = tuple( get_values_from_dict(row, key_names) )

        if key not in result2: result2[ key ] = [ 0, 0, 0 ]

        if 'k.done' in row and row[ 'k.done' ] == '완료':
            result2[key][1] = long(row['sum.value'])

        result2[key][0] += long(row['sum.value'])
        result2[key][2] += 1

    return result2 


def get_values_from_dict(dictionary, keys):
    def append_value(l, k):
        if not l:
            l = []
        l.append( dictionary[k] )
        return l 

    return reduce(append_value, keys, list())


if __name__ == '__main__':
    main()

