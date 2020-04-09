import sys,datetime,json
sys.path.append('D:/Program Files/Tinysoft/Analyse.NET')
import TSLPy3 as ts
import os

def get_sDate(beginT):
    beginT=str(beginT)
    sDate=[int(beginT)]
    beginT=datetime.datetime.strptime('{}'.format(beginT),"%Y%m%d")
    while True:
        beginT+=datetime.timedelta(days=1)
        if beginT>datetime.datetime.now():
            break
        else:
            sDate.append(int(datetime.datetime.strftime(beginT,"%Y%m%d")))
    print(sDate)
    return(sDate)

def get_next5contract(now,contract):
    #now=year+month
    now=int(now)
    s=[str(now+i) if int(str(now+i)[4:6])<=12 else str(int(str(now)[:4])+1)+str(int(str(now)[4:6])+i-12)
       for i in range(6)]#当月，次1月，次2月，次3月，次4月，次5月
    s=[temp if len(temp)==6 else temp[:4]+'0'+temp[4:] for temp in s]
    return([contract+str(temp)[2:] for temp in s])

def get_sContract(sDate,contract='sc'):
    #tempDate=year+month
    stempDate=sorted(set([str(date)[:6] for date in sDate]))
    sContract=[]
    for tempDate in stempDate:sContract.extend(get_next5contract(tempDate,contract))#当月
    return(sorted(set(sContract)))

def getData(sDate,intervel,sContract):
    #intervel 表示数据间隔要5秒的，还是一分钟数据，还是日线数据   e.g "5s"
    #contract 合约名称                                      e.g "sc"
    try:os.makedirs('E:\CLionProjects\HedgeArbitrage/data')
    except:pass
    while True:
        ts.ConnectServer('211.100.23.205', 443)
        ts.LoginServer('fuchunguang', 'Fcg=888888')
        if ts.Logined() == True:
            print('already login')
            daycount = 0
            for code in sContract:
                hdateInfo = {}
                notNullcounter = 0
                for date in sDate:
                    daycount += 1
                    if intervel=='1m':
                        temp1='cy_1m()'
                        temp2=240
                    elif intervel=='5s':
                        temp1='cy_5s()'
                        temp2=2880
                    sql = 'setsysparam(pn_stock(),\'{}\');' \
                          'setsysparam(PN_Cycle(), {});setsysparam(pn_date(),inttodate({}));' \
                          'return nday({},"mdate",datetimetostr(sp_time()),"open",Open(),"high",High(),"low",Low(),"close",Close(),"vol",Vol(),"amount",Amount());' \
                        .format(code, temp1,date, temp2)
                    result = (ts.RemoteExecute(sql, {}))[1]
                    if result:
                        for data in result:
                            temp = data[b'mdate'].decode('gbk').replace(':', '').replace('-', '').replace(' ', '')
                            if len(temp)<14:
                                temp+='0'*(14-len(temp))
                            if int(sDate[0]) <= int(temp[:8]) <= int(sDate[-1]):
                                notNullcounter += 1
                                hdateInfo[temp] = [data[b'open'],
                                                   data[b'high'],
                                                   data[b'low'],
                                                   data[b'close'],
                                                   data[b'vol'],
                                                   data[b'amount']]
                                print(temp)
                                # print('{},{},{},{},{},{},{},{}\n'.format(code,
                                #                                          temp,
                                #                                          data[b'open'],
                                #                                          data[b'high'],
                                #                                          data[b'low'],
                                #                                          data[b'close'],
                                #                                          data[b'vol'],
                                #                                          data[b'amount']))
                if notNullcounter:
                    with open('E:\CLionProjects\HedgeArbitrage/data/{}_{}.js'.format(code,intervel), 'w')as f:
                        json.dump(hdateInfo,f)
            ts.Disconnect()
            break
        else:
            print('not login ')
    return 0

def main(beginT,intervel='1m',contract='sc'):
    sDate=get_sDate(beginT)
    sContract=get_sContract(sDate,contract)
    getData(sDate,intervel,sContract)

main(20200315,'5s')
# main(19990315,'5s')