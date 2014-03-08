#! /usr/bin/env python
# -*- coding: utf_8 -*-

import sys,os
import re
#import aggrs 

sys.path.append(r'.')

gameroom_area = ['게이지리필아이템','상단나가기','신고/신고하기','알리미','이용자정보/추방하기','이용자팝업','잭팟닫기','잭팟열기','족보탭','찬스', '창크기최대','채팅send','채팅탭','초대/초대하기','캡쳐','타임','하단나가기','헬프','헬프/게임방법','헬프/자동베팅룰','히스토리']

key_func2 = "<K[^>]*(bc=[^>]*)>"
regex_func2 = "<C (goodslist=[^> ]*) p_(hl|7p)_(ground=\"\d*\") (p_level=\"\d*\") sp=\"한게임/포커게임/(7포커|하이로우)/클라이언트/경기장대기실\"[^>]*>.*?<C [^>]*(sp=\"한게임/포커게임/(7포커|하이로우)/클라이언트/(방입장|방_입장)\")[^>]*>"
kp_func2 = re.compile(key_func2)
regex_compile_func2 = re.compile(regex_func2)

key1_re = "(<K[^>]*bc=[^>]*>)"
regex1_re = "(<[^>]*sp=\"한게임/포커게임/(7포커|하이로우)/클라이언트/경기장대기실\"[^>]*>)"

"""
key2_re = "(<K[^>]*bc=[^>]*><[^>]*sp=\"한게임/포커게임/(7포커|하이로우)/클라이언트/경기장대기실\"[^>]*>)"
regex2_re = "(<[^>]*sp=\"한게임/포커게임/(7포커|하이로우)/클라이언트/게임방/게이지리필아이템\"[^>]*>)"

key3_re = "<K[^>]*(bc=[^> ]*)[^>]*>"
regex3_re = "<C (goodslist=[^> ]*) p_(hl|7p)_(ground=\"\d*\") (p_level=\"\d*\") sp=\"한게임/포커게임/(7포커|하이로우)/클라이언트/경기장대기실\"[^>]*><C [^>]*(sp=\"한게임/포커게임/(7포커|하이로우)/클라이언트/게임방/게이지리필아이템\")[^>]*>" 
"""

kp1 = re.compile(key1_re)
regex1_compile = re.compile(regex1_re)

"""
kp2 = re.compile(key2_re)
regex2_compile = re.compile(regex2_re)

kp3 = re.compile(key3_re)
regex3_compile = re.compile(regex3_re)
"""

header_once =True
split_re = ''
for line in sys.stdin:

    func = True
    for name in gameroom_area:
        key2_re = "(<K[^>]*bc=[^>]*><[^>]*sp=\"한게임/포커게임/(7포커|하이로우)/클라이언트/경기장대기실\"[^>]*>)"
        regex2_re = "(<[^>]*sp=\"한게임/포커게임/(7포커|하이로우)/클라이언트/게임방/" + name +"\"[^>]*>)"

        key3_re = "<K[^>]*(bc=[^> ]*)[^>]*>"
        regex3_re = "<C (goodslist=[^> ]*) p_(hl|7p)_(ground=\"\d*\") (p_level=\"\d*\") sp=\"한게임/포커게임/(7포커|하이로우)/클라이언트/경기장대기실\"[^>]*><C [^>]*(sp=\"한게임/포커게임/(7포커|하이로우)/클라이언트/게임방/" + name + "\")[^>]*>"

        kp2 = re.compile(key2_re)
        regex2_compile = re.compile(regex2_re)

        kp3 = re.compile(key3_re)
        regex3_compile = re.compile(regex3_re)

        result = ''
        start1 = 0
        split_re = ''
    
        mo = kp1.search(line)
        if not mo: continue
        
        for key in regex1_compile.finditer(line.strip()):
            out = mo.group(1) + split_re.strip() + line[start1:key.start()]
            split_re = key.group(1)
            result = out + split_re
            start1 = key.end()
            
            if(func): #cntShowingGameroom
                for key_func2_ in regex_compile_func2.finditer(result.strip()):
                    mo_func2 = kp_func2.search(result)
                    if not mo_func2: continue
                    print mo_func2.group(1), key_func2_.group(3),key_func2_.group(1),key_func2_.group(4),key_func2_.group(6)
                    #sys.exit()
 
            mo2 = kp2.search(result)
            if not mo2: continue
        
            start2 = 0
            split_re2 = ''
            result2 = ''
            for key2 in regex2_compile.finditer(result.strip()):
                #print key2.group(1)
            
                out2 = mo2.group(1) + split_re2.strip() + result[start2:key2.start()]
                split_re2 = key2.group(1)
                result2 = out2 + split_re2
            
                #print result2#key2.group(1)
                start2 = key2.end()

                mo3 = kp3.search(result2)
                if not mo3: continue
        
                for key3 in regex3_compile.finditer(result2.strip()):
                    print mo3.group(1), key3.group(3), key3.group(1), key3.group(4), key3.group(6)

            if(result2): 
                result2_last = mo2.group(1) + split_re2 + result.rstrip()[start2:]
                    #print result2_last            
                mo3 = kp3.search(result2_last)
                if not mo3: continue
        
                for key3 in regex3_compile.finditer(result2_last.strip()):
                    print mo3.group(1), key3.group(3), key3.group(1), key3.group(4), key3.group(6)

        func=False

        if(result):      
            result_ =  mo.group(1) + split_re + line.rstrip()[start1:]

            
            for key_func2_ in regex_compile_func2.finditer(result_.strip()):
                mo_func2 = kp_func2.search(result_)
                if not mo_func2: continue
                print mo_func2.group(1),key_func2_.group(3),key_func2_.group(1),key_func2_.group(4),key_func2_.group(6)
            
            mo2 = kp2.search(result_)
            if not mo2: continue

            start2 = 0
            split_re2 = ''
            result2 = ''
            for key2 in regex2_compile.finditer(result_.strip()):
                #print key2.group(1)

                out2 = mo2.group(1) + split_re2.strip() + result_[start2:key2.start()]
                split_re2 = key2.group(1)
                result2 = out2 + split_re2

                #print result2#key2.group(1)
                start2 = key2.end()

                mo3 = kp3.search(result2)
                if not mo3: continue

                for key3 in regex3_compile.finditer(result2.strip()):
                    print mo3.group(1), key3.group(3), key3.group(1), key3.group(4), key3.group(6)

            if(result2):
                result2_last = mo2.group(1) + split_re2 + result.rstrip()[start2:]
                #print result2_last
                mo3 = kp3.search(result2_last)
                if not mo3: continue

                for key3 in regex3_compile.finditer(result2_last.strip()):
                    print mo3.group(1), key3.group(3), key3.group(1), key3.group(4), key3.group(6)
    

        
