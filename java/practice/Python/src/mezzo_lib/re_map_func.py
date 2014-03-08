#! /usr/bin/env python
# -*- coding: utf_8 -*-

import re

"""
GRAMMAR

id :: Word( [a-zA-Z_] [a-zA-Z0-9_]* )
literals :: QuotedString
variable :: id | Word( [0-9]* )

param_list :: expr (',' expr)*
func_expr :: id '(' param_list ')'
expr :: func_expr | literals | variable
mapping :: id '=' expr
mapping_list :: mapping (',', expr)*
"""


def compile(source, func_dict=None):
    from pyparsing import Literal, Word, Forward, Group, Dict, OneOrMore, QuotedString, dblQuotedString, delimitedList, alphas, nums, alphanums
    import sys

    if not func_dict:
        import re_map_funcs
        func_dict = re_map_funcs.__dict__

    def eval_literals(args, mo):
        return args[0]

    def eval_variable(args, mo):
        return mo.group( args[0] )

    def eval_func(args, mo):
        param_list = map(lambda expr: expr[0](expr[1:], mo), args[1])
        return args[0]( *param_list )


    def func_expr_parseAction(s, loc, toks):
        id = toks.asList()[0]
        params = toks.asList()[1]
        return [eval_func, func_dict[id], params]
    
    eq = Literal('=').suppress()
    lp = Literal('(').suppress()
    rp = Literal(')').suppress()

    id = Word( alphas+'_', alphanums+'_' )
    literals = (QuotedString(quoteChar="'") | QuotedString(quoteChar='"')).setParseAction( lambda s, loc, toks: [ eval_literals ] + toks.asList() )
    variable = ( id.copy().setParseAction( lambda s, loc, toks: [ eval_variable ] + toks.asList() )
                 |  Word(nums).setParseAction( lambda s, loc, toks: [ eval_variable ] + [int(toks.asList()[0])] ) )

    func_expr = Forward()

    expr = Group(func_expr | literals | variable)
    param_list = Group(delimitedList(expr))
    func_expr << (id + lp + param_list + rp).setParseAction( func_expr_parseAction )
    mapping = Group(id + eq + expr)
    mapping_list = Dict( delimitedList(mapping) )


    results = mapping_list.parseString(source)

    return results


def eval(compiled, mo):
    results = {}
    for k, v in compiled.items():
        results[k] = v[0](v[1:], mo)
    return results


def test():
    var_dict = {}
    compiled = compile('a=concat("1", "2")')
    print compiled

    results = eval(compiled, var_dict)
    print results

    if results['a'] == '12': print 'Success'
    else: print 'Failure'


if __name__ == "__main__":
    test()
    
