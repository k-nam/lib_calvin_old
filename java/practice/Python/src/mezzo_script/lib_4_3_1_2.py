#!/usr/bin/env python
# -*- coding: utf_8 -*-

import sys, os
import getopt
#sys.path.insert(0, '../../lib')
#sys.path.append(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib')
from mezzo_util import *

def usage(progname):
    print >> sys.stderr, "\nUsage: " + progname + "\t[-d] [<date string>]\n"
    print >> sys.stderr, "\t\t-d\tDebug mode\n\n"
    
def seventeenth_new_1(debug_mode, progname):

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

    # 구매시작팝업 ~ 구매완료팝업
    regex_pattern = '<[^>]*abstype=(?P<abstype>[^> ]*)[^>]*(?P<goodstype>goodstype="(POKERAVT|EDPAVT|COMAVT|POKERITEM|POKERVPLUSAVT|POKERWPLUSAVT)")[^>]*p_level=(?P<p_level>"\d+")[^>]*pluscode=(?P<pluscode>[^> ]*)[^>]*sp="한게임/(아이템|아바타)/구매시작팝업"[^>]*>(?!<[^>]*sp="한게임/(아이템|아바타)/구매시작팝업"[^>]*>).*?<[^>]*(?P<goodsamt>goodsamt="\d*")[^>]*goodstype="(POKERAVT|EDPAVT|COMAVT|POKERITEM|POKERVPLUSAVT|POKERWPLUSAVT)"[^>]*sp="한게임/(아이템|아바타)/구매완료팝업"[^>]*>'
    regex_format = 'abstype=\g<abstype> p_level=\g<p_level> pluscode=\g<pluscode> \g<goodstype> \g<goodsamt>'

    aggr_key_pattern = '/abstype="(?P<abstype>[^> ]*)" p_level="(?P<p_level>\d+)" pluscode="(?P<pluscode>[^> ]*)" goodstype="(?P<goodstype>\w*)"/'
    aggr_key_expr = 'membership:web_membership(p,abstype,pluscode),goodstype:echo(goodstype),p_level:echo(p_level)'
    aggr_op_list = ['sum(1)','sum(/goodsamt="(?P<goodsamt>\d+)"/)']

    aggr_key_pattern1 = '/<k goodstype=(?P<goodstype>[^> ]*) membership=(?P<membership>[^> ]*) p_level=(?P<p_level>[^> ]*)>/'
    aggr_key_expr1 = 'membership:echo(membership),goodstype:echo(goodstype),p_level:echo(p_level)'
    aggr_op_list1 = ['sum(/value=(?P<value>\d+)/)','sum(/goodsamt=(?P<goodsamt>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)
    
    hexec_cmd = make_hexec_cmd(start_time,end_time,[ regex_cmd ,aggr_cmd ],progname)
    os.system(hexec_cmd)    
    
    if (debug_mode):
        print cmd

    result = os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1, 'rb')

    hash_table = {}
    for row in create_tsvreader(result):
        key = (start_time.strftime('%Y-%m-%d'), row['k.p_level'], row['k.membership'], row['k.goodstype'])
        value = (row['sum.value'], row['sum.goodsamt'])
        hash_table[key] = value
        if (debug_mode):
            print key, value

    return hash_table

if __name__ == '__main__':
    seventeenth_new_1(False,progname)
