import os

os.system("svn up && svn export . tp/")
os.system("zip -r tp.zip tp/")
