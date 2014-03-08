#! /usr/bin/env python
# -*- coding: utf_8 -*-

"""
3_3_1 포커 구매전환 분석
"""

import sys,os
import csv 
#sys.path.insert(0, os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib')
from mezzo_util import *
import time

def ignore(expr):
    n = expr.split(':')[0]
    return n + ':echo(__all__)'

def combination(seq, mutate):
    if len(seq) == 0: return [[]]

    result = []
    for i in combination(seq[1:], mutate):
        result += [ [seq[0]] + i, [mutate(seq[0])] + i ]
    return result 

def gen_uv_stat(in_file, debug=False):
    reaggr_tmpl = ( '/<[^>]*%s'
                    ' p_ispu=(?P<p_ispu>[^> ]*)[^>]*'
                    ' p_level=(?P<p_level>[^> ]*)[^>]*'
                    ' p_mny_class=(?P<p_mny_class>[^> ]*)>/' )
    reaggr_assigns = ['p_ispu:echo(p_ispu)',
                      'p_level:echo(p_level)',
                      'p_mny_class:echo(p_mny_class)']
    reaggr_op1_list = [ 'sum(/<sum value=(?P<value>[^> ]*)[^>]*>/)' ]
    reaggr_op2_list = [ 'sum(1)', 'sum(/<sum value=(?P<pv>[^> ]*)[^>]*>/)' ]

    import itertools
    gen_list = []
    for i in combination(reaggr_assigns, ignore):
        cmd1 = 'zcat ' + in_file + ' | ' + \
                make_aggr_cmd( reaggr_tmpl % (' id=(?P<id>[^ >]*)[^>]*'), \
                reaggr_op1_list, ','.join(['id:echo(id)']+i), tsv=False )
        cmd2 = " | " + make_aggr_cmd( reaggr_tmpl % (''),
                reaggr_op2_list, ','.join(i), tsv=True )
        cmd = cmd1 + cmd2
        if debug:
            print cmd
        else:
            gen_list.append( create_tsvreader( os.popen( cmd, 'rb' ) ) )

    return itertools.chain(*gen_list)

def main():
    progname = os.path.basename(sys.argv[0])
    x = progname.rfind('.')
    if x > 0: progname = progname[:x]  

    start_time, end_time = parse_timerange( sys.argv[1:] )

    execute_start = time.ctime()
    re_cmds = []
    
    re_cmds.append( (
        '<[^>]*'
        ' goodstype=\"(EDPAVT|COMAVT|POKERVPLUSAVT|POKERWPLUSAVT|POKERAVT|POKERITEM|COMITEM)\"[^>]*'
        ' (?P<p_ispu>p_ispu=[^ >]*)[^>]*'
        ' (?P<p_level>p_level=[^ >]*)[^>]*'
        ' (?P<p_mny_bal>p_mny_bal=[^ >]*)[^>]*'
        ' sp=\"한게임/(?P<p>아바타|아이템)/구매시작팝업[^>]*>',
        r'<R \g<p_level> \g<p_ispu> \g<p_mny_bal>>',
        '^<K[^>]*li=(?P<id>[^ >]*)',
        r'<L id=\g<id>>'
    ) )

    aggr_key_pattern = '/id=\"(?P<id>[^\">]*)\">' \
                       '<R p_level=\"(?P<p_level>[^\">]*)\"[^>]*' \
                       ' p_ispu=\"(?P<p_ispu>[^\">]*)\"[^>]*' \
                       ' p_mny_bal=\"(?P<p_mny_bal>[^\">]*)\"[^>]*>/'
    aggr_key_expr_dict = {
        'id':'echo(id)',
        'p_level':'echo(p_level)',
        'p_ispu':'echo(p_ispu)',
        'p_mny_class':'p_mny_class(p_mny_bal)'
        }
    aggr_op_list = [ 'sum(1)' ]

    cmd = make_hexec_cmd2(True, start_time, end_time, progname, re_cmds, aggr_key_pattern, aggr_op_list, aggr_key_expr_dict)
    os.system('mv ' + progname + '.gz /tmp/11_uv1.f3.gz')
    
    re_cmds1 = []
    re_cmds1.append( (
        '<[^>]*goodstype=\"(EDPAVT|COMAVT|POKERVPLUSAVT|POKERWPLUSAVT|POKERAVT|POKERITEM|COMITEM)\"[^>]*'
        ' (?P<p_ispu>p_ispu=[^ >]*)[^>]*'
        ' (?P<p_level>p_level=[^ >]*)[^>]*'
        ' (?P<p_mny_bal>p_mny_bal=[^ >]*)[^>]*'
        ' sp=\"한게임/(?P<p>아바타|아이템)/구매시작팝업[^>]*>'
        '((?!<[^>]*sp=\"한게임/로그인_웹(?=[ >])[^>]*>)<[^>]*>){0,30}'
        '?<[^>]* sp=\"한게임/(?P=p)/구매완료팝업[^>]*>',
        r'<R \g<p_level> \g<p_ispu> \g<p_mny_bal>>',
        '<K[^>]*li=(?P<id>[^ >]*)',
        r'<L id=\g<id>>'
    ) )    
    cmd = make_hexec_cmd2(True, start_time, end_time, progname, re_cmds1, aggr_key_pattern, aggr_op_list, aggr_key_expr_dict)
    os.system('mv ' + progname + '.gz /tmp/11_uv2.f3.gz')

    fn_uv1 = "/tmp/11_uv1.f3.gz"
    fn_uv2 = "/tmp/11_uv2.f3.gz"

    # Prepare output file
#    target_date_path = os.path.join('/home1/irteam/mezzo/out.old', start_time.strftime('%Y/%m/%d'))
    target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
    if not os.path.exists(target_date_path):
        os.makedirs(target_date_path, 0750)

#    outfile = open(os.path.join(target_date_path, '3_3_1_p_all_buy_stage.tsv'), 'wb')
    outfile = open(os.path.join('./3_3_1_p_all_buy_stage.tsv'),'wb')
    hdrs = ['date', 'p_level', 'p_ispu', 'p_mny_class', 'sp', 'pv', 'uv']

    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow( dict( zip(hdrs, hdrs) ) )

    # stage 1 calculations
    for row in gen_uv_stat(fn_uv1):
        new_row = {
            'date': start_time.strftime('%Y-%m-%d'),
            'p_level': row['k.p_level'],
            'p_ispu': row['k.p_ispu'],
            'p_mny_class': row['k.p_mny_class'],
            'sp': '구매시작팝업',
            'uv': row['sum.value'],
            'pv': row['sum.pv'],
            }
        writer.writerow( new_row )
    # stage 2 calculations
    for row in gen_uv_stat(fn_uv2):
        new_row = {
            'date': start_time.strftime('%Y-%m-%d'),
            'p_level': row['k.p_level'],
            'p_ispu': row['k.p_ispu'],
            'p_mny_class': row['k.p_mny_class'],
            'sp': '구매완료팝업',
            'uv': row['sum.value'],
            'pv': row['sum.pv'],
            }
        writer.writerow ( new_row )

    os.unlink(fn_uv1)
    os.unlink(fn_uv2)

    execute_end = time.ctime()

    print execute_start + '\t' + execute_end + '\t' + 'File creation successed : 3_3_1_p_all_buy_stage.tsv\n'



if __name__ == '__main__':
    main()
