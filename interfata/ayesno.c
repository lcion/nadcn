/* A box with two buttons and a string. Simple boxes like this
   are very usefull for asking questions
*/

#include "forms.h"

int
main(int argc, char *argv[]){


	fl_initialize(&argc, argv, "FormDemo", 0, 0);
	while( !fl_show_question("Do You want to Quit ?", 0) );
	return 0;
}