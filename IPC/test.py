import sys

try:
    command = sys.stdin.readline()
    outputstr1 = 'V {0}\n'.format(17)
    outputstr2 = 'E {<1,16>,<11,6>,<14,12>,<0,6>,<15,16>,<5,11>,<3,1>,<8,12>,<12,0>,<11,1>,<6,3>,<16,2>,<16,7>,<7,2>,<6,4>,<7,4>,<3,7>,<10,11>,<1,15>,<13,3>,<12,9>}\n'        
    sys.stdout.write(outputstr1)
    sys.stdout.write(outputstr2)
except (EOFError, KeyboardInterrupt, SystemExit):
    pass