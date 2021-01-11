// Nombre: Jacobo, Apellidos: Casado de Gracia, DNI/pasaporte: 77146828K (DDGG IG curso 20-21)
#ifndef MODELO-JER_H
#define MODELO-JER_H

#include "grafo-escena.h"
#include "objeto3d.h"


// #####################################
// Funciones usadas para modelo jerárquico:
// ####################################

class Muñeco : public NodoGrafoEscena{
private:
    Objeto3D * piernas;
    Objeto3D * nariz;
protected: // punteros a matrices
    Matriz4f * pm_altura = nullptr;
    public:
    Muñeco(const float altura_inicial); // Constructor.
    void fijarSalto( const float altura_salto);
    unsigned leerNumParametros() const ;
    void actualizarEstadoParametro( const unsigned iParam, const float t_sec );
};

class EsferaCara : public NodoGrafoEscena{
    public:
    EsferaCara(); // Constructor.
};

class Ojos : public NodoGrafoEscena{
public:
    Ojos();
};

class OjoDer : public NodoGrafoEscena{
public:
    OjoDer();
};

class Nariz : public NodoGrafoEscena{
protected: // punteros a matrices
    Matriz4f * pm_escalado = nullptr;
public:
    Nariz(const float escalado_inicial);
    void fijarAlturaNariz(const float altura_nariz);
    unsigned leerNumParametros() const ;
    void actualizarEstadoParametro( const unsigned iParam, const float t_sec );
};

class Cuerpo : public NodoGrafoEscena{
public:
    Cuerpo();
};

class Sombrero : public NodoGrafoEscena{
public:
    Sombrero();
};

class Piernas : public NodoGrafoEscena{
protected: // punteros a matrices
    Matriz4f * pm_rotacion = nullptr;
public:
    Piernas(const float alpha_inicial);
    void fijarGiroPiernas(const float angulo_giro);
    unsigned leerNumParametros() const ;
    void actualizarEstadoParametro( const unsigned iParam, const float t_sec );
};

class PiernaDer : public NodoGrafoEscena{
public:
    PiernaDer();
};

class PieDer : public NodoGrafoEscena{
public:
    PieDer();
};

class Pierna : public NodoGrafoEscena{
public:
    Pierna();
};

class BrazoDer : public NodoGrafoEscena{
public:
    BrazoDer();
};

class Brazos : public NodoGrafoEscena{
public:
    Brazos();
};

class ManoDer : public NodoGrafoEscena{
public:
    ManoDer();
};


#endif // MODELO-JER_H
