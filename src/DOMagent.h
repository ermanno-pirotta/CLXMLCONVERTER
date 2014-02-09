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


#include "grammatica.h"
#include "classfile.h"


#include <xercesc/util/XMLException.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

#include <xercesc/util/PlatformUtils.hpp>
//#include <xercesc/parsers/AbstractDOMParser.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMBuilder.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
//#include <xercesc/dom/DOMError.hpp>
//#include <xercesc/dom/DOMLocator.hpp>
//#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMNode.hpp>
//#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

XERCES_CPP_NAMESPACE_USE

/**
* @file DOMagent.h
* Contiene la definizione della classe che serve per operare su xml.
*
* @class DOMagent
* Classe che si occupa di creare il l'albero DOM e di scrivere
* il corrispondente xml usando xerces.
* Per un parsing corretto del file txt, questa classe utilizza i metodi messi a
* disposizione dall'oggetto regole (suo membro privato).
*/

class DOMagent 
{
	public:

/**
* Costruttore.
* Dichiara e instanzia il sottotipo di regole pi&ugrave adatto.
* Inoltre inizializza l'oggetto myfile per leggere il file txt.
* @param pathg ="ini","conf","reg" oppure path del file dove caricare la grammatica
*/

		DOMagent(const char* pathg,const char* pathf);

/**
* Distruttore.
*/
		~DOMagent();

/**
* Metodo che crea il DOM tree partendo dal file txt e dalle regole definite in @p regole* g.
* Si appoggia al metodo privato filltree. @see filltreefilltree()
* Lancia un eccezione in caso di fallimento
*/
		void createDOMtree(); 

/**
* Metodo che scrive il file xml partendo dal DOMtree creato da createDOMtree.
* Utilizza metodi di xerces.
* @param char* pathxml path del file xml che verrà scritto
*/		
		
		int writexml(const char* pathxml);

	private:				
		regole* g; 
		myfile* f;	
		DOMDocument *doc;
		DOMElement* filltree(DOMElement *current,string line); ///< funzione usata da createDOMtree per la creazione del DOMtree
		const string standardstring(string s); ///< ritorna una stringa che è conforme agli standard di naming per i tag element		
};