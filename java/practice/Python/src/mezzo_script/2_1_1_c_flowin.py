#! /usr/bin/env python
# -*- coding: utf_8 -*-

import sys,os
#sys.path.insert(0, os.path.join(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib'))
from mezzo_util import *

import re
import time

url_pattern = re.compile('^(?:http|ftp|client)://')
not_filter_p = re.compile('^(한게임/|http://[^/]*hangame\.com|https://[^/]*hangame\.com)')

def docref_type(docref):
    if url_pattern.match(docref):
        docref_name = ''
        ishttp = re.match('(http|https)://[^/]*/',docref)
        if ishttp:
            docref_url = ishttp.group() 
        else:
            docref_url = ''
    else:
        docref_name = docref
        docref_url = ''
    return docref_name, docref_url

def do_write(row, writer, date):
    docref = row['k.docref']
    if not_filter_p.match(docref):
        return False
    docref_name, docref_url = docref_type(docref)
    writer.writerow(
            { 'date': date,
              'docref_name': docref_name,
              'docref_url': docref_url,
              'sp': row['k.sp'],
              'sum': row['sum.value'] } )
    return True

def main():
    import csv

    progname = os.path.basename(sys.argv[0])
    x = progname.rfind('.')
    if x > 0: progname = progname[:x]
    start_time, end_time = parse_timerange( sys.argv[1:] )
    draw_cmd = make_draw_cmd(start_time,end_time)
    
    start = time.ctime()
    print 'Start: ',start
    firstnode = bcookie_firstnode( start_time, end_time, progname)
    session = bcookie_session( start_time, end_time , progname)
    
    outfile = open('./' + progname + '.tsv', 'wb')
    hdrs = ['date', 'sp', 'docref_name', 'docref_url', 'sum']
    date = start_time.strftime('%Y-%m-%d')

    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow( dict( zip(hdrs, hdrs) ) )

    new_row = {}


    for row in firstnode:

        docref = row['k.docref']

        if not not_filter_p.match(docref):
            docref_name, docref_url = docref_type(docref)
            key = (date,row['k.sp'],docref_name,docref_url)
       
            if key not in new_row.keys():
                new_row[key] = row['sum.value']
            else:
                new_row[key] = str(long(new_row[key]) + long(row['sum.value']))

    for row in session:
        docref = row['k.docref']
        if not not_filter_p.match(docref):
            docref_name, docref_url = docref_type(docref)
            key = (date,row['k.sp'],docref_name,docref_url)

            if key not in new_row.keys():
                new_row[key] = row['sum.value']
            else:
                new_row[key] = str(long(new_row[key]) + long(row['sum.value']))
    for k,v in new_row.items():
        writer.writerow({'date':k[0],'sp':k[1],'docref_name':k[2],'docref_url':k[3],'sum':v})

    end = time.ctime()
    print 'End: ',end   


#쿠키의 첫 번째 노드들의 수를 파악.
def bcookie_firstnode( start_time, end_time, progname): 
 
    print 'Cookie의 최초노드 파악...'

    regex_pattern = '<[^>]*><[^>]*docref=(?P<docref>"\S*")[^>]*sp=(?P<sp>"(한게임[^>\^ ]*|\S*\.hangame\.com\S*)")[^>]*>'    

    regex_format = 'docref=\g<docref> sp=\g<sp>'
    
    aggr_key_pattern = '/docref="(?P<docref>\S*)" sp="(?P<sp>[^> ]*)"/'
    aggr_key_expr = 'docref:echo(docref),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern2 = '/docref=(?P<docref>[^>]*) sp=(?P<sp>[^> ]*)/'
    aggr_key_expr2 = 'docref:echo(docref),sp:echo(sp)'
    aggr_op_list2 = ['sum(/value=(?P<value>\d+)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern,aggr_op_list,aggr_key_expr,False)
    aggr_cmd2 = make_aggr_cmd(aggr_key_pattern2, aggr_op_list2, aggr_key_expr2,True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd ,aggr_cmd], progname)
    os.system ( hexec_cmd )

    return create_tsvreader(os.popen('zcat ' + progname + '.gz | ' + aggr_cmd2, 'rb'))


#세션Time은 1시간
def bcookie_session( start_time, end_time, progname): 

    print '세션타임 1시간 이상인 노드 파악...'

    regex_pattern = '<[^>]*ts=(?P<ts1>"\d+")><[^>]*docref=(?P<docref>"\S*")[^>]*sp=(?P<sp>"[^> ]*")[^>]*ts=(?P<ts2>"\d+")>'
    regex_format = 'docref=\g<docref> sp=\g<sp> ts1=\g<ts1> ts2=\g<ts2>'
    regex_key_pattern = '(<[^>]*bc="[^> ]*">)'

    aggr_key_pattern = '/docref="(?P<docref>\S*)" sp="(?P<sp>[^> ]*)" ts1="(?P<ts1>\d+)" ts2="(?P<ts2>\d+)"/'
    aggr_key_expr = 'docref:echo(docref),dts:sub(ts2,ts1),sp:echo(sp)'

    regex_pattern2 = '<k docref=(?P<docref>\S*) dts=(3[6-9][0-9][1-9]|[4-9][0-9][0-9][0-9]|[1-9][0-9][0-9][0-9][0-9]+) sp=(?P<sp>[^> ]*)>'    
    regex_format2 ='docref=\g<docref> sp=\g<sp>'

    aggr_key_pattern2 = '/docref=(?P<docref>[^> ]*) sp=(?P<sp>[^> ]*)/'
    aggr_key_expr2 = 'docref:echo(docref),sp:echo(sp)'
    aggr_op_list2 = ['sum(1)']
   
    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern,'',aggr_key_expr, False)
    regex_cmd2 = make_regex_cmd(regex_pattern2, regex_format2)
    aggr_cmd2 = make_aggr_cmd(aggr_key_pattern2, aggr_op_list2, aggr_key_expr2, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd,aggr_cmd] ,progname)
    os.system ( hexec_cmd )
    
    return create_tsvreader(os.popen('zcat ' + progname + '.gz | ' + regex_cmd2 + ' | ' + aggr_cmd2, 'rb'))


if __name__ == '__main__':
    main()
