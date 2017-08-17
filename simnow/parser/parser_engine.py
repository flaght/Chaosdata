# Copyright 2017 The ChaosBase Authors. All Rights Reserved.
# -*- coding: utf-8 -*-

import json
import time
import depth_market_pb2
import struct
import pdb
class ParserEngine(object):

    def __init__(self):
        pass

    def recombinant_packet(self,packet):
        depth_market = depth_market_pb2.DepthMarket()
        depth_market.ParseFromString(packet) 
        current_time = depth_market.current_time
        format = '%Y-%m-%d'
        value = time.localtime(current_time/1000)
        dt = time.strftime(format, value)
        #pdb.set_trace()
        year = dt.split('-')[0]
        month = dt.split('-')[1]
        day = dt.split('-')[-1]
        exchange_id = depth_market.exchange_id
        contract_object = depth_market.product_id
        symbol = depth_market.symbol
        return depth_market.SerializeToString(),year + "-" + month, day, exchange_id, contract_object, symbol
    
    def parser(self, content):
        print len(content)
        struct_format = '=h'
        prefix_length = struct.calcsize(struct_format)
        unprocessed = ""
        alldata = unprocessed + content
        current_offset = 0
        fmt = struct_format
        while len(alldata) >= (current_offset + prefix_length):
            message_start = current_offset + prefix_length
            length, = struct.unpack(fmt, alldata[current_offset:message_start])
            message_end = current_offset + length
            packet = alldata[current_offset + prefix_length:message_end]
            current_offset = message_end
            yield self.recombinant_packet(packet)
