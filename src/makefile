#Copyright 2003 Ermanno Pirotta

#This file is part of CLXmlConverter.

#CLXmlConverter is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version. 
#CLXmlConverter is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with CLXmlConverter.  If not, see <http://www.gnu.org/licenses/>.


INCLUDES=-I$(XERCESCROOT)/include -I./include
CXX=g++
LD=g++
LDFLAGS=-lxerces-c -L$(XERCESCROOT)/lib
CXXFLAGS=$(INCLUDES) -W -Wall -ansi -pedantic -g
RM= rm -f

OBJS=	main.o classfile.o DOMagent.o grammatica.o

xmlconverter: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o clxmlconverter

main.o: main.cc 
	$(CXX) $(CXXFLAGS) -c main.cc -o main.o

classfile.o: classfile.cc classfile.h
	$(CXX) $(CXXFLAGS) -c classfile.cc -o classfile.o

grammatica.o: grammatica.cc grammatica.h regole.h 
	$(CXX) $(CXXFLAGS) -c grammatica.cc -o grammatica.o

DOMagent.o: DOMagent.cc DOMagent.h classfile.cc regole.h 	
	$(CXX) $(CXXFLAGS) -c DOMagent.cc -o DOMagent.o 

clean:
	$(RM) $(OBJS) clxmlconverter


