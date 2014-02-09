CLXMLCONVERTER
==============

The Command Line Xml Converter (CLXMLConverter) is a  command line program which converts different text files in XML. It supports the conversion for Windows INI files and Linux configuration (*.conf) files. Moreover, users can convert custom files by defining their syntax in grammar files.

##OVERVIEW
The CLXmlConverter uses the xerces-c++ library for building a DOM representation of the input files and for creating the XML output file. The xerces-c++ library is a C++ library which allows you to read and write XML files. More information at http://xerces.apache.org/xerces-c/.


As already mentioned, the CLXMLConverter performs the conversion of different type of files, including "custom" files defined by the user. The main idea behind the program is that each file follows a specific set of regole (italian for rules) which allow the parser component to identify the keywords for creating XML nodes and attributes.
From a high level point of view, a set of rules is an abstraction (implemented as interface) for a concrete parser: the confparser for linux conf files, the iniparser for windows INI files and the grammatica (italian for grammar) for custom files.

When using a custom "grammatica", the grammar file defines the mapping between the text file and the XML file in terms of how the tag starts and ends and how the attributes are defined.
For example, if we have the following grammar:
```
carattere pre-commento=#

//element1

nome=X

start="["+X+"]"

end="@/"+x+"@"

//element2

nome=prova

start=[prova]

end=[/prova]

//element3

nome=prova2

start=[prova2]

end=[/prova2]

//attribute

delimitatore=:

and the following input text file:
[primotag]

[prova]

 chiave:valore

[/prova]

fine

@/primotag@
```

the XML generated with by the program will be in the following form:
```
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<root>
    <primotag>
        <prova>
            <Attributo chiave="valore"/>
        </prova>fine
    </primotag>
</root>
```

##SETUP
Build xerces-c2.8 using the following instructions:

1. cd <directory>
2. wget http://mirror.switch.ch/mirror/apache/dist/xerces/c/2/sources/xerces-c-src_2_8_0.tar.gz
3. tar -xvzf xerces-c-src_2_8_0.tar.gz
4. cd xerces-c-src_2_8_0/
5. export XERCESCROOT=<directory>/xerces-c-src_2_8_0
6. cd src/xercesc
7. ./runConfigure -plinux -cgcc -xg++ -C--prefix=<install-dir> (i.e. /opt/ApacheXML)
8. make
9. make install
10. set LD_LIBRARY_PATH=<destination-dir>/lib


Re-set the XERCESCROOT to point to the xerces install directory ("<install-dir>")
cd to the CLXmlConverter source directory
Type "make" to compile the program

##USAGE
The CLXmlconverter program can be run in a bash shell by typing "./clxmlconverter [params]", using a sintax which will look familiar to any linux user. 
The "params" will vary according to the type of the file to be converted. In particular:
to convert a windows ini file into xml you should type:
```
./clxmlconverter -i <input_file_path> <output_file_path>
```

To convert a linux conf file into xml you should type:
```
./clxmlconverter -c <input_file_path> <output_file_path>
```
        
To convert a file with a custom grammar you should type:
```
./clxmlconverter -c <grammar_file_path><input_file_path> <output_file_path>
```