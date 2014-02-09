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



/**
* @file regole.h
* Contiene la definizione della classe astratta di cui le derivate rappresentano
* regole di parsing del file.	
*/


class regole 
{
	public: 

		virtual ~regole(){};
/**	
* Cerca se @p s segue le regole per essere un tag di inizio.
* @param s la stringa di cui controllare le caratteristiche
* @return "" nel caso che @p s non sia un tag di inizio, altrimenti ritorna il valore da inserire nel tag
*/
		virtual string searchstart(const string s)=0;

/**
* Cerca se @p e segue le regole per essere un tag di fine.
* @param e la stringa di cui controllare le caratteristiche
* @return "" nel caso che @p e non sia un tag di fine, "end" altrimenti
*/

		virtual string searchend(const string e)=0;	

/**
* Cerca se @p e segue le regole per essere un atributo.
* Ha come ritorno due valori : @p e modificata (cioè avente solo la parte name del nodo attributo)
* e una stringa che rappresenta il valore dell'attributo che si andrà a creare.
* @param e la stringa di cui controllare le caratteristiche
* @return "" nel caso che @p e non sia un tag di fine, "end" altrimenti
*/

		virtual string searchattribute(string& e)=0; //ritorna 2stringhe : la prima è il nome dell'attributo,la seconda il valore

/**
* Cerca se @p e segue le regole per essere un commento.
* I commenti vengono ignorati dal DOMagent nella fase di costruzione dell'albero DOM
* @param e la stringa di cui controllare le caratteristiche
* @return true nel caso che @p rappresenta un commento, false altrimenti
*/
		virtual bool searchcomment(const string e)=0;

/**
* Funzione che serve per rendere possibile la costruzione dell'albero DOM
* per file in cui la fine di un tag coincide con l'inizio di un altro tag.
* @return true se la fine di un tag coincide con l'inizio di un altro, false altrimenti
*/

		virtual const bool gettype()=0; 	
};
