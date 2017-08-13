# -*- coding: utf-8 -*-

'''
Created on 2017/08/13
@author kerry
'''
import platform
import sys
from tools.base.conf_manager import ConfManager


if __name__ == '__main__':

    if platform.system() == "Darwin" or platform.system() == "Linux":
        reload(sys)
        sys.setdefaultencoding('utf-8')
