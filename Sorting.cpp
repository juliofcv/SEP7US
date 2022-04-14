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
unsigned char T = 0x02;
unsigned char c = 0x03;

void swap(unsigned char *a, short i, short xP, short yP, short tP, short defX, short defY, short defT, short tempX, short tempY, short tempA);

/**
 * Sorting XY Ascendente (XYAsc)
   Cada minucia ya convertida isocc ISOCC con su forma (X, Y, T|T), es ordenada direcatamente con base isocc la posicón X de la misma, en orden ascendente (de menor isocc mayor)
 * @param isocc Puntero al template ISOCC
 * @param n Tamaño del array ISOCC
 */
void XYAsc(unsigned char *isocc, short n) {
    short i, j, defX, defY, defA, tempX, tempY, tempT;
    for (i = X; i < n - c; i+=c) {
        defX = i;
        defY = i+1;
        defA = i+2;
        for (j = i + c; j < n; j+=c) {
            if (isocc[j] < isocc[defX]) {
                defX = j;
                defY = j + 1;
                defA = j + 2;
            }
        }
        swap(isocc, i, 0, 1, 2, defX, defY, defA, tempX, tempY, tempT);
    }
    for (i = Y; i < n - c; i+=c) {
        defX = i-1;
        defY = i;
        defA = i+1;
        for (j = i + c; j < n; j+=c) {
            if ((isocc[j - 1] == isocc[defX]) && (isocc[j] < isocc[defY])) {
                defX = j - 1;
                defY = j;
                defA = j + 1;
            }
        }
        swap(isocc, i, -1, 0, 1, defX, defY, defA, tempX, tempY, tempT);
    }
}

void XYDsc(unsigned char *a, short n) {
    short i, j, defX, defY, defA, tempX, tempY, tempA;
    for (i = X; i < n - c; i+=c) {
        defX = i;
        defY = i+1;
        defA = i+2;
        for (j = i + c; j < n; j+=c) {
            if (a[j] > a[defX]) {
                defX = j;
                defY = j + 1;
                defA = j + 2;
            }
        }
        swap(a,i,0,1,2,defX,defY,defA,tempX,tempY,tempA);
    }
    for (i = Y; i < n - c; i+=c) {
        defX = i-1;
        defY = i;
        defA = i+1;
        for (j = i + c; j < n; j+=c) {
            if ((a[j-1] == a[defX])&&(a[j] > a[defY])) {
                defX = j - 1;
                defY = j;
                defA = j + 1;
            }
        }
        swap(a,i,-1,0,1,defX,defY,defA,tempX,tempY,tempA);
    }
}

void YXAsc(unsigned char *a, short n) {
    short i, j, defX, defY, defA, tempX, tempY, tempA;
    for (i = Y; i < n - c; i+=c) {
        defX = i-1;
        defY = i;
        defA = i+1;
        for (j = i + c; j < n; j+=c) {
            if (a[j] < a[defY]) {
                defX = j - 1;
                defY = j;
                defA = j + 1;
            }
        }
        swap(a,i,-1,0,1,defX,defY,defA,tempX,tempY,tempA);
    }
    for (i = X; i < n - c; i+=c) {
        defX = i;
        defY = i+1;
        defA = i+2;
        for (j = i + c; j < n; j+=c) {
            if ((a[j+1] == a[defY])&&(a[j] < a[defX])) {
                defX = j;
                defY = j + 1;
                defA = j + 2;
            }
        }
        swap(a,i,0,1,2,defX,defY,defA,tempX,tempY,tempA);
    }
}

void YXDsc(unsigned char *a, short n) {
    short i, j, defX, defY, defA, tempX, tempY, tempA;
    for (i = Y; i < n - c; i+=c) {
        defX = i-1;
        defY = i;
        defA = i+1;
        for (j = i + c; j < n; j+=c) {
            if (a[j] > a[defY]) {
                defX = j - 1;
                defY = j;
                defA = j + 1;
            }
        }
        swap(a,i,-1,0,1,defX,defY,defA,tempX,tempY,tempA);
    }
    for (i = X; i < n - c; i+=c) {
        defX = i;
        defY = i+1;
        defA = i+2;
        for (j = i + c; j < n; j+=c) {
            if ((a[j+1] == a[defY])&&(a[j] > a[defX])) {
                defX = j;
                defY = j + 1;
                defA = j + 2;
            }
        }
        swap(a,i,0,1,2,defX,defY,defA,tempX,tempY,tempA);
    }
}

/**
 * Intecarmbio
 * Ordenamiento burbuja base
 * @param a
 * @param i
 * @param j
 * @param xP
 * @param yP
 * @param tP
 * @param defX
 * @param defY
 * @param defT
 * @param tempX
 * @param tempY
 * @param tempA
 */
void swap(unsigned char *a, short i, short xP, short yP, short tP, short defX, short defY, short defT, short tempX, short tempY, short tempA) {
    tempX   = a[i+xP];
    tempY   = a[i+yP];
    tempA   = a[i+tP];
    a[i+xP] = a[defX];
    a[i+yP] = a[defY];
    a[i+tP] = a[defT];
    a[defX] = tempX;
    a[defY] = tempY;
    a[defT] = tempA;
}