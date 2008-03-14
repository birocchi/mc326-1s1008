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

TODO:
- Refactor the code so that we make error reporting
  less painful and repetitive
- Make the group number a command line option
"""

from StringIO import StringIO
from sys import argv, exit
import os

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

class PictureInfo(object):
  def __init__(self, data):
    self.__title = data.read(200)
    self.__type = data.read(100)
    self.__author = data.read(100)
    self.__year = data.read(4)
    self.__value = data.read(9)
    self.__imageName = data.read(7)

  def getAuthor(self):         return self.__author
  def getImageBaseName(self):  return self.__imageName[:-3]
  def getImageExtension(self): return self.__imageName[-3:]
  def getImageName(self):      return self.__imageName
  def getTitle(self):          return self.__title
  def getType(self):           return self.__type
  def getValue(self):          return self.__value
  def getYear(self):           return self.__year

class CheckDat(object):
  # Edit with the number of the group whose dat is to be checked
  GROUPNUMBER = 01

  def __init__(self):
    self.fileList = list()

  def checkConsistency(self):
    errorCount = 0
    imageNameList = list()

    for f in self.fileList:
      print "Checking %s..." % f.getName()

      # Check file size
      if f.getSize() % 420:
        print "%s: Invalid file size" % f.getName()
        errorCount += 1
        continue

      # TODO: Refactor this if-print-errorCount++ crap
      for fileChunk in f:
        # Check if we only have numbers on our year and value fields
	if not fileChunk.getYear().isdigit():
          print "  %s: Invalid year in image %s" % (f.getName(), fileChunk.getImageName())
          errorCount += 1

        if not fileChunk.getValue().strip().isdigit():
          print "  %s: Invalid value in image %s" % (f.getName(), fileChunk.getImageName())
          errorCount += 1

        # Check if we have a valid image name
        if not fileChunk.getImageBaseName().isdigit():
          print "  %s: Invalid image name %s" % (f.getName(), fileChunk.getImageName())
          errorCount += 1

	# Check the file extension
	if fileChunk.getImageExtension().lower() not in ('png', 'jpg', 'gif'):
	  print "  %s: Invalid image extension %s for %s" % (f.getName(), fileChunk.getImageExtension(), fileChunk.getImageName())
	  errorCount += 1

        # Check if the image exists in the img directory
	imageName = os.path.join("img", str(self.GROUPNUMBER).zfill(2) + fileChunk.getImageBaseName() + "." + fileChunk.getImageExtension())
        if not os.path.isfile(imageName):
          print "  %s: Image %s not found" % (f.getName(), imageName)
          errorCount += 1

        # Check if there is already an image with the same name in the dat file
        if imageName in imageNameList:
          print "  %s: Image %s has already been found in the file" % (f.getName(), imageName)
          errorCount += 1
        else:
          imageNameList.append(imageName)

    if errorCount == 0:
      print "Total: %d entries in the database" % len(imageNameList)
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
