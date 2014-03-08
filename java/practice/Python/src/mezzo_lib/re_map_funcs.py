#! /usr/bin/env python
# -*- coding: utf_8 -*-

from operator import *

gamegroup_list = { "hanyut":"캐주얼/신윷놀이", \
                 "msame":"캐주얼/세임팡팡", \
                 "same":"캐주얼/세임팡팡_싱글모드", \
                 "panpan":"캐주얼/체인지팡팡", \
                 "diff":"캐주얼/틀린그림찾기", \
                 "reverse":"캐주얼/퍼즐마작", \
                 "solitaire":"캐주얼/상하이마작", \
                 "cross":"캐주얼/가로세로퀴즈", \
                 "bingo":"캐주얼/빙고", \
                 "gmahjong":"캐주얼/신사천성", \
                 "solcard":"캐주얼/카드놀이", \
                 "hanbingo":"캐주얼/대박빙고", \
                 "billmania":"캐주얼/당구매니아", \
                 "bill3b":"캐주얼/삼구매니아", \
                 "omok":"캐주얼/오목", \
                 "battleomok":"캐주얼/배틀오목", \
                 "chess":"캐주얼/체스", \
                 "bby":"캐주얼/빵빠야", \
                 "bridge":"캐주얼/브릿지", \
                 "mahjong":"캐주얼/사천성", \
                 "hexadlx":"캐주얼/헥사", \
                 "mine":"캐주얼/지뢰찾기", \
                 "nemo":"캐주얼/네모네모", \
                 "onecard":"캐주얼/원카드", \
                 "sbbuster":"캐주얼/블로키팡팡싱글", \
                 "yutnori2":"캐주얼/윷놀이", \
                 "tet4":"캐주얼/테트리스", \
                 "dxgostop":"고스톱류/고스톱2", \
                 "gostop":"고스톱류/고스톱", \
                 "msduelgo":"고스톱류/신맞고", \
                 "newgostop":"고스톱류/신고스톱", \
                 "duelgo":"고스톱류/맞고", \
                 "doublego":"고스톱류/더블맞고", \
                 "sambong":"고스톱류/삼봉", \
                 "whatu":"고스톱류/민화투", \
                 "duelpoker":"포커류/맞포커", \
                 "baduki":"포커류/바둑이", \
                 "laspoker":"포커류/라스베가스포커", \
                 "sudda2":"포커류/섯다", \
                 "poker7":"포커류/7포커", \
                 "highlow":"포커류/하이로우", \
                 "hoola3":"포커류/파티훌라"}


def diff(a, b):
    """Returns the string of the value of a - b"""
    return str(long(a) - long(b))

def extract_paths(tags):
    import re
    p = re.compile('<[^>]* sp=(?P<sp>[^> ]*)[^>]*>')
    return ' '.join( [ m.group('sp') for m in p.finditer(tags)] )


# function  : mk_timestamps_every_5m
# parameter : a - 시작시간, b - 종료시간, c - 게임구분, d - 경로구분
# contact   : 하루를 1분 간격으로 나누고, 사용자들이 게임을 하는 시간대가 어디에 속하는지 출력한다.
def mk_timestamps_every_5m(a, b, c):
    stime = int(a)
    etime = int(b)

    sm = ( stime % 86400 ) / 300
    em = ( etime % 86400 ) / 300
    result = ""
    for x in range(sm, em + 1) :
        result += "<time=" + str(x) + " " + str(c) + ">"

    return result


# function  : get_utime_section_1
# parameter : stime - 시작 시간(타임스템프), etime - 종료 시간
# contact   : 시작 시간과 종료 시간을 통해 이용 시간을 구하고, 이용 시간을 기준에 따라 분류 한다. 
def get_utime_section_1(utime):
    """ 1:1분 미만, 2:1~5분, 3:5~10분, 4:10~20분, 5:20~30분, 6:30~1시간, 7:1~2시간, 8:2시간 이상 """
    utime = int(utime)
    if utime < 60 :
        return "1"
    elif utime < 300 :
        return "2"
    elif utime < 600 :
        return "3"
    elif utime < 1200 :
        return "4"
    elif utime < 1800 :
        return "5"
    elif utime < 3600 :
        return "6"
    else: 
        return str((utime // 3600) + 6 )

# function  : get_utime_section_2
# parameter : utime - 이용시간
# contact   : 이용 시간을 기준에 따라 분류 한다.
def get_utime_section_2(stime, etime):
    """ 1:1분 미만, 2:1~5분, 3:5~10분, 4:10~20분, 5:20~30분, 6:30~1시간, 7:1~2시간, 8:2시간 이상 """
    utime = diff(etime, stime)
    return get_utime_section_1(utime)

# function  : game_result
# parameter : 게임의 머니 변화량
# contact   : 게임머니 변화량이 - 이면 '패', + 이면 '승', 0 이면 '무승부'
def game_result(_change_money):
    money = long(_change_money)
    win = 0
    lose = 0
    draw = 0

    if money < 0:
        lose = 1
    elif money > 0:
        win = 1
    else:
        draw = 1

    cmd = "win=" + str(win) +" lose=" + str(lose) + " draw=" + str(draw)
    return cmd
        
# function  : earn_mny
# parameter : 게임전 머니, 게임후 머니
# contact   : 게임전 머니와 게임후 머니를 계산하여 잃은 머니와 딴머니를 구한다.
def earn_mny( _before_money, _after_money ):
    changing_money = long(diff(_after_money,_before_money))
    earning_money = 0
    losing_money = 0

    if changing_money > 0:
        earning_money = changing_money
    else:
        losing_money = changing_money

    cmd = "earning_money=" + str(earning_money) + " losing_money=" + str(losing_money)
    return cmd
        

# function  : ganging_gamelist
# parameter : 매칭 내용
# contact   : 매칭 내용과 게임 리스트의 Key값이 일치하는 경우, 매칭내용을 매칭 리스트의 Value값으로 변환한다.
def changing_gamelist( _value ):
    value = _value.lower()
    if value in gamegroup_list.keys():
        return gamegroup_list[value]
    return ''

def cur_time(_value):
    import time
    temp_time = str(float(time.time())*100)
    a = temp_time[0:12]
    return a
