#!/usr/bin/env python
# -*- coding: utf_8 -*-
"""
회원가입프로세스분석_20080502_박민수
"""

import sys, os
from time import strptime
import getopt
#sys.path.insert(0, os.path.join(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib'))
from mezzo_util import *
import time, datetime
import csv


def usage(program):
    print >> sys.stderr, "\nUsage: " + program + "\t[-d] [<date string>]"
    print >> sys.stderr, "\t\t-d\tDebug mode\n\n"


def main():
    progname = os.path.basename(sys.argv[0])
    x = progname.rfind('.')
    if x > 0 : progname = progname[:x]

    optlist = []
    args = sys.argv[1:]

    try :
        optlist, args = getopt.getopt( args, 'd' )
    except getopt.GetoptError:
        usage(sys.argv[0])
        sys.exit(1)

    debug_mode = False

    for opt, arg in optlist:
        if opt == '-d':
            debug_mode = True

    start_time, end_time = parse_timerange(args)
    draw_cmd = make_draw_cmd( start_time, end_time )

    print draw_cmd
    

    start = time.ctime()
    print 'Start To: ', start

    result = []
    result.append( cntSimple(start_time, end_time, progname) )
    result.append( cntOver(start_time, end_time, progname) )
    result.append( cntUnder(start_time, end_time, progname) )
    result.append( cntTotal(start_time, end_time, progname) )

    outfile = open(os.path.join('9_8_2_join_membership_process.tsv'), 'wb')
    hdrs = ['날짜','구분','유입경로','단계','PV','UV']

    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow(dict(zip(hdrs,hdrs)))

    import itertools

    for k,v in itertools.chain( *[r.items() for r in result] ):
        new_row = {'날짜':start_time.strftime('%Y-%m-%d')}
        new_row.update( dict( zip( hdrs[1:4],k ) ) )
        new_row.update( dict( zip( hdrs[4:],v ) ) )

        writer.writerow(new_row)

    outfile.close()
    
    end = time.ctime()
    print 'End To: ', end


def cntSimple( start_time, end_time, progname ):
        
    result = dict()

    agree_result = cntJoinSimpleAgreeContract( start_time, end_time, progname )
    meminfo_result = cntJoinSimpleMeminfo( start_time, end_time, progname )
    registend_result = cntJoinSimpleRegistend( start_time, end_time, progname )

    key_names = ['k.cls','k.docref','k.step']

    for row in agree_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '1.' + step
        key = (cls, docref, step)

        if key not in result: result[key] = [ 0, 0 ]
        result[key][0] = row['sum.pv']
        result[key][1] = row['sum.value']

    for row in meminfo_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '2.' + step
        key = (cls, docref, step)

        bfkey = (cls, docref, '1.약관동의')
        
        if (cls, docref, '1.약관동의') not in result.keys() : continue        
        firststep_pv = result[ bfkey ][0]
        firststep_uv = result[ bfkey ][1]
        if key not in result: result[key] = [ 0, 0 ]
        result[key][0] = row['sum.pv']
        result[key][1] = row['sum.value']

    for row in registend_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key

        step = '3.' + step
        key = (cls, docref, step)

        bfkey = (cls, docref, '1.약관동의')

        if (cls, docref, '1.약관동의') not in result.keys() : continue
        firststep_pv = result[ bfkey ][0]
        firststep_uv = result[ bfkey ][1]
        if key not in result: result[key] = [ 0, 0 ]
        result[key][0] = row['sum.pv']
        result[key][1] = row['sum.value']

    return result


def cntOver( start_time, end_time, progname ):

    result = dict()

    checkname_result = cntJoinOverCheckname( start_time, end_time, progname )
    checkagree_result = cntJoinOverAgreeContract( start_time, end_time, progname )
    checkauth_result = cntJoinOverCheckauth( start_time, end_time, progname )
    meminfo_result = cntJoinOverMeminfo( start_time, end_time, progname )
    registend_result = cntJoinOverRegistend( start_time, end_time, progname )

    key_names = ['k.cls','k.docref','k.step']

    for row in checkname_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '1.' + step
        key = (cls, docref, step)

        if key not in result: result[key] = [ 0, 0 ]
        result[key][0] = long( row['sum.pv'] )
        result[key][1] = long( row['sum.value'] )

    for row in checkagree_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '2.' + step
        key = (cls, docref, step)
        if (cls, docref, '1.실명확인') not in result.keys() : continue
        firststep_pv = result[(cls,docref,'1.실명확인')][0]
        firststep_uv = result[(cls,docref,'1.실명확인')][1]
        
        if key not in result: result[key] = [ 0, 0 ]
        result[key][0] = long( row['sum.pv'] )
        result[key][1] = long( row['sum.value'] )
        
    for row in checkauth_result:
        key = tuple( get_values_from_dict(row, key_names) )
    
        cls, docref, step = key
        step = '3.' + step
        key = (cls, docref, step)
        if (cls, docref, '1.실명확인') not in result.keys() : continue
        firststep_pv = result[(cls,docref,'1.실명확인')][0]
        firststep_uv = result[(cls,docref,'1.실명확인')][1]

        if key not in result: result[key] = [ 0, 0 ]
        result[key][0] = long( row['sum.pv'] )
        result[key][1] = long( row['sum.value'] )

    for row in meminfo_result:
        key = tuple( get_values_from_dict(row, key_names) )
        
        cls, docref, step = key
        step = '4.' + step
        key = (cls, docref, step)
        if (cls,docref,'1.실명확인') not in result.keys(): continue
        firststep_pv = result[(cls,docref,'1.실명확인')][0]
        firststep_uv = result[(cls,docref,'1.실명확인')][1]

        if key not in result: result[key] = [ 0, 0 ]
        result[key][0] = long( row['sum.pv'] )
        result[key][1] = long( row['sum.value'] )


    for row in registend_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '5.' + step
        key = (cls, docref, step)
        if (cls,docref,'1.실명확인') not in result.keys(): continue
        firststep_pv = result[(cls,docref,'1.실명확인')][0]
        firststep_uv = result[(cls,docref,'1.실명확인')][1]

        if key not in result: result[key] = [ 0, 0 ]
        result[key][0] = long( row['sum.pv'] )
        result[key][1] = long( row['sum.value'] )

    return result


def cntUnder( start_time, end_time, progname ):

    result = dict()
   
    checkname_result = cntJoinUnderCheckname( start_time, end_time, progname ) 
    agree_result = cntJoinUnderAgreeContract( start_time, end_time, progname )
    meminfo_result = cntJoinUnderMeminfo( start_time, end_time, progname )
    parentinfo_result = cntJoinUnderParentinfo( start_time, end_time, progname )
    registend_result = cntJoinUnderRegistend( start_time, end_time, progname )

    key_names = ['k.cls','k.docref','k.step']
    firststep_pv = 0
    firststep_uv = 0

    for row in checkname_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '1.' + step
        key = (cls, docref, step)

        if key not in result: result[key] = [ 0, 0 ]
        result[key][0] = long( row['sum.pv'] )
        result[key][1] = long( row['sum.value'] )

    for row in agree_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '2.' + step
        key = (cls, docref, step)
        if (cls, docref, '1.실명확인') not in result.keys() : continue
        firststep_pv = result[(cls,docref,'1.실명확인')][0]
        firststep_uv = result[(cls,docref,'1.실명확인')][1]

        if key not in result: result[key] = [ 0, 0 ]
        result[key][0] = long( row['sum.pv'] )
        result[key][1] = long( row['sum.value'] )

    for row in meminfo_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '3.' + step
        key = (cls, docref, step)
        if (cls, docref, '1.실명확인') not in result.keys() : continue
        firststep_pv = result[(cls,docref,'1.실명확인')][0]
        firststep_uv = result[(cls,docref,'1.실명확인')][1]

        if key not in result: result[key] = [ 0, 0 ]
        result[key][0] = long( row['sum.pv'] )
        result[key][1] = long( row['sum.value'] )


    for row in parentinfo_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '4.' + step
        key = (cls, docref, step)
        if (cls, docref, '1.실명확인') not in result.keys() : continue
        firststep_pv = result[(cls,docref,'1.실명확인')][0]
        firststep_uv = result[(cls,docref,'1.실명확인')][1]

        if key not in result: result[key] = [ 0, 0 ]
        result[key][0] = long( row['sum.pv'] )
        result[key][1] = long( row['sum.value'] )

    
    for row in registend_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '5.' + step
        key = (cls, docref, step)
        if (cls, docref, '1.실명확인') not in result.keys() : continue
        firststep_pv = result[(cls,docref,'1.실명확인')][0]
        firststep_uv = result[(cls,docref,'1.실명확인')][1]

        if key not in result: result[key] = [ 0, 0 ]
        result[key][0] = long( row['sum.pv'] )
        result[key][1] = long( row['sum.value'] )

    return result


def cntTotal( start_time, end_time, progname ):

    sim_agree_result = cntTotSimpleAgreeContract( start_time, end_time, progname )
    sim_meminfo_result = cntTotSimpleMeminfo( start_time, end_time, progname )
    sim_registend_result = cntTotSimpleRegistend( start_time, end_time, progname )
    
    over_checkname_result = cntTotOverCheckname( start_time, end_time, progname )
    over_agree_result = cntTotOverAgreeContract( start_time, end_time, progname )
    over_checkauth_result = cntTotOverCheckauth( start_time, end_time, progname )
    over_meminfo_result = cntTotOverMeminfo( start_time, end_time, progname )
    over_registend_result = cntTotOverRegistend( start_time, end_time, progname )
    
    under_checkname_result = cntTotUnderCheckname( start_time, end_time, progname )
    under_agree_result = cntTotUnderAgreeContract( start_time, end_time, progname )
    under_meminfo_result = cntTotUnderMeminfo( start_time, end_time, progname )
    under_parentinfo_result = cntTotUnderParentinfo( start_time, end_time, progname )
    under_registend_result = cntTotUnderRegistend( start_time, end_time, progname )

    result = dict()
    key_names = ['k.cls','k.docref','k.step']

    bfstep_pv = 0
    bfstep_uv = 0
    
    for row in sim_agree_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '1.' + step
        key = (cls, docref, step)

        if key not in result: result[key] = [0, 0]
        result[key][0] = row['sum.pv']
        result[key][1] = row['sum.value']

    for row in sim_meminfo_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '2.' + step
        key = (cls, docref, step)

        if key not in result: result[key] = [0, 0]
        result[key][0] = row['sum.pv']
        result[key][1] = row['sum.value']
    
    for row in sim_registend_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '3.' + step
        key = (cls, docref, step)

        if key not in result: result[key] = [0, 0]
        result[key][0] = row['sum.pv']
        result[key][1] = row['sum.value']

    for row in over_checkname_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '1.' + step
        key = (cls, docref, step)

        if key not in result: result[key] = [0, 0]
        result[key][0] = row['sum.pv']
        result[key][1] = row['sum.value']

        bfstep_pv = long(row['sum.pv'])
        bfstep_uv = long(row['sum.value'])

    for row in over_agree_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '2.' + step
        key = (cls, docref, step)

        if key not in result: result[key] = [0, 0]
        result[key][0] = row['sum.pv']
        result[key][1] = row['sum.value']

    for row in over_checkauth_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '3.' + step
        key = (cls, docref, step)

        if key not in result: result[key] = [0, 0]
        result[key][0] = row['sum.pv']
        result[key][1] = row['sum.value']

    for row in over_meminfo_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '4.' + step
        key = (cls, docref, step)

        if key not in result: result[key] = [0, 0]
        result[key][0] = row['sum.pv']
        result[key][1] = row['sum.value']

    for row in over_registend_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '5.' + step
        key = (cls, docref, step)

        if key not in result: result[key] = [0, 0]
        result[key][0] = row['sum.pv']
        result[key][1] = row['sum.value']

    for row in under_checkname_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '1.' + step
        key = (cls, docref, step)

        if key not in result: result[key] = [0, 0]
        result[key][0] = row['sum.pv']
        result[key][1] = row['sum.value']

        bfstep_pv = long(row['sum.pv'])
        bfstep_uv = long(row['sum.value'])

    for row in under_agree_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '2.' + step
        key = (cls, docref, step)

        if key not in result: result[key] = [0, 0]
        result[key][0] = row['sum.pv']
        result[key][1] = row['sum.value']

    for row in under_meminfo_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '3.' + step
        key = (cls, docref, step)

        if key not in result: result[key] = [0, 0]
        result[key][0] = row['sum.pv']
        result[key][1] = row['sum.value']

    for row in under_parentinfo_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '4.' + step
        key = (cls, docref, step)

        if key not in result: result[key] = [0, 0]
        result[key][0] = row['sum.pv']
        result[key][1] = row['sum.value']

    for row in under_registend_result:
        key = tuple( get_values_from_dict(row, key_names) )

        cls, docref, step = key
        step = '5.' + step
        key = (cls, docref, step)
        
        if key not in result: result[key] = [0, 0]
        result[key][0] = row['sum.pv']
        result[key][1] = row['sum.value']

    return result


def cntJoinSimpleAgreeContract( start_time, end_time, progname ):

    regex_split = r'regex -k "(?P<id><K[^>]*bc=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/한게임회원/회원가입메인페이지\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<docref>docref=\"\S*\") [^>]*sp="한게임/한게임회원/회원가입메인페이지"[^>]*>.*?<[^>]*(?P<sp>sp="한게임/한게임회원/간편ID/약관동의")[^>]*>'
    regex_format = '\g<docref> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc="(?P<id>[^> ]*)" docref=\"(?P<docref>\S*)\" [^>]*sp="(?P<sp>한게임/한게임회원/간편ID/약관동의)"/'
    aggr_key_expr = 'id:echo(id),docref:remain_domain(docref),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k docref=(?P<docref>[^>]*) id=[^>]*sp=한게임/한게임회원/간편ID/(?P<sp>약관동의)>/'
    aggr_key_expr1 = 'cls:echo(간편),docref:echo(docref),step:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_split,regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntJoinSimpleMeminfo( start_time, end_time, progname ):
    
    regex_split = r'regex -k "(?P<id><K[^>]*bc=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/한게임회원/회원가입메인페이지\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<docref>docref=\"\S*\") [^>]*sp="한게임/한게임회원/회원가입메인페이지"[^>]*>.*?<[^>]*sp="한게임/한게임회원/간편ID/약관동의"[^>]*>.*?<[^>]*(?P<sp>sp="한게임/한게임회원/간편ID/(정보입력|가입정보입력)")[^>]*>'
    regex_format = '\g<docref> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) docref=\"(?P<docref>\S*)\" [^>]*sp="(?P<sp>한게임/한게임회원/간편ID/(정보입력|가입정보입력))"/'
    aggr_key_expr = 'id:echo(id),docref:remain_domain(docref),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k docref=(?P<docref>[^>]*) id=[^>]*sp=한게임/한게임회원/간편ID/(?P<sp>(정보입력|가입정보입력))>/'
    aggr_key_expr1 = 'cls:echo(간편),docref:echo(docref),step:echo(가입정보입력)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_split,regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntJoinSimpleRegistend( start_time, end_time, progname ):
    
    regex_split = r'regex -k "(?P<id><K[^>]*bc=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/한게임회원/회원가입메인페이지\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<docref>docref=\"\S*\") [^>]*sp="한게임/한게임회원/회원가입메인페이지"[^>]*>.*?<[^>]*sp="한게임/한게임회원/간편ID/약관동의"[^>]*>.*?<[^>]*sp="한게임/한게임회원/간편ID/(정보입력|가입정보입력)"[^>]*>.*?<[^>]*(?P<sp>sp="한게임/한게임회원/간편ID/가입완료")[^>]*>'
    regex_format = '\g<docref> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*)[^>]*docref=\"(?P<docref>\S*)\" [^>]*sp="(?P<sp>한게임/한게임회원/간편ID/가입완료)"/'
    aggr_key_expr = 'id:echo(id),docref:remain_domain(docref),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k docref=(?P<docref>[^>]*) id=[^>]*sp=한게임/한게임회원/간편ID/(?P<sp>가입완료)>/'
    aggr_key_expr1 = 'cls:echo(간편),docref:echo(docref),step:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)
    hexec_cmd = make_hexec_cmd(start_time,end_time,[regex_split,regex_cmd,aggr_cmd],progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntJoinOverCheckname( start_time, end_time, progname ):

    regex_split = r'regex -k "(?P<id><K[^>]*bc=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/한게임회원/회원가입메인페이지\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<docref>docref=\"\S*\") [^>]*sp="한게임/한게임회원/회원가입메인페이지"[^>]*>.*?<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세이상/실명확인")[^>]*>'
    regex_format = '\g<docref> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) docref=\"(?P<docref>\S*)\" sp="(?P<sp>한게임/한게임회원/정회원14세이상/실명확인)"/'
    aggr_key_expr = 'id:echo(id),docref:remain_domain(docref),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k docref=(?P<docref>\S*) id=[^>]*sp=한게임/한게임회원/정회원14세이상/(?P<sp>실명확인)>/'
    aggr_key_expr1 = 'cls:echo(14세이상),docref:echo(docref),step:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_split,regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd) 

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntJoinOverAgreeContract( start_time, end_time, progname ):
    
    regex_split = r'regex -k "(?P<id><K[^>]*bc=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/한게임회원/회원가입메인페이지\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<docref>docref=\"\S*\") [^>]*sp="한게임/한게임회원/회원가입메인페이지"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세이상/실명확인"[^>]*>.*?<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세이상/약관동의")[^>]*>'
    regex_format = '\g<docref> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) docref=\"(?P<docref>\S*)\" sp="(?P<sp>한게임/한게임회원/정회원14세이상/약관동의)"/'
    aggr_key_expr = 'id:echo(id),docref:remain_domain(docref),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k docref=(?P<docref>\S*) id=[^>]*sp=한게임/한게임회원/정회원14세이상/(?P<sp>약관동의)[^>]*>/'
    aggr_key_expr1 = 'cls:echo(14세이상),docref:echo(docref),step:echo(sp)'
    aggr_op_list1 = ['sum(1)','sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd( regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd( aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd( aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_split, regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1, 'rb') )


def cntJoinOverCheckauth( start_time, end_time, progname ):

    regex_split = r'regex -k "(?P<id><K[^>]*bc=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/한게임회원/회원가입메인페이지\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<docref>docref=\"\S*\") [^>]*sp="한게임/한게임회원/회원가입메인페이지"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세이상/실명확인"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세이상/약관동의"[^>]*>.*?<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세이상/(본인인증|본인확인)")[^>]*>'
    regex_format = '\g<docref> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) docref=\"(?P<docref>\S*)\" sp="(?P<sp>한게임/한게임회원/정회원14세이상/(본인인증|본인확인))"/'
    aggr_key_expr = 'id:echo(id),docref:remain_domain(docref),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k docref=(?P<docref>\S*) id=[^>]*sp=한게임/한게임회원/정회원14세이상/(본인인증|본인확인)>/'
    aggr_key_expr1 = 'cls:echo(14세이상),docref:echo(docref),step:echo(본인확인)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_split, regex_cmd, aggr_cmd],progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1, 'rb') )


def cntJoinOverMeminfo( start_time, end_time, progname ):

    regex_split = r'regex -k "(?P<id><K[^>]*bc=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/한게임회원/회원가입메인페이지\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<docref>docref=\"\S*\") [^>]*sp="한게임/한게임회원/회원가입메인페이지"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세이상/실명확인"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세이상/약관동의"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세이상/(본인인증|본인확인)"[^>]*>.*?<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세이상/(정보입력|가입정보입력)")[^>]*>'
    regex_format = '\g<docref> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) docref=\"(?P<docref>\S*)\" sp="(?P<sp>한게임/한게임회원/정회원14세이상/(정보입력|가입정보입력))"/'
    aggr_key_expr = 'id:echo(id),docref:remain_domain(docref),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k docref=(?P<docref>\S*) id=[^>]*sp=한게임/한게임회원/정회원14세이상/(?P<sp>(정보입력|가입정보입력))>/'
    aggr_key_expr1 = 'cls:echo(14세이상),docref:echo(docref),step:echo(가입정보입력)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_split, regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntJoinOverRegistend( start_time, end_time, progname ):

    regex_split = r'regex -k "(?P<id><K[^>]*bc=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/한게임회원/회원가입메인페이지\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<docref>docref=\"\S*\") [^>]*sp="한게임/한게임회원/회원가입메인페이지"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세이상/실명확인"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세이상/약관동의"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세이상/(본인인증|본인확인)"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세이상/(정보입력|가입정보입력)"[^>]*>.*?<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세이상/가입완료")[^>]*>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_format = '\g<docref> \g<sp>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*)[^>]*docref=\"(?P<docref>\S*)\" sp="(?P<sp>한게임/한게임회원/정회원14세이상/가입완료)"/'
    aggr_key_expr = 'id:echo(id),docref:remain_domain(docref),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k docref=(?P<docref>\S*) id=[^>]*sp=한게임/한게임회원/정회원14세이상/(?P<sp>가입완료)>/'
    aggr_key_expr1 = 'cls:echo(14세이상),docref:echo(docref),step:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_split, regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1, 'rb') )


def cntJoinUnderCheckname( start_time, end_time, progname ):

    regex_split = r'regex -k "(?P<id><K[^>]*bc=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/한게임회원/회원가입메인페이지\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<docref>docref=\"\S*\") [^>]*sp="한게임/한게임회원/회원가입메인페이지"[^>]*>.*?<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세미만/실명확인")[^>]*>'
    regex_format = '\g<docref> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) docref=\"(?P<docref>\S*)\" sp="(?P<sp>한게임/한게임회원/정회원14세미만/실명확인)"/'
    aggr_key_expr = 'id:echo(id),docref:remain_domain(docref),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k docref=(?P<docref>\S*) id=[^>]*sp=한게임/한게임회원/정회원14세미만/(?P<sp>실명확인)>/'
    aggr_key_expr1 = 'cls:echo(14세미만),docref:echo(docref),step:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_split, regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntJoinUnderAgreeContract( start_time, end_time, progname ):

    regex_split = r'regex -k "(?P<id><K[^>]*bc=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/한게임회원/회원가입메인페이지\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<docref>docref=\"\S*\") [^>]*sp="한게임/한게임회원/회원가입메인페이지"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세미만/실명확인"[^>]*>.*?<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세미만/약관동의")[^>]*>'
    regex_format = '\g<docref> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) docref=\"(?P<docref>\S*)\" sp="(?P<sp>한게임/한게임회원/정회원14세미만/약관동의)"/'
    aggr_key_expr = 'id:echo(id),docref:remain_domain(docref),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k docref=(?P<docref>\S*) id=[^>]*sp=한게임/한게임회원/정회원14세미만/(?P<sp>약관동의)[^>]*>/'
    aggr_key_expr1 = 'cls:echo(14세미만),docref:echo(docref),step:echo(sp)'
    aggr_op_list1 = ['sum(1)','sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd( regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd( aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd( aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd( start_time, end_time, [regex_split, regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1, 'rb') )

 
def cntJoinUnderMeminfo( start_time, end_time, progname ):

    regex_split = r'regex -k "(?P<id><K[^>]*bc=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/한게임회원/회원가입메인페이지\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<docref>docref=\"\S*\") [^>]*sp="한게임/한게임회원/회원가입메인페이지"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세미만/실명확인"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세미만/약관동의"[^>]*>.*?<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세미만/가입정보입력")[^>]*>'
    regex_format = '\g<docref> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) docref=\"(?P<docref>\S*)\" sp="(?P<sp>한게임/한게임회원/정회원14세미만/가입정보입력)"/'
    aggr_key_expr = 'id:echo(id),docref:remain_domain(docref),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k docref=(?P<docref>\S*) id=[^>]*sp=한게임/한게임회원/정회원14세미만/(?P<sp>가입정보입력)>/'
    aggr_key_expr1 = 'cls:echo(14세미만),docref:echo(docref),step:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']
    
    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_split, regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntJoinUnderParentinfo( start_time, end_time, progname ):
   
    regex_split = r'regex -k "(?P<id><K[^>]*bc=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/한게임회원/회원가입메인페이지\"[^>]*>)" "\g<sp>"'
 
    regex_pattern = '<[^>]*(?P<docref>docref=\"\S*\") [^>]*sp="한게임/한게임회원/회원가입메인페이지"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세미만/실명확인"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세미만/약관동의"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세미만/가입정보입력"[^>]*>.*?<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세미만/부모님동의")[^>]*>'
    regex_format = '\g<docref> \g<sp> '
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) docref=\"(?P<docref>\S*)\" sp="(?P<sp>한게임/한게임회원/정회원14세미만/부모님동의)"/'
    aggr_key_expr = 'id:echo(id),docref:remain_domain(docref),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k docref=(?P<docref>\S*) id=[^>]*sp=한게임/한게임회원/정회원14세미만/(?P<sp>부모님동의)>/'
    aggr_key_expr1 = 'cls:echo(14세미만),docref:echo(docref),step:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_split, regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntJoinUnderRegistend( start_time, end_time, progname ):

    regex_split = r'regex -k "(?P<id><K[^>]*bc=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/한게임회원/회원가입메인페이지\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<docref>docref=\"\S*\") [^>]*sp="한게임/한게임회원/회원가입메인페이지"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세미만/실명확인"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세미만/약관동의"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세미만/가입정보입력"[^>]*>.*?<[^>]*sp="한게임/한게임회원/정회원14세미만/부모님동의"[^>]*>.*?<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세미만/가입완료")[^>]*>'

    regex_format = '\g<docref> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) docref=\"(?P<docref>\S*)\" sp="(?P<sp>한게임/한게임회원/정회원14세미만/가입완료)"/'
    aggr_key_expr = 'id:echo(id),docref:remain_domain(docref),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k docref=(?P<docref>\S*) id=[^>]*sp=한게임/한게임회원/정회원14세미만/(?P<sp>가입완료)>/'
    aggr_key_expr1 = 'cls:echo(14세미만),docref:echo(docref),step:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_split, regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntTotSimpleAgreeContract( start_time, end_time, progname ):

    regex_pattern = '<[^>]*(?P<sp>sp="한게임/한게임회원/간편ID/약관동의")[^>]*>'
    regex_format = '\g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) sp="(?P<sp>한게임/한게임회원/간편ID/약관동의)"/'
    aggr_key_expr = 'id:echo(id),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<[^>]*sp=한게임/한게임회원/간편ID/(?P<sp>약관동의)>/'
    aggr_key_expr1 = 'cls:echo(간편),docref:echo(전체),step:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)
    
    hexec_cmd = make_hexec_cmd( start_time, end_time, [regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntTotSimpleMeminfo( start_time, end_time, progname ):
    regex_pattern = '<[^>]*(?P<sp>sp="한게임/한게임회원/간편ID/(정보입력|가입정보입력)")[^>]*>'
    regex_format = '\g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) sp="(?P<sp>한게임/한게임회원/간편ID/(가입정보입력|정보입력))"/'
    aggr_key_expr = 'id:echo(id),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<[^>]*sp=한게임/한게임회원/간편ID/(?P<sp>(가입정보입력|정보입력))>/'
    aggr_key_expr1 = 'cls:echo(간편),docref:echo(전체),step:echo(가입정보입력)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntTotSimpleRegistend( start_time, end_time, progname ):
    regex_pattern = '<[^>]*(?P<sp>sp="한게임/한게임회원/간편ID/가입완료")[^>]*>'
    regex_format = '\g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) sp="(?P<sp>한게임/한게임회원/간편ID/가입완료)"/'
    aggr_key_expr = 'id:echo(id),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<[^>]*sp=한게임/한게임회원/간편ID/(?P<sp>가입완료)>/'
    aggr_key_expr1 = 'cls:echo(간편),docref:echo(전체),step:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntTotOverCheckname( start_time, end_time, progname ):
    regex_pattern = '<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세이상/실명확인")[^>]*>'
    regex_format = '\g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) sp="(?P<sp>한게임/한게임회원/정회원14세이상/실명확인)"/'
    aggr_key_expr = 'id:echo(id),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<[^>]*sp=한게임/한게임회원/정회원14세이상/(?P<sp>실명확인)>/'
    aggr_key_expr1 = 'cls:echo(14세이상),docref:echo(전체),step:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)
    
    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntTotOverAgreeContract( start_time, end_time, progname ):
    regex_pattern = '<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세이상/약관동의")[^>]*>'
    regex_format = '\g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) sp="(?P<sp>한게임/한게임회원/정회원14세이상/약관동의)"/'
    aggr_key_expr = 'id:echo(id),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<[^>]*sp=한게임/한게임회원/정회원14세이상/(?P<sp>약관동의)>/'
    aggr_key_expr1 = 'cls:echo(14세이상),docref:echo(전체),step:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntTotOverCheckauth( start_time, end_time, progname ):
    regex_pattern = '<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세이상/(본인인증|본인확인)")[^>]*>'
    regex_format = '\g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) sp="(?P<sp>한게임/한게임회원/정회원14세이상/(본인확인|본인인증))"/'
    aggr_key_expr = 'id:echo(id),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<[^>]*sp=한게임/한게임회원/정회원14세이상/(?P<sp>(본인확인|본인인증))>/'
    aggr_key_expr1 = 'cls:echo(14세이상),docref:echo(전체),step:echo(본인확인)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd) 

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntTotOverMeminfo( start_time, end_time, progname ):
    regex_pattern = '<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세이상/(가입정보입력|정보입력)")[^>]*>'
    regex_format = '\g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) sp="(?P<sp>한게임/한게임회원/정회원14세이상/(가입정보입력|정보입력))"/'
    aggr_key_expr = 'id:echo(id),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<[^>]*sp=한게임/한게임회원/정회원14세이상/(?P<sp>(가입정보입력|정보입력))>/'
    aggr_key_expr1 = 'cls:echo(14세이상),docref:echo(전체),step:echo(가입정보입력)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntTotOverRegistend( start_time, end_time, progname ):
    regex_pattern = '<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세이상/가입완료")[^>]*>'
    regex_format = '\g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) sp="(?P<sp>한게임/한게임회원/정회원14세이상/가입완료)"/'
    aggr_key_expr = 'id:echo(id),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k[^>]*sp=한게임/한게임회원/정회원14세이상/(?P<sp>가입완료)>/'
    aggr_key_expr1 = 'cls:echo(14세이상),docref:echo(전체),step:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntTotUnderCheckname( start_time, end_time, progname ):
    regex_pattern = '<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세미만/실명확인")[^>]*>'
    regex_format = '\g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) sp="(?P<sp>한게임/한게임회원/정회원14세미만/실명확인)"/'
    aggr_key_expr = 'id:echo(id),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<[^>]*sp=한게임/한게임회원/정회원14세미만/(?P<sp>실명확인)>/'
    aggr_key_expr1 = 'cls:echo(14세미만),docref:echo(전체),step:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntTotUnderAgreeContract( start_time, end_time, progname ):
    regex_pattern = '<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세미만/약관동의")[^>]*>'
    regex_format = '\g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) sp="(?P<sp>한게임/한게임회원/정회원14세미만/약관동의)"/'
    aggr_key_expr = 'id:echo(id),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<[^>]*sp=한게임/한게임회원/정회원14세미만/(?P<sp>약관동의)>/'
    aggr_key_expr1 = 'cls:echo(14세미만),docref:echo(전체),step:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time,end_time,[regex_cmd, aggr_cmd],progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntTotUnderMeminfo( start_time, end_time, progname ):
    regex_pattern = '<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세미만/가입정보입력")[^>]*>'
    regex_format = '\g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) sp="(?P<sp>한게임/한게임회원/정회원14세미만/가입정보입력)"/'
    aggr_key_expr = 'id:echo(id),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<[^>]*sp=한게임/한게임회원/정회원14세미만/(?P<sp>가입정보입력)>/'
    aggr_key_expr1 = 'cls:echo(14세미만),docref:echo(전체),step:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd( start_time, end_time, [regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1, 'rb') )


def cntTotUnderParentinfo( start_time, end_time, progname ):
    regex_pattern = '<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세미만/부모님동의")[^>]*>'
    regex_format = '\g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) sp="(?P<sp>한게임/한게임회원/정회원14세미만/부모님동의)"/'
    aggr_key_expr = 'id:echo(id),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<[^>]*sp=한게임/한게임회원/정회원14세미만/(?P<sp>부모님동의)>/'
    aggr_key_expr1 = 'cls:echo(14세미만),docref:echo(전체),step:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntTotUnderRegistend( start_time, end_time, progname ):
    regex_pattern = '<[^>]*(?P<sp>sp="한게임/한게임회원/정회원14세미만/가입완료")[^>]*>'

    regex_format = '\g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc=(?P<id>[^> ]*) sp="(?P<sp>한게임/한게임회원/정회원14세미만/가입완료)"/'
    aggr_key_expr = 'id:echo(id),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<[^>]*sp=한게임/한게임회원/정회원14세미만/(?P<sp>가입완료)>/'
    aggr_key_expr1 = 'cls:echo(14세미만),docref:echo(전체),step:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, aggr_cmd], progname)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def get_values_from_dict(dictionary, keys):
    def append_value(l, k):
        if not l:
            l = []
        l.append( dictionary[k] )
        return l

    return reduce(append_value, keys, list())

if __name__ == '__main__':
    main()
