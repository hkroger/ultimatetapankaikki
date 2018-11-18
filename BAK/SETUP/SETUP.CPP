#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <iostream.h>
#include <midasdll.h>

#pragma library ("midas.lib")


void vmode(short);
#pragma aux vmode=\
          "int 10h",\
		  parm [ax] modify[ax]
	 
main() {
 if (MIDASstartup())
 if (MIDASconfig()){ 
 vmode(0x3);
 if (MIDASsaveConfig("SOUND.CFG")) cout<<"Configuration saved !"<<"\n";

}

}


