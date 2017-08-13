# Copyright 2017 The ChaosBase Authors. All Rights Reserved.

from __future__ import absolute_import
from __future__ import division


import os
import uuid
from chaosdata.base import compat

class FileIO(object):
  """FileIO class that exposes methods to read / write to / from files.

  The constructor takes the following arguments:
  name: name of the file
  mode: one of 'r', 'w', 'a', 'r+', 'w+', 'a+'.

  Can be used as an iterator to iterate over lines in the file.

  The default buffer size used for the BufferedInputStream used for reading
  the file line by line is 1024 * 512 bytes.
  """

    def __init__(self, name, mode):
        self.__name = name
        self.__mode = mode
        if mode not in ("r", "w", "a", "r+", "w+", "a+"):
            print "mode is not 'r' or 'w' or 'a' or 'r+' or 'w+' or 'a+'"
        self.__read_check_passed = mode in ("r", "r+", "a+", "w+")
        self.__write_check_passed = mode in ("a", "w", "r+", "a+", "w+")


    @property
    def name(self):
        return self.__name

    @property
    def mode(self):
        return self.__mode


def is_directory(dirname):
  """Returns whether the path is a directory or not.

  Args:
    dirname: string, path to a potential directory

  Returns:
    True, if the path is a directory; False otherwise
  """

def list_directory(dirname):
  """Returns a list of entries contained within a directory.

  The list is in arbitrary order. It does not contain the special entries "."
  and "..".

  Args:
    dirname: string, path to a directory

  Returns:
    [filename1, filename2, ... filenameN] as strings

  Raises:
    errors.NotFoundError if directory doesn't exist
  """


def walk(top, in_order=True):
    top = compat.as_str_any(top)
    try:
        listing = list_directory(top)
    except :
        return
    files = []
    subdirs = []
    for item in listing:
        full_path = os.path.join(top, item)
        if is_directory(full_path):
            subdirs.append(item)
        else:
            files.append(item)
    here = (top, subdirs, files)
    
