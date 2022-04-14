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

#include "Sorting.h"

unsigned char X = 0x00;
unsigned char Y = 0x01;
unsigned char A = 0x02;
unsigned char C = 0x03;

void swap(unsigned char *a, short i, short j, short xP, short yP, short aP, short defX, short defY, short defA, short tempX, short tempY, short tempA) {
    tempX   = a[i+xP];
    tempY   = a[i+yP];
    tempA   = a[i+aP];
    a[i+xP] = a[defX];
    a[i+yP] = a[defY];
    a[i+aP] = a[defA];
    a[defX] = tempX;
    a[defY] = tempY;
    a[defA] = tempA;
}

void XYAsc(unsigned char *a, short n) {
    short i, j, defX, defY, defA, tempX, tempY, tempA;
    for (i = X; i < n - C; i+=C) {
        defX = i;
        defY = i+1;
        defA = i+2;
        for (j = i + C; j < n; j+=C) {
            if (a[j] < a[defX]) {
                defX = j;
                defY = j + 1;
                defA = j + 2;
            }
        }
        swap(a,i,j,0,1,2,defX,defY,defA,tempX,tempY,tempA);
    }
    for (i = Y; i < n - C; i+=C) {
        defX = i-1;
        defY = i;
        defA = i+1;
        for (j = i + C; j < n; j+=C) {
            if ((a[j-1] == a[defX])&&(a[j] < a[defY])) {
                defX = j - 1;
                defY = j;
                defA = j + 1;
            }
        }
        swap(a,i,j,-1,0,1,defX,defY,defA,tempX,tempY,tempA);
    }
}

void XYDsc(unsigned char *a, short n) {
    short i, j, defX, defY, defA, tempX, tempY, tempA;
    for (i = X; i < n - C; i+=C) {
        defX = i;
        defY = i+1;
        defA = i+2;
        for (j = i + C; j < n; j+=C) {
            if (a[j] > a[defX]) {
                defX = j;
                defY = j + 1;
                defA = j + 2;
            }
        }
        swap(a,i,j,0,1,2,defX,defY,defA,tempX,tempY,tempA);
    }
    for (i = Y; i < n - C; i+=C) {
        defX = i-1;
        defY = i;
        defA = i+1;
        for (j = i + C; j < n; j+=C) {
            if ((a[j-1] == a[defX])&&(a[j] > a[defY])) {
                defX = j - 1;
                defY = j;
                defA = j + 1;
            }
        }
        swap(a,i,j,-1,0,1,defX,defY,defA,tempX,tempY,tempA);
    }
}

void YXAsc(unsigned char *a, short n) {
    short i, j, defX, defY, defA, tempX, tempY, tempA;
    for (i = Y; i < n - C; i+=C) {
        defX = i-1;
        defY = i;
        defA = i+1;
        for (j = i + C; j < n; j+=C) {
            if (a[j] < a[defY]) {
                defX = j - 1;
                defY = j;
                defA = j + 1;
            }
        }
        swap(a,i,j,-1,0,1,defX,defY,defA,tempX,tempY,tempA);
    }
    for (i = X; i < n - C; i+=C) {
        defX = i;
        defY = i+1;
        defA = i+2;
        for (j = i + C; j < n; j+=C) {
            if ((a[j+1] == a[defY])&&(a[j] < a[defX])) {
                defX = j;
                defY = j + 1;
                defA = j + 2;
            }
        }
        swap(a,i,j,0,1,2,defX,defY,defA,tempX,tempY,tempA);
    }
}

void YXDsc(unsigned char *a, short n) {
    short i, j, defX, defY, defA, tempX, tempY, tempA;
    for (i = Y; i < n - C; i+=C) {
        defX = i-1;
        defY = i;
        defA = i+1;
        for (j = i + C; j < n; j+=C) {
            if (a[j] > a[defY]) {
                defX = j - 1;
                defY = j;
                defA = j + 1;
            }
        }
        swap(a,i,j,-1,0,1,defX,defY,defA,tempX,tempY,tempA);
    }
    for (i = X; i < n - C; i+=C) {
        defX = i;
        defY = i+1;
        defA = i+2;
        for (j = i + C; j < n; j+=C) {
            if ((a[j+1] == a[defY])&&(a[j] > a[defX])) {
                defX = j;
                defY = j + 1;
                defA = j + 2;
            }
        }
        swap(a,i,j,0,1,2,defX,defY,defA,tempX,tempY,tempA);
    }
}