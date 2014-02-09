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
* @file DOMagent.cc
* contiene l'implementazione della classe definita in DOMagent.h
*/

DOMagent::DOMagent(const char* p1,const char* p2)
{	 
	if(strcmp(p1,"ini")==0)			
		g=new iniparser();	
	else if(strcmp(p1,"conf")==0)
		g=new confparser();
	else
		g=new grammatica(p1);
	f=new myfile(p2); //apro il file txt, ini o config		
}


DOMagent::~DOMagent()
{	
	delete g; 
	delete f;
}

const string DOMagent::standardstring(string s)
{	//Possono contenere solo: lettere, cifre, -, _, ., :.
	//il primo carattere deve essere lettera	
	if (!((((int)s[0]>=65) && ((int)s[0]<=90)) || (((int)s[0]>=97) && ((int)s[0]<=122))))
		s="E"+s; //se incontro una lettera che non è dell'alfabeto aggiungo la E		
	//ora ricerco caratteri != da quelli permessi e li cambio con _
	for(int i=0;i<s.length();i++)
	{
		if (!((((int)s[i]>=65) && ((int)s[i]<=90)) || (((int)s[i]>=97) && ((int)s[i]<=122)) 
			|| (s[i]=='-') || (s[i]=='_') || (s[i]=='.') || (s[i]==',') || (s[i]==':') || (((int)s[i]>=48) && ((int)s[i]<=57))  ))
			s[i]='_';
	}
	return s;
}



DOMElement* DOMagent :: filltree(DOMElement *current,string line)
{	
	if(line=="eof") return 0;
	string sup="";		
	XMLCh tempStr[100];	
	if((sup=g->searchstart(line))!="") //cerca se la stringa è uno start element
	{	//un element può iniziare solo con una lettera capitale				
		XMLString::transcode(standardstring(sup).c_str(), tempStr, 150);				
		DOMElement* e1= doc->createElement(tempStr); //se nel nome c'è [ non crea
		return e1;
	}
	else if((sup=g->searchattribute(line))!="")
	{
		try
		{	
			XMLString::transcode("Attributo", tempStr, 150);				
			DOMElement* e1= doc->createElement(tempStr); //se nel nome c'è [ non crea
			current->appendChild(e1);
			XMLString::transcode(standardstring(line).c_str(), tempStr, 99); 
	
			DOMAttr* attrNode= doc->createAttribute(tempStr);//in tempstr ci deve essere la parte prima dell'uguale	
			XMLString::transcode(sup.c_str(), tempStr, 150);
			attrNode->setValue(tempStr); //con questo metodo metto la parte dopo l'uguale				
			e1->setAttributeNode(attrNode);		
			return current;
		}
		catch(DOMException e)
		{
			cout<<e.msg;
		}
		return current;
	}
	else //sulla riga c'è solo un text element
	{										
		XMLString::transcode(line.c_str(), tempStr, 99); //mi sa che devo allargare tempStr
		DOMText* textNode= doc->createTextNode(tempStr);
		current->appendChild(textNode);
		return current;
	}
}

void DOMagent::createDOMtree()
{	
	XMLCh tempStr[100];
	XMLString::transcode("LS", tempStr, 99);	
	DOMImplementation* impl =DOMImplementationRegistry::getDOMImplementation(tempStr);//, 0);
	XMLString::transcode("root", tempStr, 99);
	doc = impl->createDocument(0, tempStr, 0);
	DOMElement* root = doc->getDocumentElement();
	DOMElement* current=root;
	string line=f->get_line();		
	bool first=true;
	const bool finetag=g->gettype(); //indica se un tag finisce quando ne inizia un altro
	while(line!="eof")//il tag è SEMPRE la prima parola della stringa
	{	
		if(!(g->searchcomment(line))) //se la linea identifica un commento vado avanti
		{
			if((g->searchend(line)!="")&&(first==false)) //se è un tag di fine termino la sezione element
			{
				current=(DOMElement*)current->getParentNode(); //devo ritornare indietro
				if(finetag) //se è un ini non posso andare avanti perché la fine di
				{//un tag corrisponde con l'inizio di un altro
					root=filltree(current,line);
					current->appendChild(root);	
					current=root;
				}
			}
			else 
			{			
				first=false;
				root=filltree(current,line);
				if(current!=root) current->appendChild(root);	
				current=root;
			}
		}
		line=f->get_line();
	}	
}

int DOMagent::writexml(const char* pathxml)
{
	// construct the DOMWriter
	XMLCh tempStr[100];
	XMLString::transcode("LS", tempStr, 99);

	DOMImplementation* impl =DOMImplementationRegistry::getDOMImplementation(tempStr);
	DOMWriter* myWriter = impl->createDOMWriter();

// construct the LocalFileFormatTarget
	XMLFormatTarget *myFormatTarget = new LocalFileFormatTarget(pathxml);

	try
	{
// serialize a DOMNode to the local file "myXMLFile.xml"
		myWriter->writeNode(myFormatTarget, *doc);
	}
	catch(...)
	{	
		cout<<"Errore scrittura file xml";
		return 1;
	}
// optionally, you can flush the buffer to ensure all contents are written
	myFormatTarget->flush();
// release the memory
	myWriter->release();
//	delete myWriter;
	doc->release();
	delete myFormatTarget; 	
	return 0;
}