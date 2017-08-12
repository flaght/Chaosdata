# -*- coding: utf-8 -*-
from dataapiclient import Client
import json
import time
import os

#写入指定文件
def WriteFile(filename,result):
    s = os.path.split(filename)
    if not os.path.exists(s[0]):
        os.makedirs(s[0])
    file_object = open(filename, 'w')
    file_object.write(result)
    file_object.close()



#解析数据,获取便标的编码及开始时间和结束时间
def ParserFutu(result):
    futu_json = json.loads(result)
    retCode = futu_json['retCode']
    list = []
    if retCode == 1:
        futu_list = futu_json['data']
        for futu in futu_list:
            dict = {'exchangeCD':futu['exchangeCD'],'secID':futu['secID'],'listDate':futu['listDate'],'lastTradeDate':futu['lastTradeDate']}
            list.append(dict)
        return list
    else:
        return None

#获取行情数据
def GetMktConFutdJY(secID,exchangeCD,start_time,end_time):
    try:
        client = Client()
        client.init('c17b9229eaa7df5c1693a25d7225353b47918cbc222b7b9e693de9861f5c8ded')
        url='/api/finFutu/getMktConFutdJY.json?FIELD=&secID='+secID+'&secIDSrc=&startDate='+ start_time+'&endDate='+end_time+'&beginTime=&endTime='
        code, result = client.getData(url)
        if code == 200:
            file_name = "./data/"+exchangeCD+"/"+secID + "/" + start_time + "_" + end_time+".txt"
            WriteFile(file_name,result)
        else:
            print code
            print result
    except Exception, e:
        raise e

#获取商品期货的基本信息
def GetFutuCfCCXE(exchange):
    try:
        client = Client()
        client.init('c17b9229eaa7df5c1693a25d7225353b47918cbc222b7b9e693de9861f5c8ded')
        url = '/api/future/getFutuCfCCXE.json?field=&secID=&ticker=&exchangeCD='+exchange+'&varUniCode=&contractStatus=DE'
        code, result = client.getData(url)
        if code == 200:
            list = ParserFutu(result)
            return list
        else:
            print code
            print result
            return None
    except Exception, e:
        raise e
        return None

def CrawleFutuCfCCXE(secID,exchangeCD,start_time,end_time):
    start_pos = start_time
    end_pos = start_time + 86400 * 60
    while end_pos < end_time:
        x = time.localtime(start_pos)
        start_str = time.strftime('%Y%m%d', x)
        y = time.localtime(end_pos)
        end_str = time.strftime('%Y%m%d', y)
        GetMktConFutdJY(secID,exchangeCD,start_str,end_str)
        start_pos = end_pos + 86400
        end_pos = start_pos + (86400 * 60)



    end_pos = end_time
    x = time.localtime(start_pos)
    start_str = time.strftime('%Y%m%d', x)
    y = time.localtime(end_pos)
    end_str = time.strftime('%Y%m%d', y)
    GetMktConFutdJY(secID, exchangeCD, start_str, end_str)




def CrawleOldDataYes(exchange):
    list = GetFutuCfCCXE(exchange)
    if list is None:
        return
    for i in list:
        list_date = time.mktime(time.strptime(i['listDate'],'%Y-%m-%d %H:%M:%S'))#i['listDate']
        late_date = time.mktime(time.strptime(i['lastTradeDate'],'%Y-%m-%d %H:%M:%S'))#i ['lastTradeDate']
        symbol = i['secID']
        exchange = i['exchangeCD']
        t = time.time()

        CrawleFutuCfCCXE(symbol, exchange, list_date, t if t < late_date else late_date)


if __name__ == "__main__":
    #GetFutuCfCCXE('XZCE')
    #GetFutuCfCCXE('XSGE')
    #CrawleDataYes('XDCE')
    #CrawleDataYes('XSGE')
    #CrawleOldDataYes('XDCE')
    #CrawleOldDataYes('XDCE')  # 大连商品
    CrawleOldDataYes('XSGE')  # 上海期货
    CrawleOldDataYes('XZCE')  # 郑州商品
    #CrawleOldDataYes('CCFX')  # 中国金融
