/*
 * SEP7US Match on Card 0x7E3
 * ..........................
 * autor:	Julio Chinchilla Valenzuela
 * .....................................
 * Se reserva el uso de este código únicamente al autor, no se deberá utilizar
 * sin la autorización debida, cualquier modificación que se realice
 * sin la supervisón y/o consentimiento del corre por su cuenta y riesgo.
 * Cualquier cambio en el código alterará drásticamente los resultados de la
 * verificativa sobre cualquier aplicación PIV Smart Card
 * =============================================================================
 * -----------------------------------------------------------------------------
 */



#ifndef _Included_SEP7US
#define _Included_SEP7US
#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) unsigned char *ISOCC(unsigned char templateFormat, unsigned char *fTemplate, unsigned char sorting);
__declspec(dllexport) unsigned char *Verify(unsigned char CLA, unsigned char INS, unsigned char P1, unsigned char P2, unsigned char templateFormat, unsigned char *fTemplate, unsigned char sorting);


#ifdef __cplusplus
}
#endif
#endif
