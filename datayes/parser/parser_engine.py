# Copyright 2017 The ChaosBase Authors. All Rights Reserved.
# -*- coding: utf-8 -*-

import json

class ParserEngine(obeject):

    def __init__(self):
        pass

    def parser(self, content):
        try:
            datayes =json.loads(content)
            ret_code = datayes['retCode']
            if ret_code != 1:
                return None
            data = datayes['data']
            for unit in data:

        except Exception as e:
            return None
