#!/bin/env python
"""MEZZO Analyzer command interpreter"""

import mparser,reaggr
import sys, os, optparse

def gen_cmd_pipes(cmdlist, logdir="/home1/irteam/mezzo/logs"):
    """Returns mezzo commands connected by pipes from 'cmdlist'"""
    cmd, args = cmdlist[0]
    if cmd == 'draw':
        cmd = 'draw -I ' + logdir
    res = cmd + ' ' + args

    for cmd, args in cmdlist[1:]:
        res += ' | ' + cmd + ' ' + args

    return res

def error(msg):
    """pring 'msg' as error message and exit"""
    print >> sys.stderr, msg
    sys.exit(1)

MEZZO_HOME = '/home1/irteam/mezzo'
def main():
    """Main program: option parser and caller"""
    usage="""usage: %prog [options] [-f script_file] 'mezzo_script'"""
    parser = optparse.OptionParser(usage=usage)
    parser.add_option('-f', dest="script_file",
        help="read from SCRIPT_FILE that contains MEZZO script")
    parser.add_option("-g", "--gexec",
        action="store_true", dest="gexec", default=False,
        help="run in parallel using gexec and re-aggregate")
    parser.add_option("-G", "--gexec-only",
        action="store_true", dest="gexec_only", default=False,
        help="run in parallel using gexec but no re-aggregation")
    parser.add_option("-o", dest="out_file",
        help="store the results in OUT_FILE")
    parser.add_option("-n", "--dryrun",
        action="store_true", dest="dryrun", default=False,
        help="not run, but outputs the command")
    parser.add_option("--tsv",
        action="store_true", dest="tsv", default=False,
        help="output as Tab Separated Format")

    (opts, args) = parser.parse_args()

    if opts.script_file:
        if (len(args) > 0):
            error("Both script_file and script are given.")
        else:
            pt = mparser.command.parseFile(opts.script_file)
            if opts.gexec:
                fp = open(opts.script_file)
                script = fp.read()
                fp.close()
    elif len(args) == 1:
        pt = mparser.parse_cmd(args[0])
        script = args[0]
    elif len(args) > 1:
        error("Too many scripts")
    else:
        parser.print_help()
        sys.exit(0)

    if opts.gexec or opts.gexec_only:
        # generate yesterday if the range is not given
        if not pt.asDict().has_key('draw'):
            t1, t2 = mparser.gen_range(None)
            script = "from " + t1 + " to " + t2 + " " + script
            pt = mparser.parse_cmd(script) # syntax check

        num_hosts = len(os.environ["GEXEC_SVRS"].split())
        cmdstr = "gexec -p none -n " + str(num_hosts) + " mana '" + script + "'"

        if pt.asDict().has_key('aggr') and not opts.gexec_only:
            aggr_ops, aggr_key, aggr_map = mparser.gentuple_aggr(pt['aggr'])
            if not aggr_key: aggr_key = 'all'
            reaggr_cmd = reaggr.gen_aggr(aggr_ops, aggr_key, aggr_map)

            cmdstr += " | aggr"
            if opts.tsv: cmdstr += " --tsv"
            cmdstr += ' -k "' + reaggr_cmd[0] \
                + '" ' + ' '.join(map(lambda x:'"'+x+'"', reaggr_cmd[1]))
    else:
        cmdlist = mparser.genarg_cmd(pt)
        cmdstr = gen_cmd_pipes(cmdlist)

    if opts.dryrun:
        print `cmdstr`
    else:
        p = os.environ['PATH']
        if p.find(MEZZO_HOME+'/bin') < 0:
            p = MEZZO_HOME+'/bin:' + p
            os.environ['PATH'] = p
        fpipe = os.popen(cmdstr, 'r')
        for line in fpipe:
            sys.stdout.write(line)


if __name__ == "__main__":
    main()

