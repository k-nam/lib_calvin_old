#!/usr/bin/env python

import sys
import re
import re_map_func


num_group_re = re.compile(r"\\(\d+)")
name_group_re = re.compile(r"\\g<([^>]+)>")
map_group_re = re.compile(r"\\m<([^>]+)>")

def re_findall(key_pattern, key_format, pattern, format, input, output=sys.stdout, key_map_expr=None, map_expr=None, split=False):
    kp = None
    if key_pattern:
        kp = re.compile(key_pattern)
		
    p = re.compile(pattern)

    key_map_expr_compiled = None
    if key_map_expr:
        key_map_expr_compiled = re_map_func.compile(key_map_expr)

    map_expr_compiled = None
    if map_expr:
        map_expr_compiled = re_map_func.compile(map_expr)

    num_line = 0
    for line in input:
        try:
            num_line += 1
            
            k = ""
            if kp:
                mo = kp.search(line)
                if not mo: continue
                k = output_format( key_format, mo, key_map_expr_compiled )

            delim = ''
            start = 0

            for mo in p.finditer(line):
                output.write( k )
                if split:
                    output.write( delim )
                    output.write( line[start:mo.start()] )
                delim = output_format(format, mo, map_expr_compiled )
                output.write( delim )
                output.write( '\n' )

                if split:
                    start = mo.end()

            if split and delim:
                output.write( k )
                output.write( delim )
                output.write( line.rstrip()[start:] )
                output.write( '\n' )
        except:
            print >> sys.stderr, 'Exception: ', sys.exc_info(), 'at line', num_line
            print >> sys.stderr, line
            raise
        

def output_format(format, match, map_expr):
    def num_group(m):
        if not match: return ''
        return match.group( int(m.group(1)) )
	
    format = num_group_re.sub(num_group, format)

    def name_group(m):
        if not match: return ''
        return match.group( m.group(1) )

    format = name_group_re.sub(name_group, format)

    if map_expr:
        map_results = re_map_func.eval(map_expr, match)
    
        def map_group(m):
            if not match: return ''
            return map_results[ m.group(1) ]

        format = map_group_re.sub(map_group, format)

    return format


import optparse

def run():
    usage = r"""usage: %prog [options] (-p pattern_file | <regex> <format>)

    for each line find <regex> iteratively (e.g. id=(.*))
    and format it as <format> (e.g. login=\1)"""
	
    parser = optparse.OptionParser(usage=usage)

    parser.add_option("-k", "--key-pattern",
                      dest="key_ptn", metavar="KEY_PATTERN",
                      help="find KEY_PATTERN once for each line")
    parser.add_option("-l", "--key-format",
                      dest="key_fmt", metavar="KEY_FORMAT",
                      help="format key as KEY_FORMAT")
    parser.add_option("-j", "--key-pattern-file",
                      dest="key_ptn_file", metavar="KEY_PATTERN_FILE",
                      help="get KEY_PATTERN from KEY_PATTERN_FILE")
    parser.add_option("-m", "--key-map-expression",
                      dest="key_map_expr", metavar="KEY_MAP_EXPRESSION",
                      help="map expression converts substrings matched by KEY_PATTERN using function expression")
    parser.add_option("-n", "--map-expression",
                      dest="map_expr", metavar="MAP_EXPRESSION",
                      help="map expression converts matched substrings by <regex> using function expression")
    parser.add_option("-p", "--pattern-file",
                      dest="ptn_file", metavar="PATTERN_FILE",
                      help="get <regex> and <format> from PATTERN_FILE")
    parser.add_option("-f", "--input",
                      dest="input", metavar="INPUTFILE",
                      help="get lines from INPUTFILE (default: stdin)")
    parser.add_option("-o", "--output",
                      dest="output", metavar="OUTPUTFILE",
                      help="put results into OUTPUTFILE (default: stdout)")
    parser.add_option("-s", "--split",
                      dest="split", action="store_true", default=False,
                      help="split input lines by the occurrences of <regex>")

    (opts, args) = parser.parse_args()

    if (not opts.ptn_file and (len(args) < 2)):
        parser.print_help()
        sys.exit(1)

    if opts.input:
        ifp = file(opts.input, "r")
    else:
        ifp = sys.stdin

    if opts.output:
        ofp = file(opts.output, "w")
    else:
        ofp = sys.stdout

    kp = ""
    kf = ""

    if opts.key_ptn_file:
        fn = open(opts.key_ptn_file)
        kp = fn.readline().strip()
        kf = fn.readline().strip()
    elif opts.key_ptn:
        kp = opts.key_ptn
        kf = opts.key_fmt or ''

    p = ""
    f = ""

    if opts.ptn_file:
        pattern_fn = open(opts.ptn_file)
        p = pattern_fn.readline().strip()
        f = pattern_fn.readline().strip()
    elif len(args) == 2:
        p = args[0]
        f = args[1]
		
    re_findall(kp, kf, p, f, ifp, ofp, opts.key_map_expr, opts.map_expr, opts.split)

    if opts.output: ofp.close()
    if opts.input: ifp.close()


if __name__ == "__main__":
	run()

