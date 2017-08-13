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
        self._read_buf = None
        self._writable = None
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
    pass

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
    pass


def walk(top, in_order=True):
    """Recursive directory tree generator for directories.

        Args:
            top: string, a Directory name
            in_order: bool, Traverse in order if True, post order if False.

        Errors that happen while listing directories are ignored.

        Yields:
            Each yield is a 3-tuple:  the pathname of a directory, followed by lists of
            all its subdirectories and leaf files.
    (dirname, [subdirname, subdirname, ...], [filename, filename, ...])
        as strings
    """
    top = compat.as_str_any(top)
    try:
        listing = list_directory(top)
    except :
        return


