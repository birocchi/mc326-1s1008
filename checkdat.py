#!/usr/bin/env python

"""
checkdata.py - MC326 1s2008

Group 1:
  Anderson Birocchi
  Miguel Gaiowski
  Raphael Kubo da Costa

Checks the consistency of a dat file used
by the art catalog program.

Officialy, it should not even be supposed
to accept file names as arguments, but
it is easier to accept multiple files for
checking during our tests ;)

Internally, we have the following aggregation:
  fileList : list of files that should be checked
  - DatFile: holds information about each file
    - PictureInfo: Information about each picture
                   contained in a file

DatFile is an iterable object, and each call to
next() returns its next PictureInfo object.
"""

from StringIO import StringIO
from sys import argv, exit
import os

# TODO: Should we use getters/setters or a dict?
class DatFile(object):
  def __init__(self, name):
    self.__name = name
    self.__file = file(name)
    self.__size = os.path.getsize(name)
    self.__valid = False

  # Getters for our data
  def getFile(self): return self.__file
  def getName(self): return self.__name
  def getSize(self): return self.__size
  def isValid(self): return self.__valid

  # Make this object iterable
  def __iter__(self):
    return self

  def next(self):
    if self.getFile().tell() < self.getSize():
      return PictureInfo(StringIO(self.getFile().read(420)))

    raise StopIteration

# TODO: Should we use getters/setters or a dict?
class PictureInfo(dict):
  def __init__(self, data):
    dict.__init__(self)

    self["title"] = data.read(200)
    self["type"] = data.read(100)
    self["author"] = data.read(100)
    self["year"] = data.read(4)
    self["value"] = data.read(9)
    self["image"] = data.read(7)

class CheckDat(object):
  # Edit with the number of the group whose dat is to be checked
  GROUPNUMBER = 01

  def __init__(self):
    self.fileList = list()

  def checkConsistency(self):
    errorCount = 0

    for f in self.fileList:
      print "Checking %s..." % f.getName()

      # Check file size
      if f.getSize() % 420:
        print "%s: Invalid file size" % f.getName()
        errorCount += 1
        continue

      for fileChunk in f:
        # Check if we only have numbers on our year and value fields
        if not fileChunk["year"].isdigit():
          print "  %s: Invalid year in image %s" % (f.getName(), fileChunk["image"])
          errorCount += 1

        if not fileChunk["value"].isdigit():
          print "  %s: Invalid value in image %s" % (f.getName(), fileChunk["image"])
          errorCount += 1

        # Check if the image exists in the img directory
        imageName = os.path.join("img", str(self.GROUPNUMBER).zfill(2) + fileChunk["image"][:-3] + "." + fileChunk["image"][-3:])
        if not os.path.isfile(imageName):
          print "  %s: Image %s not found" % (f.getName(), imageName)
          errorCount += 1

    if errorCount == 0:
      print "Everything looks fine. Yay!"
    else:
      print "We had %d errors." % errorCount

  def parseArguments(self, arglist):
    if len(arglist) == 0:
      return False

    for f in arglist:
      if not os.path.isfile(f):
        print "Error: file %s does not exist" % f
        return False

      self.fileList.append(DatFile(f))

    return True

  @staticmethod
  def printHelp():
    print "checkdat.py - Check the consistency of a dat file"
    print "usage: %s FILE1 [FILE2 ... FILEN]" % argv[0]

if __name__ == "__main__":
  app = CheckDat()

  if not app.parseArguments(argv[1:]):
    CheckDat.printHelp()
    exit(2)

  app.checkConsistency()
