#include "polygone.h"

void fEcriture()
{
	FILE *fichier ;
	int i ;
	
	fichier = fopen(nomFichier, "wb") ;
	
	if(fichier != NULL)
	{
        printf("Ecriture");
		fwrite(&nbPointsPolygon, sizeof(int), 1, fichier) ;
		
		for(i = 0; i < nbPointsPolygon; i++)
		{
			fwrite(&P[i].coords[0], sizeof(double), 1, fichier) ;
			fwrite(&P[i].coords[1], sizeof(double), 1, fichier) ;
		}
		fclose(fichier) ;
        printf("Fin ecriture");
	}
}

int cestDansLaBoite(vertex *a, vertex *b, vertex *c)
{
    double xMin = MIN(a->coords[0], b->coords[0]) ;
    double yMin = MIN(a->coords[1], b->coords[1]) ;
    double xMax = MAX(a->coords[0], b->coords[0]) ;
    double yMax = MAX(a->coords[1], b->coords[1]) ;
    if((xMin <= c->coords[0]) && (xMax >= c->coords[0]) && (yMin <= c->coords[1]) && (yMax >= c->coords[1]))
        return VRAI ;
    return FAUX ;
}

int minLexico()
{
    int i ;
    int indiceReturn = -1 ;
    for(i = 0; i < nbPointsPolygon; i++)
    {
        if(i == 0)
            indiceReturn = 0 ;
        else
        {
            if(P[i].coords[0] < P[indiceReturn].coords[0])
                indiceReturn = i ;
            else if (P[i].coords[0] == P[indiceReturn].coords[0])
            {
                if (P[i].coords[1] < P[indiceReturn].coords[1])
                    indiceReturn = i ;
            }
        }
    }
    return indiceReturn ;
}

int puissance(vertex *a, vertex *b, vertex *c)
{
    double A = b->coords[1] - a->coords[1] ;
    double B = a->coords[0] - b->coords[0] ;
    double C = -(B * a->coords[1] + A * a->coords[0]) ;
    double value = (A*c->coords[0] + B*c->coords[1] + C) / sqrt(A*A + B*B) ;
    if(value < 0) return AGAUCHE ;
    else if(value > 0) return ADROITE ;
    else return ALIGNE ;
}

int estDedans(vertex* a)
{
    int nbCoupure = 0;
    int position = ALIGNE;
    int vertexDepart = 0;
    int vertexArrivee;
    int prec;
    int i;
    int puiss;
    vertex b;

    prec = nbPointsPolygon - 1;
    i = 0;
    while(i < nbPointsPolygon)
    {
        if(puissance(&P[prec], &P[i], a) == ALIGNE && cestDansLaBoite(&P[prec], &P[i], a))
            return prec;
        i++;
        prec = i - 1;
    }

    b.coords[0] = (P[minLexico()].coords[0] - 1.0);
    b.coords[1] = a->coords[1];

    position = puissance(a, &b, &P[vertexDepart]);
    while(position == ALIGNE)
    {
        vertexDepart++;
        position = puissance(a, &b, &P[vertexDepart]);
    }

    if(vertexDepart == (nbPointsPolygon - 1))
    {
        i = 0;
        vertexArrivee = i;
    }
    else
    {
        i = (vertexDepart + 1);
        vertexArrivee = i;
    }

    do
    {
        (i == 0) ? (prec = (nbPointsPolygon - 1)) : (prec = (i - 1));
        puiss = puissance(a, &b, &P[i]);
        if(position != puiss && puiss != ALIGNE)
        {
            if(puissance(&P[prec], &P[i], a) != puissance(&P[prec], &P[i], &b))
                nbCoupure++;
            position = puiss;
        }
        (i == (nbPointsPolygon - 1)) ? (i = 0) : (i++);
    }while(i != vertexArrivee);

    if((nbCoupure % 2) == 1)
        return nbPointsPolygon;
    else
        return -1;
}

void generation(int button, int state, int x, int y)
{
    double coordX = ((double)x * (double)maxX) / (double)glutGet(GLUT_WINDOW_WIDTH) ;
    double coordY = (double)maxY-((double)y * (double)maxY) / (double)glutGet(GLUT_WINDOW_HEIGHT) ;
    int enregistre = FAUX ;

    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        if(ecrireFichier)
        {
            if(nbPointsPolygon == 0)
                enregistre = VRAI ;
            else
            {
                if(P[nbPointsPolygon-1].coords[0] != coordX || P[nbPointsPolygon-1].coords[1] != coordY)
                {
                    if((pow(P[0].coords[0] - coordX, 2) + pow(P[0].coords[1] - coordY, 2)) < pow(DISTMAX, 2))
                    {
                        fEcriture() ;
                        free(P) ;
                        exit(0) ;
                    }
                    else
                        enregistre = VRAI ;
                }
            }
            if(enregistre)
            {
                if(nbPointsPolygon >= nbPointsMax)
                {
                    nbPointsMax++ ;
                    P = realloc (P, nbPointsMax * sizeof(vertex)) ;
                }
                P[nbPointsPolygon].coords[0] = coordX ;
                P[nbPointsPolygon].coords[1] = coordY ;
                nbPointsPolygon++ ;
                printf("x = %lf, y = %lf, nbPointsPolygon: %i, nbPointsMax: %i\n", coordX, coordY, nbPointsPolygon, nbPointsMax) ;
                glutPostRedisplay() ;
            }
        }
        else
        {
            clic = 0;
            glutPostRedisplay();
        }
    }
    else if((button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) && !ecrireFichier)
    {
        vertex a;
        a.coords[0] = coordX;
        a.coords[1] = coordY;
        dedans = estDedans(&a);
        clic = 1;
        glutPostRedisplay();
    }
}

void fLecture()
{
	FILE *fichier ;
    int i ;
	
	fichier = fopen(nomFichier, "r") ;
	
	if(fichier != NULL)
	{
		fread(&nbPointsPolygon, sizeof(int), 1, fichier) ;
		
		nbPointsMax = nbPointsPolygon ;
		P = realloc (P, nbPointsMax * sizeof(vertex)) ;
		
		i = 0 ;
		while(!feof(fichier) && i < nbPointsPolygon)
		{
			fread(&P[i].coords[0], sizeof(double), 1, fichier) ;
			fread(&P[i].coords[1], sizeof(double), 1, fichier) ;
            P[i].intersect = FAUX ;
            P[i].concave = FAUX ;
			i++ ;
		}
		fclose(fichier) ;
	}
}

int verification()
{
    int i = 0 ;
    int j = 0 ;

    vertex *Ptemp = calloc(3, sizeof(vertex)) ;

    if(nbPointsPolygon < 2)
    {
        printf("pas assez d\'element : %i (minimum 3)\n", nbPointsPolygon) ;
        return FAUX ;
    }

    Ptemp[0] = P[0] ;
    j++ ;

    for(i = 1; i < nbPointsPolygon; i++)
    {
        if(P[i].coords[0] != Ptemp[j-1].coords[0] || P[i].coords[1] != Ptemp[j-1].coords[1])
        {
            if((j > 1 && (P[i].coords[0] != Ptemp[j-2].coords[0] || P[i].coords[1] != Ptemp[j-2].coords[1])) || j == 1)
            {
                Ptemp[j] = P[i] ;
                j++ ;
                if (j > 2)
                {
                    if(puissance(&Ptemp[0], &Ptemp[1], &Ptemp[2]) != 2)
                        return VRAI ;
                    else
                        j-- ;
                }
            }
        }
    }
    printf("points alignes ou points superposes\n") ;
    return FAUX ;
}

int intersection()
{
    int i ;
    int j ;
    int nextJ ;
    int polygonePasCool = FAUX ;
    for(i = 0; i < nbPointsPolygon-1; i++)
    {
        for(j = i+1; j < nbPointsPolygon; j++)
        {
            nextJ = j + 1 ;
            if(nextJ >= nbPointsPolygon)
                nextJ = 0;
            int a = puissance(&P[i], &P[i+1], &P[j]) ;
            int b = puissance(&P[i], &P[i+1], &P[nextJ]) ;
            if((a == ADROITE && b == AGAUCHE) || (a == AGAUCHE && b == ADROITE))
            {
                int a = puissance(&P[j], &P[nextJ], &P[i]) ;
                int b = puissance(&P[j], &P[nextJ], &P[i+1]) ;
                if((a == ADROITE && b == AGAUCHE) || (a == AGAUCHE && b == ADROITE))
                {
                    P[i].intersect = VRAI ;
                    P[j].intersect = VRAI ;
                    polygonePasCool = VRAI ;
                    printf("ca va couper\n") ;
                }
            }
            if(((a == ALIGNE) && (i+1 != j) && (cestDansLaBoite(&P[i], &P[i+1], &P[j])))
                || ((b == ALIGNE) && (i != nextJ) && (cestDansLaBoite(&P[i], &P[i+1], &P[nextJ]))))
            {
                P[i].intersect = VRAI ;
                P[j].intersect = VRAI ;
                polygonePasCool = VRAI ;
                printf("Aligne pas cool\n") ;
            }
        }
    }
    return polygonePasCool ;
}

int verifSensTrigo()
{
    int indiceMinLexico = minLexico();
    int prec = (indiceMinLexico == 0) ? (nbPointsPolygon - 1) : (indiceMinLexico - 1) ;
    int suiv = (indiceMinLexico == (nbPointsPolygon - 1)) ? (0) : (indiceMinLexico + 1) ;
    return puissance(&P[prec], &P[indiceMinLexico], &P[suiv]) ;
}

void inverseSensPolygone()
{
    vertex* Ptemp = calloc(nbPointsPolygon, sizeof(vertex)) ;
    int i = 0 ;
    for(i = 0; i < nbPointsPolygon; i++)
        Ptemp[i] = P[nbPointsPolygon-i-1] ;
    memcpy(P, Ptemp,nbPointsPolygon * sizeof(vertex)) ;
}

void verifeConcave()
{
    int i = 0 ;
    int prec ;
    int suiv ;
    for(i = 0; i < nbPointsPolygon; i++)
    {
        prec = (i == 0) ? (nbPointsPolygon - 1) : (i - 1) ;
        suiv = (i == (nbPointsPolygon - 1)) ? (0) : (i + 1) ;
        if(!puissance(&P[prec], &P[i], &P[suiv]))
            P[i].concave = VRAI;
    }
}

void winInit()
{
	gluOrtho2D(minX, maxX, minY, maxY) ;
}

void display(void)
{
    int n = nbPointsPolygon ;
    int suiv ;

    glColor3f(0.0, 0.0, 0.0) ;
	glClear(GL_COLOR_BUFFER_BIT) ;
	
	if(ecrireFichier)
	{
		glBegin(GL_LINE_STRIP) ;
		glColor3f(1.0, 1.0, 1.0) ;

		while (--n >= 0)
			glVertex2f(P[n].coords[0], P[n].coords[1]) ;
	}
	else
	{
        glBegin(GL_LINE_LOOP) ;
        if(clic && ((dedans == -1) || (dedans == nbPointsPolygon)))
        {
            if(dedans == -1)
                glColor3f(1.0, 0.0, 0.0) ;
            else
                glColor3f(0.0, 1.0, 0.0) ;

            while (--n >= 0)
                glVertex2f(P[n].coords[0], P[n].coords[1]) ;
        }
        else
        {
            glColor3f(1.0, 1.0, 1.0) ;
            while (--n >= 0)
            {
                suiv = (n == (nbPointsPolygon - 1)) ? (0) : (n + 1) ;

                if(P[n].intersect)
                    glColor3f(1.0, 0.0, 0.0) ;
                if(P[n].concave || P[suiv].concave)
                    glColor3f(0.0, 1.0, 0.0) ;
                if(clic && dedans == n)
                    glColor3f(0.0, 0.0, 1.0) ;

                glVertex2f(P[suiv].coords[0], P[suiv].coords[1]) ;
                glVertex2f(P[n].coords[0], P[n].coords[1]) ;
                glColor3f(1.0, 1.0, 1.0) ;
            }
        }
	}
	glEnd() ;
	glFlush() ;
}

int main(int argc, char **argv)  
{
	if(argc > 3)
	{
		printf("trop d\'arguments.\n") ;
		return EXIT_FAILURE ;
	}
	
	if(argc > 1)
	{
        if(!strcmp(argv[1], "w"))
            ecrireFichier = VRAI ;
        else if(!strcmp(argv[1], "r"))
            ecrireFichier = FAUX ;
		else
		{
            printf("premier argument incorrect: %s\nr: lecture, w: ecriture\n", argv[1]) ;
			return EXIT_FAILURE ;
		}
		
		if(argc > 2)
			nomFichier = argv[2] ;
    }
	
	P = calloc(nbPointsMax, sizeof(vertex)) ;
	
	glutInit(&argc, argv) ;
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA) ;

	glutInitWindowPosition(5,5) ;
	glutInitWindowSize(300,300) ;

    glutCreateWindow("Polygone") ;
	winInit() ;
	
    if(!ecrireFichier)
	{
		fLecture() ;
		if(!verification())
			return EXIT_FAILURE ;
        if(!intersection())
        {
            if(!verifSensTrigo())
                inverseSensPolygone();
            verifeConcave();
        }
	}
    else
    {
        printf("Cliquer dans la fenêtre pour enregistrer les points du polygone.\n");
        printf("Pour finir, recliquer assez proche du point de départ.\n");
    }
    glutMouseFunc(generation) ;

	glutDisplayFunc(display) ;
	glutMainLoop() ;

	return EXIT_SUCCESS ;
}  
