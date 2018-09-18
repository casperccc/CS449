//Jack Anderson (jja54)

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char **argv) {
	FILE* somefile = fopen(argv[1], "rb"); //only need to read the binary file
	if (somefile == NULL){
		printf("Error in opening file, file invalid");
		return 0;
	}
	
	char printable;
	int counter = 0;
	char temp_str_holder[5];
	
	while (!feof(somefile)){
		while (counter < 4) {
			if (!feof(somefile)) {
				printable = fgetc(somefile); //gets next unsigned char from stream + advances pos
				if (isprint(printable) || printable == '\t') { //checks whether passed char is printable
					
					//also makes sure to count tab characters as well!
					//MIGHT NEED TO REMOVE THIS FEATURE -- WRITEUP SAYS DONT NEED
					
					temp_str_holder[counter] = printable;
					counter++;
					if (counter == 4){
						//counter suprassed 4 printables so we can print
						//start with the sequence of 4:
						printf("%s", temp_str_holder);
					}
				}	
				else { //got nonprintable char before sequence hit 4+, restart
					counter = 0;
				}	
			}
			else break;
		}
		
		//now check for more in a row like before, but print immediately if can:
		if (!feof(somefile)) {
			printable = fgetc(somefile);
			if (isprint(printable) || printable == '\t') {
				putchar(printable); //writes an unsigned char to stdout
			}
			else {
				printf("\n");
				counter = 0; //reset counter to start over
			}	
		}
	}
	
	fclose(somefile); //close and exit safely
	return 0;
}