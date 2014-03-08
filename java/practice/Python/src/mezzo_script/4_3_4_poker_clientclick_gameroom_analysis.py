#!/usr/bin/env python
# -*- coding: utf_8 -*-
"""
포커클라이언트로그기록분석_게임방클릭관련_박민수
"""

import sys, os
from time import strptime
import getopt
#sys.path.insert(0, os.path.join(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib'))
from mezzo_util import *
import time, datetime
import csv


gameroom_area = ['게이지리필아이템','상단나가기','신고/신고하기','알리미','옵션팝업','이용자정보/추방하기','이용자팝업','잭팟닫기','잭팟열기','족보탭', \
                 '찬스', '창크기최대','채팅send','채팅탭','초대/초대하기','캡쳐','타임','하단나가기','헬프','헬프/게임방법','헬프/자동베팅룰','히스토리', \
                 'help/게임방법','help/자동베팅룰']



hdrs = ['날짜','게임구분','섹션구분','경기장','회원제','등급','영역','PV','UV']
start_time, end_time = parse_timerange(sys.argv[1:])
#target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
#os.system('mkdir -p ' + target_date_path)
#outfile = open(os.path.join(target_date_path,'4_3_4_poker_clientclick_gameroom_analysis.tsv'), 'wb')

outfile = open(os.path.join('./4_3_4_poker_clientclick_gameroom_analysis.tsv'),'wb')

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

    execute_start = time.ctime()

    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow(dict(zip(hdrs,hdrs)))
    
    showing_gameroom = cntShowingGameroom( start_time, end_time, progname )
    click_gameroom_area = cntClickGameroomArea( start_time, end_time, progname )

    execute_end = time.ctime()

    print 'START TIME : ' + execute_start + '\t' + 'END TIME :' + execute_end + '\t' + '4_3_4_poker_clientclick_gameroom_analysis.py\n'
    

# 경기장 게임방 노출 PV/UV
def cntShowingGameroom( start_time, end_time, progname ):
    
    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" "\g<sp>"'

    regex_pattern = '<[^>]*(?P<goodslist>goodslist=[^> ]*)[^>]*p_(hl|7p|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*>.*?<[^>]*(?P<sp>sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/(방입장|방_입장)")[^>]*>'
    regex_format = '\g<ground> \g<goodslist> \g<p_level> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" ground="(?P<ground>[^> ]*)" goodslist="(?P<goodslist>[^> ]*)" p_level="(?P<p_level>\d*)" sp="한게임/포커게임/(?P<cls>7포커|하이로우|로우바둑이|맞포커)/클라이언트/(?P<sp>(방입장|방_입장))"/'
    aggr_key_expr = 'id:echo(id),ground:echo(ground),goodslist:client_membership_type(goodslist),p_level:echo(p_level),cls:echo(cls),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) goodslist=(?P<goodslist>[^> ]*) ground=(?P<ground>[^> ]*) id=[^> ]* p_level=(?P<p_level>\d*) sp=(?P<sp>(방입장|방_입장))>/'
    aggr_key_expr1 = 'cls:echo(cls),section_cls:echo(게임방),goodslist:echo(goodslist),ground:echo(ground),p_level:echo(p_level),sp:echo(방입장)'
    aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

    regex_cmd1 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, aggr_cmd], progname, True)
    os.system( hexec_cmd )

    print '방입장 노드 처리중...'

    write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd1 )


# 게임방 영역 클릭 PV/UV
#   - 게이지리필아이템, 상단나가기, 신고하기, 알리미, 이용자정보/추방하기, 이용자팝업, 잭팟닫기, 잭팟열기, 족보탭, 찬스, 창크기최대, 채팅탭, 초대하기, 캡쳐
#   - 타임, 하단나가기, 헬프, 게임방법, 자동베팅룰, 히스토리

def cntClickGameroomArea( start_time, end_time, progname ):

    regex_cmd = r'regex -k "(?P<id><K[^>]*li=[^>]*>)" -l "\g<id>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" "\g<sp>"'

    for gameroom_re in gameroom_area:
        regex_cmd1 = r'regex -k "(?P<pre><K[^>]*li=[^>]*><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실\"[^>]*>)" -l "\g<pre>" -s "(?P<sp><[^>]*sp=\"한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/' + gameroom_re +r'\"[^>]*>)" "\g<sp>"'
        
        regex_pattern = '<[^>]*(?P<goodslist>goodslist=[^> ]*)[^>]*p_(hl|7p|ba|du)_(?P<ground>ground="\d*")[^>]*(?P<p_level>p_level="\d*")[^>]*sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/경기장대기실"[^>]*><[^>]*(?P<sp>sp="한게임/포커게임/(7포커|하이로우|로우바둑이|맞포커)/클라이언트/게임방/(' + gameroom_re  + ')")[^>]*>'
        regex_format = '\g<ground> \g<goodslist> \g<p_level> \g<sp>'
        regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
        regex_key_format = '\g<id> '

        aggr_key_pattern = '/li="(?P<id>[^> ]*)" ground="(?P<ground>[^> ]*)" goodslist="(?P<goodslist>[^> ]*)" p_level="(?P<p_level>\d*)" sp="한게임/포커게임/(?P<cls>(7포커|하이로우|로우바둑이|맞포커))/클라이언트/(?P<sp>게임방/(' + gameroom_re + '))"/'
        aggr_key_expr = 'id:echo(id),ground:echo(ground),goodslist:client_membership_type(goodslist),p_level:echo(p_level),cls:echo(cls),sp:echo(sp)'
        aggr_op_list = ['sum(1)']

        aggr_key_pattern1 = '/<k cls=(?P<cls>(7포커|하이로우|로우바둑이|맞포커)) goodslist=(?P<goodslist>[^> ]*) ground=(?P<ground>[^> ]*) id=[^> ]* p_level=(?P<p_level>\d*) sp=게임방/(?P<sp>('+ gameroom_re  + '))>/'
        aggr_key_expr1 = 'cls:echo(cls),section_cls:echo(게임방),goodslist:echo(goodslist),ground:echo(ground),p_level:echo(p_level),sp:echo(sp)'
        aggr_op_list1 = ['sum(1)', 'sum(/value=(?P<pv>\d+)/)']

        regex_cmd2 = make_regex_cmd(regex_pattern,regex_format,regex_key_pattern,regex_key_format)
        aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
        aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

        hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, regex_cmd2, aggr_cmd], progname, True)
        os.system( hexec_cmd )
        
        print str(gameroom_re) + '처리중...'

        write_to_file( 'zcat ' + progname + '.gz | ' + aggr_cmd1 )

def write_to_file( _hexec_cmd ):

    optlist = []
    args = sys.argv[1:]
    oplist, args = getopt.getopt( args, 'd' )
    start_time, end_time = parse_timerange(args)

    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')

    for row in create_tsvreader(os.popen(_hexec_cmd,'rb')):
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
