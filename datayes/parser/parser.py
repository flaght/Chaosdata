# -*- coding: utf-8 -*-

'''
Created on 2017/08/13
@author kerry
'''
import sys
sys.path.append('../../')
import platform


from file_engine import FileEngine


if __name__ == '__main__':

    if platform.system() == "Darwin" or platform.system() == "Linux":
        reload(sys)
        sys.setdefaultencoding('utf-8')
    
    file_engine = FileEngine("/kywk/fin/futures/data", "*.txt")
    for file_unit in file_engine.fetch_files():
        print file_unit.exchange + '-->' + file_unit.symbol + '-->' + file_unit.full_filename
