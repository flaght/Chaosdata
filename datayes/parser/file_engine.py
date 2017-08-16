# Copyright 2017 The ChaosBase Authors. All Rights Reserved.
# -*- coding: utf-8 -*-


from __future__ import absolute_import
from __future__ import division
import os
import fnmatch
import struct
from base.lib.io import file_io
import pdb
class FileUnit(object):
    def __init__(self):
        self.__exchange = None
        self.__symbol = None
        self.__full_filename = None

    @property
    def exchange(self):
        return self.__exchange

    @property
    def symbol(self):
        return self.__symbol

    @property
    def full_filename(self):
        return self.__full_filename

    def set_unit(self, dir, filename):
        #/kywk/fin/futures/data/XDCE/A1203.XDCE
        self.__symbol = os.path.split(dir)[-1]
        parent_dir = os.path.split(dir)[0]
        self.__exchange = os.path.split(parent_dir)[-1]
        self.__full_filename = filename

    def read(self):
        return file_io.read_file_to_string(self.__full_filename)



class FileIO(object):
    def __init__(self, base_path):
        self.__base_path = base_path

    def write_date_mkt(self, exchange_id, contract_object, symbol, name, content): #写入日数据
        #/root/XDCE/FB/FB1604/201708.txt
        dirname = self.__base_path + "/" + exchange_id + "/" + contract_object + "/" + symbol
        self.write_binary_stream(dirname, name, content)

    def write_binary_stream(self, dirname, name, content):
        stream_len = 2 + len(content)
        str_format = '=h%ds' %(len(content))
        binary_stream = struct.pack(str_format,stream_len,content)
        if not os.path.exists(dirname):
            os.makedirs(dirname)
        full_filename = dirname + "/" + name
        print full_filename
        file_object = open(full_filename, 'ab')
        file_object.write(binary_stream)
        file_object.close()


class FileEngine(object):

    def __init__(self, path, filter = "*.*"):
        self.__path = path
        self.__filter = filter

    @property
    def path(self):
        return self.__path

    @property
    def filter(self):
        return self.__filter

    def fetch_files(self):
        for root, _, filenames in file_io.walk(self.__path):
            for f in fnmatch.filter(filenames, '*.txt'):
                path = os.path.join(root, f)
                file_unit = FileUnit()
                file_unit.set_unit(root, path)
                yield file_unit
