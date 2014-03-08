#!/usr/bin/env python
# -*- coding: utf_8 -*-

import sys, os
import getopt
sys.path.insert(0, './lib')
sys.path.append(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib')
from mezzo_util import *

def usage(progname):
    print >> sys.stderr, "\nUsage: " + progname + "\t[-d] [<date string>]\n"
    print >> sys.stderr, "\t\t-d\tDebug mode\n\n"

def eighteenth_3(progname, debug_mode):
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

    # 올인팝업부터 포커머니_무료충전까지 longest match
    sevenpoker_allin_popup_p = '<[^>]*goodslist=(?P<goodslist>[^> ]*) p_ispu=(?P<p_ispu>[^> ]*) p_level=(?P<p_level>"\d+")[^>]*sp="한게임/포커게임/7포커/클라이언트/올인/올인팝업"[^>]*>'

    regex_pattern = '(<[^>]*sp="한게임/포커게임/7포커/클라이언트/올인/올인팝업"[^>]*>|<[^>]*sp="한게임/포커게임/7포커/클라이언트/방_입장"[^>]*>)'
    regex_format = r'\1'

    regex_pattern1 = '(?P<after_allin>' + sevenpoker_allin_popup_p + '.+)'
    regex_format1 = r'\g<after_allin>'

    # 무료충전이 발생한 경우
    charge_regex_pattern2 = '<[^>]*sp="한게임/포커게임/7포커/클라이언트/올인/올인팝업"[^>]*>.*?<[^>]*p_mny_recharge=(?P<p_mny_recharge>"\d+")[^>]*sp=(?P<sp>[^> ]*/웹/포커머니_무료충전")[^>]*>'
    charge_regex_format2 = r'sp=\g<sp>'
    charge_regex_key_pattern2 = '<[^>]*goodslist=(?P<goodslist>[^> ]*)[^>]*p_ispu=(?P<p_ispu>[^> ]*)[^>]*' \
                         'p_level=(?P<p_level>"\d+")[^>]*sp=[^> ]*/7포커/클라이언트/올인/올인팝업[^>]*>'
    charge_regex_key_format2 = r'goodslist=\g<goodslist> p_ispu=\g<p_ispu> p_level=\g<p_level> '

    charge_aggr_key_pattern = '/goodslist="(?P<goodslist>[^> ]*)" p_ispu="(?P<p_ispu>[^> ]*)" p_level="(?P<p_level>\d+)" sp="(?P<sp>[^> ]*)"/'
    charge_aggr_key_expr = 'membership:client_membership_type(goodslist),p_ispu:echo(p_ispu),p_level:echo(p_level),sp:echo(sp)'
    charge_aggr_op_list = [ 'sum(1)' ]

    charge_re_aggr_key_pattern = '/membership=(?P<membership>[^> ]*) p_ispu=(?P<p_ispu>[^> ]*) p_level=(?P<p_level>[^> ]*) sp=(?P<sp>[^> ]*)/'
    charge_re_aggr_op_list = [ 'sum(/value=(?P<value>\d+)/)' ]


    # 로그아웃을 한 경우 (로그아웃 기록이 없을 경우 -> 로그인_웹으로 대체가능)
    logout_regex_pattern2 = '<[^>]*sp="한게임/포커게임/7포커/클라이언트/올인/올인팝업"[^>]*>.*?<[^>]*sp=(?P<sp>[^> ]*/(로그아웃|로그인_웹)[^> ]*)[^>]*>'
    logout_regex_format2 = r'sp=\g<sp>'
    logout_regex_key_pattern2 = '(<[^>]*goodslist=(?P<goodslist>[^> ]*)[^>]*p_ispu=(?P<p_ispu>[^> ]*)[^>]*' \
                               'p_level=(?P<p_level>"\d+")[^>]*sp=[^> ]*/7포커/클라이언트/올인/올인팝업[^>]*>)'
    logout_regex_key_format2 = r'goodslist=\g<goodslist> p_ispu=\g<p_ispu> p_level=\g<p_level> '

    logout_aggr_key_pattern = '/goodslist="(?P<goodslist>[^> ]*)" p_ispu="(?P<p_ispu>[^> ]*)" p_level="(?P<p_level>\d+)"' \
                              ' sp="(?P<sp>[^> ]*)"/'
    logout_aggr_key_expr = 'membership:client_membership_type(goodslist),p_ispu:echo(p_ispu),p_level:echo(p_level),sp:echo(sp)'
    logout_aggr_op_list = [ 'sum(1)' ]

    logout_re_aggr_key_pattern = '/membership=(?P<membership>[^> ]*) p_ispu=(?P<p_ispu>[^> ]*) p_level=(?P<p_level>[^> ]*) sp=(?P<sp>[^> ]*)/'
    logout_re_aggr_op_list = ['sum(/value=(?P<value>\d+)/)']


    # 기타 행동에 대한 내용을 추출
    etc_regex_pattern2 = '(<[^>]*goodslist=(?P<goodslist>[^> ]*)[^>]*p_ispu=(?P<p_ispu>[^> ]*)[^>]*' \
                         'p_level=(?P<p_level>"\d+")[^>]*sp="한게임/포커게임/7포커/클라이언트/올인/올인팝업"[^>]*>)((?!<[^>]*sp="한게임/((로그아웃)|(로그인_웹)|(아바타/구매시작팝업)|(아이템/구매시작팝업)|(Plus회원/회원가입시작)|(엔돌핀/엔돌핀_교환_완료_웹)|([^> ]*/웹/포커머니_무료충전))"[^>]*>)<[^>]*>)*\n'
    etc_regex_format = 'goodslist=\g<goodslist> p_ispu=\g<p_ispu> p_level=\g<p_level>'
    etc_aggr_key_pattern = '/goodslist="(?P<goodslist>[^> ]*)" p_ispu="(?P<p_ispu>[^> ]*)" p_level="(?P<p_level>\d+)"/'
    etc_aggr_key_expr = 'membership:client_membership_type(goodslist),p_ispu:echo(p_ispu),p_level:echo(p_level)'
    etc_aggr_op_list = [ 'sum(1)' ]

    etc_re_aggr_key_pattern= '/membership=(?P<membership>[^> ]*) p_ispu=(?P<p_ispu>[^> ]*) p_level=(?P<p_level>[^> ]*)/'
    etc_re_aggr_op_list = ['sum(/value=(?P<value>\d+)/)']

    # draw 및 regex를 생성 (regex -s를 사용한 이유는 Collision이 발생하는것을 막기위해서)
    draw_cmd = make_draw_cmd(start_time, end_time) #orginal
    agrs = []
    agrs.extend([dbl_quote(regex_pattern), dbl_quote(regex_format)])
    regex_cmd = 'regex -s ' + ' '.join(agrs)
    regex_cmd1 = make_regex_cmd(regex_pattern1, regex_format1)

    # 무료충전이 발생한 경우
    charge_regex_cmd2 = make_regex_cmd(charge_regex_pattern2, charge_regex_format2, charge_regex_key_pattern2, charge_regex_key_format2)
    charge_aggr_cmd = make_aggr_cmd(charge_aggr_key_pattern, charge_aggr_op_list, charge_aggr_key_expr)
    charge_re_aggr_cmd = make_aggr_cmd(charge_re_aggr_key_pattern, charge_re_aggr_op_list, None, True) 
    
    charge_hexec_cmd = make_hexec_cmd(start_time, end_time , [regex_cmd, regex_cmd1, charge_regex_cmd2, charge_aggr_cmd],progname, True)
    
    charge_cmd = 'zcat ' + progname + '.gz | ' + charge_re_aggr_cmd

    # 로그아웃을 한 경우
    logout_regex_cmd2 = make_regex_cmd(logout_regex_pattern2, logout_regex_format2, logout_regex_key_pattern2, logout_regex_key_format2)
    logout_aggr_cmd = make_aggr_cmd(logout_aggr_key_pattern, logout_aggr_op_list, logout_aggr_key_expr)
    logout_re_aggr_cmd = make_aggr_cmd(logout_re_aggr_key_pattern, logout_re_aggr_op_list, None, True)

    logout_hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, logout_regex_cmd2, logout_aggr_cmd], progname, True)
    
    logout_cmd = 'zcat ' + progname + '.gz | ' + logout_re_aggr_cmd
    
    # 기타 행동에 대한 gexec생성
    etc_regex_cmd2 = make_regex_cmd(etc_regex_pattern2, etc_regex_format)
    etc_aggr_cmd = make_aggr_cmd(etc_aggr_key_pattern, etc_aggr_op_list, etc_aggr_key_expr)
    etc_re_aggr_cmd = make_aggr_cmd(etc_re_aggr_key_pattern, etc_re_aggr_op_list, None, True)

    etc_hexec_cmd = make_hexec_cmd(start_time, end_time,  [regex_cmd, regex_cmd1, etc_regex_cmd2, etc_aggr_cmd], progname, True)

    etc_cmd = 'zcat ' + progname + '.gz | ' + etc_re_aggr_cmd

    if (debug_mode):
        print charge_cmd
        print logout_cmd
        print etc_cmd

    hash_table = {}
                        
    # 무료충전이 발생한 경우를 추출한 결과저장
    os.system(charge_hexec_cmd)
    charge_result = os.popen(charge_cmd, 'rb')

    for row in create_tsvreader( charge_result):
        key = (row['k.p_level'], row['k.p_ispu'], row['k.membership'], row['k.sp'])
        value = row['sum.value']
        hash_table[key] = value
        if (debug_mode):
            print "무료충전 : %s    %s" % (key, value)

    # 로그아웃이 발생한 경우를 추출한 결과저장
    os.system(logout_hexec_cmd)
    logout_result = os.popen(logout_cmd, 'rb')

    for row in create_tsvreader(logout_result):
        key = (row['k.p_level'], row['k.p_ispu'], row['k.membership'], row['k.sp'])
        value = row['sum.value']
        hash_table[key] = value
        if (debug_mode):
            print "로그아웃 : %s    %s" % (key, value)

    # 기타의 경우가 발생한 경우를 추출한 결과저장
    os.system(etc_hexec_cmd)
    etc_result = os.popen(etc_cmd, 'rb')
    for row in create_tsvreader(etc_result):
        key = (row['k.p_level'], row['k.p_ispu'], row['k.membership'], '기타')
        value = row['sum.value']
        hash_table[key] = value
        if (debug_mode):
            print "기타 : %s    %s" % (key, value)
 
    return hash_table


if __name__ == '__main__':
    eighteenth_3(False)
