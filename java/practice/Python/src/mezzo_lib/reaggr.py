import re

re_key = re.compile(r"\(?P<(?P<name>[^>]*)>(?P<regex>[^\)]*)\)")
re_func = re.compile(r"(?P<op>\w+)\((?P<arg>.*)\)")
re_asfunc = re.compile(r"(?P<var>[^,()=: \t\n\r\f\v]*)\s*[:=]\s*(?P<func>[^,()=: \t\n\r\f\v]*)\((?P<args>[^)]*)\)")

assign_regex = { "replace_if_null": r'\"[^\"]*\"' }

def gen_aggr(op_patt, key_patt='all', assign=None):
    """Generate final aggr arguments
    op_patt: list of "reduce_operation(argument)"
    key_patt: key pattern (string)
    returns (key_pattern, list of "reduce_operation(arguments)")
    """
    #Check if key_patt is quoted-string
    if key_patt[0] == '"' and key_patt[-1] == '"':
        key_patt = key_patt.strip('"')

    r_key = ""
    if assign: #assignment map available
        pt = re_asfunc.findall(assign[0].strip('"'))
        regex = "[^> ]*"
        for name, func, args in pt:
            if assign_regex.has_key(func):
                r_key += name + "=(?P<" + name + ">" + assign_regex[func] + ")" + '|'
            else:
                r_key += name + "=(?P<" + name + ">" + regex + ")" + '|'
    elif key_patt[0] == '/':
        keys = re_key.findall(key_patt)
        for name, regex in keys:
            regex = regex.replace('\"', '"')
            r_key += name + "=(?P<" + name + ">" + regex + ")" + '|'
    else:  # if not assign and key_patt[0] != '/':
        r_key = key_patt

    if assign or key_patt[0] == '/':
        r_key = "/<k([ ]+(" + r_key[:-1] + "))+>/"

    ml = map(re_func.match, op_patt)
    r_args = []
    for m in ml:
        op = m.group('op')
        arg = m.group('arg')
        if arg[0] == '/':
            vals = re_key.findall(arg)
            if len(vals) == 1:
                n,r = vals[0]
                newarg = '/<' + op + ' %s=(?P<%s>%s)>/' % (n, n, r)
            elif len(vals) > 1:
                newarg = '/<' + op + '([ ]+(' \
                    + '|'.join(map(lambda x:'%s=(?P<%s>%s)'%(x[0],x[0],x[1]), vals)) \
                    + '))*>/'
            elif len(vals) < 1:
                newarg = '/<' + op + ' value=(?P<value>[^> ]*)>/'
        else:
            newarg = '/<' + op + ' value=(?P<value>[^> ]*)>/'
        r_args.append(op + "(" + newarg + ")")

    return r_key, r_args
