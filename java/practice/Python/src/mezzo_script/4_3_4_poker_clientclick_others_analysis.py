#!/usr/bin/env python
# -*- coding: utf_8 -*-
"""
포커클라이언트로그기록분석_기타영역클릭_20080521_박민수
"""

import sys, os
from time import strptime
import getopt
#sys.path.insert(0, os.path.join(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib'))
from mezzo_util import *
import time, datetime
import csv

hdrs = ['날짜','게임구분','섹션구분','경기장','회원제','등급','영역','PV','UV']
start_time, end_time = parse_timerange(sys.argv[1:])
#target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
#os.system('mkdir -p ' + target_date_path)
#outfile = open(os.path.join(target_date_path,'4_3_4_poker_clientclick_others_analysis.tsv'), 'wb')
outfile = open(os.path.join('./4_3_4_poker_clientclick_others_analysis.tsv'), 'wb')

def usage(program):
    print >> sys.stderr, "\nUsage: " + program + "\t[-d] [<date string>]"
    print >> sys.stderr, "\t\t-d\tDebug mode\n\n"

def main():
    optlist = []
    args = sys.argv[1:]

    progname = os.path.basename(sys.argv[0])
    x = progname.rfind('.')
    if x > 0: progname = progname[:x]

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
    
    execute_start = time.ctime()

    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow(dict(zip(hdrs,hdrs)))
    
    end_popup = cntEndPopup( start_time, end_time, progname )
    click_alimi_time = cntClickAlimiTime( start_time, end_time, progname )
    click_alimi_out = cntClickAlimiOut( start_time, end_time, progname )
    click_alimi_outtime = cntClickAlimiOutTime( start_time, end_time, progname )
    click_option_help = cntClickOptionHelp( start_time, end_time, progname )
    click_option_sound = cntClickOptionSound( start_time, end_time, progname )
    click_option_background = cntClickOptionBackground( start_time, end_time, progname )
    click_option_chatreject = cntClickOptionChatreject( start_time, end_time, progname )
    click_option_appchatreject = cntClickOptionAppChatreject( start_time, end_time, progname )
    click_option_voice = cntClickOptionVoice( start_time, end_time, progname )
    click_makeroom_secret = cntClickMakeroomSecret( start_time, end_time, progname )
    click_makeroom_super = cntClickMakeroomSuper( start_time, end_time, progname )
    click_makeroom_gametype = cntClickMakeroomGametype( start_time, end_time, progname )
    click_makeroom_betrule = cntClickMakeroomBetrule( start_time, end_time, progname )

    execute_end = time.ctime()
    print 'START TIME : ' + execute_start + '\t' + 'END TIME : ' + execute_end + '\t' + 'File creation Success : 4_3_4_poker_client_click_other_analysis.tsv'
    
        
# 종료팝업 Click PV/UV
def cntEndPopup( statr_time, end_time, progname ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" "\g<sp>"'

    regex_cmd1 = r'regex -k "(?P<pre><K[^>]*li=[^>]*><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" -l "\g<pre>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/종료팝업/게임종료_(예|아니오)\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<goodslist>goodslist=[^> ]*)[^>]*p_(hl|7p|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*>(?!<[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*>).*?<[^>]*(?P<sp>sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/종료팝업/게임종료_(예|아니오)")[^>]*>'
    regex_format = '\g<ground> \g<goodslist> \g<p_level> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" ground="(?P<ground>[^> ]*)" goodslist="(?P<goodslist>[^> ]*)" p_level="(?P<p_level>\d*)" sp="한게임/포커게임/(?P<cls>(7포커|하이로우|로우바둑이|맞포커))/클라이언트/(?P<sp>종료팝업/(게임종료_예|게임종료_아니오))"/'
    aggr_key_expr = 'id:echo(id),ground:echo(ground),goodslist:client_membership_type(goodslist),p_level:echo(p_level),cls:echo(cls),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) goodslist=(?P<goodslist>[^> ]*) ground=(?P<ground>[^> ]*) id=[^> ]* p_level=(?P<p_level>\d*) sp=(?P<sp>종료팝업/(게임종료_예|게임종료_아니오))>/'
    aggr_key_expr1 = 'cls:echo(cls),section_cls:echo(기타),goodslist:echo(goodslist),ground:echo(ground),p_level:echo(p_level),sp:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd2 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, regex_cmd2, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    print '종료팝업 처리중...'
    
    write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd1 )
    

# 알리미 팝업의 시간알리미 영역 클릭에 대한 PV/UV
def cntClickAlimiTime( start_time, end_time, progname ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" "\g<sp>"'
    regex_cmd1 = r'regex -k "(?P<pre><K[^>]*li=[^>]*><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" -l "\g<pre>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/알리미팝업\"[^>]*>)" "\g<sp>"'
 
    regex_pattern = '<[^>]*(?P<goodslist>goodslist=[^> ]*)[^>]*p_(hl|7p|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*><[^>]*alimitype="(?P<alimitype>[^>, ]*)"[^>]*sp="(?P<sp>한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/알리미팝업)"[^>]*>'
    regex_format = '\g<ground> \g<goodslist> \g<p_level> sp=\g<sp>/시간알리미\g<alimitype>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" ground="(?P<ground>[^> ]*)" goodslist="(?P<goodslist>[^> ]*)" p_level="(?P<p_level>\d*)" sp=한게임/포커게임/(?P<cls>(7포커|하이로우|로우바둑이|맞포커))/클라이언트/(?P<sp>게임방/알리미팝업[^> ]*)/'
    aggr_key_expr = 'id:echo(id),ground:echo(ground),goodslist:client_membership_type(goodslist),p_level:echo(p_level),cls:echo(cls),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) goodslist=(?P<goodslist>[^> ]*) ground=(?P<ground>[^> ]*) id=[^> ]* p_level=(?P<p_level>\d*) sp=게임방/알리미팝업/(?P<sp>[^> ]*)>/'
    aggr_key_expr1 = 'cls:echo(cls),section_cls:echo(알리미팝업),goodslist:echo(goodslist),ground:echo(ground),p_level:echo(p_level),sp:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd2 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)
    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, regex_cmd2, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    print '시간알리미 처리중...'

    write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd1 )


# 알리미 팝업의 나가기 예약 영역 클릭에 대한 PV/UV

def cntClickAlimiOut( start_time, end_time, progname ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" "\g<sp>"'

    regex_cmd1 = r'regex -k "(?P<pre><K[^>]*li=[^>]*><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" -l "\g<pre>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/알리미팝업\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<goodslist>goodslist=[^> ]*)[^>]*p_(hl|7p|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*><[^>]*outoption="(?P<outoption>[^>, ]*)"[^>]*sp="(?P<sp>한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/알리미팝업)"[^>]*>'
    regex_format = '\g<ground> \g<goodslist> \g<p_level> sp=\g<sp>/나가기예약선택\g<outoption>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" ground="(?P<ground>[^> ]*)" goodslist="(?P<goodslist>[^> ]*)" p_level="(?P<p_level>\d*)" sp=한게임/포커게임/(?P<cls>(7포커|하이로우|로우바둑이|맞포커))/클라이언트/(?P<sp>게임방/알리미팝업[^> ]*)/'
    aggr_key_expr = 'id:echo(id),ground:echo(ground),goodslist:client_membership_type(goodslist),p_level:echo(p_level),cls:echo(cls),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) goodslist=(?P<goodslist>[^> ]*) ground=(?P<ground>[^> ]*) id=[^> ]* p_level=(?P<p_level>\d*) sp=게임방/알리미팝업/(?P<sp>[^> ]*)>/'
    aggr_key_expr1 = 'cls:echo(cls),section_cls:echo(알리미팝업),goodslist:echo(goodslist),ground:echo(ground),p_level:echo(p_level),sp:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd2 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, regex_cmd2, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    print '나가기 예약 처리중...'

    write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd1)


# 알리미 팝업의 나가기 예약시간 영역에 클릭에 대한 PV/UV

def cntClickAlimiOutTime( start_time, end_time, progname ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" "\g<sp>"'

    regex_cmd1 = r'regex -k "(?P<pre><K[^>]*li=[^>]*><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" -l "\g<pre>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/알리미팝업\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<goodslist>goodslist=[^> ]*)[^>]*p_(hl|7p|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*><[^>]*outtime="(?P<outtime>[^>, ]*)"[^>]*sp="(?P<sp>한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/알리미팝업)"[^>]*>'
    regex_format = '\g<ground> \g<goodslist> \g<p_level> sp=\g<sp>/나가기예약시간\g<outtime>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" ground="(?P<ground>[^> ]*)" goodslist="(?P<goodslist>[^> ]*)" p_level="(?P<p_level>\d*)" sp=한게임/포커게임/(?P<cls>(7포커|하이로우|로우바둑이|맞포커))/클라이언트/(?P<sp>게임방/알리미팝업[^> ]*)/'
    aggr_key_expr = 'id:echo(id),ground:echo(ground),goodslist:client_membership_type(goodslist),p_level:echo(p_level),cls:echo(cls),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) goodslist=(?P<goodslist>[^> ]*) ground=(?P<ground>[^> ]*) id=[^> ]* p_level=(?P<p_level>\d*) sp=게임방/알리미팝업/(?P<sp>[^> ]*)>/'
    aggr_key_expr1 = 'cls:echo(cls),section_cls:echo(알리미팝업),goodslist:echo(goodslist),ground:echo(ground),p_level:echo(p_level),sp:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd2 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, regex_cmd2, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    print '나가기 예약시간 처리중...'

    write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd1 )

# 옵션 팝업의 포커도우미사용여부 클릭에 대한 PV/UV

def cntClickOptionHelp( start_time, end_time, progname ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" "\g<sp>"'

    regex_cmd1 = r'regex -k "(?P<pre><K[^>]*li=[^>]*><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" -l "\g<pre>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/옵션팝업\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<goodslist>goodslist=[^> ]*)[^>]*p_(hl|7p|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*><[^>]*pokerhelpyn="(?P<pokerhelpyn>[^>, ]*)"[^>]*sp="(?P<sp>한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/옵션팝업)"[^>]*>'
    regex_format = '\g<ground> \g<goodslist> \g<p_level> sp=\g<sp>/포커도우미/\g<pokerhelpyn>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" ground="(?P<ground>[^> ]*)" goodslist="(?P<goodslist>[^> ]*)" p_level="(?P<p_level>\d*)" sp=한게임/포커게임/(?P<cls>(7포커|하이로우|로우바둑이|맞포커))/클라이언트/(?P<sp>게임방/옵션팝업/포커도우미[^> ]*)/'
    aggr_key_expr = 'id:echo(id),ground:echo(ground),goodslist:client_membership_type(goodslist),p_level:echo(p_level),cls:echo(cls),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) goodslist=(?P<goodslist>[^> ]*) ground=(?P<ground>[^> ]*) id=[^> ]* p_level=(?P<p_level>\d*) sp=게임방/옵션팝업/(?P<sp>포커도우미[^> ]*)>/'
    aggr_key_expr1 = 'cls:echo(cls),section_cls:echo(옵션팝업),goodslist:echo(goodslist),ground:echo(ground),p_level:echo(p_level),sp:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd2 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, regex_cmd2, aggr_cmd],progname, True)
    os.system(hexec_cmd)

    print '포커도우미 사용여부 처리중...'

    write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd1 )

# 옵션 팝업의 사운드효과 체크여부에 대한 PV/UV
def cntClickOptionSound( start_time, end_time, progname ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" "\g<sp>"'

    regex_cmd1 = r'regex -k "(?P<pre><K[^>]*li=[^>]*><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" -l "\g<pre>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/옵션팝업\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<goodslist>goodslist=[^> ]*)[^>]*p_(hl|7p|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*><[^>]*soundyn="(?P<soundyn>(y|n))"[^>]*sp="(?P<sp>한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/옵션팝업)"[^>]*>'
    regex_format = '\g<ground> \g<goodslist> \g<p_level> sp=\g<sp>/사운드효과/\g<soundyn>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" ground="(?P<ground>[^> ]*)" goodslist="(?P<goodslist>[^> ]*)" p_level="(?P<p_level>\d*)" sp=한게임/포커게임/(?P<cls>(7포커|하이로우|로우바둑이|맞포커))/클라이언트/(?P<sp>게임방/옵션팝업/사운드효과[^> ]*)/'
    aggr_key_expr = 'id:echo(id),ground:echo(ground),goodslist:client_membership_type(goodslist),p_level:echo(p_level),cls:echo(cls),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) goodslist=(?P<goodslist>[^> ]*) ground=(?P<ground>[^> ]*) id=[^> ]* p_level=(?P<p_level>\d*) sp=게임방/옵션팝업/(?P<sp>사운드효과[^> ]*)>/'
    aggr_key_expr1 = 'cls:echo(cls),section_cls:echo(옵션팝업),goodslist:echo(goodslist),ground:echo(ground),p_level:echo(p_level),sp:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd2 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, regex_cmd2, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    print '사운드 효과 사용여부 처리중...'

    write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd1 )

    
# 옵션 팝업의 배경음악 체크여부에 대한 PV/UV
def cntClickOptionBackground( start_time, end_time, progname ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" "\g<sp>"'

    regex_cmd1 = r'regex -k "(?P<pre><K[^>]*li=[^>]*><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" -l "\g<pre>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/옵션팝업\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<goodslist>goodslist=[^> ]*)[^>]*p_(hl|7p|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*><[^>]*backgroundyn="(?P<backgroundyn>(y|n))"[^>]*sp="(?P<sp>한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/옵션팝업)"[^>]*>'
    regex_format = '\g<ground> \g<goodslist> \g<p_level> sp=\g<sp>/배경음악/\g<backgroundyn>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" ground="(?P<ground>[^> ]*)" goodslist="(?P<goodslist>[^> ]*)" p_level="(?P<p_level>\d*)" sp=한게임/포커게임/(?P<cls>(7포커|하이로우|로우바둑이|맞포커))/클라이언트/(?P<sp>게임방/옵션팝업/배경음악[^> ]*)/'
    aggr_key_expr = 'id:echo(id),ground:echo(ground),goodslist:client_membership_type(goodslist),p_level:echo(p_level),cls:echo(cls),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) goodslist=(?P<goodslist>[^> ]*) ground=(?P<ground>[^> ]*) id=[^> ]* p_level=(?P<p_level>\d*) sp=게임방/옵션팝업/(?P<sp>배경음악[^> ]*)>/'
    aggr_key_expr1 = 'cls:echo(cls),section_cls:echo(옵션팝업),goodslist:echo(goodslist),ground:echo(ground),p_level:echo(p_level),sp:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd2 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, regex_cmd2, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    print '배경음악 사용여부 처리중...'

    write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd1)


# 옵션 팝업의 대화거부 체크여부에 대한 PV/UV
def cntClickOptionChatreject( start_time, end_time, progname ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" "\g<sp>"'

    regex_cmd1 = r'regex -k "(?P<pre><K[^>]*li=[^>]*><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" -l "\g<pre>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/옵션팝업\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<goodslist>goodslist=[^> ]*)[^>]*p_(hl|7p|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*><[^>]*chatrejectyn="(?P<chatrejectyn>(y|n))"[^>]*sp="(?P<sp>한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/옵션팝업)"[^>]*>'
    regex_format = '\g<ground> \g<goodslist> \g<p_level> sp=\g<sp>/대화거부/\g<chatrejectyn>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" ground="(?P<ground>[^> ]*)" goodslist="(?P<goodslist>[^> ]*)" p_level="(?P<p_level>\d*)" sp=한게임/포커게임/(?P<cls>(7포커|하이로우|로우바둑이|맞포커))/클라이언트/(?P<sp>게임방/옵션팝업/대화거부[^> ]*)/'
    aggr_key_expr = 'id:echo(id),ground:echo(ground),goodslist:client_membership_type(goodslist),p_level:echo(p_level),cls:echo(cls),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) goodslist=(?P<goodslist>[^> ]*) ground=(?P<ground>[^> ]*) id=[^> ]* p_level=(?P<p_level>\d*) sp=게임방/옵션팝업/(?P<sp>대화거부[^> ]*)>/'
    aggr_key_expr1 = 'cls:echo(cls),section_cls:echo(옵션팝업),goodslist:echo(goodslist),ground:echo(ground),p_level:echo(p_level),sp:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd2 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)
    
    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, regex_cmd2, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    print '대화거부 사용여부 처리중...'

    write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd1 )


# 옵션 팝업의 관전자 대화거부 체크여부에 대한 PV/UV
def cntClickOptionAppChatreject( start_time, end_time, progname ):
    
    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" "\g<sp>"'

    regex_cmd1 = r'regex -k "(?P<pre><K[^>]*li=[^>]*><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" -l "\g<pre>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/옵션팝업\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<goodslist>goodslist=[^> ]*)[^>]*p_(hl|7p|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*><[^>]*obschatrejectyn="(?P<obschatrejectyn>(y|n))"[^>]*sp="(?P<sp>한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/옵션팝업)"[^>]*>'
    regex_format = '\g<ground> \g<goodslist> \g<p_level> sp=\g<sp>/관전자대화거부/\g<obschatrejectyn>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" ground="(?P<ground>[^> ]*)" goodslist="(?P<goodslist>[^> ]*)" p_level="(?P<p_level>\d*)" sp=한게임/포커게임/(?P<cls>(7포커|하이로우|로우바둑이|맞포커))/클라이언트/(?P<sp>게임방/옵션팝업/관전자대화거부[^> ]*)/'
    aggr_key_expr = 'id:echo(id),ground:echo(ground),goodslist:client_membership_type(goodslist),p_level:echo(p_level),cls:echo(cls),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) goodslist=(?P<goodslist>[^> ]*) ground=(?P<ground>[^> ]*) id=[^> ]* p_level=(?P<p_level>\d*) sp=게임방/옵션팝업/(?P<sp>관전자대화거부[^> ]*)>/'
    aggr_key_expr1 = 'cls:echo(cls),section_cls:echo(옵션팝업),goodslist:echo(goodslist),ground:echo(ground),p_level:echo(p_level),sp:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd2 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, regex_cmd2, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    print '관전자 대화거부 사용여부 처리중...'

    write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd1 )


# 옵션 팝업의 보이스 선택에 대한 구분 PV/UV
def cntClickOptionVoice( start_time, end_time, progname ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" "\g<sp>"'
    
    regex_cmd1 = r'regex -k "(?P<pre><K[^>]*li=[^>]*><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" -l "\g<pre>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/옵션팝업\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<goodslist>goodslist=[^> ]*)[^>]*p_(hl|7p|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*><[^>]*sp="(?P<sp>한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/옵션팝업)"[^>]*voicetype="(?P<voicetype>[^>, ]*)"[^>]*>'
    regex_format = '\g<ground> \g<goodslist> \g<p_level> sp=\g<sp>/보이스선택/\g<voicetype>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" ground="(?P<ground>[^> ]*)" goodslist="(?P<goodslist>[^> ]*)" p_level="(?P<p_level>\d*)" sp=한게임/포커게임/(?P<cls>(7포커|하이로우|로우바둑이|맞포커))/클라이언트/(?P<sp>게임방/옵션팝업/보이스선택/[^> ]*)/'
    aggr_key_expr = 'id:echo(id),ground:echo(ground),goodslist:client_membership_type(goodslist),p_level:echo(p_level),cls:echo(cls),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) goodslist=(?P<goodslist>[^> ]*) ground=(?P<ground>[^> ]*) id=[^> ]* p_level=(?P<p_level>\d*) sp=게임방/옵션팝업/(?P<sp>보이스선택/[^> ]*)>/'
    aggr_key_expr1 = 'cls:echo(cls),section_cls:echo(옵션팝업),goodslist:echo(goodslist),ground:echo(ground),p_level:echo(p_level),sp:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd2 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, regex_cmd2, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    print '보이스 타입 선택여부 처리중...'

    write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd1 )

# 방만들기 팝업의 비밀방여부 영역에 대한 선택 PV/UV
def cntClickMakeroomSecret( start_time, end_time, progname ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" "\g<sp>"'

    regex_cmd1 = r'regex -k "(?P<pre><K[^>]*li=[^>]*><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" -l "\g<pre>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/대기실/방만들기팝업\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<goodslist>goodslist=[^> ]*)[^>]*p_(hl|7p|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*><[^>]*privacyyn="(?P<privacyyn>[^>, ]*)"[^>]*sp="(?P<sp>한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/대기실/방만들기팝업)"[^>]*>'
    regex_format = '\g<ground> \g<goodslist> \g<p_level> sp=\g<sp>/비밀방/\g<privacyyn>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" ground="(?P<ground>[^> ]*)" goodslist="(?P<goodslist>[^> ]*)" p_level="(?P<p_level>\d*)" sp=한게임/포커게임/(?P<cls>(7포커|하이로우|로우바둑이|맞포커))/클라이언트/(?P<sp>대기실/방만들기팝업/비밀방[^> ]*)/'
    aggr_key_expr = 'id:echo(id),ground:echo(ground),goodslist:client_membership_type(goodslist),p_level:echo(p_level),cls:echo(cls),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) goodslist=(?P<goodslist>[^> ]*) ground=(?P<ground>[^> ]*) id=[^> ]* p_level=(?P<p_level>\d*) sp=대기실/방만들기팝업/(?P<sp>비밀방[^> ]*)>/'
    aggr_key_expr1 = 'cls:echo(cls),section_cls:echo(방만들기팝업),goodslist:echo(goodslist),ground:echo(ground),p_level:echo(p_level),sp:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd2 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, regex_cmd2, aggr_cmd], progname, True)
    os.system(hexec_cmd)
    print '비밀방 여부 처리중...'

    write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd1 )

#방만들기 팝업의 포커슈퍼방 여부 영역에 대한 선택 PV/UV
def cntClickMakeroomSuper( start_time, end_time, progname ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" "\g<sp>"'   
    regex_cmd1 = r'regex -k "(?P<pre><K[^>]*li=[^>]*><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" -l "\g<pre>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/대기실/방만들기팝업\"[^>]*>)" "\g<sp>"'
 
    regex_pattern = '<[^>]*(?P<goodslist>goodslist=[^> ]*)[^>]*p_(hl|7p|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*><[^>]*roomtype="(?P<roomtype>[^>, ]*)[^>]*sp="(?P<sp>한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/대기실/방만들기팝업)"[^>]*>'
    regex_format = '\g<ground> \g<goodslist> \g<p_level> sp=\g<sp>/포커슈퍼방/\g<roomtype>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" ground="(?P<ground>[^> ]*)" goodslist="(?P<goodslist>[^> ]*)" p_level="(?P<p_level>\d*)" sp=한게임/포커게임/(?P<cls>(7포커|하이로우|로우바둑이|맞포커))/클라이언트/(?P<sp>대기실/방만들기팝업/포커슈퍼방/[^> ]*)/'
    aggr_key_expr = 'id:echo(id),ground:echo(ground),goodslist:client_membership_type(goodslist),p_level:echo(p_level),cls:echo(cls),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) goodslist=(?P<goodslist>[^> ]*) ground=(?P<ground>[^> ]*) id=[^> ]* p_level=(?P<p_level>\d*) sp=대기실/방만들기팝업/(?P<sp>포커슈퍼방/[^> ]*)>/'
    aggr_key_expr1 = 'cls:echo(cls),section_cls:echo(방만들기팝업),goodslist:echo(goodslist),ground:echo(ground),p_level:echo(p_level),sp:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd2 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    print '포커 슈퍼방 처리중...'

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, regex_cmd2, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd1) 


#방만들기 팝업의 게임타입 영역에 대한 선택 PV/UV
def cntClickMakeroomGametype( start_time, end_time, progname ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" "\g<sp>"'

    regex_cmd1 = r'regex -k "(?P<pre><K[^>]*li=[^>]*><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" -l "\g<pre>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|맞포커|로우바둑이)/클라이언트/대기실/방만들기팝업\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<goodslist>goodslist=[^> ]*)[^>]*p_(hl|7p|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*><[^>]*gametype=(?P<gametype>[^>, ]*)[^>]*sp="(?P<sp>한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/대기실/방만들기팝업)"[^>]*>'
    regex_format = '\g<ground> \g<goodslist> \g<p_level> sp=\g<sp>/게임타입/\g<gametype>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" ground="(?P<ground>[^> ]*)" goodslist="(?P<goodslist>[^> ]*)" p_level="(?P<p_level>\d*)" sp=한게임/포커게임/(?P<cls>(7포커|하이로우|맞포커|로우바둑이))/클라이언트/(?P<sp>대기실/방만들기팝업/게임타입/[^> ]*)/'
    aggr_key_expr = 'id:echo(id),ground:echo(ground),goodslist:client_membership_type(goodslist),p_level:echo(p_level),cls:echo(cls),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k cls=(?P<cls>(7포커|하이로우|맞포커|로우바둑이)) goodslist=(?P<goodslist>[^> ]*) ground=(?P<ground>[^> ]*) id=[^> ]* p_level=(?P<p_level>\d*) sp=대기실/방만들기팝업/(?P<sp>게임타입/[^> ]*)>/'
    aggr_key_expr1 = 'cls:echo(cls),section_cls:echo(방만들기팝업),goodslist:echo(goodslist),ground:echo(ground),p_level:echo(p_level),sp:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd2 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    print '게임타입 역역 처리중...'

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, regex_cmd2, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd1 )


#방만들기 팝업의 베팅룰 영역에 대한 선택 PV/UV
def cntClickMakeroomBetrule( start_time, end_time, progname ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" "\g<sp>"'

    regex_cmd1 = r'regex -k "(?P<pre><K[^>]*li=[^>]*><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" -l "\g<pre>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/대기실/방만들기팝업\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<goodslist>goodslist=[^> ]*)[^>]*p_(hl|7p|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*><[^>]*betrule=(?P<betrule>[^> ]*)"[^>]*sp="(?P<sp>한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/대기실/방만들기팝업)"[^>]*>'
    regex_format = '\g<ground> \g<goodslist> \g<p_level> sp=\g<sp>/베팅룰/\g<betrule>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" ground="(?P<ground>[^> ]*)" goodslist="(?P<goodslist>[^> ]*)" p_level="(?P<p_level>\d*)" sp=한게임/포커게임/(?P<cls>(7포커|하이로우|로우바둑이|맞포커))/클라이언트/(?P<sp>대기실/방만들기팝업/베팅룰/[^> ]*)/'
    aggr_key_expr = 'id:echo(id),ground:echo(ground),goodslist:client_membership_type(goodslist),p_level:echo(p_level),cls:echo(cls),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) goodslist=(?P<goodslist>[^> ]*) ground=(?P<ground>[^> ]*) id=[^> ]* p_level=(?P<p_level>\d*) sp=대기실/방만들기팝업/(?P<sp>베팅룰/[^> ]*)>/'
    aggr_key_expr1 = 'cls:echo(cls),section_cls:echo(방만들기팝업),goodslist:echo(goodslist),ground:echo(ground),p_level:echo(p_level),sp:echo(sp)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd2 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    print '베팅룰 영역 처리중...'

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, regex_cmd2, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd1 )


def write_to_file( _gexec_cmd ):

    optlist = []
    args = sys.argv[1:]
    oplist, args = getopt.getopt( args, 'd' )
    start_time, end_time = parse_timerange(args)

    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')

    for row in create_tsvreader(os.popen(_gexec_cmd,'rb')):
        new_row = {}
        new_row['날짜'] = start_time.strftime('%Y-%m-%d')
        new_row['게임구분'] = row['k.cls']
        new_row['섹션구분'] = row['k.section_cls']
        new_row['경기장'] = row['k.ground']
        new_row['회원제'] = row['k.goodslist']
        new_row['등급'] = row['k.p_level']
        new_row['영역'] = row['k.sp']
        new_row['PV'] = row['sum.pv']
        new_row['UV'] = row['sum.value']

        writer.writerow(new_row)


if __name__ == '__main__':
    main()
