# victor matheus de araujo oliveira
# Ra072589
#   grupo 10
#   disciplina MC326 - estrutura de arquivos
#   15/03/2008

import os, time, math, tempfile
import Numeric
from Numeric import NewAxis
import Gnuplot, Gnuplot.PlotItems, Gnuplot.funcutils

if __name__=="__main__":
	
	gp = Gnuplot.Gnuplot(persist = 1)
	gp('set xrange [%s:%s]' % (0,255))
	gp('set xtics 0,10')
	gp('set ytics 0,10')

	datar = []
	datag = []
	datab = []
	
	c = 0
	f = open('histograma.dat','r')
	for i in f.xreadlines():
		k = i.split(" ")
		datar.append( (c, int(k[0]) ) )
		datag.append( (c, int(k[1]) ) )
		datab.append( (c, int(k[2]) ) )
		c=c+1
	
	f.close()
	
	plot1 = Gnuplot.PlotItems.Data(datar, with = 'linespoints')
	plot2 = Gnuplot.PlotItems.Data(datag, with = 'linespoints')
	plot3 = Gnuplot.PlotItems.Data(datab, with = 'linespoints')
	
	wait('smooth csplines')
	gp.plot(plot1,plot2,plot3)
