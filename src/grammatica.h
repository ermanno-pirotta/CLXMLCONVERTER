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


#include <fstream>
#include <iostream>
#include "classfile.h"
#include "regole.h"
using namespace std;

/**
* @file grammatica.h
* Contiene la definizione delle classi derivate dalla classe regole 
* definita in regole.h.
*/


/**
* @struct tag
* Struct utilizzato dalla classe iniparser.
*/
struct tag
{
	tag() { startag="";endtag=""; }
	string startag; ///<carattere di inizio tag
	string endtag; ///<carattere di fine tag
};


/**
* @class item
* La classe item rappresenta un elemento della lista contenuta nella classe grammatica.
* E' un elemento fondamentale per il parsing partendo da una grammatica generata da un file di regole.	
*/

class item
{
	public:
/**
* Costruttore di default, azzera i membri.
*/
		item();
/**
* Costruttore di inizializzazione
* @param string nome = nome del tag 
* @param string start = stringa che contraddistingue l'inizio di un tag
* @param string end = stringa che contraddistingue la fine di un tag
* @param item* next = elemento della lista da associare come successivo
* @param item* prev = elemento della lista da associare come precedente
*/
		item(string nome,string start,string end,item* next,item* prev);
/**
* Costruttore di copia.
*/
		item(item& x);

		~item();

/**
* @return string nome = nome dell'element rappresentato da this
*/
		string getnome();

/**
* @return string end = stringa che contraddistingue la fine dell'element rappresentato da this
*/
		string getend();

/**
* @return string start = stringa che contraddistingue l'inizio dell'element rappresentato da this
*/
		string getstart();

/**
* @return item next = puntatore all'elemento successivo nella lista degli element
*/
		item* getnext(); //ricontrolla l'uso dei reference e puntatori

/**
* @return item next = puntatore all'elemento precedente nella lista degli element
*/
		item* getprev();

/**
* @param string n = nome che rappresenterà l'element
*/
		void setnome(string n);

/**
* @param string n = stringa che contraddistingue l'inizio dell'element rappresentato da this
*/
		void setstart(string n);

/**
* @param string n = stringa che contraddistingue la fine dell'element rappresentato da this
*/
		void setend(string n);

/**
* @param item next = puntatore all'elemento successivo nella lista degli element
*/
		void setnext(item* next);

/**
* @param item prev = puntatore all'elemento precedente nella lista degli element
*/
		void setprev(item* prev);		

	private:
		item* next;
		item* prev; 
		string nome; ///<nome del'element, tutto il resto è o attribute o text
		string start; ///<carattere o stringa con cui riconosco l'inizio dell'element
		string end; ///<carattere o stringa con cui riconosco la fine dell'element
};

/**
* @class iniparser
* Classe che definisce le regole per il parsing di file ini.
* Implementa la classe astratta regole
*/

class iniparser: public regole
{	
	public:
/**
* Costruttore della classe : vengono create le regole specifiche
* inizializzando i campi privati start e end
*/
		iniparser();

/**
* Costruttore di copia
*/
		iniparser(const iniparser& x);

/**
* Distruttore
*/
		~iniparser();

/**
* Controlla se @p e e' tag di inizio, confrontandolo con il campo start
*/
		virtual string searchstart(const string s);

/**
* Controlla se @p e e' tag di fine, confrontandolo con il campo end
*/
		virtual string searchend(const string e);	

/**
* Controlla se @p e e' un attributo.Nel caso dei file ini
* basta controllare che nella stringa vi sia un = .
* La parte prima dell'uguale viene ritornata modificando il valore di e, la restante e' il ritorno della funzione
*/
		virtual string searchattribute(string& e);

/**
* Cerca se @p e segue le regole per essere un commento.
* Per i file ini, i commenti iniziano con ;
* @param e la stringa di cui controllare le caratteristiche
* @return true nel caso che @p rappresenta un commento, false altrimenti
*/
		virtual bool searchcomment(const string e);

/**
* Dato che nei file ini la fine di un tag coincide con l'inizio di un altro, ritorna true.
* @return true
*/

		virtual const bool gettype();
	private:

		tag start,end; //caratteri di start e di end per un tag, non servono in grammatica
};



/**
* @class confparser
* Classe che definisce le regole per il parsing di file conf.
* Dato che per i file conf non esiste uno standard vero e proprio,
* ho considerato come esempio il file xorg.conf.
* Implementa la classe astratta regole
*/

class confparser: public regole
{
	public:

		confparser();

		~confparser();

/**
* Controlla se @p e e' tag di fine. Cerco nella stringa un occorrenza di
* Section o section (da notare come nello stesso file ci siano incongruenze
* di upcase o lowcase).
*/
		virtual string searchstart(const string s);

/**
* Controlla se @p e e' tag di fine. Confronto la stringa con 
* EndSection o EndSubSection.
*/
		virtual string searchend(const string e);	

/**
* Controlla se @p e e' un attributo.Nel caso dei file conf
* controllo che nella stringa ci sia o = o uno spazio.
* La parte prima dell'uguale viene ritornata modificando il valore di e, la restante e' il ritorno della funzione
*/

		virtual string searchattribute(string& e);

/**
* Cerca se @p e segue le regole per essere un commento.
* Per i file conf, i commenti iniziano con #
* @param e la stringa di cui controllare le caratteristiche
* @return true nel caso che @p rappresenta un commento, false altrimenti
*/
		virtual bool searchcomment(const string e);

/**
* @return false
*/
		virtual const bool gettype();
};

/**
* @class grammatica
* Classe che definisce regole &quot; dinamiche &quot;.
* Le regole di parsing sono definite da un file di grammatica.
* Per la sintassi del file di grammatica si rimanda alla documentazione.
*/

class grammatica : public regole //grammatica è una "specializzazione" di regole
{
	public:

/**
* Costruttore. Ha come parametro la path del file di grammatica,
* apre il file di grammatica e ne effettua il parsing. @see parse()
* @param char* path
*/
		grammatica(const char* path); 

/**
* Costruttore di copia
*/
		grammatica(const grammatica& x);

/**
* Distruttore
*/

		~grammatica();

/**
* Controlla se @p s è un tag di inizio, confrontandolo 
* con gli elementi presenti nella lista delle regole creata dal metodo parse.
*/
		virtual string searchstart(const string s);

/**
* Controlla se @p s è un tag di fine, confrontandolo 
* con gli elementi presenti nella lista delle regole creata dal metodo parse.
*/
		virtual string searchend(const string e);

/**
* Controlla se @p s è un attributo.
* Per fare ciò, viene verificata l'occorrenza della stringa attribute
* appartenente ai membri privati. @see attribute
*/
		virtual string searchattribute(string& e);//ritorna la parte del valore, la parte di chiave viene ritornata in e

/**
* Controlla se @p s è un commento. In questa versione sono permessi solo commenti a inizio riga.
* Per fare ciò, viene verificata l'occorrenza della stringa commento
* appartenente ai membri privati. @see commento
*/

		virtual bool searchcomment(const string e);
		virtual const bool gettype();
		
	private:
        item* index; ///<index è l'elemento che mi serve per scorrere la lista che andrò a creare
        item* first; ///<first è il primo elemento della lista   		
        myfile *f; ///<elemento di tipo file che mi consente di leggere da file		
		string getstartpro(string start,string s);
		bool parse(); ///<metodo che legge il file e costruisce la lista della grammatica
		string commento; ///<stringa o carattere che contraddistingue l'inizio di un commento
		string attribute; ///<carattere che definisce gli attributi
};

