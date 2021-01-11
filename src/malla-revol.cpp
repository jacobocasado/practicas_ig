// Nombre: Jacobo, Apellidos: Casado de Gracia, DNI/pasaporte: 77146828K (DDGG IG curso 20-21)
// *********************************************************************
// **
// ** Informática Gráfica, curso 2019-20
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include "ig-aux.h"
#include "tuplasg.h"
#include "lector-ply.h"
#include "matrices-tr.h"
#include "malla-revol.h"
#include "malla-ind.h"

using namespace std ;

// *****************************************************************************


// Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
// a partir de un perfil y el número de copias que queremos de dicho perfil.
void MallaRevol::inicializar
(
   const std::vector<Tupla3f> & perfil,     // tabla de vértices del perfil original
   const unsigned               num_copias  // número de copias del perfil
)
{
    int m = perfil.size();

    // Práctica 4: Cálculo de las coordenadas de textura
        vector<float> d;
        vector<float> t;
        float sum_distancias = 0.0;

        for ( unsigned i = 0; i < m - 1; ++i )
        {
            d.push_back(sqrt((perfil[i+1] - perfil[i]).lengthSq()));
            sum_distancias += d[i];
        }
        
        // Cálculo del vector t
        t.push_back(0);
        for( unsigned i = 1; i < m - 1; ++i)
        {
            float sum_j = 0.0;

            for(int j = 0; j < i; ++j)
            {
                sum_j += d[j];
            }

            t.push_back(sum_j/sum_distancias);
        }
    
    t.push_back(1);

   

   // PARA LOS VERTICES
   // Bucle for por cada copia del perfil. Dentro de cada copia representamos los vertices.
    
    // NORMALES DE ARISTAS
    vector<Tupla3f> nor_aristas;
    for( unsigned i = 0; i < m - 1; ++i)
        {
            // Obtenemos la arista que forman los vertices
            Tupla3f arista = perfil[i+1] - perfil[i];
            // La rotamos 90 grados
            Tupla3f normal = MAT_Rotacion(90, 0, 0, 1) * arista;
            //    La normalizamos
            if(normal.lengthSq() != 0.0)
                normal = normal.normalized();
            else
                normal = {0, 0, 0};

            nor_aristas.push_back(normal);
        }
    
    // CÁLCULO NORMALES DE LOS VÉRTICES DEL PERFIL
        vector<Tupla3f> nor_ver_perf;
        
        // Primer vértice
        nor_ver_perf.push_back(nor_aristas[0]);

        for(int i = 1; i < m - 1; ++i)
        {
            nor_ver_perf.push_back(nor_aristas[i-1] + nor_aristas[i]);
        }

        nor_ver_perf.push_back(nor_aristas[nor_aristas.size() - 1]);
    
    // ROTACION DE VERTICES Y NORMALES
   for (int i = 0; i < num_copias; ++i){

      // Calculamos el angulo que deben de hacer los vertices.
      float angulo = ((2 * M_PI *  i) / (num_copias - 1));

      for (int j = 0; j < m ; ++j){

         // Aplicamos la rotacion a los vertices.
         // Rotamos el vértice del ejeX (perfilj0) y el vértice del eje z. El vértice y (perfilj1) lo dejamos como está.

         Tupla3f vertices_rotados = { ( perfil[j][0] * cosf(angulo) - perfil[j][2] * sinf(angulo) ) , // Vertice X
                                  perfil[j][1], // Vertice Y
                                  ( perfil[j][0] * sinf(angulo) - perfil[j][2] * cosf(angulo) )
                                  }; // Vertice Z

         // Añadimos el vértice rotado a la lista de vértices.
         vertices.push_back(vertices_rotados);

          // Calculo de normales
          Tupla3f normal_rotada = {nor_ver_perf[j][0]*cos(angulo) - nor_ver_perf[j][2]*sin(angulo),
                                                       nor_ver_perf[j][1],
                                            nor_ver_perf[j][0]*sin(angulo) + nor_ver_perf[j][2]*cos(angulo)};

          nor_ver.push_back(normal_rotada);
          // Introducción de las coordenadas de textura
          cc_tt_ver.push_back({(i*1.00)/(num_copias * 1.00 - 1), (1 - t[j])});
          

      }
   }

   // PARA LOS TRIANGULOS (CARAS)
   for (int i = 0; i < (num_copias - 1); ++i){
      for (int j = 0; j < (m - 1); ++j){
         int k = (i * m + j);
         triangulos.push_back({k, k + m, k + m + 1});
         triangulos.push_back({k, k + m + 1, k + 1});
      }
   }
}

// -----------------------------------------------------------------------------
// constructor, a partir de un archivo PLY

MallaRevolPLY::MallaRevolPLY
(
   const std::string & nombre_arch,
   const unsigned      nperfiles
)
{
   ponerNombre(std::string("Malla por revolución del perfil en '"+ nombre_arch + "'" ));
   // COMPLETAR: práctica 2: crear la malla de revolución
   // Leer los vértice del perfil desde un PLY, después llamar a 'inicializar'
   std::vector<Tupla3f> vertices;
   LeerVerticesPLY(nombre_arch, vertices);
   inicializar(vertices, nperfiles);
}

Cilindro::Cilindro
(const int num_verts_per, const unsigned nperfiles)
{
   // La base tiene centro en el origen. La altura y el radio son 1.
   std::vector<Tupla3f> perfil;
    for (int i = 1; i <= num_verts_per; ++i){
        float y = 1/i;
        Tupla3f vertice = {1.0, y, 0.0};
        perfil.push_back(vertice);
    }
    

   inicializar(perfil, nperfiles);
}

Cono::Cono
(const int num_verts_per, const unsigned nperfiles)
{
   // La base tiene centro en el origen. La altura y el radio son 1.
   std::vector<Tupla3f> perfil;
    for (int i = 1; i <= num_verts_per; ++i){
        float y = 1/i;
        float x = 1 - y;
        Tupla3f vertice = {x, y, 0.0};
        perfil.push_back(vertice);
    }
   inicializar(perfil, nperfiles);
}

Esfera::Esfera
(const int num_verts_per, const unsigned nperfiles)
{
{
   std::vector<Tupla3f> perfil;

   Tupla3f q, p;
   p = { 0.0, -1.0, 0.0 };

   for (int i = 0; i <= num_verts_per; i++) {
      float ang = ((M_PI * i) / (num_verts_per - 1));
      float sen = sin(ang);
      float cose = cos(ang);
      q = { p(0) * cose + p(1) * sen , p(1) * cose - p(0) * sen, p(2)};
      perfil.push_back(q);
   }

   ponerNombre(std::string("Malla por revolución del perfil. Esfera"));
   inicializar(perfil, nperfiles);
    
    
    for (int i = 0; i < nor_ver.size(); ++i){
        nor_ver[i] = nor_ver[i] * -1.00;
    }
}
}

