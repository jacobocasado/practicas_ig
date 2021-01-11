// Nombre: Jacobo, Apellidos: Casado de Gracia, DNI/pasaporte: 77146828K (DDGG IG curso 20-21)
#include "modelo-jer.h"
#include "grafo-escena.h"
#include "objeto3d.h"
#include "malla-ind.h"
#include "malla-revol.h"
#include "matrices-tr.h"
#include <cmath>

Muñeco::Muñeco(const float altura_inicial){
    const Tupla3f color = {1.0, 1.0, 1.0};
    ponerColor(color);
    unsigned ind1 = agregar(MAT_Traslacion(0.0,altura_inicial,0.0));
    agregar (new EsferaCara());
    nariz = new Nariz (1.0);
    agregar (nariz);
    agregar (new Cuerpo());
    agregar (new Ojos());
    agregar (new Sombrero());
    agregar (new Brazos());
    piernas = new Piernas(0.0);
    agregar (piernas);
    
    pm_altura = leerPtrMatriz(ind1);

}


unsigned Muñeco::leerNumParametros() const {
    return 1;
};

void Muñeco::fijarSalto(const float altura_salto){
    * pm_altura = MAT_Traslacion(0.0,altura_salto,0.0);
}

void Muñeco::actualizarEstadoParametro( const unsigned iParam, const float t_sec){
    //assert (0 <= iParam < leerNumParametros()); // Comprobamos que iParam es correcto.

    if (iParam == 0){
        float altura_salto = (3/2) + (3/2) * sin(2*M_PI*1*t_sec);
        fijarSalto(altura_salto);
        piernas->actualizarEstadoParametro(iParam, t_sec);
        nariz->actualizarEstadoParametro(iParam,t_sec);
    }
};

EsferaCara::EsferaCara(){
    const Tupla3f color = {252.0/255.0, 208.0/255.0, 180.0/255.0};
    ponerColor(color);
    agregar (new Esfera(20,50));
}

Ojos::Ojos(){
    const Tupla3f color = {0.0, 0.0, 0.0};
    ponerColor(color);
    agregar (new OjoDer);
    agregar (MAT_Escalado(-1.0, 1.0, 1.0));
    agregar (new OjoDer);
}

OjoDer::OjoDer(){
    agregar(MAT_Traslacion(0.5, 0.4, 0.6));
    agregar(MAT_Escalado(0.2, 0.2, 0.2));
    agregar(new Esfera(20, 50));
}

Nariz::Nariz(const float escalado_inicial){
    agregar(MAT_Traslacion(0.0,0.0,1.0));
    agregar(MAT_Escalado(0.2, 0.2, 1.0));
    agregar(MAT_Rotacion(90, {1.0,0.0,0.0}));
    unsigned ind1 = agregar(MAT_Escalado(1.0, 1.0, escalado_inicial));
    agregar(new Cono (20,50));
    
    pm_escalado = leerPtrMatriz(ind1);
}
void Nariz::fijarAlturaNariz (const float altura_nariz){
    * pm_escalado = MAT_Escalado(1.0, 1.0, altura_nariz);
}

unsigned Nariz::leerNumParametros() const {
    return 1;
};

void Nariz::actualizarEstadoParametro( const unsigned iParam, const float t_sec){
    //assert (0 <= iParam < leerNumParametros()); // Comprobamos que iParam es correcto.
    
    if (iParam == 0){
        float nueva_altura = (3/2) + (3/2) * sin(2*M_PI*1*t_sec);
        fijarAlturaNariz(nueva_altura);
    }
};



Cuerpo::Cuerpo(){
    const Tupla3f color = {0.0/255.0, 170.0/255.0, 228.0/255.0};
    ponerColor(color);
    agregar(MAT_Traslacion(0.0,-2.0,0.0));
    agregar(MAT_Escalado(1.0, 1.5, 1.0));
    agregar(new Esfera(20,50));
}

Sombrero::Sombrero(){
    const Tupla3f color = {255.0/255.0, 0.0/255.0, 0.0/255.0};
    ponerColor(color);
    agregar(MAT_Traslacion(0.0, 0.8, 0.0));
    agregar(MAT_Escalado(0.6, 1.0, 0.6));
    agregar (new Cono(20,50));
}

Piernas::Piernas(const float rotacion_inicial){
    agregar(MAT_Traslacion(0.4,-3.2,0.3));
    unsigned ind1 = agregar(MAT_Rotacion(rotacion_inicial, {1,0.0,0.0}));
    agregar (new Pierna());
    agregar (MAT_Traslacion(-0.8, 0.0, 0.0));
    agregar (new Pierna());
    
    pm_rotacion = leerPtrMatriz(ind1);
}

unsigned Piernas::leerNumParametros() const {
    return 1;
};

void Piernas::fijarGiroPiernas(const float angulo_giro){
    * pm_rotacion = MAT_Rotacion(angulo_giro * (-1), {1,0.0,0.0});
}

void Piernas::actualizarEstadoParametro( const unsigned iParam, const float t_sec){
    //assert (0 <= iParam < leerNumParametros() ); // Comprobamos que iParam es correcto.
    if (iParam == 0){
        float nuevo_alpha = (45/2) + (45/2) * sin(2*M_PI*1*t_sec);
        fijarGiroPiernas(nuevo_alpha);
    }
    
};



PiernaDer::PiernaDer(){
    const Tupla3f color = {253.0/255.0, 253.0/255.0, 150.0/255.0};
    ponerColor(color);
    //agregar(MAT_Traslacion(0.4,-4.0,0.0));
    agregar(MAT_Escalado(0.2, 1.5, 0.2));
    agregar(new Cilindro(20,50));
}

Pierna::Pierna(){
    agregar(MAT_Traslacion(0.0,-1.0,0.0));
    agregar(new PiernaDer());
    agregar(MAT_Traslacion(0.0,0.0,0.1));
    agregar(new PieDer());
}

PieDer::PieDer(){
    const Tupla3f color = {78.0/255.0, 59.0/255.0, 49.0/255.0};
    ponerColor(color);
    //agregar(MAT_Traslacion(0.4,-4.2,0.2));
    agregar(MAT_Escalado(0.4, 0.4, 0.6));
    agregar(new Esfera(20,50));
}

BrazoDer::BrazoDer(){
    const Tupla3f color = {255.0/255.0, 0.0/255.0, 0.0/255.0};
    ponerColor(color);
    agregar(new ManoDer());
    agregar(MAT_Traslacion(1.1,-1.5,0.0));
    agregar(MAT_Rotacion(90, {1.0,0.0,0.0}));
    agregar(MAT_Escalado(0.2, 1.0, 0.3));
    agregar(new Cilindro(20,50));
    }

Brazos::Brazos(){
    agregar(new BrazoDer());
    agregar (MAT_Escalado(-1.0, 1.0, 1.0));
    agregar (new BrazoDer());
}

ManoDer::ManoDer(){
    const Tupla3f color = {0.0/255.0, 170.0/255.0, 228.0/255.0};
    ponerColor(color);
    agregar(MAT_Traslacion(1.1,-1.5,1.0));
    agregar(MAT_Rotacion(90, {1.0,0.0,0.0}));
    agregar(MAT_Escalado(0.3, 0.3, 0.3));
    agregar(new Esfera(20,50));
}
