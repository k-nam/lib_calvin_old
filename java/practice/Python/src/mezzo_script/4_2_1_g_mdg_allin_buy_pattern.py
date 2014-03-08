#!/usr/bin/env python
# -*- coding: utf_8 -*-

"""
4_2_1 신맞고 올인시 구매패턴 분석
"""


import sys, os
import getopt
import time
#sys.path.insert(0, '/home1/irteam/minsu/lib')
#sys.path.append(os.getenv('MEZZO_HOME', '/home1/irteam/minsu') + '/scripts/lib')
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
                                                            
    # 올인팝업부터 구매완료팝업까지 longest match """

    mdg_allin_popup_p = '(?P<allin><[^>]*g_ispu=(?P<g_ispu>[^> ]*)[^>]*g_level=(?P<g_level>"\d+")[^>]*goodslist=(?P<goodslist>[^> ]*)[^>]*mshiptype=(?P<mshiptype>[^> ]*)[^>]*sp=[^> ]*/신맞고/클라이언트/올인/올인팝업[^>]*>)'
    g_buy_complete_p = '<[^>]*goodstype="((GOSTOPAVT)|(REDAVT)|(EDPAVT)|(COMAVT)|(GOSTOPVPLUSAVT)|(GOSTOPWPLUSAVT))"[^>]*sp=[^> ]*/구매완료팝업[^>]*>'
    some_nodes_p = '(?!<[^>]*sp="한게임/고스톱게임/신맞고/클라이언트/((방_입장)|(올인/올인팝업))"[^>]*>).*?'

    regex_key_pattern1='<[^>]*li=[^>]*>'
    regex_pattern1 = mdg_allin_popup_p + '(?P<after_allin>' + some_nodes_p + g_buy_complete_p + ')'
    regex_format1 = r'g_ispu=\g<g_ispu> g_level=\g<g_level> goodslist=\g<goodslist> mshiptype=\g<mshiptype> \g<after_allin>'
    
    # 구매완료팝업의 상품정보 추출 """

    regex_pattern2 = '<[^>]*goodstype=(?P<goodstype>("GOSTOPAVT")|("REDAVT")|("EDPAVT")|("COMAVT")|("GOSTOPVPLUSAVT")|("GOSTOPWPLUSAVT"))[^>]*sp=[^> ]*/구매시작팝업[^>]*>(?!<[^>]*goodstype=(?P=goodstype)[^>]*sp=[^> ]*/구매시작팝업[^>]*>).*?<[^>]*goodsid=(?P<goodsid>"[^> ]+")[^>]*[^>]*goodstype=(?P=goodstype)[^>]*sp=[^> ]*/구매완료팝업[^>]*>'
    regex_format2 = r'goodstype=\g<goodstype> goodsid=@\g<goodsid>'
    regex_key_pattern2 = 'g_ispu=(?P<g_ispu>[^> ]*)[^>]*g_level=(?P<g_level>"\d+")[^>]*goodslist=(?P<goodslist>[^> ]*)[^>]*mshiptype=(?P<mshiptype>[^> ]*)'
    regex_key_format2 = 'g_ispu=\g<g_ispu> g_level=\g<g_level> goodslist=\g<goodslist> mshiptype=\g<mshiptype> '

    regex_pattern3 = r'@([^@>\n ]+)'
    regex_format3 = r'\1'
    regex_key_pattern3 = 'g_ispu=(?P<g_ispu>[^> ]*)[^>]*g_level=(?P<g_level>"\d+")[^>]*goodslist=(?P<goodslist>[^> ]*)[^>]*mshiptype=(?P<mshiptype>[^> ]*)[^>]*goodstype=(?P<goodstype>"[^> ]+")[^>]*goodsid='
    regex_key_format3 = r'g_ispu=\g<g_ispu> g_level=\g<g_level> goodslist=\g<goodslist> mshiptype=\g<mshiptype> goodstype=\g<goodstype> goodsid='

    aggr_key_pattern = '/g_ispu="(?P<g_ispu>[^> ]*)"[^>]*g_level="(?P<g_level>\d+)"[^>]*goodslist="(?P<goodslist>[^> ]*)"[^>]*mshiptype="(?P<mshiptype>[^> ]*)"[^>]*goodstype="(?P<goodstype>[^> ]+)"[^>]*goodsid="(?P<goodsid>[^> ]+)"/'

    aggr_key_exprs = {'membership':'client_membership(mshiptype,goodslist)',
                      'g_level':'echo(g_level)',
                      'goodstype':'echo(goodstype)',
                      'goodsid':'echo(goodsid)'}
    aggr_op_list = [ 'sum(1)' ]

    re_list = [(regex_pattern1, regex_format1,regex_key_pattern1), (regex_pattern2, regex_format2, regex_key_pattern2, regex_key_format2), (regex_pattern3, regex_format3, regex_key_pattern3, regex_key_format3)]

    if (debug_mode):
        target_date_path = os.path.join(target_path + ".old", start_time.strftime('%Y/%m/%d'))
    else:
        target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
    os.system('mkdir -p ' + target_date_path)

    outfile = open(os.path.join('./4_2_1_g_mdg_allin_buy_pattern.tsv'), 'wb')

    hdrs = ['date', 'mshiptype', 'g_level', 'goodstype', 'goodsid', 'num_buy_done']
    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow( dict( zip(hdrs, hdrs) ) )

    if (debug_mode):
        cmd = make_mana_cmd2(start_time, end_time, re_list, aggr_key_pattern, aggr_op_list, aggr_key_exprs, True)
        print cmd

    for row in mana_exec2(True, start_time, end_time, progname, re_list, aggr_key_pattern, aggr_op_list, aggr_key_exprs):
        new_row = {
            'date': start_time.strftime('%Y-%m-%d'),
            'mshiptype': row['k.membership'],
            'g_level': row['k.g_level'],
            'goodstype': row['k.goodstype'],
            'goodsid': row['k.goodsid'],
            'num_buy_done': row['sum.value'],
         }
        writer.writerow(new_row)
        if (debug_mode):
            print new_row

    execute_end = time.ctime()
    print execute_start + '\t' + execute_end + '\t' + 'File creation successed : 4_2_1_g_mdg_allin_buy_pattern.tsv\n'
                        
if __name__ == '__main__':
    main()
