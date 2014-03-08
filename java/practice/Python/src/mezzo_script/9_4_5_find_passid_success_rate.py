#!/usr/bin/env python
# -*- coding: utf_8 -*-
"""
ID비밀번호찾기_단계별_전환율분석_박민수_20081216
"""

import sys, os
import time, datetime
from time import strptime
import getopt
#sys.path.insert(0,os.path.join(os.getenv('MEZZO_HOME', '/home1/irteam/mezzo') + '/scripts/lib'))
from mezzo_util import *
import time, datetime
import csv


def usage(program):
    print >> sys.stderr, "\nUsage: " + program + "\t[-d] [<date string>]"
    print >> sys.stderr, "\t\t-d\tDebug mode\n\n"


def main():

    optlist = []
    args = sys.argv[1:]
    progname = os.path.basename(sys.argv[0])
    x = progname.rfind('.')
    if x > 0: progname = progname[:x]    

    try :
        optlist, args = getopt.getopt( args, 'd' )
    except getopt.GetoptError:
        usage(sys.argv[0])
        sys.exit(1)

    debug_mode = False

    for opt, arg in optlist:
        if opt == '-d':
            debug_mode = True

    start_time, end_time = parse_timerange(args)
#    target_date_path = os.path.join(target_path, start_time.strftime('%Y/%m/%d'))
#    os.system('mkdir -p ' + target_date_path)
#    outfile = open(os.path.join(target_date_path, '9_4_5_find_passid_detail_analysis.tsv'), 'wb')
    outfile = open(os.path.join('./' + progname + '.tsv'), 'wb')

    start = time.ctime()
    print 'Start To: ', start

    hdrs = ['날짜','하위구분','인증구분','SP','PV','UV']

    import csv
    writer = csv.DictWriter(outfile, hdrs, dialect='excel-tab')
    writer.writerow(dict(zip(hdrs,hdrs)))

    findPass = cntFindPass( start_time, end_time, progname )
    changePass = cntChangePass( start_time, end_time, progname )
    otherAuthCheck = cntOtherAuthCheck( start_time, end_time, progname )

    
    for row in otherAuthCheck:
        result = dict()
        result['날짜'] = start_time.strftime('%Y-%m-%d')

        result['하위구분'] = row['k.find_cls']
        
        result['인증구분'] = 'D'
        result['SP'] = row['k.sp']

        if result['SP'] == '신분증보내기_정보입력' or result['SP'] == '신분증보내기_확인완료':
            result['인증구분'] = 'F'
        result['PV'] = row['sum.pv']
        result['UV'] = row['sum.value']

        writer.writerow(result)

    for row in findPass:
        sp = row['k.sp']

        result = dict()
        result['날짜'] = start_time.strftime('%Y-%m-%d')
        if(sp == '비밀번호찾기'):
            result['하위구분'] = 'P'
        else:
            result['하위구분'] = 'I'
        result['인증구분'] = 'X'
        result['SP'] = sp
        result['PV'] = row['sum.pv']
        result['UV'] = row['sum.value']        
         
        writer.writerow(result)

    temp = dict()

    for row in changePass:
        result = dict()
        result['날짜'] = start_time.strftime('%Y-%m-%d')
        result['하위구분'] = row['k.find_cls']
        result['인증구분'] = row['k.authtype']
        result['SP'] = row['k.sp']
        result['PV'] = row['sum.pv']
        result['UV'] = row['sum.value']

        writer.writerow(result)

        temp_key = (result['하위구분'],result['인증구분'])

        if temp_key not in temp.keys():
            temp[temp_key] = [0,0]

        if result['SP'] != '비밀번호변경_완료':
            temp[temp_key][0] = temp[temp_key][0] + int(result['PV'])
            temp[temp_key][1] = temp[temp_key][1] + int(result['UV'])

    for row in temp:

        result=dict()
        result['날짜'] = start_time.strftime('%Y-%m-%d')
        result['하위구분'] = row[0]
        result['인증구분'] = row[1]

        if row[1] == 'H':
            result['SP'] = '휴대폰 인증'
        elif row[1] == 'I':
            result['SP'] = '신용카드 인증'
        elif row[1] == 'P':
            result['SP'] = '공인인증서 인증'
        elif row[1] == 'A':
            result['SP'] = '본인인증 질문답변'

        result['PV'] = temp[row][0]
        result['UV'] = temp[row][1]

        writer.writerow(result)

    end = time.ctime()
    print 'End To: ', end


# ID비밀번호 찾기 > ID찾기 > 비밀번호찾기 | 전체ID보기
def cntFindPass( start_time, end_time, progname ):
    
    regex_pattern = '<[^>]*(?P<sp>sp="한게임/한게임회원/ID비밀번호찾기/ID찾기/(비밀번호찾기|전체ID보기))[^>]*>'
    regex_format = '\g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc="(?P<id>[^> ]*)" sp="(?P<sp>한게임/한게임회원/ID비밀번호찾기/ID찾기/(비밀번호찾기|전체ID보기))/'
    aggr_key_expr = 'id:echo(id),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/id=[^> ]* sp=한게임/한게임회원/ID비밀번호찾기/ID찾기/(?P<sp>비밀번호찾기|전체ID보기)/'
    aggr_key_expr1 = 'sp:echo(sp)'
    aggr_op_list1 = ['sum(1)','sum(/value=(?P<pv>\d*)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, aggr_cmd], progname)
    os.system( hexec_cmd )

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


# ID비밀번호 찾기 > ID찾기 > 비밀번호변경 | 비밀번호변경_완료
def cntChangePass( start_time, end_time, progname ):
    
    regex_pattern = '<[^>]*(?P<authtype>authtype=[^> ]*)[^>]*(?P<find_cls>find_cls=[^> ]*)[^>]*(?P<sp>sp="한게임/한게임회원/ID비밀번호찾기/ID찾기/(비밀번호변경_완료|비밀번호변경|본인인증실패))[^>]*>'
    regex_format = '\g<authtype> \g<find_cls> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc="(?P<id>[^> ]*)" authtype="(?P<authtype>[^> ]*)" find_cls="(?P<find_cls>[^> ]*)" sp="(?P<sp>한게임/한게임회원/ID비밀번호찾기/ID찾기/(비밀번호변경_완료|비밀번호변경|본인인증실패))/'
    aggr_key_expr = 'id:echo(id),authtype:echo(authtype),find_cls:echo(find_cls),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/authtype=(?P<authtype>[^> ]*) find_cls=(?P<find_cls>[^> ]*) id=[^> ]* sp=한게임/한게임회원/ID비밀번호찾기/ID찾기/(?P<sp>비밀번호변경_완료|비밀번호변경|본인인증실패)/'
    aggr_key_expr1 = 'authtype:echo(authtype),find_cls:echo(find_cls),sp:echo(sp)'
    aggr_op_list1 = ['sum(1)','sum(/value=(?P<pv>\d*)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, aggr_cmd], progname)
    os.system( hexec_cmd )

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1, 'rb') )


# ID비밀번호 찾기 > ID 찾기 > 비밀번호 찾기 > 신분증보내기_정보입력 | 신분증보내기_확인완료 | 주민증발급일자_등록완료 | 주민증발급일자_정보입력
def cntOtherAuthCheck( start_time, end_time, progname ):
    regex_pattern = '<[^>]*(?P<find_cls>find_cls=[^> ]*)[^>]*(?P<sp>sp="한게임/한게임회원/ID비밀번호찾기/ID찾기/비밀번호찾기/(신분증보내기_정보입력|신분증보내기_확인완료|주민증발급일자_등록완료|주민증발급일자_정보입력))[^>]*>'
    regex_format = '\g<find_cls> \g<sp>'
    regex_key_pattern = '<K[^>]*(?P<id>bc=[^> ]*)[^>]*>'
    regex_key_format = '\g<id> '

    aggr_key_pattern = '/bc="(?P<id>[^> ]*)" find_cls="(?P<find_cls>[^> ]*)" sp="(?P<sp>한게임/한게임회원/ID비밀번호찾기/ID찾기/비밀번호찾기/(신분증보내기_정보입력|신분증보내기_확인완료|주민증발급일자_등록완료|주민증발급일자_정보입력))/'
    aggr_key_expr = 'id:echo(id),find_cls:echo(find_cls),sp:echo(sp)'
    aggr_op_list = ['sum(1)']

    aggr_key_pattern1 = '/find_cls=(?P<find_cls>[^> ]*) id=[^> ]* sp=한게임/한게임회원/ID비밀번호찾기/ID찾기/비밀번호찾기/(?P<sp>신분증보내기_정보입력|신분증보내기_확인완료|주민증발급일자_등록완료|주민증발급일자_정보입력)/'
    aggr_key_expr1 = 'find_cls:echo(find_cls),sp:echo(sp)'
    aggr_op_list1 = ['sum(1)','sum(/value=(?P<pv>\d*)/)']

    regex_cmd = make_regex_cmd(regex_pattern, regex_format, regex_key_pattern, regex_key_format)
    aggr_cmd = make_aggr_cmd(aggr_key_pattern, aggr_op_list, aggr_key_expr, False)
    aggr_cmd1 = make_aggr_cmd(aggr_key_pattern1, aggr_op_list1, aggr_key_expr1, True)

    hexec_cmd = make_hexec_cmd(start_time, end_time, [regex_cmd, aggr_cmd], progname)
    os.system( hexec_cmd )

    return create_tsvreader( os.popen('zcat ' + progname + '.gz | ' + aggr_cmd1,'rb') )


if __name__ == '__main__':
    main()
