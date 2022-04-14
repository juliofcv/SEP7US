![SEP7US](https://i.imgur.com/bFzPNrr.png)

# SEP7US MatchOnMard Auxiliar

Librería auxiliar para convertir templates ISO19794 y ANSI378  a la forma ISOCC clave para la verificación biométrica match on card de los chips basados en los estándares ISO7816-4


## Autor
* [**Julio Chinchilla**](https://www.linkedin.com/in/juliofcv)
> Antes que nada considero muy importante, el detallar brevemente como funciona esta librería, ya que *no he visto documentación alguna de los estándares biométricos en español*

## Disclaimer
SEP7US Match on Card 0x7E3

Cualquier modificación que se realice sin la supervisón y/o consentimiento del corre por su cuenta y riesgo. Cualquier cambio en el código alterará drásticamente los resultados de la verificativa sobre cualquier aplicación PIV Smart Card

## Herramientas utilizadas
* C++
* Java Native Interface
***
# SEP7US

Generación plantilla ISOCC con uso directo para Aplicaciones PIV en tarjetas inteligentes ISO7816
## Identificación del template
La compatibilidad de esta herramienta se basa en los 2 siguientes tipos de template
* ISO7816
* ANSI378
## Proceso Interno
- Conteo de Minucias
- Recuantización Espacial
- Recuantización Angular
- Sorting de Minucias
## Métodos externos principales
- Obtención directa de la plantilla ISOCC
- Adición de cabeceras ISO7816
***
## Identificación del template
Es importante delimitar la posición de la data, dependiendo el template que se reciba
### ISO19794
Posición donde comienza la data en plantillas ISO19794
```c++
posDataTemplate = 0x12;
```
### ANSI378
Posición donde comienza la data en plantillas ANSI378
```c++
posDataTemplate = 0x14;
```
***
## Proceso Interno
### Conteo de minucias

```c++
short numMinutiae = (short) fTemplate[posDataTemplate+9] & 0xFF;
```
#### Tamaño del array que tendrá todo el template ISOCC
Dada la forma del template (X, Y, T|A) el tamaño del array estará determinado por el númer de minucias multiplicado por 3
```c++
short sizeISOCC = numMinutiae*3;
```
***

### Recuantización Espacial
Este proceso sirve para expresar las coordenadas de las minucias en términos de 0.1mm ***CoordMM***
#### Expresión base
```nashorn js
CoordMM         = 10 * Coord / RES
CoordUNITS	= CoordMM / 0.1
CoordCC	        = 0.5 + CoordUNITS
```
Es importante primero obtener la resolución del template

#### Cálculo de la resolución del template
```c++
short xres = (short) (fTemplate[posDataTemplate+0] << 8 | fTemplate[posDataTemplate+1]) & 0xFF;
short yres = (short) (fTemplate[posDataTemplate+2] << 8 | fTemplate[posDataTemplate+3]) & 0xFF;
```
La resolución se encuentra expresada en *2 bytes*, por lo tanto se debe de trasladar a un dato de tipo ***short***, para ello el primer byte se corre 8 bits **<<8**, luego con el segundo byte se hace una operación de compuerta lógica ***OR*** **|**, y por último se debe de hacer la operación ***AND*** **& 0xFF**
#### Recuantización espacial X
Para calcular la coordenada X en formato ISOCC y usando la expresión base, quedará de la siguiente manera
```c++
*pcoordmmX      = 10.0 * (double) *ptmpx / xres;
*pcoordunitsX   = *pcoordmmX / 0.1;
*pcoordccX      = (short)(.5 + *pcoordunitsX);
```
#### Recuantización espacial Y
Para calcular la coordenada Y en formato ISOCC y usando la expresión base, quedará de la siguiente manera
```c++
*pcoordmmY      = 10.0 * (double) *ptmpy / yres;
*pcoordunitsY   = *pcoordmmY / 0.1;
*pcoordccY      = (short)(.5 + *pcoordunitsY);
```
***
### Recuantización Angular
La recuantización angular sirve para representar en 6 bits el ángulo de la minucia teniendo en cuenta que la resolución de los angluos varia en los 2 formatos ANSI e ISO, para representar en 6 bits un ángulo cuyo valor máximo será 360, se hará de la siguiente manera.
####Cálculo del divisor base para plantillas ISOCC
```
360/64 = 5.625
```
Entonces se puede delimitar como constante
```c++
float ISOCC_ANGLE_RESOLUTION = 5.625f;
```
Previo a la recuantización angular es necesario expresar el angulo en formato normal (**360°**), para ello cada tipo de template representa los angulos de manera distinta
#### Angulo para formato ISO19794
El angulo se expresa en 8 bits **0xFF** 
```
360/256 = 1.40625
```
Por lo tanto se puede delimitar como constante de la siguiente forma
```c++
ANGLE_RESOLUTION = 1.40625f;
```

#### Angulo para formato ANSI378
Resolución del ángulo para formato ANSI378 expresado en su forma normal dividido 2 (**180°**)
```
360/180 = 2
```
Por lo tanto se puede delimitar como constante de la siguiente forma
```c++
ANGLE_RESOLUTION = 2;
```
#### Tipo de minucia
El tipo de minucia se guarda en la posición +10 de donde inicia los datos del template
```c++
posDataTemplate+10;
```
Solamente ocupa los 2 primeros bits de este, los 6 bits restantes corresponden a una coordenada **X** mayor a **0xFF** siendo este un axuliar del siguiente byte, los 2 bits del tipo de minucia son claves para la representación del tercer tipo en la recuantización angular, por lo tanto debemos de extraerlos con la operación lógica **AND 0xC0**
```
& 0xC0
Donde C0 = 11000000 (Expresado en bits)
```
La operación lógica AND por cero, anula todos los bytes restantes despues del segundo bit, aunque en pruebas varias los 14 bits restantes nunca se utilizan, realizamos la operación para casos especiales, ya que el mismo estándar señala que los 14 bits restantes si corresponden al uso de la coordenada X, por lo tanto la operación quedará de la siguiente forma
```c++
*ptmpt    = (short) fTemplate[j] & 0xC0;
```
Teniendo en cuenta todo lo anterior el cálculo final de la recuantización angular, queda asi
```c++
tmpCAngle = ANGLE_RESOLUTION * (*ptmpa);
tmpFAngle = tmpCAngle/ISOCC_ANGLE_RESOLUTION;
short t = (*ptmpt | tmpFAngle) & 0xFF;
```
***
### Sorting de Minucias
Aunque algunas tarjetas ISO7816 no necesitan ningun tipo de sorting, se incluyen las 4 funciones principales
#### Sorting XY Ascendente (XYAsc)
Cada minucia ya convertida a ISOCC con su forma (X, Y, T|A), es ordenada direcatamente con base a la posicón **X** de la misma, en orden ascendente (de menor a mayor)
```c++
void XYAsc(unsigned char *a, short n);
```
#### Sorting XY Descendente (XYDsc)
Cada minucia ya convertida a ISOCC con su forma (X, Y, T|A), es ordenada direcatamente con base a la posicón **X** de la misma, en orden descendente (de mayor a menor)
```c++
void XYDsc(unsigned char *a, short n);
```
#### Sorting YX Ascendente (YXAsc)
Cada minucia ya convertida a ISOCC con su forma (X, Y, T|A), es ordenada direcatamente con base a la posicón **Y** de la misma, en orden ascendente (de menor a mayor)
```c++
void YXAsc(unsigned char *a, short n);
```
#### Sorting YX Descendente (YXDsc)
Cada minucia ya convertida a ISOCC con su forma (X, Y, T|A), es ordenada direcatamente con base a la posicón **Y** de la misma, en orden descendente (de mayor a menor)
```c++
void YXDsc(unsigned char *a, short n);
```
***
## Métodos externos principales
### ISOCC
Obtiene el template convertido a ISO Compact Card
* **templateFormat**: Se debe especificar el formato del template 0xFF para formato ISO19794, y 0x7F para formato ANSI378
* **fTemplate**: EL puntero que apunta al array que contiene el template base
* **sorting**: Se debe especificar el sorting en el parámetro con alguno de los siguientes valores en formato byte
```text
0x00 : Obtiene el sorting XY Ascendente (XYAsc)
0x0F : Obtiene el sorting XY Descendente (XYDsc)
0x10 : Obtiene el sorting YX Ascendente (YXAsc)
0x1F : Obtiene el sorting YX Descendente (YXDsc)
```

```c++
__declspec(dllexport) unsigned char *ISOCC(unsigned char templateFormat, unsigned char *fTemplate, unsigned char sorting);
```
### Verify
Obtiene el template convertido a ISO Compact Card, añadiendo las cabeceras del comando de verificación principal PIV
>PIV: Acrónimo de Personal Identification Verify está delimitado
```
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
```
Generalmente el comando de aplicación usado es **0x00 0x21**, se deja libre elección para casos especiales, en los bytes 5 y 6 se añade la cabecera
>**7F2E**: Es indicativa de ***"Biometric data template"***, indicado en el estándar ISO/IEC 19794-2
* **CLA**: Class of instruction
* **INS**: Instruction code
* **P1**: Instruction parameter 1
* **P2**: Instruction parameter 2
* **templateFormat**: Se debe especificar el formato del template 0xFF para formato ISO19794, y 0x7F para formato ANSI378
* **fTemplate**: EL puntero que apunta al array que contiene el template base
* **sorting**: Se debe especificar el sorting en el parámetro con alguno de los siguientes valores en formato byte
```text
0x00 : Obtiene el sorting XY Ascendente (XYAsc)
0x0F : Obtiene el sorting XY Descendente (XYDsc)
0x10 : Obtiene el sorting YX Ascendente (YXAsc)
0x1F : Obtiene el sorting YX Descendente (YXDsc)
```
```c++
__declspec(dllexport) unsigned char *Verify(unsigned char CLA, unsigned char INS, unsigned char P1, unsigned char P2, unsigned char templateFormat, unsigned char *fTemplate, unsigned char sorting);
```
## Licencia

MIT