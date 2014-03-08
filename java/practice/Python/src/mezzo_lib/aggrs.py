#!/usr/bin/env python
# These functions are called so many times.
# Code them EFFICIENTLY as possible

def sum(result, value):
	try:
		result = long(result)
	except ValueError:
		result = 0
	try:
		value = long(value)
	except ValueError:
		value = 0
	return result + value


def fsum(result, value):
	try:
		result = float(result)
	except ValueError:
		result = 0.
	try:
		value = float(value)
	except ValueError:
		value = 0.
	return result + value


def echo(value):
    return value


def sub(v1, v2):
	return long(v1) - long(v2)


#	import math
#	if not value: return long(0)
#	return long( math.floor(long(value) / long(divider)) )

def divide(_value, _divider):
    return long(_value)/long(_divider)


def divide_floor(divider, value):
    import math
#       if not value: return long(0)
    quotient = 0

    try:
        value = long(value)
    except:
        return long(0)
    quotient = long( math.floor(value / long(divider)) )

    if quotient > 2:
        quotient = 2

    return quotient

def divide_thirty(value1, value2):
    import math

    temp_value = 0
    temp_cls = 0

    try:
        value1 = long(value1)
        value2 = long(value2)
    except:
        return temp_cls

    temp_diff = value1 - value2    

    temp_value = round(float(temp_diff) / float(600), 1)

    if temp_value < 1:
        temp_cls = 0
    elif temp_value >=1 and temp_value <= 2:
        temp_cls = 1
    elif temp_value > 2 and temp_value <= 3:
        temp_cls = 2
    elif temp_value > 3 and temp_value <= 6:
        temp_cls = 3
    elif temp_value > 6 and temp_value <= 12:
        temp_cls = 4
    elif temp_value > 12 and temp_value <= 18:
        temp_cls = 5
    elif temp_value > 18 and temp_value <= 30:
        temp_cls = 6
    elif temp_value > 30:
        temp_cls = 7

    return temp_cls

####################################################################
# 엔돌핀 데이터가 1.xxxE7과 같은 형식으로 나왔을 때 사용하던 내용.
# 2009년 4월 이후로는 사용하지 않음 (버그 수정으로 인해)
def endolphin( _value ):
    try:
        value = long(_value)
    except:
        place = _value.find('E')
        multiple_num = 1
        for i in range(long(_value[place+1:])):
            multiple_num = multiple_num * 10
        value = long(float(_value[:place]) * multiple_num)
        return value
    return value

#####################################################################


def sub_divide_floor(v1, v2, divider):
    return divide_floor( divider, sub(v1, v2) )

def abs2mship(game, abstype):
	if abstype == '': return ''
	if len(abstype.split('^')) < 3:
		return abstype 

	mships = abstype.split('#')

	gtype = game[0]
	for mship in mships:
		fields = mship.split('^')
		if len(fields) < 3: return abstype
		id, status, months = fields
		if (gtype == 'g' and id == 'GR' or gtype == 'p' and id == 'PB') \
		       and (status == '1' or status == '51'):
			return id

	for mship in mships:
		fields = mship.split('^')
		if len(fields) < 3: return abstype
		id, status, months = fields
		if (id == 'P1' or id == 'P2' or id == 'P3') \
		   and (status == '1' or status == '51'):
			return id

	return ''


def pluscode(game,value):
    if(value==''): return ''
    type=value.split('#')
    for x in type:
        if (game[0]=='p' and x[:2]=='PM'):
            code=x.split('^')
            if (code[1]=='1'):
                return code[0]
        elif (game[0]=='g' and x[:2]=='GM'):
            code=x.split('^')
            if (code[1]=='1'):
                return code[0]
        else:
            return ''


def del_matched_pattern(value):
    import re
    if(value == ''): return ''
    changedValue = re.sub(r"(?:=[^ ]* )"," ", value)    
    return changedValue
# 현재시간을 구하는 공식
def cur_time():
    import time
    a = str(time.time()[:12])
    b = float(a)
    return b

# 아바타 구매시 건 수를 계산하는 함수
def count_purchasing_avatar( _goodslist ):
    import re

    delimeters = re.findall('@',_goodslist)
    count = len(delimeters) + 1

    return count


#------------------------------------------------------
# membership : abstype과 pluscode를 하나의 필드에 표현.
#------------------------------------------------------


# 구 플러스 회원제와 신 플러스 회원제가 동시에 사용되던 시점에 사용한 회원제구분 - 웹 
def web_membership( _gametype, _abstype, _pluscode ):
    if _abstype == '' and _pluscode == '':
        return ''
    elif _abstype == '' and _pluscode != '':
        type = _pluscode.split('#')
        pPluscode = ''
        gPluscode = ''

        for pluscode in type:
            code = pluscode.split('^')
            if len(code) < 2 : return _pluscode
            if (_gametype[0] == 'p' and pluscode[:2] == 'PM' and code[1] == '1'):
                pPluscode = code[0]
            elif (_gametype[0] == 'g' and pluscode[:2] == 'GM' and code[1] == '1'):
                gPluscode = code[0]

        if (_gametype[0] == 'p'):
            return pPluscode
        elif (_gametype[0] == 'g'):
            return gPluscode
        else:
            return ''
    elif _pluscode == '' and _abstype != '':
        if len(_abstype.split('^')) < 3:
            return _abstype

        mships = _abstype.split('#')

        gtype = _gametype[0]
        for mship in mships:
            fields = mship.split('^')
            if len(fields) < 3: return _abstype
            id, status, months = fields
            if ((gtype == 'g' and id == 'GR') or (gtype == 'p' and id == 'PB')) \
               and (status == '1' or status == '51'):
                return id

        for mship in mships:
            fields = mship.split('^')
            if len(fields) < 3: return _abstype
            id, status, months = fields
            if (id == 'P1' or id == 'P2' or id == 'P3') \
               and (status == '1' or status == '51'):
                return id
        return ''


# 구 플러스 회원제와 신 플러스 회원제가 동시에 사용되던 시점에 사용한 회원제 구분 - 클라이언트
def client_membership( _mshiptype, _goodslist ):
    import re

    if ( _mshiptype == '' and _goodslist == '' ):
        return ''
    elif ( _mshiptype == '' and _goodslist != ''):
        goodslist = re.findall('(GM\d+|PM\d+),', _goodslist)

        highest_goodslist = '' 
        for row in goodslist:
            if(highest_goodslist < row):
                highest_goodslist = row
        return highest_goodslist
    elif ( _goodslist == '' and _mshiptype != ''):
        return _mshiptype[:2]


# 신 플러스 회원제에 대한 회원제 구분 - 클라이언트
def client_membership_type( _goodslist ):
    import re

    if ( _goodslist == '' ):
        return ''
    else:
        goodslist = re.findall('(GM\d+|PM\d+),', _goodslist)

        highest_goodslist = ''
        for row in goodslist:
            if(highest_goodslist < row):
                highest_goodslist = row
        return highest_goodslist


def multiple( _value1, _value2 ):
    try:
        value1 = long(_value1)
    except ValueError:
        value1 = 0

    try:
        value2 = long(_value2)
    except ValueError:
        value2 = 0

    multipleValue = value1 * value2

    return multipleValue


def price_membership( _goodslist ):
    import re

    if ( _goodslist == '' ):
        return '0'
    else:
        sumPrice = 0
        goodslist = re.findall(',(?!\[)(?P<price>[^\]]+)',_goodslist)
        for price in goodslist:
            sumPrice = sumPrice + long(price)
        return str(sumPrice)

#-------------------------------------------------
# url을 domain만 끊어서 사용
#-------------------------------------------------
def remain_domain( _url ):
    import re
    domain = re.match('(http|https)://[^>/ ]+/',_url)
    if domain:
        return domain.group()
    else:
        return _url


# 회원제에 상관없이 회원제 가입여부 판단
def is_membership( _membership ):
    if _membership == '':
        ismember = 'N'
    else:
        ismember = 'Y'

    return ismember


#def len(str): same as default function len()
len=len

# 금액을 구간별로 설정
def p_mny_class(p_mny):
    l = len(p_mny)
    if l == 0: return ''
    if l <= 7:
        l = 7
    elif l >= 14:
        l = 14
    return str(l)


# ts를 이용해서 ts의 시간이 언제인지 확인하는 함수
def time_is(value):
    try:
        value = long(value)
        hour = ((value + 32400) % 86400) / 3600
    except:
        return long(0)

    return hour




def p_mny_class2(p_mny):
    l = len(p_mny)
    if l == 0: return ''
    if l <= 7:
        l = 7
    elif l >= 14:
        l = 14
    return '"' + str(l) + '"'

def replace_if_null(primary, secondary):
    primary = primary.strip('"')
    if primary: # != ''
        return '"' + primary + '"'
    else:
        return '"%s"' % secondary.strip('"')


# 금칙어 사용 구간 설정 (1~10회이상)
def abuse_using_slice(_value):
    value = long(_value)

    if value < 10:
        return str(value)
    else:
        return str(10)

# 포커 판수 구하는 함수 
# 파라미터 - 무승부, 승리, 패배
def cnt_playgame( _draw, _win, _lose ):
    tot_cnt = long(_draw) + long(_win) + long(_lose)
    return tot_cnt

