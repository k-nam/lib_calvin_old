#!/bin/env python
"""Parser for MEZZO language framework"""

from pyparsing import *
from string import digits

# BEGIN: Grammar (parsed by pyparsing)
# Date & hour specificaton
YYYY = Regex("(19|20)[0-9][0-9]")
MM = Regex("0[1-9]|1[0-2]")
DD = Regex("0[1-9]|[12][0-9]|3[01]")
hh = Regex("[01][0-9]|2[0-3]")
date = YYYY + MM + DD
datehour = date + Suppress('_') + hh

ident = Word(alphas + '_', alphanums + '_')
delims = ':,()=' + string.whitespace

# Data range (draw part)
date_range = (Keyword('for') + (datehour | date)) \
            | (Keyword('from') + datehour + Keyword('to') + datehour)

# Regular expression
# TODO: refine double quote escape and shell argument translation
regex = dblQuotedString | CharsNotIn('/"\t\n')
regex_fmt = dblQuotedString | Word(alphanums)
key_spec = Keyword('foreach').suppress() + regex.setResultsName('key_regex') \
            + Keyword('starts').suppress() + Keyword('with').suppress() \
            + regex_fmt.setResultsName('key_fmt')
ptn_spec = Suppress('format') + Suppress('pattern') \
            + regex.setResultsName('ptn_regex') \
            + Suppress('as') + regex_fmt.setResultsName('ptn_fmt')
# regex part: returns dictionary keyed by 'key_regex', 'key_fmt',
# 'ptn_regex', and 'ptn_fmt'. The value of each entry is string.
format = Optional(key_spec) + ptn_spec

# aggr part: returns dictionary keyed by 'reduce_fn' and 'group_regex'(optional)
# the value of 'reduce_fn' is list of reduce functions
reduce_op = 'sum'
#ptn_or_const = ('/' + regex + '/') | dblQuotedString | Word(alphanums)
ptn_or_const = ('/' + regex + '/') | dblQuotedString
reduce_fn = reduce_op + '(' + ( ('/' + regex + '/') | Word(alphanums) ) + ')'
assignment = ident + Literal('=').suppress() \
        + Combine( (ident + '(' + delimitedList(CharsNotIn(delims),combine=True) + ')') \
            | ident.copy().setParseAction(lambda s,l,t: 'echo('+t[0]+')') )
assign_spec = delimitedList(Group(assignment)).setResultsName('assignment')
group_spec = Keyword('group').suppress() + Keyword('by').suppress() \
        + Optional(Keyword('assignment').suppress() \
            + assign_spec + Keyword('from').suppress()) \
        + ptn_or_const.setResultsName('group_regex')
aggr = Keyword('calculate').suppress() \
        + delimitedList(Group(reduce_fn)).setResultsName('reduce_fn') \
        + Optional(group_spec)

# command part: returns dictionary keyed by commands
command = Optional(Group(date_range).setResultsName('draw')) \
        + delimitedList(Group(format), delim='and').setResultsName('regex') \
        + Optional(Keyword('and').suppress() + aggr.setResultsName('aggr'))
# END: Grammar

def parse_cmd(instr):
    try:
        r = command.parseString(instr)
    except ParseException, args:
        print `instr`
        print args
        raise
    return r

def parse_range(instr):
    return date_range.parseString(instr)

def parse_format(instr):
    return format.parseString(instr)

def parse_aggr(instr):
    try:
        r =aggr.parseString(instr)
    except ParseException, args:
        print `instr`
        print args
        raise
    return r


import datetime, time
def gen_range(date=None, start_of_a_day=6):
    """Generate time range if the time nor the date is not given."""
    if date:
        year, month, day = map(int, date)
    else:
        t = time.localtime(time.time() - (24 + start_of_a_day )*3600)
        year, month, day = t[:3]
    df = datetime.datetime(year, month, day, start_of_a_day)
    dt = df + datetime.timedelta(hours=23)
    return (df.strftime("%Y%m%d_%H"), dt.strftime("%Y%m%d_%H"))

def genarg_draw(presult, start_of_a_day=6):
    """generate -s and -e arguments of 'draw' command from a parse result"""
    if presult == None:
        t1, t2 = gen_range(None, start_of_a_day)
    elif presult[0] == 'for': # Day case
        if len(presult) <= 4:
            t1, t2 = gen_range(presult[1:4], start_of_a_day)
        else:
            year, month, day, hour = map(int, presult[1:5])
            df = datetime.datetime(year, month, day, hour)
            t1 = df.strftime("%Y%m%d_%H")
            t2 = t1
    else: # interval case
        assert len(presult) == 10, `presult`
        assert presult[0] == 'from', `presult`
        assert presult[5] == 'to', `presult`
        t1 = ''.join(presult[1:4]) + '_' + presult[4]
        t2 = ''.join(presult[6:9]) + '_' + presult[9]

    return "-s " + t1 + " -e " +t2

def genarg_regex(presult):
    """generate arguments of 'regex' command from a parse result"""
    res = ''
    if presult.asDict().has_key('key_regex'):
        res = '-k' + presult['key_regex'] + ' -l' + presult['key_fmt'] + ' '
    res += presult['ptn_regex'] + ' ' + presult['ptn_fmt']
    return res

def genarg_aggr(presult):
    """generate arguments of 'aggr' command from a parse result"""
    res = ''
    ops, key, asgn = gentuple_aggr(presult)
    if asgn:
        res += '-j ' + asgn[0] + ' '
    if key:
        res += '-k ' + key + ' '
    res += ' '.join(map(lambda x: '"'+x+'"',ops)) 
    return res

def gentuple_aggr(presult):
    """generate key argument and operator arguments list for 'reaggr' function"""
    key_patt, asgns = None, None
    if presult.asDict().has_key('group_regex'):
        key_patt = presult['group_regex']
    if presult.asDict().has_key('assignment'):
        lst = presult['assignment'].asList()
        names = map(lambda x: x[0], lst)
        arg = '"' + ','.join(map(':'.join,lst)) + '"'
        asgns = (arg, names)
    op_patts = map(lambda x: ''.join(x.asList()) ,presult['reduce_fn'])
    return (op_patts, key_patt, asgns)

def genarg_cmd(presult):
    """generate all the commands and their arguments from a parse result"""
    res = []
    if presult.asDict().has_key('draw'):
        args = genarg_draw(presult['draw'])
    else:
        args = genarg_draw(None)
    res.append(('draw', args))

    for item in presult['regex']:
        args = genarg_regex(item)
        res.append(('regex', args))

    if presult.asDict().has_key('aggr'):
        args = genarg_aggr(presult['aggr'])
        res.append(('aggr', args))

    return res

import sys
def main():
    instr = sys.argv[1]
    try:
        pr = cmd_parse(instr)
    except ParseException, e:
        print e
        print "Instr:", `instr`
        sys.exit(1)

    r = genarg_cmd(pr)

    for i in r:
        print i[0] + ": " + `i[1]`

if __name__ == "__main__":
    main()
