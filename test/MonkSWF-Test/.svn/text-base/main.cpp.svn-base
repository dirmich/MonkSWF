#include "MonkSWf.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace MonkSWF;

int main (int argc, char * const argv[]) {

	fstream swf_file; 
	
	swf_file.open ("beer.swf", fstream::in | fstream::binary | ios::ate );
	
	if( swf_file.is_open() )
	{
		ifstream::pos_type size = swf_file.tellg();
		char * memblock = new char[size];
		swf_file.seekg( ios::beg );
		swf_file.read( memblock, size );
		
		Reader r( memblock, size );
		
		SWF swf;
		swf.initialize();
		swf.read( &r );
		swf.print();
		
		delete [] memblock;
	}
	
	swf_file.close();

    return 0;
}
