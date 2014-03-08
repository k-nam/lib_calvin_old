#!/usr/bin/env python

# -*- coding: utf_8 -*-

import sys, os
import getopt
#sys.path.insert(0, './lib')
#sys.path.append(os.getenv('mezzo_home', '/home1/irteam/mezzo') + '/scripts/lib')
from mezzo_util import *

def usage(progname):
    print >> sys.stderr, "\nusage: " + progname + "\t[-d] [<date string>]\n"
    print >> sys.stderr, "\t\t-d\tdeb ug mode\n\n"
    
def eighteenth_0(debug_mode, progname):

    optlist = []
    args = sys.argv[1:]
    try:
        optlist, args = getopt.getopt(args, 'd')
    except getopt.getopterror:
        usage(sys.argv[0])
        sys.exit(2)
    debug_mode = False
    for o, a in optlist:
        if (o == "-d"):
            debug_mode = True
            
    start_time, end_time = parse_timerange(args)

    # 올인팝업부터 구매완료팝업까지 longest match
    sevenpoker_allin_popup_p = '(<[^>]*goodslist=(?P<goodslist>[^> ]*)[^>]*p_ispu=(?P<p_ispu>[^> ]*)[^>]*p_level=(?P<p_level>"\d+")[^>]*sp=[^> ]*/7포커/클라이언트/올인/올인팝업[^>]*>)'

    regex_pattern = '(<[^>]*sp=[^> ]*/7포커/클라이언트/올인/올인팝업[^>]*>|<[^>]*sp=[^> ]*/7포커/클라이언트/방_입장[^>]*>)'
    regex_format = r'\1' 

    regex_pattern1 = '(?P<after_allin>' + sevenpoker_allin_popup_p + '.*)'
    regex_format1 = r'\g<after_allin>'


    # 포커 아바타일 경우
    pkavt_regex_pattern2 = '<[^>]*goodstype=(?P<goodstype>"(POKERAVT)")[^>]*sp=[^> ]*/구매시작팝업[^>]*>(?!<[^>]*goodstype=(?P=goodstype)[^>]*' \
          'sp=[^> ]*/구매시작팝업[^>]*>).*?<[^>]*goodsid=(?P<goodsid>"[^> ]+")[^>]*[^>]*goodstype=(?P=goodstype)[^>]*sp=[^> ]*/아이템/구매완료팝업[^>]*>'
    pkavt_regex_format2 = r'goodstype=\g<goodstype> goodsid=\g<goodsid>'
    pkavt_regex_key_pattern2 = '(<[^>]*goodslist=(?P<goodslist>[^> ]*)[^>]*p_ispu=(?P<p_ispu>[^> ]*)[^>]*' \
                               'p_level=(?P<p_level>"\d+")[^>]*sp=[^> ]*/7포커/클라이언트/올인/올인팝업[^>]*>)'
    pkavt_regex_key_format2 = r'goodslist=\g<goodslist> p_ispu=\g<p_ispu> p_level=\g<p_level> '

    pkavt_regex_pattern3 = r'(goodsid="|@)(?P<goods>\w+)'
    pkavt_regex_format3 = r'goodsid=\g<goods>'
    pkavt_regex_key_pattern3 = 'goodslist=(?P<goodslist>[^> ]*) p_ispu=(?P<p_ispu>[^> ]*) ' \
                               'p_level=(?P<p_level>"\d+") goodstype=(?P<goodstype>[^> ]*)'
    pkavt_regex_key_format3 = r'goodslist=\g<goodslist> p_ispu=\g<p_ispu> p_level=\g<p_level> goodstype=\g<goodstype> '
                
    pkavt_aggr_key_pattern = '/goodslist="(?P<goodslist>[^> ]*)" p_ispu="(?P<p_ispu>[^> ]*)" ' \
                       'p_level="(?P<p_level>\d+)" goodstype="(?P<goodstype>[^> ]*)" goodsid=(?P<goodsid>[^> ]*)/'
    pkavt_aggr_key_expr = 'membership:client_membership_type(goodslist),p_ispu:echo(p_ispu),p_level:echo(p_level),goodstype:echo(goodstype),goodsid:echo(goodsid)'
    pkavt_aggr_op_list = [ 'sum(1)' ]

    pkavt_re_aggr_key_pattern = '/goodsid=(?P<goodsid>[^> ]*) goodstype=(?P<goodstype>[^> ]*) membership=(?P<membership>[^> ]*) ' \
                          'p_ispu=(?P<p_ispu>[^> ]*) p_level=(?P<p_level>[^> ]*)/'
    pkavt_re_aggr_op_list = ['sum(/value=(?P<value>\d+)/)']

    
    # 포커 아이템일 경우
    item_regex_pattern2 = '<[^>]*goodstype=(?P<goodstype>("POKERITEM"|"COMITEM"))[^>]*sp=[^> ]*/구매시작팝업[^>]*>(?!<[^>]*goodstype=(?P=goodstype)[^>]*' \
                          'sp=[^> ]*/구매시작팝업[^>]*>).*?<[^>]*goodscnt=(?P<goodscnt>[^> ]*)[^>]*goodsid=(?P<goodsid>"[^> ]+")[^>]*' \
                          'goodstype=(?P=goodstype)[^>]*sp=[^> ]*/아이템/구매완료팝업[^>]*>'
    item_regex_format2 = r'goodstype=\g<goodstype> goodsid=\g<goodsid> goodscnt=\g<goodscnt>'
    item_regex_key_pattern2 = '(<[^>]*goodslist=(?P<goodslist>[^> ]*)[^>]*p_ispu=(?P<p_ispu>[^> ]*)[^>]*' \
                              'p_level=(?P<p_level>"\d+")[^>]*sp=[^> ]*/7포커/클라이언트/올인/올인팝업[^>]*>)'
    item_regex_key_format2 = r'goodslist=\g<goodslist> p_ispu=\g<p_ispu> p_level=\g<p_level> '

    item_aggr_key_pattern = '/goodslist="(?P<goodslist>[^> ]*)" p_ispu="(?P<p_ispu>[^> ]*)" ' \
                       'p_level="(?P<p_level>\d+)" goodstype="(?P<goodstype>[^> ]*)" goodsid="(?P<goodsid>[^> ]*)"/'
    item_aggr_key_expr = 'membership:client_membership_type(goodslist),p_ispu:echo(p_ispu),p_level:echo(p_level),goodstype:echo(goodstype),goodsid:echo(goodsid)'
    item_aggr_op_list = [ 'sum(/goodscnt="(?P<goodscnt>\d+)"/)' ]

    item_re_aggr_key_pattern = '/goodsid=(?P<goodsid>[^> ]*) goodstype=(?P<goodstype>[^> ]*) membership=(?P<membership>[^> ]*) ' \
                          'p_ispu=(?P<p_ispu>[^> ]*) p_level=(?P<p_level>[^> ]*)/'
    item_re_aggr_op_list = ['sum(/goodscnt=(?P<value>\d+)/)']


    # 아바타일 경우
    avt_regex_pattern2 = '<[^>]*goodstype=(?P<goodstype>"(EDPAVT|COMAVT|POKERVPLUSAVT|POKERWPLUSAVT)")[^>]*sp=[^> ]*/구매시작팝업[^>]*>' \
                         '(?!<[^>]*goodstype=(?P=goodstype)[^>]*sp=[^> ]*/구매시작팝업[^>]*>).*?' \
                         '<[^>]*goodsid=(?P<goodsid>"[^> ]+")[^>]*goodstype=(?P=goodstype)[^>]*sp=[^> ]*/아바타/구매완료팝업[^>]*>'
    avt_regex_format2 = r'goodstype=\g<goodstype> goodsid=\g<goodsid>'
    avt_regex_key_pattern2 = '(<[^>]*goodslist=(?P<goodslist>[^> ]*)[^>]*p_ispu=(?P<p_ispu>[^> ]*)[^>]*' \
                               'p_level=(?P<p_level>"\d+")[^>]*sp=[^> ]*/7포커/클라이언트/올인/올인팝업[^>]*>)'
    avt_regex_key_format2 = r'goodslist=\g<goodslist> p_ispu=\g<p_ispu> p_level=\g<p_level> '

    avt_regex_pattern3 = r'(goodsid="|@)(?P<goods>\w+)'    
    avt_regex_format3 = r'goodsid=\g<goods>'
    avt_regex_key_pattern3 = 'goodslist=(?P<goodslist>[^> ]*) p_ispu=(?P<p_ispu>[^> ]*) ' \
                               'p_level=(?P<p_level>"\d+") goodstype=(?P<goodstype>[^> ]*)'
    avt_regex_key_format3 = r'goodslist=\g<goodslist> p_ispu=\g<p_ispu> p_level=\g<p_level> goodstype=\g<goodstype> '

    avt_aggr_key_pattern = '/goodslist="(?P<goodslist>[^> ]*)" p_ispu="(?P<p_ispu>[^> ]*)" ' \
                       'p_level="(?P<p_level>\d+)" goodstype="(?P<goodstype>[^> ]*)" goodsid=(?P<goodsid>[^> ]*)/'
    avt_aggr_key_expr = 'membership:client_membership_type(goodslist),p_ispu:echo(p_ispu),p_level:echo(p_level),goodstype:echo(goodstype),goodsid:echo(goodsid)'
    avt_aggr_op_list = [ 'sum(1)' ]

    avt_re_aggr_key_pattern = '/goodsid=(?P<goodsid>[^> ]*) goodstype=(?P<goodstype>[^> ]*) membership=(?P<membership>[^> ]*) ' \
                          'p_ispu=(?P<p_ispu>[^> ]*) p_level=(?P<p_level>[^> ]*)/'
    avt_re_aggr_op_list = ['sum(/value=(?P<value>\d+)/)']
    


    # 포커 아바타/아이템에 관계없이 draw, regex를 생성
    args = []
    args.extend( [dbl_quote(regex_pattern),dbl_quote(regex_format)])
    regex_cmd = 'regex -s ' + ' '.join(args)
    regex_cmd1 = make_regex_cmd(regex_pattern1, regex_format1)


    # 포커아바타일 경우
    pkavt_regex_cmd2 = make_regex_cmd(pkavt_regex_pattern2, pkavt_regex_format2, pkavt_regex_key_pattern2, pkavt_regex_key_format2)
    pkavt_regex_cmd3 = make_regex_cmd(pkavt_regex_pattern3, pkavt_regex_format3, pkavt_regex_key_pattern3, pkavt_regex_key_format3)
    pkavt_aggr_cmd = make_aggr_cmd(pkavt_aggr_key_pattern, pkavt_aggr_op_list, pkavt_aggr_key_expr)
    pkavt_re_aggr_cmd = make_aggr_cmd(pkavt_re_aggr_key_pattern, pkavt_re_aggr_op_list, None, True)

    pkavt_hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, pkavt_regex_cmd2, pkavt_regex_cmd3, pkavt_aggr_cmd], progname)
    pkavt_cmd = 'zcat ' + progname + '.gz | ' + pkavt_re_aggr_cmd

    # 포커아이템일 경우
    item_regex_cmd2 = make_regex_cmd(item_regex_pattern2, item_regex_format2, item_regex_key_pattern2, item_regex_key_format2)
    item_aggr_cmd = make_aggr_cmd(item_aggr_key_pattern, item_aggr_op_list, item_aggr_key_expr)
    item_re_aggr_cmd = make_aggr_cmd(item_re_aggr_key_pattern, item_re_aggr_op_list, None, True)

    item_hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, item_regex_cmd2, item_aggr_cmd], progname)
    pokeritem_cmd = 'zcat ' + progname + '.gz | ' + item_re_aggr_cmd

    # 아바타일 경우
    avt_regex_cmd2 = make_regex_cmd(avt_regex_pattern2, avt_regex_format2, avt_regex_key_pattern2, avt_regex_key_format2)
    avt_regex_cmd3 = make_regex_cmd(avt_regex_pattern3, avt_regex_format3, avt_regex_key_pattern3, avt_regex_key_format3)
    avt_aggr_cmd = make_aggr_cmd(avt_aggr_key_pattern, avt_aggr_op_list, avt_aggr_key_expr)
    avt_re_aggr_cmd = make_aggr_cmd(avt_re_aggr_key_pattern, avt_re_aggr_op_list, None, True)

    avt_hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, avt_regex_cmd2, avt_regex_cmd3, avt_aggr_cmd], progname)
    avt_cmd = 'zcat ' + progname + '.gz | ' + avt_re_aggr_cmd
    
    if (debug_mode):
        print pkavt_cmd
        print pokeritem_cmd
        print avt_cmd

    hash_table = {}

    # 포커 아바타에 관한 내용을 추출하기 위한 gexec를 실행시킨다.

    os.system(pkavt_hexec_cmd)
    result_pkavt = os.popen( pkavt_cmd, 'rb' )

    for row in create_tsvreader(result_pkavt):
        key = (row['k.p_level'], row['k.p_ispu'], row['k.membership'], row['k.goodstype'], row['k.goodsid'])
        value = row['sum.value']
        hash_table[key] = value
        if (debug_mode):
            print "PKAVT : %s,   %s" % (key, value)

    # 포커 아이템에 관한 내용을 추출하기 위한 gexec를 실행시킨다.

    os.system(item_hexec_cmd)
    result_pokeritem = os.popen( pokeritem_cmd, 'rb' )

    for row in create_tsvreader(result_pokeritem):
        key = (row['k.p_level'], row['k.p_ispu'], row['k.membership'], row['k.goodstype'], row['k.goodsid'])
        value = row['sum.value']
        hash_table[key] = value
        if (debug_mode):
            print "ITEM : %s,   %s" %(key, value)

    # 아바타에 관한 내용을 추출하기 위한 gexec를 실행시킨다.

    os.system(avt_hexec_cmd)
    result_avt = os.popen( avt_cmd, 'rb' )

    for row in create_tsvreader(result_avt):
        key = (row['k.p_level'], row['k.p_ispu'], row['k.membership'], row['k.goodstype'], row['k.goodsid'])
        value = row['sum.value']
        hash_table[key] = value
        if (debug_mode):
            print "AVT : %s,   %s" %(key, value)

    return hash_table

if __name__ == '__main__':
    eighteenth_0(False,  progname)
