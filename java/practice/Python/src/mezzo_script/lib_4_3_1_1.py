#!/usr/bin/env python
# -*- coding: utf_8 -*-

import sys, os
import getopt
#sys.path.insert(0, '../../lib')
#sys.path.append(os.getenv('MEZZO_HOME', '/home1/irteam/minsu') + '/scripts/lib')
from mezzo_util import *

def usage(progname):
    print >> sys.stderr, "\nUsage: " + progname + "\t[-d] [<date string>]\n"
    print >> sys.stderr, "\t\t-d\tDebug mode\n\n"
    
def seventeenth_new_0(debug_mode, progname):

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

    # 구매시작팝업
    buy_start_popup_p = '(?P<buy_start><[^>]*abstype=(?P<abstype>[^> ]*)[^>]*goodstype=(?P<goodstype>"(POKERAVT|EDPAVT|COMAVT|POKERITEM|POKERVPLUSAVT|POKERWPLUSAVT)")[^>]*p_level=(?P<p_level>"\d+")[^>]*pluscode=(?P<pluscode>[^> ]*)[^>]*sp=[^> ]*/구매시작팝업[^>]*>)'

    regex_pattern = buy_start_popup_p
    regex_format = r'abstype=\g<abstype> goodstype=\g<goodstype> p_level=\g<p_level> pluscode=\g<pluscode>'
    re_list = [(regex_pattern, regex_format)]

    aggr_key_pattern = '/abstype="(?P<abstype>\w*)"[^>]*goodstype="(?P<goodstype>\w+)"[^>]*p_level="(?P<p_level>\d+)"[^>]*pluscode="(?P<pluscode>\w*)/'
    aggr_key_expr = 'membership:web_membership(p,abstype,pluscode),goodstype:echo(goodstype),p_level:echo(p_level)'
    aggr_op_list = [ 'sum(1)' ]

    regex_cmd = make_regex_cmd(regex_pattern, regex_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    gexec_cmd = make_hexec_cmd(start_time,end_time,[ regex_cmd ,aggr_cmd ],progname)
    print gexec_cmd
    sys.exit()

    os.system(gexec_cmd)
    
    aggr_key_pattern1 = '/<k goodstype=(?P<goodstype>[^> ]*) membership=(?P<membership>[^> ]*) p_level=(?P<p_level>[^> ]*)>/'
    aggr_key_expr1 = 'membership:echo(membership),goodstype:echo(goodstype),p_level:echo(p_level)'
    aggr_op_list1 = ['sum(/value=(?P<value>\d+)/)']
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)
    cmd = 'zcat ' + progname + '.gz | ' + aggr_cmd1
    
    if (debug_mode):
        print cmd

    hash_table = {}
    for row in create_tsvreader(os.popen(cmd, 'rb')):
        key = (start_time.strftime('%Y-%m-%d'), row['k.p_level'], row['k.membership'], row['k.goodstype'])
        value = row['sum.value']
        hash_table[key] = value
        if (debug_mode):
            print key, value

    return hash_table

if __name__ == '__main__':
    seventeenth_0(False)
