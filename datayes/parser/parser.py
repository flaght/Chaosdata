# -*- coding: utf-8 -*-

'''
Created on 2017/08/13
@author kerry
'''
import platform
import sys
import FileEngine, FileUnit


if __name__ == '__main__':

    if platform.system() == "Darwin" or platform.system() == "Linux":
        reload(sys)
        sys.setdefaultencoding('utf-8')

    file_engine = FileEngine("/Users/kerry/work/pj/gitfork/chaosdata", "*.txt")
    for file_unit in file.fetch_files():
        print "exchange:" + file_unit.exchange() + "||symbol:" + file_unit.symbol() + "||filepath" + file_unit.full_filename() 
