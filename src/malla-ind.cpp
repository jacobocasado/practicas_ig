// Nombre: Jacobo, Apellidos: Casado de Gracia, DNI/pasaporte: 77146828K (DDGG IG curso 20-21)
// *********************************************************************
// **
// ** Informática Gráfica, curso 2020-21
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

//#include <set>   // std::set
#include "ig-aux.h"
#include "tuplasg.h"
#include "malla-ind.h"   // declaración de 'ContextoVis'
#include "lector-ply.h"


// *****************************************************************************
// funciones auxiliares

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // nombre por defecto
   ponerNombre("malla indexada, anónima");
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
}

//-----------------------------------------------------------------------------
// calcula la tabla de normales de triángulos una sola vez, si no estaba calculada

void MallaInd::calcularNormalesTriangulos()
{

   // si ya está creada la tabla de normales de triángulos, no es necesario volver a crearla
   const unsigned nt = triangulos.size() ;
   assert( 1 <= nt );
   if ( 0 < nor_tri.size() )
   {
      assert( nt == nor_tri.size() );
      return ;
   }

   // COMPLETAR: Práctica 4: creación de la tabla de normales de triángulos
   // ....

    for( unsigned i = 0; i < triangulos.size(); ++i )
    {
        // Cálculo de los vectores correspondientes a dos aristas del triángulo
        Tupla3f v1 = vertices[triangulos[i][1]] - vertices[triangulos[i][0]];
        Tupla3f v2 = vertices[triangulos[i][2]] - vertices[triangulos[i][0]];

        // Cálculo del vector perpendicular a la cara

    //    Tupla3f vp = {(v1[1]v2[2] - v1[2]v2[2]),
    //                      (v1[0]v2[2] - v1[2]v2[0]) * -1,
    //                      (v1[0]v2[1] - v1[1]v2[0])};
        Tupla3f vp = v1.cross(v2);

        //Cálculo del vector normal
        Tupla3f vn;

        if(vp.lengthSq() != 0.0) // Si no tiene distancia nula, se normaliza
            vn = vp.normalized();
        else                          // Si tiene distancia nula se le asigna el vector nulo
            vn = {0,0,0};


        //Introducimos el vector normal en la lista de normales de los triángulos
        nor_tri.push_back(vn);
    }
}

// Método que rota un vertice los grados que le demos en el eje que queramos.
// Se le pasa de parámetro el vértice, el eje y los grados.

Tupla3f MallaInd::rotarVerticeGrados(Tupla3f vertice, char eje, float grados){
    
    float sen = sin(grados);
    float cose = cos(grados);
    
    
    switch (eje){
        case 'x': // Queremos rotar en el eje x.
            vertice = { vertice[0],
                        vertice[1] * cose - vertice[2] * sen,
                        vertice[1] * sen + vertice[2] * cose};
            break;
            
        case 'y':
            vertice = { vertice[0] * cose - vertice[2] * sen ,
                        vertice[1],
                        vertice[0] * sen + vertice[2] * cose};
            break;
            
        case 'z':
            vertice = { vertice[0] * cose - vertice[1] * sen ,
                        vertice[1] * cose + vertice[0] * sen ,
                        vertice[2] };
            break;
            
        default:
            std::cout << "La rotación no se ha producido. Eje incorrecto";
            break;
    }
    
    return vertice;
    
}


// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcularNormales()
{
   // COMPLETAR: en la práctica 4: calculo de las normales de la malla
   // se debe invocar en primer lugar 'calcularNormalesTriangulos'
    calcularNormalesTriangulos();


   //Inicializamos el vector de normales de los vértices con todas las normales nulas
   nor_ver.resize(vertices.size(), {0,0,0});

   // Almacenamos en nor_ver la suma de las normales de las caras adyacentes a cada
   // vértice
   for( unsigned i = 0; i < triangulos.size(); ++i)
   {
      int ind_vert_0 = triangulos[i][0];
      int ind_vert_1 = triangulos[i][1];
      int ind_vert_2 = triangulos[i][2];

      nor_ver[ind_vert_0] = nor_ver[ind_vert_0] + nor_tri[i];
       nor_ver[ind_vert_1] = nor_ver[ind_vert_1] + nor_tri[i];
      nor_ver[ind_vert_2] = nor_ver[ind_vert_2] + nor_tri[i];
   }

    // Normalizamos los vectores
    for( unsigned i = 0; i < nor_ver.size(); ++i)
    {
       if(nor_ver[i].lengthSq() != 0.0)
        nor_ver[i] = nor_ver[i].normalized();
        else
          nor_ver[i] = {0,0,0};
    }

}

// ----------------------------------------------------------------------------


void MallaInd::visualizarGL( ContextoVis & cv )
{

   using namespace std ;
   assert( cv.cauce_act != nullptr );

   if ( triangulos.size() == 0 || vertices.size() == 0 )
   {  cout << "advertencia: intentando dibujar malla vacía '" << leerNombre() << "'" << endl << flush ;
      return ;
   }

   // guardar el color previamente fijado
   const Tupla4f color_previo = leerFijarColVertsCauce( cv );
    

   // COMPLETAR: práctica 1: si el puntero 'array_verts' es nulo, crear el objeto ArrayVerts
    int num_valores_tupla = 3;
    
    // Creamos el array de vértices.
    //   * en el constructor se dan los datos de la tabla de coordenadas de vértices (tabla 'vertices')
    //   * después hay que invocar a 'fijarIndices', usando el formato y datos de la tabla de triángulos ('triangulos')
    //   * si las tablas 'col_ver', 'cc_tt_ver' o 'nor_ver' no están vacías, hay que invocar los métodos
    //     'fijarColores', 'fijarCoordText' y 'fijarNormales', como corresponda.
    

    if(array_verts == nullptr){
          array_verts = new ArrayVertices(GL_FLOAT, 3, vertices.size(), vertices.data());
       }

       array_verts->fijarIndices(GL_UNSIGNED_INT, triangulos.size()*3, triangulos.data());
    
    
    if (!col_ver.empty())
        array_verts->fijarColores(GL_FLOAT, num_valores_tupla, col_ver.data());
    if (!cc_tt_ver.empty())
        array_verts->fijarCoordText(GL_FLOAT, 2 ,cc_tt_ver.data());
    if (!nor_ver.empty())
        array_verts->fijarNormales(GL_FLOAT, nor_ver.data());
   

   // COMPLETAR: práctica 1: visualizar según el modo (en 'cv.modo_envio')
   //   ** inmediato begin/end       : usar método 'visualizarGL_MI_BVE' de 'ArrayVerts'
   //   ** inmediato con una llamada : usar método 'visualizarGL_MI_DAE' de 'ArrayVerts'
   //   ** diferido (con un VAO)     : usar método 'visualizarGL_MD_VAO' de 'ArrayVerts'
    
    if (cv.modo_envio == ModosEnvio::inmediato_begin_end)
        array_verts->visualizarGL_MI_BVE(GL_TRIANGLES);
    if (cv.modo_envio == ModosEnvio::inmediato_drawelements)
        array_verts->visualizarGL_MI_DAE(GL_TRIANGLES);
    if (cv.modo_envio == ModosEnvio::diferido_vao)
        array_verts->visualizarGL_MD_VAO(GL_TRIANGLES);

   // restaurar el color previamente fijado
   glColor4fv( color_previo );
}

// *****************************************************************************


// ****************************************************************************
// Clase 'MallaPLY'

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre( std::string("Malla leída del archivo '") + nombre_arch + "'" );

   // COMPLETAR: práctica 2: leer archivo PLY e inicializar la malla

   // Leemos del archivo PLY. 
   LeerPLY(nombre_arch, vertices, triangulos);

   // COMPLETAR: práctica 4: invocar  a 'calcularNormales' para el cálculo de normales
    calcularNormales();
}

// ****************************************************************************
// Clase 'Cubo

Cubo::Cubo()
:  MallaInd( "cubo 8 vértices" )
{
    ponerIdentificador(10);

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;



   triangulos =
    {  {0,1,3}, {0,3,2}, // X-
       {4,7,5}, {4,6,7}, // X+ (+4)

       {0,5,1}, {0,4,5}, // Y-
       {2,3,7}, {2,7,6}, // Y+ (+2)

       {0,6,4}, {0,2,6}, // Z-
       {1,5,7}, {1,7,3}  // Z+ (+1)
    } ;

    calcularNormales();
}
// -----------------------------------------------------------------------------------------------
// Clase Tetraedro

Tetraedro::Tetraedro()
:  MallaInd( "Tetraedro" )
{

   vertices =
      {  { +0.0, +1.0, +0.0 }, // 0
         { +0.0, +0.0, +1.0 }, // 1
         { +1.0, +0.0, +0.0 }, // 2
         { -1.0, +0.0, -1.0 }, // 3
      } ;


    // Numero de triangulos igual al numero de caras.
    // Los indices son los vertices.
   triangulos =
    {  {0,1,2}, {2,3,0}, // X-
       {0,1,3}, {1,2,3}, // X+ (+4)
    } ;
    
    // O bien toco los vértices de manera individual, o bien
    // llamo a poner color con una única tupla3f.
    col_ver =
    {
        {1, 1, 153.00/255.00},
        {128.00/255.00, 0, 128.00/255.00},
        {0, 128.00/255.00, 128.00/255.00},
        {1, 128.00/255.00, 128.00/255.00}
        
    };
    
    calcularNormales();

}

CuboColores::CuboColores()
:  MallaInd( "Cubo de colores 8 vértices" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;



   triangulos =
    {  {0,1,3}, {0,3,2}, // X-
       {4,7,5}, {4,6,7}, // X+ (+4)

       {0,5,1}, {0,4,5}, // Y-
       {2,3,7}, {2,7,6}, // Y+ (+2)

       {0,6,4}, {0,2,6}, // Z-
       {1,5,7}, {1,7,3}  // Z+ (+1)
    } ;

   col_ver =
   {  
      { 0.0, 0.0, 0.0 }, // 0
      { 0.0, 0.0, +1.0 }, // 1
      { 0.0, +1.0, 0.0 }, // 2
      { 0.0, +1.0, +1.0 }, // 3
      { +1.0, 0.0, 0.0 }, // 4
      { +1.0, 0.0, +1.0 }, // 5
      { +1.0, +1.0, 0.0 }, // 6
      { +1.0, +1.0, +1.0 }, // 7
      };
}

EstrellaZ::EstrellaZ(unsigned n)
: MallaInd("Estrella de examen de ayer")
{
    
    Tupla3f centro = {0.0, 0.0, 0.0};
    vertices.push_back(centro);
    
    // Hacemos esfera exterior
    Tupla3f vertice_exterior_inicial = {0.0, 0.5, 0.0};
    
    // Hacemos esfera interior
    Tupla3f vertice_interior_inicial = {0.0, 0.25, 0.0};
    // Vamos a rotar este vertice para colocarlo en la bisectriz.
    vertice_interior_inicial = rotarVerticeGrados(vertice_interior_inicial,'z', (2 * M_PI) /(2*n));
    
    // A estos dos vertices le aplicamos una rotacion de 360/n, i veces.
    for (int i = 0; i < n; ++i){ // Vamos a rotarlo n veces.
        float angulo = (2 * M_PI * i) / n;
        Tupla3f vertice_exterior_rotado = rotarVerticeGrados(vertice_exterior_inicial, 'z', angulo);
        vertices.push_back(vertice_exterior_rotado);
        
        Tupla3f vertice_interior_rotado = rotarVerticeGrados(vertice_interior_inicial, 'z', angulo );
        vertices.push_back(vertice_interior_rotado);
        
    }
    
    // Le damos color y reescalamos las tuplas al eje x e y 0.5
    
    for (int i = 0; i < vertices.size(); ++i){
        vertices[i] = {vertices[i][0] + 0.5 , vertices[i][1] + 0.5, vertices[i][2]};
        col_ver.push_back(vertices[i]);
    }
    col_ver[0] = {1,1,1};
    
    for (int i = 1; i < vertices.size(); ++i){
        
        if (i < vertices.size() - 1 ){
            triangulos.push_back({0, i, (i+1)}) ;
        }
        else
            triangulos.push_back({0, i, 1});
    } // Hacer modulo.
}

CasaX::CasaX()
: MallaInd("Casa X del examen de hoy")
{
    
    vertices =
       {  { 0.0, 0.0, 0.0 }, // 0
          { +1.0, 0.0, 0.0 }, // 1
          { 0.0, 0.0, 0.8 }, // 2
          { +1.0, 0.0, +0.8 }, // 3
          { 0.0, 0.7, 0.0 }, // 4
          { 1.0, 0.7, 0.0 }, // 5
          { 0.0, 0.7, +0.8}, // 6
          { +1.0, +0.7, +0.8 }, // 7
           {+1.0, +1.0, +0.4}, // 8
           {+0.0, +1.0, +0.4} // 9
       } ;
    
    triangulos =
     {  {0,1,4}, {4,1,5},
        {6,7,2}, {2,7,3},

        {6,0,4}, {0,2,6},
        {1,3,7}, {1,5,7},
         
         {8,7,5},{8,6,7},
         {8,5,4},{9,6,4},
         {9,6,5}, {9,4,5},
         {8,9,6},{8,9,4}
     } ;
    
    col_ver =
    {
        { 0.0, 0.0, 0.0 }, // 0
           { +1.0, 0.0, 0.0 }, // 1
           { 0.0, 0.0, 0.8 }, // 2
           { +1.0, 0.0, +0.8 }, // 3
           { 0.0, 1.0, 0.0 }, // 4
           { 1.0, 1.0, 0.0 }, // 5
           { 0.0, +1.0, +0.8}, // 6
           { +1.0, +1.0, +0.8 }, // 7
            {+1.0, +1.0, +0.4}, // 8
            {+0.0, +1.0, +0.4} // 9
        
    };
    

}

RejillaX::RejillaX(unsigned n, unsigned m){
    
    assert (n > 1 && m > 1);
    
    // Bucle para los vertices
    for (int i = 0; i < n; ++i){ // Metemos m vertices en Y
        for (int j = 0; j < m; ++j){ // Metemos n vertices en Z
            vertices.push_back({0, ((1/n) * i), ((1/m) * j)});
            col_ver.push_back({0, ((1/n) * i), ((1/m) * j)});
        }
        
    // Bucle para los triangulos
        
        
    }
    
    
}

CuadradoRejilla::CuadradoRejilla(){ // Sustitucion a la rejilla
    vertices = {
        {0,0,0},
        {0,0,1},
        {1,0,0},
        {1,0,1}
    };
    triangulos = {
        {0,1,2},{1,2,3}
    };
}

Cubo24::Cubo24()
{
    vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7

            { -1.0, -1.0, -1.0 }, // 8
         { -1.0, -1.0, +1.0 }, // 9
         { -1.0, +1.0, -1.0 }, // 10
         { -1.0, +1.0, +1.0 }, // 11
         { +1.0, -1.0, -1.0 }, // 12
         { +1.0, -1.0, +1.0 }, // 13
         { +1.0, +1.0, -1.0 }, // 14
         { +1.0, +1.0, +1.0 }, // 15

            { -1.0, -1.0, -1.0 }, // 16
         { -1.0, -1.0, +1.0 }, // 17
         { -1.0, +1.0, -1.0 }, // 18
         { -1.0, +1.0, +1.0 }, // 19
         { +1.0, -1.0, -1.0 }, // 20
         { +1.0, -1.0, +1.0 }, // 21
         { +1.0, +1.0, -1.0 }, // 22
         { +1.0, +1.0, +1.0 }  // 23
      } ;



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {8,13,9}, {8,12,13}, // Y-
         {10,11,15}, {10,15,14}, // Y+ (+2)

         {16,22,20}, {16,18,22}, // Z-
         {17,21,23}, {17,23,19}  // Z+ (+1)
      } ;

    cc_tt_ver = {
      {0.0, 1.0}, // 0
        {1.0, 1.0}, // 1
      {0.0, 0.0}, // 2
        {1.0, 0.0}, // 3
        {0.0, 1.0}, // 4
        {1.0, 1.0}, // 5
      {0.0, 0.0}, // 6
        {1.0, 0.0}, // 7
        {0.0, 0.0}, // 8
        {1.0, 0.0}, // 9
      {0.0, 1.0}, // 10
        {1.0, 1.0}, // 11
        {0.0, 1.0}, // 12
        {1.0, 1.0}, // 13
        {0.0, 0.0}, // 14
        {1.0, 0.0}, // 15
        {1.0, 1.0}, // 16
        {0.0, 1.0}, // 17
        {1.0, 0.0}, // 18
        {0.0, 0.0}, // 19
        {0.0, 1.0}, // 20
        {1.0, 1.0}, // 21
        {0.0, 0.0}, // 22
        {1.0, 0.0}, // 23
   };

    calcularNormales();

}
