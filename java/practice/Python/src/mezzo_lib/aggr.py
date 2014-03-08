#! /usr/bin/env python
# -*- coding: utf_8 -*-
"""
SYNOPSIS
  aggr.py [-tsv] <key_pattern> (<reduce_op> <value_pattern>)+

OUTPUT
  <k (kk=kv)+>(<reduce_op (vk=vv)+>)+
"""


import sys, re, aggrs

verbose = False

def execute(key_extractor, reducers, input):
	"""Execute reducers on the input lines and returns the result as the table:
		table = dictionary
		table[key_values] = sequence of results
			key_values: tuple of (key_name, key_value) pairs
		table[key_values][index] = (operator, {name:value, ...})
			index: int. the order of results
			operator: string. name of operator (e.g. 'sum').
					  It should be a procedure of aggrs module.
	"""
	table = {}

	n = len(reducers)
#	table['oplist'] = tuple([x[0] for x in reducers])

	num_line = 0
	for line in input:
		try:
			num_line += 1
			line = line.strip()

			key = key_extractor(line).items()
			key.sort()
			key = tuple(key)

			if not table.has_key(key):
				table[key] = [{} for x in reducers]
			rvs = table[key]

			for i in range(n):
				f, ve = reducers[i]
				if verbose: print >> sys.stderr, f, ve
				for vk, vv in ve(line).items():
					if not rvs[i].has_key(vk):
						rvs[i][vk] = ''
					if not vv:
						vv = ''
					rvs[i][vk] = eval("aggrs." + f)(rvs[i][vk], vv)
		except ValueError, err:
			print >> sys.stderr, "ValueError:", err, "at line", num_line
			print >> sys.stderr, line
			sys.exit(1)

	return table


def debug_print_kv(key, value):
	print "%7d\t<N:%s" % (value['sum']['value'], key[0][1])

class OutputFormatter:
	"""Class for output related functions"""
	def __init__(self, key_attr_names, reduce_names, output=sys.stdout, format=None):
		self.ofp = output
		self.key_attr_names = key_attr_names
		self.reduce_names = reduce_names
		self.set_format(format)

	def set_format(self, format):
		if format in [None, 'tsv', 'tsv-without-headers']:
			self.format = format
		else:
			raise ValueError, "Unknown format"

	def print_table(self, table):
		if not self.format:
			for k, v in table.items():
				self._print_kv(k, v)
		elif self.format[:3] == 'tsv':
			if self.format[3:] != '-without-headers':
				self._print_tsv_headers(self.key_attr_names, self.reduce_names)

			for k, v in table.items():
				self._print_kv_tsv(k, v)

	def _print_kv(self, key, value):
		self._print_key(key)
		map(self._print_rv,
			[(self.reduce_names[i], value[i]) for i in range(len(self.reduce_names))])
		self.ofp.write("\n")

	def _print_key(self, key):
		self.ofp.write("<k")
		map(self._print_attr, key)
		self.ofp.write(">")

	def _print_rv(self, rv):
		self.ofp.write("<%s" % rv[0][0])
		map(self._print_attr, rv[1].items())
		self.ofp.write(">")

	def _print_attr(self, attr):
		self.ofp.write(" %s" % '='.join( map(str, attr) ))


	def _print_tsv_headers(self, key, value):
		key.sort()
		self.ofp.write('\t'.join( ['k.' + k for k in key] ))

		for i in range(len(self.reduce_names)):
			self.ofp.write('\t')
			self.ofp.write('\t'.join([value[i][0] + '.' + k for k in value[i][1]]))
		self.ofp.write('\n')
		
	def _print_kv_tsv(self, key, value):
		self.ofp.write('\t'.join( [str(v) for k, v in key] ))

		for rvv in value:
			keys = rvv.keys()
			keys.sort()
			vs = '\t'.join( map(str, [rvv[k] for k in keys]) )
			self.ofp.write('\t' + vs)
		self.ofp.write("\n")
	

def create_val_extractor(pattern, exp=None):
	pattern = pattern.strip()
	if pattern[0] == '/':
		extractor = re_extractor(pattern[1:-1])
	else:
		extractor = value_extractor(pattern)

	if not exp:
		return extractor.extract, extractor.names

	val_evaluator, names = compile_exp(exp)

	return lambda line: val_evaluator(extractor.extract(line)), names

def compile_exp(exp):
	"""
	CharsNotIn instead of Word, because of korean characters.
	Refinements should be necessary.
	Whitespaces are not allowed between tokens.
	"""
	from pyparsing import CharsNotIn, Literal, delimitedList, Group, Dict
	import string

	delims = ':,()' + string.whitespace
	param_exp = CharsNotIn(delims) + Literal('(').suppress() \
		    + delimitedList( CharsNotIn(delims), delim=',' ) + Literal(')').suppress()
	param = Group( CharsNotIn(delims) + Literal(':').suppress() + param_exp )
	exp_parser = Dict( delimitedList(param, delim=',') )

	params = exp_parser.parseString(exp)
	param_names = params.keys()
	param_names.sort()
	
	return lambda vals: params_eval(params.items(), vals), param_names

def params_eval(params, vals):
	result = dict()
	for n, v in params:
		result[n] = param_val_eval(v, vals)
	return result

def param_val_eval(param, vals):
	args = []
	for arg in param[1:]:
		if arg in vals:
			args.append( vals[arg] )
		else:
			args.append( arg )
	return aggrs.__dict__[param[0]](*args)


class re_extractor:
	p = None
	names = []
	
	def __init__(self, pattern):
		self.p = re.compile(pattern)
		self.names = self.p.groupindex.keys()
		self.names.sort()

	def extract(self, line):
		m = self.p.search(line)
		if not m:
			raise ValueError, `self.p.pattern` + ' does not match'
		return m.groupdict()

class value_extractor:
	names = ['value']

	def __init__(self, value):
		self.value = value

	def extract(self, line):
		return { 'value': self.value }
	

import re
re_func = re.compile(r"(?P<op>\w+)\((?P<arg>.*)\)")
def funcparse(string):
	m = re_func.match(string)
	op = m.group('op')
	arg = m.group('arg')
	value_extractor, names = create_val_extractor(arg)
	return (op, value_extractor, names) 

import optparse
def run():
	usage = r"""usage: %prog [options] [-k <key_pattern>] [-j <key_expression>] <reduce_op>(<value_pattern>) ...

	Perform <reduce_op>(<value_pattern>), e.g. sum(1), for input lines
	grouped by <key_pattern>, e.g. /<L id=(?P<id>[^> ]*)/ if it is given.

	<key_pattern> and <value_pattern> are recognized as strings or numbers
	unless surrounded by slashes(/.../). If the pattern is surrounded by
	slashes, it is recognized as a RE(regular expression) and the
	sub-expression, which is surrounded by parenthesis, is taken as the
	key_value or the argument of <reduce_op>. If the sub-expression is
	named by P extension, e.g. (?P<name>\w*), the name is used as the
	key_name or the value_name of the output line.

	%prog outputs the result of <result_op> for each <key_pattern>
	as following format unless --tsv option is given:
		<K (key_name=key_value)+>(<reduce_op (value_name=result)+>)+
	"""
	parser = optparse.OptionParser(usage=usage)
	
	parser.add_option("--tsv",
		action="store_true", dest="tsv", default=False,
		help="output in TSV(tab separated values) format")

	parser.add_option("-k", "--key-pattern",
		dest="key_ptn", metavar="KEY_PATTERN",
		help="find KEY_PATTERN once for each line")

	parser.add_option("-j", "--key-expression",
			  dest="key_exp", metavar="KEY_EXPRESSION",
			  help="evaluate key values using the functions defined in aggrs.py")

	(opts, args) = parser.parse_args()

	if opts.key_ptn:
		key_extractor, key_attr_names = create_val_extractor(opts.key_ptn, opts.key_exp)
	else:
		key_extractor, key_attr_names = create_val_extractor('all')

	reducers = map(funcparse, args)
	table = execute(key_extractor, [r[:2] for r in reducers], sys.stdin)

	reduce_names = [(t[0], t[2]) for t in reducers]
	fmt = OutputFormatter(key_attr_names, reduce_names)
	if opts.tsv:
		fmt.set_format('tsv')

	fmt.print_table(table)

if __name__ == "__main__":
	run()
