#! /usr/bin/env python
# -*- coding: utf_8 -*-

import sys,os
import time
#sys.path.insert(0, os.path.join(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib'))
from mezzo_util import *

import re
url_pattern = re.compile('^(?:http|ftp|client)://')
def docref_type(docref):
    if url_pattern.match(docref):
        name = ''
        url = docref
    else:
        name = docref
        url = ''
    return name, url

class FreqDict:
    """Store frequencies by key"""
    def __init__(self):
        self.clear()

    def clear(self):
        self.freqdict = {}

    def add_freq(self, key, freq):
        d = self.freqdict
        if d.has_key(key):
            d[key] += freq
        else:
            d[key] = freq

def main():
    progname = os.path.basename(sys.argv[0])
    x = progname.rfind('.')
    if x > 0: progname = progname[:x]

    start_time, end_time = parse_timerange( sys.argv[1:] )
    
    execute_start = time.ctime()    

    date = start_time.strftime('%Y-%m-%d')

    #로그인yes
    regex1_key_pattern = '(<[^>]*li=[^>]*>)'
    regex1_key_format = r'\1'
    regex1_pattern = ( '<[^>]*(?P<sp>sp="[^ >]*")[^>]*>'
            '<[^>]*(?P<docref>docref="[^">]*")[^>]*sp="한게임/로그아웃"[^>]*>')
    regex1_format = '\g<docref> \g<sp>'

    aggr1_key_pattern = '/docref="(?P<docref>[^">]*)" sp="(?P<sp>[^> ]*)"/'
    aggr1_op_list = [ 'sum(1)' ]
    aggr1_key_exprs = { 'lastpage':'replace_if_null(docref,sp)' }

    #로그인no
    regex2_pattern = (
            '(?!<[^>]*sp="한게임/(로그아웃|로그인_웹)"[^>]*>)'
            '<[^>]*(?P<sp>sp="[^>"]*")[^>]*>'
            '<[^>]*sp="한게임/로그인_웹"[^>]*>' )
    regex2_format = '\g<sp>'
    regex2_key_pattern = regex1_key_pattern
    regex2_key_format = regex1_key_format
    aggr2_op_list = aggr1_op_list
    aggr2_key_pattern = '/sp="(?P<lastpage>[^>"]*)"/'

    #로그인yes
    regex3_pattern = ( '<[^>]*li=[^>]*>'
            '(?:(?!<[^>]*sp="한게임/(로그아웃|로그인_웹)"[^>]*>)'
                '.*)'
            '(?!<[^>]*sp="한게임/(로그아웃|로그인_웹)"[^>]*>)'
                '<[^>]*(?P<sp>sp="[^"]*")[^>]*>\$' )
    regex3_format = '\g<sp>'

    aggr3_op_list = aggr1_op_list
    aggr3_key_pattern = aggr2_key_pattern
    
    target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
    if not os.path.exists(target_date_path):
        os.makedirs(target_date_path, 0750)

    import csv
    tbl = FreqDict()
    for row in mana_exec2(True,start_time, end_time, progname,
            [(regex1_pattern, regex1_format, regex1_key_pattern, regex1_key_format)],
            aggr1_key_pattern, aggr1_op_list, aggr1_key_exprs):
        tbl.add_freq(row['k.lastpage'], int(row['sum.value']))

    for row in mana_exec2(False,start_time, end_time, progname,
            [(regex2_pattern, regex2_format, regex2_key_pattern, regex2_key_format)],
            aggr2_key_pattern, aggr2_op_list):
        tbl.add_freq(row['k.lastpage'], int(row['sum.value']))

    for row in mana_exec2(True,start_time, end_time, progname,
            [(regex3_pattern, regex3_format)],
            aggr3_key_pattern, aggr3_op_list):
        tbl.add_freq(row['k.lastpage'], int(row['sum.value']))
    
    outfile = open('./' + progname + '.tsv','wb')

    hdrs = ['date', 'lastpage_name', 'lastpage_url', 'sum']
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow( dict( zip(hdrs, hdrs) ) )
    
    for lastpage, sum in tbl.freqdict.items():
        name, url = docref_type(lastpage)
        new_row = {
            'date': date,
            'lastpage_name': name,
            'lastpage_url': url,
            'sum': sum
        }
        writer.writerow( new_row )

    outfile.close()

    execute_end = time.ctime()

    print execute_start + '\t' + execute_end + '\t' + 'File creation successed : 2_1_2_c_exit.tsv\n'


if __name__ == '__main__':
    main()
