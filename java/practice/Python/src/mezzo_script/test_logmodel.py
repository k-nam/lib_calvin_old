#! /usr/bin/env python
# -*- coding: utf_8 -*-

import sys,os
from mezzo_util import *
import re

def testnode():
    aggr_cmd2 = 'aggr -k "/docref=(?P<docref>[^>]*) sp=(?P<sp>[^> ]*)/" -j "docref:echo(docref),sp:echo(sp)" --tsv "sum(/value=(?P<value>\d+)/)"'
    return create_tsvreader(os.popen('zcat 2_1_1_c_flowin_logmodel.gz | ' + aggr_cmd2, 'rb'))


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

not_filter_p = re.compile('^(한게임/|http://[^/]*hangame\.com|https://[^/]*hangame\.com)')
firstnode = testnode()
for row in firstnode:
    #print row
    docref = row['k.docref']

    if not not_filter_p.match(docref):
        docref_name, docref_url = docref_type(docref)
        key = (date,row['k.sp'],docref_name,docref_url)

        if key not in new_row.keys():
            new_row[key] = row['sum.value']
        else:
            new_row[key] = str(long(new_row[key]) + long(row['sum.value']))

