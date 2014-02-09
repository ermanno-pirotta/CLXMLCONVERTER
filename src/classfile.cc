/*
Copyright 2003 Ermanno Pirotta

This file is part of CLXmlConverter.

CLXmlConverter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version. 
CLXmlConverter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with CLXmlConverter.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "classfile.h"
using namespace std;

/**
* @file classfile.cc
* contiene l'implementazione della classe myfile
*/

myfile::myfile(const char* p)
{
	if(strcmp(p,"")==0) throw "Errore!";	
	f.open(p,fstream::in);
	if(f.is_open()==false) throw "Errore!";		
	path=new char[strlen(p)];
	for(int i=0;i<strlen(p);i++)
		path[i]=p[i];
}

myfile::myfile(const myfile& x)
{
	path=new char[strlen(x.path)];
	for(int i=0;i<strlen(x.path);i++)
		path[i]=x.path[i];
	f.open(path,fstream::in);
}

myfile::~myfile()
{
	closefile(); 
}

bool myfile::closefile()
{
	if(f.is_open())
	{
		try
		{
			f.close();			
			if(f.is_open()) throw "ERRORE!!!";
		}
		catch(char* e)
		{
			return false;
		}
	}	
	return true;
}


string myfile :: clear(string s,const int i) 
{ 
	int j=i;
	if((int)s[s.length()-1]=='\r') s=s.substr(0,s.length()-1);
	if(i==0)
	{		
		for(;i<s.length();j++)	
			if((s[j]!=' ')&& ((int)s[j]!=9)) break;			
		return(s.substr(j,s.length()-j));
	}
	else
	{		
		for(;j>0;j--)
		{		
			if((s[j]!=' ')&& ((int)s[j]!=9)) break;			
		}
		return(s.substr(0,j+1));
	}
}

string myfile :: getword(string part) //ritorna la stringa che segue part
{
    string line="";
	line=get_line(); //leggo una linea del file
	if((line!="eof") || (line!="error"))
	{
		const int i=line.find(part,0);
        if (i == string::npos) return "";
        else
        { //ho trovato un'occorrenza di part
            const string sup=line.substr(i+part.length(),line.length()-i);
			//pulisco eventuali spazi finali		
			return(clear(sup,sup.length()-1));
        }
	}
	else return(line); //line=error se file chiuso, eof se end of file
}

const bool myfile::isopen()
{
	return(f.is_open());
}

string myfile::get_line()
{
 	if(f.eof()) return("eof");
	else
	{
		string line="";
		if(f.is_open()==false) throw "Errore";
		getline(f,line);
		if( ((int)line[0]==13) || (line=="") ) line=get_line();
		line=clear(line,0);
		if(line=="") line=get_line();
		return(line); //ritorno già la stringa senza gli eventuali spazi iniziali
	}            
}
