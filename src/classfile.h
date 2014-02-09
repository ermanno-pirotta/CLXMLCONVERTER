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

#include <iostream>
#include <fstream>
#include <cstring>
#ifndef __CLASSFILE_H
#define __CLASSFILE_H

using namespace std;
/**
*@file classfile.h
* Contiene la definizione della classe myfile.
* @class myfile
* Rappresenta una classe che incapsula le operazioni di lettura e scrittura su file.
*/

class myfile
{
	public :

/**
* Costruttore. Apre il file
* @param char* p path del file da aprire
*/
		myfile(const char* p);

/**
* Costruttore di copia
*/
		myfile(const myfile& x);

/**
* Distruttore. Chiude il file
*/
		~myfile(); 
/**
* restituisce una stringa letta da file, 
* a partire dalla posizione successiva all'occorrenza della stringa @p part
* @param string part stringa dopo la quale bisogna leggere
*@return stringa letta che succede a @p part nella linea del file
*/
		string getword(string part);		

/**
* Metodo che legge una linea del file per volta. Se incontra una linea vuota 
* la salta leggendo la successiva.
* @return "eof" se si è raggiunta la fine del file, "error" se il file non è aperto e la stringa pulita di spazi iniziali
* @see clear()
*/
		string get_line(); 

/**
* Chiude il file.
* @return true se è stato possibile chiudere il file, false altrimenti
*/
		bool closefile();
/**
* Funzione che ritorna se il file è aperto o meno.
* @return true se il file è aperto, false altrimenti
*/
		const bool isopen();
	
	private:
		fstream f;		
		char* path;
		string clear(string s,const int i); //elimina gli spazi iniziali da s		
};

#endif
