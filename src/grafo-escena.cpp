// Nombre: Jacobo, Apellidos: Casado de Gracia, DNI/pasaporte: 77146828K (DDGG IG curso 20-21)
// *********************************************************************
// **
// ** Gestión de una grafo de escena (implementación)
// ** Copyright (C) 2016 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#include "ig-aux.h"
#include "matrices-tr.h"
#include "grafo-escena.h"
#include "malla-ind.h"
#include "malla-revol.h"
#include "materiales-luces.h"

using namespace std ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
   assert( pObjeto != NULL );
   tipo   = TipoEntNGE::objeto ;
   objeto = pObjeto ;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const Matriz4f & pMatriz )
{
   tipo    = TipoEntNGE::transformacion ;
   matriz  = new Matriz4f() ; // matriz en el heap, puntero propietario
   *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
   assert( pMaterial != NULL );
   tipo     = TipoEntNGE::material ;
   material = pMaterial ;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE()
{
   /**  no fnciona debido a que se hacen copias (duplicados) de punteros
   if ( tipo == TipoEntNGE::transformacion )
   {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
   }
   * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL( ContextoVis & cv )
{
   // COMPLETAR: práctica 3: recorrer las entradas y visualizar cada nodo.
   // Guardamos el modelview actual
    
    const Tupla4f color_previo = leerFijarColVertsCauce( cv );
    
    Material * material_previo = cv.iluminacion ? cv.material_act : nullptr;
    
    cv.cauce_act->pushMM();
    
    // Recorrer todas las entradas del array que hay en el nodo
    for (unsigned i = 0; i < entradas.size(); ++i){
        switch(entradas[i].tipo){
            case TipoEntNGE::objeto : //En el caso de que sea un objeto la entrada:
                entradas[i].objeto->visualizarGL(cv);
                break;
            case TipoEntNGE::transformacion : // En el caso de que sea una transformación la entrada:
                cv.cauce_act->compMM(*(entradas[i].matriz));
                break;
            case TipoEntNGE::material :
                if ( cv.iluminacion && !cv.modo_seleccion){
                    cv.material_act = entradas[i].material ; // registrar material
                    cv.material_act->activar(*(cv.cauce_act)); // activar material
                }
                break ;
        }
    }
    
    
    // Restaurar el color previamente fijado
    glColor4fv( color_previo );

    // Practica 4. Si hemos puesto un nuevo material, lo activamos.
    if (material_previo != nullptr){
            cv.material_act = material_previo;
            cv.material_act->activar(*(cv.cauce_act));
}
    cv.cauce_act->popMM();
}

// *****************************************************************************
// visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...
// ignora el colo o identificador del nodo, ignora las entradas de tipo material
// (se supone que el estado de OpenGL está fijado antes de esta llamada de alguna forma adecuada)




// -----------------------------------------------------------------------------

NodoGrafoEscena::NodoGrafoEscena()
{

}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica (de cualqiuer tipo de entrada)

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada )
{
   
    entradas.push_back(entrada); // Metemos la entrada en el vector de entradas.
    
    return (entradas.size() - 1); // Devolvemos el índice de las entradas.

}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D * pObjeto )
{
   return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const Matriz4f & pMatriz )
{
   return agregar( EntradaNGE( pMatriz ) );
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar( Material * pMaterial )
{
   return agregar( EntradaNGE( pMaterial ) );
}

// devuelve el puntero a la matriz en la i-ésima entrada
Matriz4f * NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{
   // COMPLETAR: práctica 3: devolver puntero la matriz en ese índice
    // Comprobamos que en esa entrada haya una matriz de transformación
    assert( indice < entradas.size() );
    assert (entradas[indice].tipo == TipoEntNGE::transformacion);
    assert( entradas[indice].matriz != nullptr );
    
    
   return entradas[indice].matriz; // Devolvemos la matriz en ese indice.


}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{

    // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
      //    en coordenadas de objeto (hay que hacerlo recursivamente)
      //   (si el centro ya ha sido calculado, no volver a hacerlo)
      if ( !centro_calculado ) {
         Matriz4f mat = MAT_Ident();
         Tupla3f suma={0,0,0};
         float num_centros=0;

         for (unsigned i=0; i<entradas.size(); i++) {
            if (entradas[i].tipo == TipoEntNGE::objeto) {
               entradas[i].objeto->calcularCentroOC();
               suma = suma + (mat * entradas[i].objeto->leerCentroOC() );
               num_centros++;
            } else if ( entradas[i].tipo == TipoEntNGE::transformacion ) {
               mat = mat*(*entradas[i].matriz);
            }
         }

         Tupla3f centro = suma/num_centros;
          //std::cout << leerNombre() + "\n";
          //std::cout << "CENTRO : (" << centro(0) << "," <<centro(1) << "," << centro(2) << ")\n";

         ponerCentroOC( centro );
         centro_calculado = true;
      }

}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto
(
   const int         ident_busc, // identificador a buscar
   const Matriz4f &  mmodelado,  // matriz de modelado
   Objeto3D       ** objeto,     // (salida) puntero al puntero al objeto
   Tupla3f &         centro_wc   // (salida) centro del objeto en coordenadas del mundo
)
{
   assert( 0 < ident_busc );

    // 1. calcula el centro del objeto, (solo la primera vez)
       calcularCentroOC();

       // 2. si el identificador del nodo es el que se busca, ya está (terminar)
       if(leerIdentificador() == ident_busc){
          centro_wc = mmodelado*leerCentroOC();
          if ( objeto == nullptr ) {
            cout << "\t Identificador encontrado con puntero asociado nulo" << endl;
          }
          *objeto = this ;
          return true;
       }

       // 3. El nodo no es el buscado: buscar recursivamente en los hijos
       //    (si alguna llamada para un sub-árbol lo encuentra, terminar y devolver 'true')
       Matriz4f mat = mmodelado;

       bool encontrado = false;
       for (unsigned i=0; i<entradas.size() && !encontrado; i++) {
          if ( entradas[i].tipo == TipoEntNGE::objeto)
             encontrado =  entradas[i].objeto->buscarObjeto(ident_busc, mat, objeto, centro_wc );
          else if (entradas[i].tipo == TipoEntNGE::transformacion)
             mat = mat * (*entradas[i].matriz) ;
       }

       // ni este nodo ni ningún hijo es el buscado: terminar
       return encontrado ;
}

GrafoCubos::GrafoCubos(){
    agregar(new CaraAbajo());
    agregar(new CaraAdelante());
    agregar(new CaraIzquierda());
    agregar(MAT_Rotacion(180,{0,0,1}));
    agregar(new CaraAbajo());
    agregar(new CaraAdelante());
    agregar(new CaraIzquierda());
   
}


CaraAbajo::CaraAbajo(){
    agregar(new CuboMovimiento(0));
    agregar(MAT_Traslacion(-0.5,-0.5,-0.5));
    //agregar (new CuadradoRejilla());
}

CaraAdelante::CaraAdelante(){
    agregar(MAT_Rotacion(-90,{1,0,0}));
    agregar (new CaraAbajo());
}

CaraIzquierda::CaraIzquierda(){
    agregar(MAT_Rotacion(-90,{0,0,1}));
    agregar (new CaraAbajo());
}

CuboMovimiento::CuboMovimiento(float angulo){
    agregar(MAT_Rotacion(angulo,{0,1,0}));
    agregar(MAT_Traslacion(0.0,-0.75,0.0));
    agregar(MAT_Escalado(0.25,0.25,0.25));
            agregar(new Cubo());
}

LataPeones::LataPeones(){
    // añadir el objeto 'MallaRevolPLY a la lista de objetos de esta escena:
    agregar (new PeonMadera());
    agregar (new PeonBlanco());
    agregar (new PeonNegro());
    agregar (new LataCocaCola());

}

LataCocaCola::LataCocaCola(){
    ponerNombre("Lata de Coca Cola");
    ponerIdentificador(1);
    agregar(MAT_Traslacion(2.0,0.0,-2.0));
    Textura * textura = new Textura("../recursos/imgs/lata-coke.jpg");
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

PeonMadera::PeonMadera(){
    ponerNombre("Peón de Madera");
    ponerIdentificador(2);
    MallaRevolPLY * peon = new MallaRevolPLY("../recursos/plys/peon.ply", 10);
    Textura * textura = new Textura("../recursos/imgs/text-madera.jpg");
    agregar(new Material(textura,0.5,0.0,0.2,5.00));
    agregar(peon);
}

PeonBlanco::PeonBlanco(){
    ponerNombre("Peón Blanco");
    ponerIdentificador(3);
    MallaRevolPLY * peon = new MallaRevolPLY("../recursos/plys/peon.ply", 10);
    agregar(MAT_Traslacion(-2.0,0.0,-2.0));
    const Tupla3f color = {255.0/255.0, 255.0/255.0, 255.0/255.0};
    ponerColor(color);
    agregar(new Material(0.0,2.0,0.0,0.00));
    agregar(peon);
}

PeonNegro::PeonNegro(){
    ponerNombre("Peón Negro");
    ponerIdentificador(4);
    MallaRevolPLY * peon = new MallaRevolPLY("../recursos/plys/peon.ply", 10);
    agregar(MAT_Traslacion(2.0,0.0,2.0));
    agregar(new Material(0.2,0.1,1.0,1.0));
    const Tupla3f color = {0.0/255.0, 0.0/255.0, 0.0/255.0};
    ponerColor(color);
    agregar(peon);
}

NodoCubo24::NodoCubo24(){
    Textura * textura = new Textura("../recursos/imgs/window-icon.jpg");
    agregar(new Material(textura,0.5,0.0,0.2,5.00));
    agregar (new Cubo24());
}

    
// La malla disco al ponerla aquí como malla indexada (declarada en grafoescena.h heredando de MallaInd) me daba error al compilar, y para seguir con el examen, la he metido en la clase MallaInd, si no no me daba tiempo.

  NodoDiscoP4::NodoDiscoP4()
  {
    ponerNombre("Nodo práctica 4, examen 27 enero");
    Textura * textura = new Textura("../recursos/imgs/cuadricula.jpg");
    agregar(new Material(textura,0.5,0.0,0.2,5.00));
    agregar( new MallaDiscoP4() );
  }


// Práctica 5
GrafoEsferasP5::GrafoEsferasP5()
  {
      const unsigned
          n_filas_esferas    = 8,
          n_esferas_x_fila   = 5 ;

      agregar( MAT_Escalado( 2.5/n_esferas_x_fila, 2.5/n_esferas_x_fila, 2.5/n_esferas_x_fila ));

    // i es, por tanto, el contador de la fila.
      for( unsigned i = 0 ; i < n_filas_esferas ; i++ )
      {
          NodoGrafoEscena * fila_esferas = new NodoGrafoEscena() ;
          fila_esferas->agregar( MAT_Traslacion( 3.0, 0.0, 0.0 ));
          
          // j es el contador de la esfera DENTRO DE CADA FILA.
          for( unsigned j = 0 ; j < n_esferas_x_fila ; j++ )
          {
              // He tenido que añadir dos parametros a la esfera porque no tenemos el constructor sin parámetros de esfera
              MallaInd * esfera = new Esfera(30,30) ;
              fila_esferas->agregar( MAT_Traslacion( 2.5, 0.0, 0.0 ));
            // No se si hace falta poner ID, pero lo pongo por si acaso.
              // SI QUISIESEMOS HACER QUE ESTA SALIDA SEA EN LA FUNCIÓN SELECCIÓN, deberíamos de usar este identificador.
              esfera->ponerIdentificador((i+1)*(j+1));
              string nombreEsfera = "Se ha seleccionado la esfera " + to_string(j+1) + " de la fila número " + to_string(i+1) + ".";
              esfera->ponerNombre (nombreEsfera);
              fila_esferas->agregar( esfera );
          }
          agregar( fila_esferas );
          agregar( MAT_Rotacion( 360.0/n_filas_esferas, { 0.0, 1.0, 0.0 }));
      }
  }
  
