# Copyright 2017 The ChaosBase Authors. All Rights Reserved.
# -*- coding: utf-8 -*-


from __future__ import absolute_import
from __future__ import division
import os
import fnmatch
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

    def read():
        return file_io.read(self.__full_filename)



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
