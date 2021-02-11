

#include <stdio.h>
#include <stdlib.h>




// Converts binary data of length=len to base64 characters.
// Length of the resultant string is stored in flen
// (you must pass pointer flen).
char* base64( const void* binaryData, int len, int *flen );



unsigned char* unbase64( char* ascii, int len, int *flen , unsigned char *bin);
