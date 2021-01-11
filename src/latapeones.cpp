// Nombre: Jacobo, Apellidos: Casado de Gracia, DNI/pasaporte: 77146828K (DDGG IG curso 20-21)
// *********************************************************************
// **
// ** Informática Gráfica, curso 2020-21
// ** Declaraciones de la clase LataPeones.hpp
// **
// *********************************************************************


#include "latapeones.h"


// Constructor de la clase LataPeones

VariasLatasPeones::VariasLatasPeones(){
    agregar (new PeonMadera());
    agregar (new PeonBlanco());
    agregar (new PeonNegro());
    agregar (new LataCocaCola());
    agregar (new LataPepsi());
    agregar (new LataUGR());
}

LataPepsi::LataPepsi(){
    ponerNombre("Lata de Pepsi");
    ponerIdentificador(5);
    agregar(MAT_Traslacion(0.0,0.0,-4.0));
    Textura * textura = new Textura("../recursos/imgs/lata-pepsi.jpg");
    agregar(new Material(textura,0.5,0.0,0.2,5.00));
    MallaRevolPLY * perfil = new MallaRevolPLY("../recursos/plys/lata-pcue.ply", 10);
    agregar(perfil);
    agregar(new Material(0.5,0.0,0.2,5.00));
    MallaRevolPLY * superior = new MallaRevolPLY("../recursos/plys/lata-psup.ply", 10);
    agregar(superior);
    agregar(new Material(0.5,0.0,0.2,5.00));
    MallaRevolPLY * inferior = new MallaRevolPLY("../recursos/plys/lata-pinf.ply", 10);
    agregar(inferior);
}

LataUGR::LataUGR(){
    ponerNombre("Lata de la UGR");
    ponerIdentificador(6);
    agregar(MAT_Traslacion(4.0,0.0,0.0));
    Textura * textura = new Textura("../recursos/imgs/window-icon.jpg");
    agregar(new Material(textura,0.5,0.0,0.2,5.00));
    MallaRevolPLY * perfil = new MallaRevolPLY("../recursos/plys/lata-pcue.ply", 10);
    agregar(perfil);
    agregar(new Material(0.5,0.0,0.2,5.00));
    MallaRevolPLY * superior = new MallaRevolPLY("../recursos/plys/lata-psup.ply", 10);
    agregar(superior);
    agregar(new Material(0.5,0.0,0.2,5.00));
    MallaRevolPLY * inferior = new MallaRevolPLY("../recursos/plys/lata-pinf.ply", 10);
    agregar(inferior);
}
