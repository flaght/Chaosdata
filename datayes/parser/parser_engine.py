# Copyright 2017 The ChaosBase Authors. All Rights Reserved.
# -*- coding: utf-8 -*-

import json
import time
import datayes_mktfut_pb2
import pdb
class ParserEngine(object):

    def __init__(self):
        pass

    def parser(self, content):
        try:
            #pdb.set_trace()
            datayes =json.loads(content)
            ret_code = datayes['retCode']
            if ret_code != 1:
                yield None
            data = datayes['data']
            for unit in data:
                mkt_fut = datayes_mktfut_pb2.MKTFut()
                if unit.has_key('secID'):
                    mkt_fut.sec_id = unit['secID']
                else:
                    mkt_fut.sec_id = ""

                if unit.has_key('ticker'):
                    mkt_fut.ticker = unit['ticker']
                else:
                    mkt_fut.ticker = ""

                if unit.has_key('secShortName'):
                    mkt_fut.short_name = unit['secShortName']
                else:
                    mkt_fut.short_name = ""

                if unit.has_key('exchangeCD'):
                    mkt_fut.exchange_id = unit['exchangeCD']
                else:
                    mkt_fut.exchange_id = ""

                if unit.has_key('tradeDate'):
                    #2016-04-13 00:00:00
                    trade_date_str = unit['tradeDate']
                    time_array = time.strptime(trade_date_str, "%Y-%m-%d %H:%M:%S")
                    mkt_fut.trade_date = int(time.mktime(time_array))
                    now = unit['tradeDate'].split(' ')[0].split('-')
                    filename = now[0] + "-" + now[1]
                else:
                    mkt_fut.trade_date = -1
                    filename = "unkonw"

                if unit.has_key('contractObject'):
                    mkt_fut.contract_object = unit['contractObject']
                else:
                    mkt_fut.contract_object = ""

                if unit.has_key('contractMark'):
                    mkt_fut.contract_mark = unit['contractMark']
                else:
                    mkt_fut.contract_mark = ""

                if unit.has_key('preSettlPrice'):
                    mkt_fut.pre_settl_price =float(unit['preSettlPrice'])
                else:
                    mkt_fut.pre_settl_price = -1.0

                if unit.has_key('preClosePrice'):
                    mkt_fut.pre_close_price = float(unit['preClosePrice'])
                else:
                    mkt_fut.pre_close_price = -1.0

                if unit.has_key('openPrice'):
                    mkt_fut.open_price = float(unit['openPrice'])
                else:
                    mkt_fut.open_price = -1.0

                if unit.has_key('hightestPrice'):
                    mkt_fut.hightest_price = float(unit['hightestPrice'])
                else:
                    mkt_fut.hightest_price = -1.0

                if unit.has_key('lowestPrice'):
                    mkt_fut.lowest_price = float(unit['lowestPrice'])
                else:
                    mkt_fut.lowest_price = -1.0

                if unit.has_key('settlPrice'):
                    mkt_fut.settl_price = float(unit['settlPrice'])
                else:
                    mkt_fut.settl_price = -1.0

                if unit.has_key('closePrice'):
                    mkt_fut.close_price = float(unit['closePrice'])
                else:
                    mkt_fut.close_price = -1.0

                if unit.has_key('turnoverVol'):
                    mkt_fut.turnover_vol = float(unit['turnoverVol'])
                else:
                    mkt_fut.turnover_vol = -1.0

                if unit.has_key('turnoverValue'):
                    mkt_fut.turnover_value = float(unit['turnoverValue'])
                else:
                    mkt_fut.turnover_value = -1.0

                if unit.has_key('openInt'):
                    mkt_fut.open_int = float(unit['openInt'])
                else:
                    mkt_fut.open_int = -1.0

                if unit.has_key('updateTime'):
                    update_time_str = unit['updateTime']
                    #2016-08-27 17:05:22
                    time_array = time.strptime(update_time_str, "%Y-%m-%d %H:%M:%S")
                    mkt_fut.update_time = int(time.mktime(time_array))
                else:
                    mkt_fut.update_time = -1

                yield mkt_fut.SerializeToString(), mkt_fut.contract_object, filename

        except Exception as e:
            print "========>"
            print e
            print unit
            print "<========="
            yield None
