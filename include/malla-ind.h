// Nombre: Jacobo, Apellidos: Casado de Gracia, DNI/pasaporte: 77146828K (DDGG IG curso 20-21)
// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#ifndef IG_MALLAIND_HPP
#define IG_MALLAIND_HPP

#include <vector>          // usar std::vector

#include "objeto3d.h"   // declaración de 'Objeto3D'
#include "array-verts.h" // clase 'ArrayVert'

// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaInd : public Objeto3D
{
   

   protected:
      // COMPLETAR: incluir aquí las variables y métodos privados que sean
      // necesarios para una malla indexada
      // ......

      std::vector<Tupla3f> vertices ;
      std::vector<Tupla3i> triangulos ;

      std::vector<Tupla3f> col_ver ;  // colores de los vértices
      std::vector<Tupla3f> nor_ver ;  // normales de vértices
      std::vector<Tupla3f> nor_tri ;  // normales de triangulos
      std::vector<Tupla2f> cc_tt_ver ; // coordenadas de textura de los vértices

     
      // array de vértices con información de tablas para visualizar
      // (se crea bajo demanda: la primera vez que se llama a 'visualizarGL')
      ArrayVertices * array_verts = nullptr ; 



      // normales de triángulos y vértices
      void calcularNormales();

      // calculo de las normales de triángulos (solo si no están creadas ya)
      void calcularNormalesTriangulos() ;



   public:
      // crea una malla vacía (nombre: "malla indexada nueva vacía")
      MallaInd() ;

      // crea una malla vacía con un nombre concreto:
      MallaInd( const std::string & nombreIni );

      // visualizar el objeto con OpenGL
      virtual void visualizarGL( ContextoVis & cv ) ;

    //Metodo para rotar un vertice n grados en el eje que le demos.
    Tupla3f rotarVerticeGrados(Tupla3f vertice, char eje, float grados );




} ;
// ---------------------------------------------------------------------
// Clase para mallas obtenidas de un archivo 'ply'
// es un tipo de malla indexada que define un nuevo constructor
// que recibe el nombre del archivo ply como parámetro

class MallaPLY : public MallaInd
{
   public:
      MallaPLY( const std::string & nombre_arch ) ;
} ;


// ---------------------------------------------------------------------

class Cubo : public MallaInd
{
   public:
      Cubo();
};

class CuboColores : public MallaInd
{
   public:
      CuboColores();
};

class Tetraedro : public MallaInd
{
   public:
      Tetraedro();
};

// --------------------------------------------------------------------

class Cubo24 : public MallaInd
{
  public:
      Cubo24();
} ;

class EstrellaZ : public MallaInd
{
public:
    EstrellaZ(unsigned n);
};

class CasaX : public MallaInd
{
public:
    CasaX();
};

class RejillaX : public MallaInd
{
public:
    RejillaX(unsigned n, unsigned m);
};

class CuadradoRejilla : public MallaInd{
public:
    CuadradoRejilla();
};

class Tetraedro12 : public MallaInd{
public:
    Tetraedro12();
};

class MallaDiscoP4 : public MallaInd{
    public:
        MallaDiscoP4();
};



#endif
