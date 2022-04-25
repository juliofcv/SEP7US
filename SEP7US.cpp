/*
 * SEP7US Match on Card 0x7E3
 * ..........................
 * autor: Julio Chinchilla Valenzuela
 * .....................................
 * Se reserva el uso de este código únicamente al autor, no se deberá utilizar
 * sin la autorización debida, cualquier modificación que se realice
 * sin la supervisón y/o consentimiento corre por su cuenta y riesgo.
 * Cualquier cambio en el código alterará drásticamente los resultados de la
 * verificativa sobre cualquier aplicación PIV Smart Card
 * =============================================================================
 * -----------------------------------------------------------------------------
 */

#include"SEP7US.h"
#include "Sorting.h"

unsigned char *toISOCC(unsigned char templateFormat, unsigned char *fTemplate, unsigned char sorting);

/**
 * Obtiene el template en formato ISOCC agreangando las cabeceras del comando de verificación PIV, basado en estándares ISO7816
 * @param CLA Class of instruction
 * @param INS Instruction code
 * @param P1 Instruction parameter 1
 * @param P2 Instruction parameter 2
 * @param templateFormat Se debe especificar el formato del template 0xFF para formato ISO19794, y 0x7F para formato ANSI INCITS 378-2004
 * @param fTemplate EL puntero que apunta al array que contiene el template base
 * @param sorting Se debe especificar el sorting en el parámetro con alguno de los siguientes valores en formato byte
 *  0x00 : Obtiene el sorting XY Ascendente (XYAsc)
    0x0F : Obtiene el sorting XY Descendente (XYDsc)
    0x10 : Obtiene el sorting YX Ascendente (YXAsc)
    0x1F : Obtiene el sorting YX Descendente (YXDsc)
 * @return
 */
__declspec(dllexport) unsigned char *Verify(unsigned char CLA, unsigned char INS, unsigned char P1, unsigned char P2, unsigned char templateFormat, unsigned char *fTemplate, unsigned char sorting) {
    unsigned char *ISOCC = toISOCC(templateFormat, fTemplate, sorting);
    short sizeISOCC = sizeof(ISOCC);
    unsigned char *APDUVerify = new unsigned char[sizeISOCC+10];
    APDUVerify[0]=CLA;
    APDUVerify[1]=INS;
    APDUVerify[2]=P1;
    APDUVerify[3]=P2;
    APDUVerify[4]=sizeISOCC+5;
    APDUVerify[5]=0x7F;
    APDUVerify[6]=0x2E;
    APDUVerify[7]=sizeISOCC+2;
    APDUVerify[8]=0x81;
    APDUVerify[9]=sizeISOCC;
    for(short fingerT=0;fingerT<sizeISOCC;fingerT++)
        APDUVerify[fingerT+10] = ISOCC[fingerT];
    return APDUVerify;
}

/**
 * Obtiene el tempalte directo en formato ISOCC
 * @param templateFormat Se debe especificar el formato del template 0xFF para formato ISO19794, y 0x7F para formato ANSI INCITS 378-2004
 * @param fTemplate EL puntero que apunta al array que contiene el template base
 * @param sorting  Se debe especificar el sorting
 * 0x00 XYAsc
 * 0x0F XYDsc
 * 0x10 YXAsc
 * 0x1F YXDsc
 * @return
 */
__declspec(dllexport) unsigned char *ISOCC(unsigned char templateFormat, unsigned char *fTemplate, unsigned char sorting) {
    return toISOCC(templateFormat, fTemplate, sorting);
}

/**
 * Conversión a ISOCC
 * @param templateFormat Se debe especificar el formato del template 0xFF para formato ISO19794, y 0x7F para formato ANSI INCITS 378-2004
 * @param fTemplate EL puntero que apunta al array que contiene el template base
 * @param sorting Se debe especificar el sorting
 * 0x00 XYAsc
 * 0x0F XYDsc
 * 0x10 YXAsc
 * 0x1F YXDsc
 * @return
 */
unsigned char *toISOCC(unsigned char templateFormat, unsigned char *fTemplate, unsigned char sorting) {
    float ISOCC_ANGLE_RESOLUTION = 5.625f;
    float ANGLE_RESOLUTION;
    short posDataTemplate;
    if ( templateFormat == 0xFF)  {
        //Resolución del ángulo para formato ISO 19794 expresado en 0xFF => 360/255 => 8 bits
        ANGLE_RESOLUTION = 1.40625f;
        //Inicio de la posición de la data crítica para templates formato ISO19794
        posDataTemplate = 0x12;
    } else if ( templateFormat == 0x7F )  {
        //Resolución del ángulo para formato ANSI INCITS 378-2004 expresado en 360/2 => 180
        ANGLE_RESOLUTION = 2;
        //Inicio de la posición de la data crítica para templates formato ANSI INCITS 378-2004
        posDataTemplate = 0x14;
    }
    // Cálculo de la resolución del template
    short xres = (short) (fTemplate[posDataTemplate+0] << 8 | fTemplate[posDataTemplate+1]) & 0xFF;
    short yres = (short) (fTemplate[posDataTemplate+2] << 8 | fTemplate[posDataTemplate+3]) & 0xFF;
    // Obtención del número de minucias
    short numMinutiae = (short) fTemplate[posDataTemplate+9] & 0xFF;
    short sizeMin = sizeof(numMinutiae);
    // Tamaño del template ISOCC determinado por el número de minucias
    // Forma del template (X, Y, T|T)
    short sizeISOCC = numMinutiae*3; // tamaño del array determinado por el tipo de template
    unsigned char *ISOCC = new unsigned char[sizeISOCC];
    short i;  short k = 0;
    short tmpt, tmpx1, tmpx2, tmpx, tmpy1, tmpy2, tmpy, tmpa;
    short* ptmpt=0; short* ptmpx1=0; short* ptmpx2=0; short* ptmpx=0; short* ptmpy1=0; short* ptmpy2=0; short* ptmpy=0; short* ptmpa=0;
    ptmpt=&tmpt; ptmpx1=&tmpx1; ptmpx2=&tmpx2; ptmpx=&tmpx;ptmpy1=&tmpy1;ptmpy2=&tmpy2;ptmpy=&tmpy;ptmpa=&tmpa;
    double coordmmX, coordunitsX, coordmmY, coordunitsY;
    double* pcoordmmX=0; double* pcoordunitsX=0; double* pcoordmmY=0; double* pcoordunitsY=0;
    pcoordmmX=&coordmmX; pcoordunitsX=&coordunitsX; pcoordmmY=&coordmmY; pcoordunitsY=&coordunitsY;
    short coordccX, coordccY;
    short *pcoordccX=0; short *pcoordccY =0;
    pcoordccX=&coordccX; pcoordccY=&coordccY;
    short tmpFAngle; float tmpCAngle;
    short j = posDataTemplate+10;
    // Recorrido
    for (i=0;i<sizeMin;i++) {
        /*
         * Byte inicial del template guarda en los 2 primeros bits
         * el tipo de minucia, y los 6 bits restantes corresponden
         * a una coordenada X mayor a 0xFF que es auxiliar del siguiente byte
         * T[j] = ttXXXXXX
         * donde t es el tipo y X es el auxiliar de la coordenada X
         * la posición inicial está delimitada por posDataTemplate que
         * varía según el tipo de template,
         */
        *ptmpt    = (short) fTemplate[j] & 0xC0;      j++;
        //Operación con compuerta lógica OR para el cálculo de la posicón X de la minucia en x1 y x2
        *ptmpx    = (short) fTemplate[j] & 0xFF;      j++;
        // Byte que almacena el auxiliar a la derecha coordenada Y debe ser corrido 8 bits
        *ptmpy1   = (short) fTemplate[j] << 8;        j++;
        // Byte que almacena el auxiliar a la izquierda coordenada Y
        *ptmpy2   = (short) fTemplate[j] & 0xFF;      j++;
        //Operación con compuerta lógica OR para el cálculo de la posicón X de la minucia en x1 y x2
        *ptmpy    = (short) (*ptmpy1|*ptmpy2);
        //Angulo en el que se encuentra la minucia
        *ptmpa    = (fTemplate[j]); j++; j++;
        //Recuantización espacial X
        *pcoordmmX = 10.0 * (double) *ptmpx / xres;
        *pcoordunitsX = *pcoordmmX / 0.1;
        *pcoordccX = (short)(.5 + *pcoordunitsX);
        ISOCC[k] = *pcoordccX; k++;
        //Recuantización espacial Y
        *pcoordmmY = 10.0 * (double) *ptmpy / yres;
        *pcoordunitsY = *pcoordmmY / 0.1;
        *pcoordccY = (short)(.5 + *pcoordunitsY);
        ISOCC[k] = *pcoordccY; k++;
        //Calculo del ángulo del template por la resolución ya sea ISO o ANSI INCITS 378-2004
        tmpCAngle = ANGLE_RESOLUTION * (*ptmpa);
        //Recuantización angular
        tmpFAngle = tmpCAngle/ISOCC_ANGLE_RESOLUTION;
        short T = (*ptmpt | tmpFAngle) & 0xFF;
        ISOCC[k] = T; k++;
    }
    if (sorting == 0x00 ) {
        XYAsc(ISOCC,sizeISOCC);
    } else if (sorting == 0x0F )  {
        XYDsc(ISOCC,sizeISOCC);
    } else if (sorting == 0x10 )  {
        YXAsc(ISOCC,sizeISOCC);
    } else if (sorting == 0x1F )  {
        YXDsc(ISOCC,sizeISOCC);
    }  else {
        // UNSORTING
    }
    return ISOCC;
}