#! /usr/bin/env python

import os, sys

name = sys.argv[1]
file = open(name)
new = open('newbase.dat','w')
size = os.path.getsize(name)

for i in range(size/420):
    new.write(file.read(200))
    new.write(file.read(100))
    new.write('%125s' % file.read(100) )
    new.write(file.read(4))
    new.write('%s' % file.read(9).zfill(12))
    new.write('%s' % name.strip('base.dat') + file.read(7))
