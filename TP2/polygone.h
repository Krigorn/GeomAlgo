#ifndef __POLYGONE_H__
#define POLYGONE_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>  
#include <math.h>  
#include <unistd.h>  
#include <GL/glut.h> 
#include <string.h>

/*! Dimension de l'espace */
#define DIM 2
#define VRAI 1
#define FAUX 0
#define DISTMAX 10
#define ADROITE 0
#define AGAUCHE 1
#define ALIGNE 2

#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))


int ecrireFichier = VRAI ;
char* nomFichier = "test.txt";

/*! Structure pour representer un point. */
typedef struct
{
	double coords[DIM] ;
    int intersect ;
    int concave ;
} vertex ;

/*! Tailles diverses du systemes de fenetrage .*/
const double minX = 0,
             maxX = 500,
	         minY = 0,
	         maxY = 500,
	         margin = 10;

/*! Nombre de "sites" dans le programme, modifiable par la ligne
 * de commande de la fonction main().
 */
int nbPointsPolygon = 0;
int nbPointsMax = 10;
int clic = 0;
int dedans = 0;

/*! Tableau gobal des sommets */
vertex *P = NULL;

char *lineOption[] = { "GL_POINTS", "GL_LINES", "GL_LINE_STRIP", "GL_LINE_LOOP", "GL_POLYGON"};

#endif
