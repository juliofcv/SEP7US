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

#ifndef SEP7US_SORTING_H
#define SEP7US_SORTING_H

void XYAsc(unsigned char *a, short n);
void XYDsc(unsigned char *a, short n);
void YXAsc(unsigned char *a, short n);
void YXDsc(unsigned char *a, short n);

#endif
