#! /usr/bin/env python
# -*- coding: utf_8 -*-

import os, sys
import time, datetime


if 'GEXEC_SVRS' not in os.environ:
    os.environ['GEXEC_SVRS'] = 'logh-a002 logh-a003 logh-a004 logh-a005 logh-a006 ' + \
                               'logh-a007 logh-a008 logh-a009 logh-a010 logh-a011 ' + \
                               'logh-a012 logh-a013 logh-a014 logh-a015 logh-a016 ' + \
                               'logh-a017 logh-a018 logh-a019 logh-a020 logh-a021 ' + \
                               'logh-a022 logh-a023 logh-a024 logh-a025 logh-a026 ' + \
                               'logh-a027 logh-a028 logh-a029 logh-a030'

if 'MEZZO_HOME' not in os.environ:
    os.environ['MEZZO_HOME'] = '/home1/irteam/mezzo'

if 'HG_DATA_HOME' not in os.environ:
    os.environ['HG_DATA_HOME'] = '/home1/irteam/hg_data'

mezzo_home = os.getenv('MEZZO_HOME')
bin_path = os.path.join(mezzo_home, 'bin')
target_path = os.path.join(mezzo_home, 'out')

hg_data_home = os.getenv('HG_DATA_HOME')

def mana_exec(start_time, end_time, re_pattern, re_format, aggr_key_pattern, aggr_op_list, aggr_key_exprs=None, tsv=True):
    """mana를 실행하고 iterator of result rows를 리턴한다."""
    return mana_exec2(start_time, end_time, [(re_pattern, re_format)], aggr_key_pattern, aggr_op_list, aggr_key_exprs, tsv)

def mana_exec2(start_time, end_time, re_list, aggr_key_pattern=None, aggr_op_list=None, aggr_key_exprs=None, tsv=True):
    """mana를 실행하고 iterator of result rows를 리턴한다."""
    import csv

    mana_cmd = make_mana_cmd2(start_time, end_time, re_list, aggr_key_pattern, aggr_op_list, aggr_key_exprs, tsv)
    mana_result = os.popen(mana_cmd, 'rb')

    return create_tsvreader(mana_result)

def create_tsvreader(sequence):
    import csv
    hdrs = sequence.next().strip().split(csv.excel_tab.delimiter)
    return csv.DictReader(sequence, hdrs, dialect='excel-tab')

def make_mana_cmd(start_time, end_time, re_pattern, re_format, aggr_key_pattern, aggr_op_list, aggr_key_exprs=None, tsv=True):
    """mana command string을 만든다."""
    return make_mana_cmd2(start_time, end_time, [(re_pattern, re_format)], aggr_key_pattern, aggr_op_list, aggr_key_exprs, tsv)

def make_mana_cmd2(start_time, end_time, re_list, aggr_key_pattern=None, aggr_op_list=None, aggr_key_exprs=None, tsv=True):
    """mana command string을 만든다."""
    draw_expr = 'from ' + start_time.strftime('%Y%m%d_%H') + ' to ' + end_time.strftime('%Y%m%d_%H')

    re_cmds = []
    for re in re_list:
        re_cmd = 'format pattern ' + dbl_quote(re[0]) + ' as ' + dbl_quote(re[1])
        if len(re) == 4:
            re_cmd = 'foreach ' + dbl_quote(re[2]) + ' starts with ' + dbl_quote(re[3]) + ' ' + re_cmd
        re_cmds.append( re_cmd )

    if aggr_key_pattern and aggr_op_list:
        aggr_cmd = ' and calculate ' + ','.join(aggr_op_list)
        if aggr_key_exprs:
            aggr_cmd += ' group by assignment ' + \
                        ','.join( [k + '=' + v for k, v in aggr_key_exprs.items()] ) + \
                        ' from ' + dbl_quote(aggr_key_pattern)
        else:
            aggr_cmd += ' group by ' + dbl_quote(aggr_key_pattern)
    else:
        aggr_cmd = ''

    mana_cmd = 'cd ' + mezzo_home + ';' + \
               'PATH=' + bin_path + ':$PATH; '
    if tsv:
        mana_cmd += 'mana --tsv -g ' + quote(draw_expr + ' ' + ' and '.join(re_cmds) + aggr_cmd)
    else:
        mana_cmd += 'mana -g ' + quote(draw_expr + ' ' + ' and '.join(re_cmds) + aggr_cmd)
               
    return mana_cmd

def make_hexec(cmds, start_time, end_time, use_p_none_option=False):
    """command들을 pipe로 연결한 gexec command string을 만든다."""
    if (use_p_none_option):
        p_none_option_str = '-p none '
    else:
        p_none_option_str = ''
    
    piped_cmds = ' | '.join( cmds )
    cmd = 'hadoop jar /usr/lib/mezzo/hexec.jar ' + make_draw(start_time, end_time) + ' result ' + quote(piped_cmds) + ' --draw \'-pk bc\''
    return cmd

def make_draw(start_time, end_time):
    """draw command string을 만든다."""
    return '/mezzo/log_hangame/ ' + start_time.strftime('%Y%m%d_%H') + ' ' + end_time.strftime('%Y%m%d_%H')

def make_gexec_cmd(cmds, use_p_none_option=False):
    """command들을 pipe로 연결한 gexec command string을 만든다."""
    if (use_p_none_option):
        p_none_option_str = '-p none '
    else:
        p_none_option_str = ''
    
    piped_cmds = ' | '.join( cmds )
    cmd = 'cd ' + mezzo_home + '; ' + \
          'PATH=' + bin_path + ':$PATH; ' + \
          'gexec ' + p_none_option_str + '-n 0 /bin/sh -c ' + quote(piped_cmds)
    return cmd

def make_hg_cmd(cmds):
    """command들을 pipe로 연결한 내용을 command string으로 만들어 준다 """
    pipe_cmds = " | ".join( cmds )
    cmd = 'cd ' + hg_data_home + '; ' + \
          'PATH=' + bin_path + ':$PATH; ' + \
          pipe_cmds
    return cmd

def make_draw_cmd(start_time, end_time):
    """draw command string을 만든다."""
    return 'draw ' + ' '.join( make_draw_args(start_time, end_time) )

def make_draw_args(start_time, end_time):
    """draw command의 argument list를 만든다."""
    return ['-I', 'logs',
            '-s', start_time.strftime('%Y%m%d_%H'),
            '-e', end_time.strftime('%Y%m%d_%H')]

#def make_regex_cmd(pattern, format, key_pattern=None, key_format=None, split=False):
#    """regex command string을 만든다."""
#    args = []
#    if split: args.append('--split')
#    if key_pattern: args.extend(['-k', dbl_quote(key_pattern)])
#    if key_format: args.extend(['-l', dbl_quote(key_format)])
#    args.extend([dbl_quote(pattern), dbl_quote(format)])
#    cmd = 'regex ' + ' '.join(args)
#    return cmd

def make_regex_cmd(pattern, format, key_pattern=None, key_format=None, map_pattern=None, split=False, input_file=None):
    """regex command string을 만든다."""
    args = []
    if input_file: args.extend(['-f', dbl_quote(input_file)])
    if key_pattern: args.extend(['-k', dbl_quote(key_pattern)])
    if key_format: args.extend(['-l', dbl_quote(key_format)])
    if split: args.append('--split')
    args.extend([dbl_quote(pattern)])
    if map_pattern: args.extend(['-n', dbl_quote(map_pattern)])
    args.extend([dbl_quote(format)])
    cmd = 'regex ' + ' '.join(args)
    return cmd

def make_aggr_cmd(key_pattern, op_list, key_expression=None, tsv=False):
    """aggr command string을 만든다."""
    args = ['-k', dbl_quote(key_pattern)]
    if key_expression: args.extend( ['-j', dbl_quote(key_expression)] )
    if tsv: args.append( '--tsv' )
    args.extend( [dbl_quote(op) for op in op_list] )
    cmd = 'aggr ' + ' '.join(args)
    return cmd

def make_checkpoint_pattern(checkpoint):
    """sp parameter가 checkpoint를 포함하는 노드를 찾는 패턴을 만든다."""
    return '<[^>]* sp=[^> ]*' + checkpoint + '[^>]*>'

def make_checkpoints_pattern(checkpoints):
    """여러 개의 checkpoint들을 지나는 노드의 sequence를 찾는 패턴을 만든다."""
    cpps = [make_checkpoint_pattern(cp) for cp in checkpoints]
    p = '(?P<path>' + \
        '(<[^>]*>)*?'.join( cpps ) + \
        ')'
    return p

def parse_timerange( timerange_str ):
    """시작 시각과 끝 시각을 나타내는 문자열의 쌍을 datetime 객체로 변환한다."""
    if len(timerange_str) == 1:
        start_time = mezzo_strptime( timerange_str[0] )
        end_time = start_time + datetime.timedelta(days=1, hours=-1)
    elif len(timerange_str) >= 2:
        start_time = mezzo_strptime( timerange_str[0] )
        end_time = mezzo_strptime( timerange_str[1] )
    else:
        thirtyhoursago = datetime.datetime.now() - datetime.timedelta(hours=30)
        start_time = thirtyhoursago.replace(hour=06, minute=0, second=0, microsecond=0)
        end_time = start_time + datetime.timedelta(days=1, hours=-1)

    return start_time, end_time

def mezzo_strptime(string):
    """'YYYYMMDD_HH' 형태의 문자열을 datetime 객체로 변환한다."""
    from time import strptime
    return datetime.datetime( *strptime(string, '%Y%m%d_%H')[0:6] )

def quote(string):
    """문자열 앞뒤에 (')를 붙이고 문자열 속의 (') 앞에 (\)를 추가한다."""
    return "'" + string.replace("'", r"\'") + "'"

def dbl_quote(string):
    """문자열 앞뒤에 (")를 붙이고 문자열 속의 (") 앞에 (\)를 추가한다."""
    return '"' + string.replace('"', r'\"') + '"'


def ignore(expr):
    n = expr.split(':')[0]
    return n + ':echo(__all__)'

def combination(seq, mutate):
    if len(seq) == 0: return [[]]

    result = []
    for i in combination(seq[1:], mutate):
        result += [ [seq[0]] + i, [mutate(seq[0])] + i ]
    return result


class StopWatch:
    """Stopwatch that measures time elapsed"""
    def __init__(self, progname=sys.argv[0]):
        self.pname = progname

    def start(self):
        """starts stop watch and returns start time"""
        self.s_time = time.time()
        print >> sys.stderr, self.pname, "started at", time.ctime(self.s_time)

    def stop(self):
        """starts stop watch and print out elapsed time from s_time"""
        e_time = time.time()
        print >> sys.stderr, self.pname, "  ended at", time.ctime(e_time)
        print >> sys.stderr, self.pname, "takes", int(e_time - self.s_time+0.5), "seconds"

