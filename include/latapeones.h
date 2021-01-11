// Nombre: Jacobo, Apellidos: Casado de Gracia, DNI/pasaporte: 77146828K (DDGG IG curso 20-21)
#ifndef LATAPEONES_H
#define LATAPEONES_H

#include "objeto3d.h"
#include "practicas.h"
#include "escena.h"
#include "tuplasg.h"
#include "malla-ind.h"   // declaración de 'ContextoVis'
#include "lector-ply.h"
#include "ig-aux.h"
#include "matrices-tr.h"
#include "grafo-escena.h"
#include "malla-ind.h"
#include "malla-revol.h"
#include "materiales-luces.h"

class VariasLatasPeones : public NodoGrafoEscena
{
public:
    VariasLatasPeones();
};

class LataPepsi : public NodoGrafoEscena{
public:
    LataPepsi();
};

class LataUGR : public NodoGrafoEscena{
public:
    LataUGR();
};


#endif // SELECCION_H
