# Copyright 2017 The ChaosBase Authors. All Rights Reserved.
# -*- coding: utf-8 -*-

'''
Created on 2017/08/13
@author kerry
'''
import sys
sys.path.append('../../')
import platform
import pdb

from file_engine import FileEngine
from file_engine import FileIO
from parser_engine import ParserEngine

if __name__ == '__main__':

    if platform.system() == "Darwin" or platform.system() == "Linux":
        reload(sys)
        sys.setdefaultencoding('utf-8')

    file_engine = FileEngine("/kywk/fin/futures/data", "*.txt")
    file_io = FileIO("/kywk/fin/pb/futures")
    for file_unit in file_engine.fetch_files():
        parsers = ParserEngine()
        for i,co,filename in parsers.parser(file_unit.read()):
            #(exchange_id, contract_object, symbol, name, conten
            file_io.write_binary_stream(file_unit.exchange, co, file_unit.symbol, filename, i)
