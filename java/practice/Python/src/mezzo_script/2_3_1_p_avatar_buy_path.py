#! /usr/bin/env python
# -*- coding: utf_8 -*-

"""
2_3_1 포커 아바타 가격대별 구매경로 분석
"""


import sys,os
import time
#sys.path.insert(0, os.path.join(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib'))
from mezzo_util import *


def main():
    progname = os.path.basename(sys.argv[0])
    x = progname.rfind('.')
    if x > 0: progname = progname[:x]

    start_time, end_time = parse_timerange( sys.argv[1:] )

    execute_start = time.ctime()

    avatar_results = find_avatar_buy(start_time, end_time, progname)
    item_results = find_item_buy(start_time, end_time,progname)

    target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
    os.system('mkdir -p ' + target_date_path)

    #outfile = sys.stdout
    #outfile = open(os.path.join(target_date_path, './2_3_1_p_avatar_buy_path.tsv'), 'wb')
    outfile = open('./2_3_1_p_avatar_buy_path.tsv', 'wb')

    hdrs = ['date', 'goodstype', 'goodsid', 'docref', 'num_buy']

    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow( dict( zip(hdrs, hdrs) ) )

    import itertools
    for row in itertools.chain( avatar_results, item_results ):
       new_row = {
            hdrs[0]: start_time.strftime('%Y-%m-%d'),
            hdrs[1]: row['k.goodstype'],
            hdrs[2]: row['k.goodsid'],
            hdrs[3]: row['k.docref']
            }

       if 'sum.goodscnt' in row: new_row[ hdrs[4] ] = row['sum.goodscnt']
       else: new_row[ hdrs[4] ] = row['sum.value']

       writer.writerow( new_row )

    execute_end = time.ctime()

    print execute_start +'\t' + execute_end + '\t' + 'File creation successed : 2_3_1_p _avatar_buy_path.tsv\n'


#아이템 구매를 시도한 건수
def find_avatar_buy(start_time, end_time, progname):
    re_cmds = []
    re_cmds.append( (
        '<[^>]*(?P<docref>docref="[^>" ]*")[^>]*(?P<goodstype>goodstype="(COMAVT|EDPAVT|POKERVPLUSAVT|POKERWPLUSAVT)")[^>]*sp=[^> ]*/아바타/구매시작팝업[^>]*>' +
        '(?!<[^>]*sp=[^> ]*구매시작팝업[^>]*>).*?' +
        '<[^>]*goodsid=(?P<goodsid>[^> ]*)[^>]*(?P=goodstype)[^>]*sp=[^> ]*/아바타/구매완료팝업[^>]*>',
        r'<f \g<goodstype> \g<docref> goodsid=\g<goodsid>>',
        '(<[^>]*li=[^>]*>)',
        r'\1'
        ) )

    re_cmds.append( (
        '(goodsid="|@)(?P<goods>\w+)',
        r' goodsid=\g<goods>>',
        ' (goodstype=[^ ]*) (docref=[^ ]*)',
        r'<f \1 \2'
        ) )

    aggr_key_pattern = '/ goodstype="(?P<goodstype>[^> ]*)" docref="(?P<docref>[^> ]*)" goodsid=(?P<goodsid>[^> ]*)/'
    aggr_op_list = [ 'sum(1)' ]

    return mana_exec2(True, start_time, end_time, progname, re_cmds, aggr_key_pattern, aggr_op_list)


# 아이템 구매를 완료한 건수
def find_item_buy(start_time, end_time, progname):
    re_cmds = []

    re_cmds.append( (
        '<[^>]*(?P<docref>docref="[^"]*")[^>]*(?P<goodstype>goodstype="POKERAVT")[^>]*sp="한게임/아이템/구매시작팝업"[^>]*>' +
        '(?!<[^>]* (?P=goodstype)[^>]*sp="한게임/아이템/구매시작팝업"[^>]*>).*?' +
        '<[^>]*(?P<goodscnt>goodscnt=[^> ]*)[^>]*(?P<goodsid>goodsid=[^> ]*)[^>]*(?P=goodstype)[^>]*sp="한게임/아이템/구매완료팝업"[^>]*>',
        r'<f \g<goodstype> \g<docref> \g<goodsid> \g<goodscnt>>'
        ) )

    aggr_key_pattern = '/ goodstype="(?P<goodstype>[^> ]*)" docref="(?P<docref>[^> ]*)" goodsid="(?P<goodsid>[^> ]*)"/'
    aggr_op_list = [ 'sum(/ goodscnt="(?P<goodscnt>[^> ]*)"/)' ]
    
    return mana_exec2(False, start_time, end_time, progname, re_cmds, aggr_key_pattern, aggr_op_list)


if __name__ == '__main__':
    main()
