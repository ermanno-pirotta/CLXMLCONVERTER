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
#include "grammatica.h"
using namespace std;

/**
* @file grammatica.cc
* Contiene le implementazioni delle classi definite in grammatica.h
*/

//ERRORE GRAVE! centra qualcosa di richiamare il costruttore di regole in quello delle sottoclassi
//grammatica.cc:99: warning: base class 'class regole' should be explicitly initialized in the copy constructor

item::item()
{
    setnome("");
    setstart("");
    setend("");
    setnext(0);
    setprev(0);
}

item::item(string nome,string start,string end,item* next,item* prev)
{
	setnome(nome);
    	setstart(start);
    	setend(end);
    	setnext(next);
    	setprev(prev);
}

item::item(item& x) //costruttore di copia
{
	setnome(x.getnome());
	setstart(x.getstart());
	setend(x.getend());		
}

item::~item()
{	
}

string item::getnome()
{
    return(nome);
}

string item::getstart()
{
    return(start);
}

string item::getend()
{
    return(end);
}

item* item::getnext()
{
    return next;
}

item* item::getprev()
{
    return prev;
}

void item::setnext(item* n)
{
    next=n;
}

void item::setprev(item* p)
{
    prev=p;
}

void item::setnome(const string n)
{
    nome=n;
}

void item::setend(const string e)
{
    end=e;
}

void item::setstart(const string s)
{
    start=s;
}

//----------------------------------------
iniparser::iniparser()
{
	start.startag="[";
	start.endtag="]";
	end.startag="[";
	end.endtag="]";	
}

iniparser::iniparser(const iniparser& x)
{
	start.startag=x.start.startag;
	start.endtag=x.start.endtag;
	end.startag=x.end.startag;
	end.endtag=x.end.endtag;
}

iniparser::~iniparser()
{
}

//CERCA DI VEDERE PERCHE' MANCA UN RETURN
string iniparser::searchstart(const string s)
{
	int i=s.find(start.startag,0);	 //cerco se la parte iniziale è uguale a un tag di inizio
    if ((i == string::npos) || (i!=0)) return "";
	i=s.find(start.endtag,0);	//cerco se la parte finale è uguale a un tag di fine
    if ((i == string::npos) || (s[s.length()-2]=='/')) return "";
	if(i+start.endtag.length()==s.length()) return (s.substr(1,s.length()-2));
	return(""); //se arrivo a questo punto è successo qualcosa di errato
}

string iniparser::searchend(const string s)
{
	int i=s.find(end.startag,0);	 //cerco se la parte iniziale è uguale a un tag di inizio
    if ((i == string::npos) || (i!=0)) return "";
	i=s.find(end.endtag,0);	//cerco se la parte finale è uguale a un tag di fine
	if (i == string::npos) return "";
	if(i+end.endtag.length()==s.length()) return ("end");
	return("");
}

string iniparser::searchattribute(string& e)
{//per i file ini stringhe come RuntimeVersion=v1.1.4322
 //che contengono cioè un = sono rappresentati in xml come attributi
	string s="";
	int i=e.find("=",0);
    if (i == string::npos) return s; //non ho trovato nessun = : è un text element
	else
	{		
		s=e.substr(i+1,e.length()-i-1);		
		e=e.substr(0,i);
		return(s); //riutilizzo lo struct tag per non dichiararne un altro
	}
}

bool iniparser::searchcomment(const string e)
{//nei file ini i commenti iniziano con ;
	if(e[0]==';') return true;
	else return false;
}

const bool iniparser::gettype()
{
	return(true);
}

//-----------------------------------

confparser::confparser()
{
		
}

confparser::~confparser()
{
}

string confparser::searchstart(const string s)
{//appena trovo la keyword "Section" inizia un element
	int i=s.find("Section",0);	 //cerco se la parte iniziale è uguale a un tag di inizio
    if (i == string::npos) 
	{
		i=s.find("section",0); //alcune volte nei SubSection usano la minuscola -_-'
		if (i == string::npos) return "";
	}
	return(s);
}

string confparser::searchend(const string s)
{//appena trovo "EndSection", per come sono fatti i conf posso confrontare tutta s
	if((s=="EndSection") || (s=="EndSubSection")) return("end");
	else return("");
}

string confparser::searchattribute(string& e)
{//per i file ini stringhe come RuntimeVersion=v1.1.4322
 //che contengono cioè un = sono rappresentati in xml come attributi
	string s="";
	int i=e.find("=",0);
    if (i == string::npos) 
	{
		i=e.find(" ",0);
		if (i == string::npos) return s; //non ho trovato nessun = : è un text element
	}	
	s=e.substr(i+1,e.length()-i-1);		
	e=e.substr(0,i);
	return(s); //riutilizzo lo struct tag per non dichiararne un altro
}

bool confparser::searchcomment(const string e)
{//nei file ini i commenti iniziano con ;
	if(e[0]=='#') return true;
	else return false;
}

const bool confparser::gettype()
{
	return(false);
}
/*la classe grammatica elabora un file di input di sintassi predefinita (vedere
documentazione) e ne ricava una lista che rappresenta*/

grammatica::grammatica(const char* path)
{
	f=new myfile(path);
	parse();
}

grammatica::grammatica(const grammatica& x)
{	
	item *x2=x.first;
	item *p1,*p2;
	first=new item(*x2);
	p1=first;
	while(x2->getnext()!=x.first)
	{
		x2=x2->getnext();	
		p2=new item(*x2);
		p1->setnext(p2);
		p2->setprev(p1);
		p1=p2;
	}
	first->setprev(p1);
	p1->setnext(first);

	f=new myfile(*(x.f));
	commento=x.commento;
	attribute=x.attribute;
	index=first;
}

grammatica::~grammatica() // devo cancellare tutti gli elementi item!!!
{
	index=first->getprev();
	while(!(first->getnext()==index))
	{
		first=first->getnext();
		delete first->getprev();		
	}
	delete first->getnext();
	delete first;
}

bool grammatica::parse() //creo la lista della grammatica, in fase di traduzione del file txt in xml controllo questa lista
{//quando trovo //element1 significa che inizia la definizione del 1o element
    string line,sup;
	line="";
	line=f->get_line(); //leggo la riga per il commento
	int i=line.find("=",0); //cerco l'uguale
	commento=line.substr(i+1,line.length()-22);    
    sup="//element1"; //appena trovo //-- incremento l'indice di element1 -> element2
	if (i != string::npos) //nel caso in cui uno non preveda i commenti non devo leggere un'altra linea
		line=f->get_line();
    if(line=="error") return false;
    while((line!="//attribute") && (line!="eof"))//leggo tutto il file riga per riga
    {
        if(line==sup) //ho trovato l'inizio di un element
        {//devo aggiornare la lista
            if(sup=="//element1")
            {
                string s1=f->getword("nome=");
				string s2=f->getword("start=");
				string s3=f->getword("end=");
				first=new item(s1,s2,s3,0,0);
                index=first; //aggiorno index
            }
            else
            {	
				
                string s1=f->getword("nome=");
				string s2=f->getword("start=");
				string s3=f->getword("end=");
				item *it=new item(s1,s2,s3,0,index);
                index->setnext(it);
                index=it; //incremento indice
            }
            char *c=&sup[9]; //aggiorno la stringa element1 incrementando l'indice
            *c=(char)((int)sup[9]+1);
        }
        line=f->get_line();
    }   
	if(line=="//attribute")
	{
		line=f->get_line();	
		attribute=line.substr(13,line.length()-13);
	}
	//faccio si che la mia lista diventi circolare
	first->setprev(index); //ERRORE DI MERDA..
	index->setnext(first);
    index=first; //riporto indietro l'indice
    return true;
}

string grammatica::getstartpro(string start,string s) //risolve cose del tipo "["+X+"]"
{//ritorna il valore di X <- similitudine al sistema prolog
	//trovo il preambolo dell'element
	if(start[0]!='"') return(""); //il carattere " è riservato a espressioni complesse
	int i=1; 
	for(i=1;i<start.length();i++)
	{//cerco l'indice del secondo "
		if(start[i]=='"') break;
	}
	string sup1=start.substr(1,i-1);
	int si=s.find(sup1,0); //cerco in s la corrispondenza dell'inizio tag
    if (si == string::npos) return "";
    else
    { //probabilmente inizia un tag, devo fare la stessa cosa di prima con il fondo
		i+=5; //salto +X+
		string sup=start.substr(i,start.length()-i-1);
		int ei=s.find(sup,si+1); //cerco in s la corrispondenza dell'inizio tag
		if (ei == string::npos) return "";
		return(s.substr(si+sup1.length(),s.length()-(sup1.length()+sup.length())));
    }	
}

string grammatica::searchstart(const string s) //cerco da index in poi, nel caso di element annidati
{//qui faccio le ricerce tipo "["+X+"]" e calcolo cosa restituire
	bool exit=false;
	item* index2=index;
	do
	{		
		string sup=getstartpro(index2->getstart(),s);
		if(sup!="") return sup;
		if(index2->getstart()==s) 
		{	
			index=index2;
			return(index->getnome()); //elemento trovato, ritorno il nome del tag
		}
		else if (index2->getnext()==index) exit=true;
		else index2=index2->getnext(); //vado all'elemento successivo
    }
	while(!exit); //scorro la lista fino in fondo	
	return(""); //ritorno stringa vuota se non c'è uno start corrispondente ad s
}

//CONTROLLARE!!!!!!!
string grammatica::searchend(const string e)
{//quando trovo la fine di un element sposto index indietro
//vado in avanti, trovo e poi torno in dietro	
	if(index->getend()==e)  return index->getnome(); //non trovato, significa che un item non è stato chiuso, errore di parsing
    else
    {
 		item* index2=index;
		bool exit=false;
		do
		{
			string sup=getstartpro(index2->getend(),e);
			if(sup!="") return "end";			
			if(index2->getend()==e) 
			{	
				index=index2;
				return(index->getnome()); //elemento trovato, ritorno il nome del tag
			}
			else if (index2->getprev()==index) exit=true;
			else index2=index2->getprev(); //vado all'elemento precedente
		}
		while(!exit); //scorro la lista fino in fondo	
		return(""); //ritorno stringa vuota se non c'è uno start corrispondente ad s
    }
}

string grammatica::searchattribute(string& e)
{	
	string s="";
	int i=e.find(attribute,0);
    if (i == string::npos) return s;
	s=e.substr(i+1,e.length()-i-1);		
	e=e.substr(0,i);
	return(s);
}

bool grammatica::searchcomment(const string e)
{//situazioni in cui il commento è dopo un tag non sono previste al momento
	int i=e.find(commento,0);
    if (i == string::npos) return false;
	return true;
}

const bool grammatica::gettype()
{
	return(false);
}