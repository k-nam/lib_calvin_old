#!/usr/bin/env python
# -*- coding: utf_8 -*-
"""
금칙어KPI_금칙어출현현황_박민수
"""

import sys, os
import time, datetime
from time import strptime
import getopt
#sys.path.insert(0,os.path.join(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib'))
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
#    target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
#    os.system('mkdir -p ' + target_date_path)
#    outfile = open(os.path.join(target_date_path, '9_4_2_using_abuse_word.tsv'), 'wb')

    outfile = open('./9_4_2_using_abuse_word.txt','wb')
   
    
    start = time.ctime()
    print 'Start To: ', start

    word_tot = cntUsingAbusingWordTot(start_time, end_time, progname)                 # 금칙어 출현한 건수 - 전체 기준
    word_group = cntUsingAbusingWordGamegroup(start_time, end_time, progname)         # 금칙어 출현한 건수 - 게임그룹 기준
    word_game = cntUsingAbusingWordGame(start_time, end_time, progname)               # 금칙어 출현한 건수 - 게임 기준
    wordnum_tot = cntAbusingWordNumTot(start_time, end_time, progname)                # 금칙어 단어 개수 - 전체 기준
    wordnum_group = cntAbusingWordNumGamegroup(start_time, end_time, progname)        # 금칙어 단어 개수 - 게임그룹 기준
    wordnum_game = cntAbusingWordNumGame(start_time, end_time, progname)              # 금칙어 단어 개수 - 게임 기준
    wordusing_tot = cntAbusingWordUsingIdTot(start_time, end_time, progname)          # 금칙어 사용ID - 전체 기준
    wordusing_group = cntAbusingWordUsingIdGamegroup(start_time, end_time, progname)  # 금칙어 사용ID - 게임그룹 기준
    wordusing_game = cntAbusingWordUsingIdGame(start_time, end_time, progname)        # 금칙어 사용ID - 게임 기준

    hdrs = ['날짜','게임군','게임','금칙어_사용건수','금칙어_단어개수','금칙어_사용ID']

    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow(dict(zip(hdrs,hdrs)))
 
    temp = {}

    # 전체
    for row in word_tot:
        game = row['k.game']
        game_group = row['k.game_group']
        temp[(game_group,game)] = ['0','0','0']
        temp[(game_group,game)][0] = str(row['sum.value'])
    
    for row in wordnum_tot:
        game = row['k.game']
        game_group = row['k.game_group']
        temp[(game_group,game)][1] = str(row['sum.value'])

    for row in wordusing_tot:
        game = row['k.game']
        game_group = row['k.game_group']
        temp[(game_group,game)][2] = str(row['sum.value'])

    # 게임군
    for row in word_group:
        game = row['k.game']
        game_group = row['k.game_group']
        temp[(game_group,game)] = ['0','0','0']
        temp[(game_group,game)][0] = row['sum.value']

    for row in wordnum_group:
        game = row['k.game']
        game_group = row['k.game_group']
        temp[(game_group,game)][1] = row['sum.value']
    for row in wordusing_group:
        game = row['k.game']
        game_group = row['k.game_group']
        temp[(game_group,game)][2] = row['sum.value']

    # 게임
    for row in word_game:
        game = row['k.game']
        game_group = row['k.game_group']
        temp[(game_group,game)] = ['0','0','0']
        temp[(game_group,game)][0] = row['sum.value']

    for row in wordnum_game:
        game = row['k.game']
        game_group = row['k.game_group']
        temp[(game_group,game)][1] = row['sum.value']

    for row in wordusing_game:
        game = row['k.game']
        game_group = row['k.game_group']
        temp[(game_group,game)][2] = row['sum.value']

    # 기록
    for game_group,game in temp.keys():
        result = dict()
        result['날짜'] = start_time.strftime('%Y-%m-%d')
        result['게임군'] = game_group
        result['게임'] = game
        result['금칙어_사용건수'] = temp[(game_group,game)][0]
        result['금칙어_단어개수'] = temp[(game_group,game)][1]
        result['금칙어_사용ID'] = temp[(game_group,game)][2]
        
        writer.writerow(result)

    end = time.ctime()
    print 'End To: ', end

   

# 금칙어가 출현한 건수
def cntUsingAbusingWordTot( start_time, end_time, progname ):

    regex_pattern = '<[^>]*(?P<sp>sp="한게임/채팅룸/금칙어")[^>]*>'
    regex_format = '\g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/sp="(?P<sp>한게임/채팅룸/금칙어)"/'
    aggr_key_expr = 'sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/sp=(?P<sp>한게임/채팅룸/금칙어)/'
    aggr_key_expr1 = 'sp:echo(sp),game_group:echo(전체),game:echo(전체)'
    aggr_op_list1 = ['sum(/value=(?P<value>\d*)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd], progname)
    print hexec_cmd
    os.system(hexec_cmd)
    sys.exit()    
    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )

    
def cntUsingAbusingWordGamegroup( start_time, end_time, progname ):
    regex_pattern = '<[^>]*game_cls="(?P<game_cls>[^> ]*)" sp="한게임/채팅룸/금칙어"[^>]*>'
    regex_format = 'game=\m<game>'
    regex_map_pattern = 'game=changing_gamelist(game_cls)'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = ''

    regex_pattern1 = r'(?P<game>game=[^>/ ]*/[^>]*)\n'
    regex_format1 = '\g<game>'

    aggr_key_pattern = '/game=(?P<game_group>[^>/ ]*)/[^>]*/'
    aggr_key_expr = 'game_group:echo(game_group)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/game_group=(?P<game_group>[^> ]*)/'
    aggr_key_expr1 = 'game_group:echo(game_group),game:echo(game_group)'
    aggr_op_list1 = ['sum(/value=(?P<value>\d*)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format, regex_map_pattern)
    regex_cmd1 = make_regex_cmd(regex_pattern1, regex_format1)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntUsingAbusingWordGame( start_time, end_time, progname ):
    regex_pattern = '<[^>]*game_cls="(?P<game_cls>[^> ]*)" sp="한게임/채팅룸/금칙어"[^>]*>'
    regex_format = 'game=\m<game>'
    regex_map_pattern = 'game=changing_gamelist(game_cls)'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = ''

    regex_pattern1 = r'(?P<game>game=[^>/ ]*/[^>]*)\n'
    regex_format1 = '\g<game>'

    aggr_key_pattern = '/game=(?P<game_group>[^>/ ]*)/(?P<game>[^> ]*)[^>]*/'
    aggr_key_expr = 'game_group:echo(game_group),game:echo(game)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/game=(?P<game>[^> ]*) game_group=(?P<game_group>[^> ]*)/'
    aggr_key_expr1 = 'game:echo(game),game_group:echo(game_group)'
    aggr_op_list1 = ['sum(/value=(?P<value>\d*)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format, regex_map_pattern)
    regex_cmd1 = make_regex_cmd(regex_pattern1, regex_format1)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntAbusingWordNumTot( start_time, end_time, progname ):
    regex_pattern = '<[^>]*(?P<abuse_word>abuse_word=[^> ]*)[^>]*(?P<sp>sp="한게임/채팅룸/금칙어")[^>]*>'
    regex_format = '\g<abuse_word> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = ''

    aggr_key_pattern = '/abuse_word="(?P<abuse_word>[^> ]*)" sp="(?P<sp>한게임/채팅룸/금칙어)"/'
    aggr_key_expr = 'abuse_word:echo(abuse_word),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/abuse_word=(?P<abuse_word>[^> ]*) sp=(?P<sp>한게임/채팅룸/금칙어)/'
    aggr_key_expr1 = 'sp:echo(sp),game_group:echo(전체),game:echo(전체)'
    aggr_op_list1 = ['sum(1)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd], progname, True)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd + ' | ' + aggr_cmd1,'rb') )


def cntAbusingWordNumGamegroup( start_time, end_time, progname ):
    regex_pattern = '<[^>]*(?P<abuse_word>abuse_word=[^> ]*)[^>]*game_cls="(?P<game_cls>[^> ]*)"[^>]*sp="한게임/채팅룸/금칙어"[^>]*>'
    regex_format = 'game=\m<game> \g<abuse_word>'
    regex_map_pattern = 'game=changing_gamelist(game_cls)'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = ''

    regex_pattern1 = r'(?P<game>game=[^>/ ]*/[^>]*)\n'
    regex_format1 = '\g<game>'

    aggr_key_pattern = '/game=(?P<game_group>[^>/ ]*)/[^> ]* abuse_word="(?P<abuse_word>[^> ]*)"/'
    aggr_key_expr = 'game_group:echo(game_group),abuse_word:echo(abuse_word)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/abuse_word=[^> ]* game_group=(?P<game_group>[^> ]*)/'
    aggr_key_expr1 = 'game_group:echo(game_group),game:echo(game_group)'
    aggr_op_list1 = ['sum(1)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format, regex_map_pattern)
    regex_cmd1 = make_regex_cmd(regex_pattern1, regex_format1)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1], progname, True)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd + ' | ' + aggr_cmd1,'rb') )


def cntAbusingWordNumGame( start_time, end_time, progname ):
    regex_pattern = '<[^>]*(?P<abuse_word>abuse_word=[^> ]*)[^>]*game_cls="(?P<game_cls>[^> ]*)"[^>]*sp="한게임/채팅룸/금칙어"[^>]*>'
    regex_format = 'game=\m<game> \g<abuse_word>'
    regex_map_pattern = 'game=changing_gamelist(game_cls)'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = ''

    regex_pattern1 = r'(?P<game>game=[^>/ ]*/[^>]*)\n'
    regex_format1 = '\g<game>'

    aggr_key_pattern = '/game=(?P<game_group>[^>/ ]*)/(?P<game>[^> ]*) abuse_word="(?P<abuse_word>[^> ]*)"/'
    aggr_key_expr = 'game_group:echo(game_group),game:echo(game),abuse_word:echo(abuse_word)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/abuse_word=[^> ]* game=(?P<game>[^> ]*) game_group=(?P<game_group>[^> ]*)/'
    aggr_key_expr1 = 'game:echo(game),game_group:echo(game_group)'
    aggr_op_list1 = ['sum(1)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format, regex_map_pattern)
    regex_cmd1 = make_regex_cmd(regex_pattern1, regex_format1)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1], progname, True)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd + ' | ' + aggr_cmd1,'rb') )


def cntAbusingWordUsingIdTot( start_time, end_time, progname ):
    regex_pattern = '<[^>]*(?P<sp>sp="한게임/채팅룸/금칙어")[^>]*>'
    regex_format = '\g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" sp="(?P<sp>한게임/채팅룸/금칙어)"/'
    aggr_key_expr = 'id:echo(id),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/id=(?P<id>[^> ]*) sp=(?P<sp>한게임/채팅룸/금칙어)/'
    aggr_key_expr1 = 'sp:echo(sp),game_group:echo(전체),game:echo(전체)'
    aggr_op_list1 = ['sum(1)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntAbusingWordUsingIdGamegroup( start_time, end_time, progname ):
    regex_pattern = '<[^>]*game_cls="(?P<game_cls>[^> ]*)"[^>]*sp="한게임/채팅룸/금칙어"[^>]*>'
    regex_format = 'game=\m<game>'
    regex_map_pattern = 'game=changing_gamelist(game_cls)'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    regex_pattern1 = r'(?P<game>li=[^> ]* game=[^>/ ]*/[^>]*)\n'
    regex_format1 = '\g<game>'


    aggr_key_pattern = '/li="(?P<id>[^> ]*)" game=(?P<game_group>[^>/ ]*)/[^> ]*/'
    aggr_key_expr = 'game_group:echo(game_group),id:echo(id)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/game_group=(?P<game_group>[^> ]*) id=[^> ]*/'
    aggr_key_expr1 = 'game_group:echo(game_group),game:echo(game_group)'
    aggr_op_list1 = ['sum(1)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format, regex_map_pattern)
    regex_cmd1 = make_regex_cmd(regex_pattern1, regex_format1)   
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


def cntAbusingWordUsingIdGame( start_time, end_time, progname ):
    regex_pattern = '<[^>]*game_cls="(?P<game_cls>[^> ]*)"[^>]*sp="한게임/채팅룸/금칙어"[^>]*>'
    regex_format = 'game=\m<game>'
    regex_map_pattern = 'game=changing_gamelist(game_cls)'
    regex_key_pattern = '<K[^>]*(?P<id>li=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    regex_pattern1 = r'(?P<game>li=[^> ]* game=[^>/ ]*/[^>]*)\n'
    regex_format1 = '\g<game>'

    aggr_key_pattern = '/li="(?P<id>[^> ]*)" game=(?P<game_group>[^>/ ]*)/(?P<game>[^> ]*)/'
    aggr_key_expr = 'game_group:echo(game_group),game:echo(game),id:echo(id)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/game=(?P<game>[^> ]*) game_group=(?P<game_group>[^> ]*) id=[^> ]*/'
    aggr_key_expr1 = 'game:echo(game),game_group:echo(game_group)'
    aggr_op_list1 = ['sum(1)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format, regex_map_pattern)
    regex_cmd1 = make_regex_cmd(regex_pattern1, regex_format1)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)
    
    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, regex_cmd1, aggr_cmd], progname, True)
    os.system(hexec_cmd)

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


if __name__ == '__main__':
    main()
