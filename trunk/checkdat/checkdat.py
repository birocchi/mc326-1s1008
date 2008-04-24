#!/usr/bin/env python
"""
checkdata.py - MC326 1s2008

Copyright 2008 Anderson Birocchi, Miguel Gaiowski, Raphael Kubo da Costa

*-*-*-*-*-*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*-*-*-*-*-*

Checks the consistency of a dat file used by the art catalog program.

Officialy, it should not even be supposed to accept file names as
arguments, but it is easier to accept multiple files for checking
during our tests ;)

Internally, we have the following aggregation:
  - fileList : list of files that should be checked
    - DatFile: holds information about each file
      - ArtworkInfo: Information about each picture
                     contained in a file

DatFile is an iterable object, and each call to
next() returns its next ArtworkInfo object.

TODO:
- Maybe turn this into a real unit test if possible?
- Create an option to print the values (interactively?)?
"""

from StringIO import StringIO
from sys import argv, exit
import os

class ArtworkInfo(object):
  """
  This class holds information about the smallest package of
  information we have: an artwork entry.
  """
  def __init__(self, data):
    self.__title = data.read(200)
    self.__type = data.read(100)
    self.__author = data.read(125)
    self.__year = data.read(4).strip()
    self.__value = data.read(12).strip()
    self.__imageName = data.read(9)

  # Getters
  def getAuthor(self):         return self.__author
  def getImageBaseName(self):  return self.__imageName[:-3]
  def getImageExtension(self): return self.__imageName[-3:]
  def getImageName(self):      return self.__imageName
  def getTitle(self):          return self.__title
  def getType(self):           return self.__type
  def getValue(self):          return self.__value
  def getYear(self):           return self.__year

class DatFile(object):
  """
  Container class which (in abstraction) holds as many
  ArtworkInfo classes as needed.
  This is an iterable class which returns a new ArtworkInfo
  object in each iteration.
  """
  def __init__(self, name):
    self.__name = name
    self.__file = file(name)
    self.__size = os.path.getsize(name)

  # Getters for our data
  def getName(self): return self.__name
  def getSize(self): return self.__size

  # Make this object iterable
  def __iter__(self):
    return self

  def next(self):
    if self.__file.tell() < self.getSize():
      return ArtworkInfo(StringIO(self.__file.read(450)))

    raise StopIteration

class ErrorQueue(list):
  """
  Class to make it easier to print a formatted error message.
  Even though it inherits a list, it works as a queue.
  """
  def add(self, msg):
    """
    Equivalent to an error in a test case.
    Add an error message, but continue running.
    """
    self.append("** %s: %s" % (self.__fileName, msg))

  def coreDump(self):
    """
    Print all the error messages we have, in order of arrival.
    """
    while len(self) > 0:
      print self.pop(0)

  def fail(self, msg):
    """
    Indicates a severe failure. Add the message to the queue,
    but after that print all the messages and clean up the queue.
    """
    self.add(msg)
    self.coreDump()

  def setFileName(self, name):
    self.__fileName = name

class CheckDat(object):
  def __init__(self):
    self.fileList = list()

  def checkConsistency(self):
    errors = ErrorQueue()
    imageNameList = list()

    for f in self.fileList:
      print ">> Checking %s..." % f.getName()
      errors.setFileName(f.getName())

      # Check file size
      if f.getSize() % 450:
        errors.fail("Invalid file size. Skipping to the next file...")
        continue

      for fileChunk in f:
        # Check if we only have numbers on our year and value fields
        if not fileChunk.getYear().isdigit():
          errors.add("Invalid year in image %s" % fileChunk.getImageName())
          
        if not fileChunk.getValue().strip().isdigit():
          errors.add("Invalid value in image %s" % fileChunk.getImageName())

        # Check if we have a valid image name
        if not fileChunk.getImageBaseName().isdigit():
          errors.add("Invalid image name in %s" % fileChunk.getImageName())

        # Check the file extension
        if fileChunk.getImageExtension().lower() not in ('png', 'jpg', 'gif'):
          errors.add("Invalid image extension '%s' for %s" % (fileChunk.getImageExtension(), fileChunk.getImageName()))

        # Check if the image exists in the img directory
        imageName = os.path.join("img", fileChunk.getImageBaseName() + "." + fileChunk.getImageExtension())
        if not os.path.isfile(imageName):
          errors.add("Image %s not found" % imageName)

        # Check if there is already an image with the same name in the dat file
        if imageName in imageNameList:
          errors.add("Image %s has already been found in the file" % imageName)
        else:
          imageNameList.append(imageName)

      print ">> Entries in the databasee: %d" % len(imageNameList)
      print ">> Total: %d errors." % len(errors)
      errors.coreDump()
      
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
