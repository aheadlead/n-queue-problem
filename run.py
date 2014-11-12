#!/usr/bin/env python
# coding=utf-8

import os
import sys
import time
import json
import re
import threading

answer = [0, 1, 0, 0, 2, 10, 4, 40, 92, 352, 724, 2680, 14200, 73712, 365596, 2279184, 14772512, 14772512, 666090624, 4968057848, 39029188884, 314666222712, 2691008701644, 24233937684440, 227514171973736, 2207893435808352, 22317699616364044]

is_timeout = False
spec = ''
timelimit = 2
repeat = 10
compile_para = ''
testrange = [4, len(answer)]
t = threading.Thread()

def killall():
    global is_timeout
    is_timeout = True
    os.system('killall -SIGINT a.out')
    return

def test(N):
    global is_timeout, timelimit, t
    # generate a input file named "in"
    os.system('echo %d > in' % N)
    # run a.out with a timer
    is_timeout = False
    t = threading.Timer(timelimit, killall)
    t.start()
    ret = os.popen('(time ./a.out) 2>&1').read()
    if is_timeout == False:
        t.cancel()
    # delete input file
    os.remove('./in') 

    ret = ret.split('\n')
    if is_timeout == True:
        raise Exception, 'TLE' # Time limit exceed
    else:
        for line in ret:
            if line.isdigit() and answer[N] != int(line):
                raise Exception, 'WA %d %s' % (answer[N], line) # Wrong answer
            else:
                r = re.match('^user\t(?P<minute>\d+)m(?P<second1>\d+)\.(?P<second2>\d+)s$', line)
                if r != None:
                    time = 60*int(r.group('minute')) + int(r.group('second1')) + 0.001*int(r.group('second2'))
                    return time
    raise Exception, 'Unexcepted error'
    return 

def main():
    global timelimit, spec, repeat, compile_para, testrange
    p = iter(sys.argv)
    try:
        while True:
            para = p.next()
            if para == '-f':
                spec = p.next()
            elif para == '-t':
                timelimit = int(p.next())
            elif para == '-n':
                repeat = int(p.next())
            elif para == '-c':
                compile_para = p.next()
            elif para == '-r':
                testrange[0] = int(p.next())
                testrange[1] = testrange[0]+1
    except:
        pass


    result = dict()
    for a in os.listdir('.'):
        if a.isdigit():
            if spec != '' and a != spec:
                continue
            print "Found folder ", a
            os.system('gcc %s ./%s/main.c' % (compile_para, a))
            result[a] = dict()
            for N in range(testrange[0], testrange[1]): # chessboard size
                print "\tN=%d" % (N)
                running_time = []
                try:
                    for i in range(repeat):  
                        ret = test(N)
                        running_time.append(ret)
                        print "\t\ttest %02d : %.3f sec(s)" % (i, ret)
                except Exception, e:
                    result[a][N] = str(e)
                    print "\t", "Exception: ", e
                else:
                    result[a][N] = sum(running_time)/len(running_time)
                    print "\t", "Average time:", result[a][N]
            os.remove('./a.out')
    
    hd = open('output.json', 'w')
    hd.write(json.dumps(result))
    hd.close()

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        t.cancel()
        print "terminated"
        sys.exit(1)
