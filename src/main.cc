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

#include "DOMagent.h"


/**
* @file main.cc
* Contiene, oltre al main, la funzione usage che rappresenta l'help
* del programma.
* Nel main viene inizializzato il sistema xerces e viene creato l'oggetto
* DOMagent d che andrà a operare leggendo su txt e scrivendo su xml.
*/

void usage()
{	
	XERCES_STD_QUALIFIER cout << "\nUsage:\n"
	"    xmlconverter [options] <grammar_file file_txt output_xml_file>\n\n"
    "The program takes a text file as input and converts it into an XML file \n"
    " according to the rule specified in the specific grammar file. "
    "Options:\n"
    "  -i      states that the file to be processed has the same form  \n "
	"              as a windows ini file \n"	 
    "  -c	  states that the file to be processed has the form of a  \n"				
	"              linux conf file \n"	
	"  -g      states that the file to be processes follows a custom grammar\n"
    "  -?          shows this help.\n\n"          
    << XERCES_STD_QUALIFIER endl;
}


int main(int argc, char* argv[])
{	
	if( (argc<2) || (argc>5) || (strcmp(argv[1],"-?")==0) || (argv[1][0]!='-'))
	{		
		usage();
		return 1;
	}

	//inizializzo il sistema XML
	try
    	{
        	XMLPlatformUtils::Initialize();
    	}
    	catch (const XMLException& toCatch)
    	{        
		cout<<"Errore di inizializzazione di xerces"<<endl;
       	 	return 1; //exit code di errore
    	}
    // Do your actual work with Xerces-C++ here.		

	const char* pathg;
	const char* pathf;
	const char* pathxml;
	int v=0; //v rappresenta il numero minimo di opzioni
	if(strcmp(argv[1],"-g")==0)
	{
		v=5;
		pathg=argv[2];
		pathf=argv[3];
		pathxml=argv[4];
	}
	else
	{
		pathf=argv[2];
		pathxml=argv[3];
		v=4;
		if(strcmp(argv[1],"-i")==0) pathg="ini";
		else if(strcmp(argv[1],"-c")==0) pathg="conf";
		else
		{			
			XMLPlatformUtils::Terminate();
			usage();
			return 1;
		}

	}
	if(argc!=v) 
	{		
		XMLPlatformUtils::Terminate();
		usage();
		return 1;
	}

	DOMagent *d;	
	try
	{
		d=new DOMagent(pathg,pathf);
	}
	catch(char& s)
	{
		cout<<"ERRORE GESTIONE DEL FILE TXT"<<endl;
		return 1;
	}
	try
	{
		d->createDOMtree();
	}
	catch(char* s)
	{
		cout<<"ERRORE APERTURA DEL FILE TXT"<<endl;
		XMLPlatformUtils::Terminate();
		delete d;
		system("PAUSE");
		return 1;
	}	
	d->writexml(pathxml);
	delete d; //cancellando d cancello anche le risorse allocate da tutto il programma
    XMLPlatformUtils::Terminate();

	cout<<"File xml creato con successo"<<endl;
    // Other terminations and cleanup.
	system("PAUSE");
    return 0;
}
