// Nombre: María,  Apellidos: Cribillés Pérez, Titulación: GIM
// email: mariacribilles@correo.ugr.es, DNI o pasaporte: 75573410X

// *********************************************************************
// **
// ** Asignatura: INFORMÁTICA GRÁFICA
// ** 
// ** Gestión de grafos de escena (implementación). Clase 'NodoGrafoEscena' y relacionadas.
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Implementación de: 
// **     + Clase 'NodoGrafoEscena' (derivada de 'Objeto3D')
// **     + Clase 'EntradaNGE' (una entrada de un nodo del grafo de escena)
// **     + Tipo enumerado 'TipoEntNGE' (tipo de entradas del nodo del grafo de escena)
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
#include "grafo-escena.h"
#include "aplicacion-ig.h"
#include "seleccion.h"   // para 'ColorDesdeIdent' 


using namespace glm;


// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
   assert( pObjeto != nullptr );
   tipo   = TipoEntNGE::objeto ;
   objeto = pObjeto ;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const glm::mat4 & pMatriz )
{
   tipo    = TipoEntNGE::transformacion ;
   matriz  = new glm::mat4() ; // matriz en el heap, puntero propietario
   *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
   assert( pMaterial != nullptr );
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

NodoGrafoEscena::NodoGrafoEscena()
{

}

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL(  )
{
   using namespace std ;
   assert( aplicacionIG != nullptr );

   //cout << "Entra " << __FUNCTION__ << ": " << leerNombre() << endl ;

    // comprobar que hay un cauce y una pila de materiales y recuperarlos.
   Cauce *          cauce           = aplicacionIG->cauce ;           assert( cauce != nullptr );
   PilaMateriales * pila_materiales = aplicacionIG->pila_materiales ; assert( pila_materiales != nullptr );



   // COMPLETAR: práctica 3: implementar la visualización del nodo
   //
   // Se deben de recorrer las entradas y llamar recursivamente de visualizarGL, pero 
   // teniendo en cuenta que, al igual que el método visualizarGL de las mallas indexadas,
   // si el nodo tiene un color, debemos de cambiar el color del cauce (y hacer push/pop). 
   // Además, hay que hacer push/pop de la pila de modelado. 
   // Así que debemos de dar estos pasos:
   //
   // 1. Si el objeto tiene un color asignado (se comprueba con 'tieneColor')
   //     - hacer push del color actual del cauce (con 'pushColor') y después
   //     - fijar el color en el cauce (con 'fijarColor'), usando el color del objeto (se lee con 'leerColor()')
   if (tieneColor()) {
      cauce->pushColor();
      cauce->fijarColor(leerColor());
   }

   if (aplicacionIG->iluminacion) {
      aplicacionIG->pila_materiales->push();
   }


   // 2. Guardar copia de la matriz de modelado (con 'pushMM'), 
   cauce->pushMM();

   // 3. Para cada entrada del vector de entradas:
   //     - si la entrada es de tipo objeto: llamar recursivamente a 'visualizarGL'
   //     - si la entrada es de tipo transformación: componer la matriz (con 'compMM')
   for (int i = 0; i < entradas.size(); i++) {
      if (entradas[i].tipo == TipoEntNGE::objeto)
         entradas[i].objeto->visualizarGL();
      else if (entradas[i].tipo == TipoEntNGE::transformacion)
         cauce->compMM(*entradas[i].matriz);
      else if (entradas[i].tipo == TipoEntNGE::material)
         aplicacionIG->pila_materiales->activar(entradas[i].material);
   }

   // 4. Restaurar la copia guardada de la matriz de modelado (con 'popMM')
   cauce->popMM();

   // 5. Si el objeto tiene color asignado:
   //     - restaurar el color original a la entrada (con 'popColor')
   if (tieneColor())
      cauce->popColor();


   // COMPLETAR: práctica 4: añadir gestión de los materiales cuando la iluminación está activada    
   //
   // Si 'apl->iluminacion' es 'true', se deben de gestionar los materiales:
   //
   //   1. al inicio, hacer 'push' de la pila de materiales (guarda material actual en la pila)
   //   2. si una entrada es de tipo material, activarlo usando a pila de materiales
   //   3. al finalizar, hacer 'pop' de la pila de materiales (restaura el material activo al inicio)

   // ......

   if (aplicacionIG->iluminacion) {
      aplicacionIG->pila_materiales->pop();
   }


}

// *****************************************************************************
// visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...

void NodoGrafoEscena::visualizarGeomGL(  )
{
   using namespace std ;
   // comprobar que hay un cauce 
   assert( aplicacionIG != nullptr );
   Cauce * cauce = aplicacionIG->cauce; assert( cauce != nullptr );
  
   // COMPLETAR: práctica 3: implementar la visualización del nodo (ignorando colores)
   //
   // Este método hace un recorrido de las entradas del nodo, parecido a 'visualizarGL', pero más simple,
   // Se dan estos pasos:
   //
   // 1. Guardar copia de la matriz de modelado (con 'pushMM'),
   cauce->pushMM();

   // 2. Para cada entrada del vector de entradas:
   //         - Si la entrada es de tipo objeto: llamar recursivamente a 'visualizarGeomGL'.
   //         - Si la entrada es de tipo transformación: componer la matriz (con 'compMM').
   for(int i=0;i<entradas.size();i++){
      if(entradas[i].tipo == TipoEntNGE::objeto){
         entradas[i].objeto->visualizarGeomGL();
      }else if(entradas[i].tipo == TipoEntNGE::transformacion){
         cauce->compMM(*(entradas[i].matriz));
      }
   }

   //   3. Restaurar la copia guardada de la matriz de modelado (con 'popMM')
   cauce->popMM();

}

// -----------------------------------------------------------------------------
// Visualizar las normales de los objetos del nodo

void NodoGrafoEscena::visualizarNormalesGL(  )
{
   using namespace std ;

   // comprobar que hay un cauce 
   assert( aplicacionIG != nullptr );
   Cauce * cauce = aplicacionIG->cauce; assert( cauce != nullptr );
  

   // COMPLETAR: práctica 4: visualizar las normales del nodo del grafo de escena
   //
   // Este método hace un recorrido de las entradas del nodo, parecido a 'visualizarGL', teniendo 
   // en cuenta estos puntos:
   //
   // - usar push/pop de la matriz de modelado al inicio/fin (al igual que en visualizatGL)
   cauce->pushMM();

   // - recorrer las entradas, llamando recursivamente a 'visualizarNormalesGL' en los nodos u objetos hijos
   for (int i = 0; i < entradas.size(); i++) {
      if (entradas[i].tipo == TipoEntNGE::objeto)
         entradas[i].objeto->visualizarNormalesGL();
      else if (entradas[i].tipo == TipoEntNGE::transformacion)
         cauce->compMM(*entradas[i].matriz);
   }
   // - ignorar el color o identificador del nodo (se supone que el color ya está prefijado antes de la llamada)
   // - ignorar las entradas de tipo material, y la gestión de materiales (se usa sin iluminación)
   cauce->popMM();
   // .......
   
   


}

// -----------------------------------------------------------------------------
// visualizar el objeto en 'modo seleccion', es decir, sin iluminación y con los colores 
// basados en los identificadores de los objetos
void NodoGrafoEscena::visualizarModoSeleccionGL()
{
   using namespace std ;
   assert( aplicacionIG != nullptr );
   Cauce * cauce = aplicacionIG->cauce ; assert( cauce != nullptr );

   // COMPLETAR: práctica 5: visualizar este nodo en modo selección.
   //
   // Se debe escribir código para dar estos pasos:
   // 
   // 2. Leer identificador (con 'leerIdentificador'), si el identificador no es -1 
   //      + Guardar una copia del color actual del cauce (con 'pushColor')
   //      + Fijar el color del cauce de acuerdo al identificador, (usar 'ColorDesdeIdent'). 
   // 3. Guardar una copia de la matriz de modelado (con 'pushMM')
   // 4. Recorrer la lista de nodos y procesar las entradas transformación o subobjeto:
   //      + Para las entradas subobjeto, invocar recursivamente a 'visualizarModoSeleccionGL'
   //      + Para las entradas transformación, componer la matriz (con 'compMM')
   // 5. Restaurar la matriz de modelado original (con 'popMM')   
   // 6. Si el identificador no es -1, restaurar el color previo del cauce (con 'popColor')
   //
   // ........
   int ident = leerIdentificador();
   if (ident != -1) {
      cauce->pushColor();
      cauce->fijarColor(ColorDesdeIdent(ident));
   }

   cauce->pushMM();

   for (int i = 0; i < entradas.size(); i++) {
      if (entradas[i].tipo == TipoEntNGE::objeto)
         entradas[i].objeto->visualizarModoSeleccionGL();
      else if (entradas[i].tipo == TipoEntNGE::transformacion)
         cauce->compMM(*entradas[i].matriz);
   }

   cauce->popMM();

   if (ident != -1)
      cauce->popColor();


}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica (de cualqiuer tipo de entrada)

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada )
{
   // COMPLETAR: práctica 3: agregar la entrada al nodo, devolver índice de la entrada agregada
   entradas.push_back(entrada);
   return entradas.size()-1;

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

unsigned NodoGrafoEscena::agregar( const glm::mat4 & pMatriz )
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
glm::mat4 * NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{
   // COMPLETAR: práctica 3: leer un puntero a una matriz en una entrada de un nodo
   //
   // Devolver el puntero a la matriz en la entrada indicada por 'indice'. 
   // Debe de dar error y abortar si: 
   //   - el índice está fuera de rango 
   //   - la entrada no es de tipo transformación
   //   - el puntero a la matriz es nulo 
   //
   // Sustituir 'return nullptr' por lo que corresponda.
   //
   if(indice >= entradas.size()){
      std::cerr << "ERR: indice fuera de rango" << std::endl;
      exit(1);
   }else if(entradas[indice].tipo != TipoEntNGE::transformacion){
      std::cerr << "ERR: la entrada no es de tipo transformacion" << std::endl;
      exit(1);
   }else if(entradas[indice].matriz == nullptr){
      std::cerr << "ERR: el puntero a la matriz es nulo" << std::endl;
      exit(1);
   }

   return entradas[indice].matriz;


}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{
   using namespace std ;
   using namespace glm ;

   // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
   //    en coordenadas de objeto (hay que hacerlo recursivamente)
   //   (si el centro ya ha sido calculado, no volver a hacerlo)
   // ........

    if (centro_calculado)
      return;
   
   int contadorCentros = 0;
   mat4 matrizModelado(1.0f);
   vec3 centroAcumulado = vec3(0.0, 0.0, 0.0);

   for (unsigned int i = 0; i < entradas.size(); i++){
      if (entradas[i].tipo == TipoEntNGE::transformacion){
         matrizModelado = matrizModelado * (*entradas[i].matriz);
      }
      else if (entradas[i].tipo == TipoEntNGE::objeto){
         entradas[i].objeto->calcularCentroOC();
         centroAcumulado = centroAcumulado + vec3(matrizModelado * vec4(entradas[i].objeto->leerCentroOC(),1.0f));
         contadorCentros++;
      }
   }

   for (int i = 0; i < 3; i++) {
      centroAcumulado[i] /= contadorCentros;
   }
   ponerCentroOC( centroAcumulado);
   centro_calculado = true;

}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto
(
   const int          ident_busc, // identificador a buscar
   const glm::mat4 &  mmodelado,  // matriz de modelado
   Objeto3D       **  objeto,     // (salida) puntero al puntero al objeto
   glm::vec3 &        centro_wc   // (salida) centro del objeto en coordenadas del mundo
)
{
   using namespace std ;
   using namespace glm ;
   
   assert( 0 < ident_busc );

   // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
   // Se deben de dar estos pasos:

   // 1. calcula el centro del objeto, (solo la primera vez)
   // ........
   calcularCentroOC();

   // 2. si el identificador del nodo es el que se busca, ya está (terminar)
   // ........
    if (ident_busc == leerIdentificador()) {
      *objeto = this;
      centro_wc = leerCentroOC();
      return true;
   }


   // 3. El nodo no es el buscado: buscar recursivamente en los hijos
   //    (si alguna llamada para un sub-árbol lo encuentra, terminar y devolver 'true')
   // ........
   mat4 matrizmod = mmodelado;

   for(int i=0; i<entradas.size(); i++){
        if(entradas[i].tipo == TipoEntNGE::objeto){
            if(entradas[i].objeto->buscarObjeto(ident_busc, matrizmod, objeto, centro_wc)) return true;
        }
        else if(entradas[i].tipo == TipoEntNGE::transformacion) matrizmod = matrizmod*(*entradas[i].matriz);
   }


   // ni este nodo ni ningún hijo es el buscado: terminar
   return false ;
}

//GrafoEstrellaX--------------------------------------------------------------

GrafoEstrellaX::GrafoEstrellaX(unsigned n)
{
   
   //estrella plana de al practica 1
   
   //antes la estrella el centro estaba en { 0.5, 0.5, 0.0 }, luego la tenemos que trasladar al (0,0,0)
   unsigned ind_rot_estrella= agregar(rotate(float(M_PI), vec3(1.0,0.0,0.0)));
   agregar(rotate(float(M_PI/2.0), vec3(0.0,1.0,0.0)));


   //Conos
   NodoGrafoEscena *cono_punta = new NodoGrafoEscena();
   // cono_punta->agregar(scale(vec3((0.14, 0.15, 0.14))));
   float angulo = radians(360.0f/n);
   //cono_punta->agregar(translate(vec3(cos(angulo*i)*0.5,sin(angulo*i)*0.5,0.0))); 
   
   cono_punta->agregar(translate(vec3(1.3,0.0,0.0)));
   cono_punta->agregar(rotate(radians(-90.0f),(vec3(0.0,0.0,1.0))));
   cono_punta->agregar(scale(vec3(0.14/2.0,0.15/3.0,0.14/2.0)));
   cono_punta->agregar(new Cono(20,20));
   agregar(cono_punta);
   

   for (int i=0; i<n; i++){
      agregar(rotate(angulo, vec3{ 0.0,0.0,1.0 }));
      agregar(cono_punta);
   }      

   //antes la distancia del centro a los vertices era de 1, ahora es de 1.3
   agregar(scale(vec3(1.3/0.5, 1.3/0.5, 1.3/0.5)));
   agregar(translate(vec3(-0.5,-0.5,0.0)));
   agregar(new EstrellaZ(n));
   
   //estrellaX->ponerColor({245.0,245.0,220.0});



   pm_rot_estrella = leerPtrMatriz(ind_rot_estrella);
}

unsigned GrafoEstrellaX::leerNumParametros() const {
    return 1;
}

void GrafoEstrellaX::actualizarEstadoParametro(const unsigned iParam, const float t_sec) {
   
   //*pm_rot_estrella = rotate(float( M_PI  * sin( 2*M_PI *2.5* t_sec / 10.0)), vec3(0.0, 0.0, 1.0));
   alpha = 2*M_PI*2.5*t_sec;

   *pm_rot_estrella = (rotate(radians(alpha), vec3{ 1.0,0.0,0.0 }));

}


//GrafoCubos--------------------------------------------------------------

GrafoCubos::GrafoCubos()
{
   NodoGrafoEscena *cubo = new NodoGrafoEscena();
   unsigned ind_rot_cubo=cubo->agregar(rotate(float(0.0), vec3({1.0,0.0,0.0})));

  
   cubo->agregar(translate(vec3(0.5+0.7/2,0.0,0.0)));
   cubo->agregar(scale(vec3(0.7/2,0.5/2,0.5/2)));
   cubo->ponerColor({0.5,1.0,0.0});
   cubo->agregar(new Cubo());
   agregar(cubo);

   NodoGrafoEscena *malla = new NodoGrafoEscena();
  
   malla->agregar(translate(vec3(0.5,-0.5,-0.5)));
   malla->agregar(rotate(float(M_PI/2), vec3(0.0,0.0,1.0)));
   malla->agregar(new RejillaY(3,3));
   agregar(malla);

   
   for (int i=0; i<3; i++){
      agregar(rotate(radians(90.0f),vec3{ 0.0,0.0,1.0 }) );
      agregar(malla);
      agregar(cubo);
   }

   agregar(rotate(radians(90.0f),vec3{ 0.0,1.0,0.0 }) );
   agregar(malla);
   agregar(cubo);

   agregar(rotate(radians(180.0f),vec3{ 0.0,1.0,0.0 }) );
   agregar(malla);
   agregar(cubo);
   
   
   

   pm_rot_cubo= cubo->leerPtrMatriz(ind_rot_cubo);
}

unsigned GrafoCubos::leerNumParametros() const {
    return 1;
}

void GrafoCubos::actualizarEstadoParametro(const unsigned iParam, const float t_sec) {
   
   

   *pm_rot_cubo= rotate(radians(alpha), vec3{ 1.0,0.0,0.0 });


}

// -------------NodoCubo24--------------------------------------------------------
NodoCubo24::NodoCubo24()
{
   //identificador unico
   unsigned identCubo = 0;

   Textura* logo_ugr = new Textura("window-icon.jpg"); //definimos la textura que va a ser logo ugr
   Material* materialCubo = new Material(logo_ugr, 0.75, 0.65, 0.25, 50.0);

   Cubo24* cubo_ugr = new Cubo24();
   cubo_ugr->ponerNombre("Cubo 24");
   cubo_ugr->ponerIdentificador(identCubo);

   agregar(materialCubo);
   agregar(cubo_ugr);
}


// -------------NodoMiEjercicio--------------------------------------------------------
// NodoMiEjercicio::NodoMiEjercicio()
// {
//    //identificador unico
//    //unsigned identPiramide = 0;

//    Textura* rayas = new Textura("textura-examen.jpg"); //definimos la textura que va a ser logo ugr
//    Material* materialPiramide = new Material(rayas, 0.75, 0.65, 0.25, 50.0);

//    Piramide* piramide_rayada= new Piramide();
//    //piramide_rayada->ponerNombre("Piramide rayada");
//    //piramide_rayada->ponerIdentificador(identPiramide);

//    agregar(materialPiramide);
//    agregar(piramide_rayada);

// }

// // -------------NodoMiEjercicioHorizontal--------------------------------------------------------
// NodoMiEjercicioHorizontal::NodoMiEjercicioHorizontal()
// {
//    //identificador unico
//    //unsigned identPiramide = 0;

//    TexturaTriangulo* rayas = new TexturaTriangulo("textura-examen.jpg"); //definimos la textura que va a ser logo ugr
//    Material* materialPiramide = new Material(rayas, 0.75, 0.65, 0.25, 50.0);

//    Piramide* piramide_rayada= new Piramide();
//    //piramide_rayada->ponerNombre("Piramide rayada");
//    //piramide_rayada->ponerIdentificador(identPiramide);

//    agregar(materialPiramide);
//    agregar(piramide_rayada);

// }

// // -------------NodoMiEjercicio--------------------------------------------------------
// NodoMiEjercicio2::NodoMiEjercicio2()
// {
//    //identificador unico
//    //unsigned identPiramide = 0;

//    Textura* tejado = new Textura("ventanas-tejado-2048x1024.jpg");
//    Material* materialCasa = new Material(tejado, 0.75, 0.65, 0.25, 50.0);

//    MallaMiEjercicio2* casa_tejado= new MallaMiEjercicio2();
//    //piramide_rayada->ponerNombre("Piramide rayada");
//    //piramide_rayada->ponerIdentificador(identPiramide);

//    agregar(materialCasa);
//    agregar(casa_tejado);

// }

// MiCasa::MiCasa(unsigned i){
//    pos = i;
//    pm_rot = leerPtrMatriz(agregar(glm::translate(glm::vec3(0.0))));
//    agregar(new NodoP4());
// }

// bool MiCasa::cuandoClick(const glm::vec3 & centro_wc) {
//     float angulo = 1.0*M_PI/6;
//     *pm_rot *= rotate(angulo,glm::vec3(0.0,1.0,0.0));
//    std::cout << "Has hecho click en la casa número " << pos << std::endl;

//    return true;
// }

// NodoUrbaP5::NodoUrbaP5(int n){

//     NodoGrafoEscena *fila_casas = new NodoGrafoEscena();
//     for(int j=0; j<n; j++){
//         MiCasa *casa = new MiCasa(j);
//         casa->ponerIdentificador(j);
//         fila_casas->agregar(glm::translate(glm::vec3(2.0,0.0,0.0)));
//         fila_casas->agregar(casa);
//     }
//     agregar(fila_casas);
// }

// ****************************************************************************
// Clase MiEsferaE1

// MiCasa_mia::MiCasa_mia(unsigned i)
// {
//    pos = 0;
//    pm_rot = leerPtrMatriz(agregar(rotate((float) 0.0,glm::vec3(1.0,0.0,0.0))));
//    agregar(new NodoMiEjercicio2());
// }

// bool MiCasa_mia::cuandoClick(const glm::vec3 & centro_wc) {
//    //cada vez que pincho en una casa se gira 45 grados
//    cout<<"he pinchado en la casa de pos: "<<pos<<endl; 
//    float angulo=pos*M_PI/8;
//    *pm_rot=(rotate(angulo , vec3(0.0, 1.0, 0.0)));
//    pos+=1%8; 
//    return true; 
// }

// NodoUrbaP5_mio::NodoUrbaP5_mio(int n){
// const unsigned
//       n_filas_esferas= 1,
//       n_esferas_x_fila= 5 ;
//    const float
//       e = 0.4/n_esferas_x_fila ;
   
//    //agregar(scale(vec3(e,e,e)));

//    for( unsigned i = 0 ; i < n_filas_esferas ; i++ )
//    {
//       NodoGrafoEscena * fila_esferas = new NodoGrafoEscena() ;
//       for( unsigned j = 0 ; j < n_esferas_x_fila ; j++ )
//       {
//          MiCasa_mia * micasa = new MiCasa_mia(5) ;
//          micasa->ponerIdentificador(i*n_esferas_x_fila+j+1); 
//          fila_esferas->agregar(translate(vec3(1.0, 0.0, 0.0)));
//          fila_esferas->agregar( micasa );
         
//       }
//       agregar( fila_esferas );
//       agregar(translate(vec3(0.0, 0.0, 5.0)));
//    }
// }

// -------------NodoDiscoP4--------------------------------------------------------
NodoDiscoP4::NodoDiscoP4()
{
   ponerNombre("Nodo ejercicio adicional pr4");
   //Material* material_tablero_ajedrez = new Material(new TexturaXY("cuadricula.jpg"), 0.75, 0.65, 0.25, 50.0);
   //añadiendo en MallaDiscoP4 en malla-ind.cpp cc_tt_ver.push_back({0,0}); ya me funciona
   
   //sin utilizar texturaXY: 
   Material* material_tablero_ajedrez = new Material(new Textura("cuadricula.jpg"), 0.75, 0.65, 0.25, 50.0);
   
   agregar(material_tablero_ajedrez);
   agregar(new MallaDiscoP4);
}

// -------------Beethoven--------------------------------------------------------
Beethoven::Beethoven(bool horizontal)
{

   if(horizontal){
      ponerNombre("Ejercicio 3 adicional: beethoven de madera HORIZONTAL");
      
      TexturaBeethoven* textura_madera = new TexturaBeethoven("text-madera.jpg", horizontal);
      Material* material = new Material(textura_madera, 0.2, 0.65, 0.1, 10.0);
      agregar(material);
      agregar( new MallaPLY("../plys/beethoven.ply"));
   }else{
      ponerNombre("Ejercicio 3 adicional: beethoven de madera VERTICAL");
      TexturaBeethoven* textura_madera = new TexturaBeethoven("text-madera.jpg", horizontal);

      Material* material = new Material(textura_madera, 0.2, 0.65, 0.1, 10.0);
      agregar(material);
      
      agregar( new MallaPLY("../plys/beethoven.ply"));
      
   }


  
   
   /*
   
   ka (Componente ambiental): Este factor determina cuánto de la luz ambiental afecta al material. Un valor demasiado bajo hará que el objeto aparezca oscuro en ausencia de iluminación directa.

   kd (Componente difusa): Controla la intensidad de la luz difusa. Esto afecta cómo se perciben los colores de la textura bajo la iluminación.

   ks (Componente especular): Define cuán brillante o reflectante es el objeto. Si este valor es demasiado alto, puede causar que las vetas de madera no se distingan bien, ya que las reflexiones especulares pueden sobrescribir los detalles de la textura.

   shininess (Exponente especular): Controla el tamaño de los reflejos especulares. Valores altos generan reflejos pequeños y concentrados (efecto brillante), mientras que valores bajos producen reflejos más suaves y dispersos.
*/
   
   
   
}

// -------------Beethoven--------------------------------------------------------
BeethovenDiagonal::BeethovenDiagonal(bool horizontal)
{

   if(horizontal){
      ponerNombre("Ejercicio 3 adicional: beethoven de madera HORIZONTAL");
      
      TexturaBeethovenDiagonal* textura_madera = new TexturaBeethovenDiagonal("text-madera.jpg", horizontal);
      Material* material = new Material(textura_madera, 0.2, 0.65, 0.1, 10.0);
      agregar(material);
      agregar( new MallaPLY("../plys/beethoven.ply"));
   }else{
      ponerNombre("Ejercicio 3 adicional: beethoven de madera VERTICAL");
      TexturaBeethovenDiagonal* textura_madera = new TexturaBeethovenDiagonal("text-madera.jpg", horizontal);

      Material* material = new Material(textura_madera, 0.2, 0.65, 0.1, 10.0);
      agregar(material);
      
      agregar( new MallaPLY("../plys/beethoven.ply"));
      
   }


  
   
   /*
   
   ka (Componente ambiental): Este factor determina cuánto de la luz ambiental afecta al material. Un valor demasiado bajo hará que el objeto aparezca oscuro en ausencia de iluminación directa.

   kd (Componente difusa): Controla la intensidad de la luz difusa. Esto afecta cómo se perciben los colores de la textura bajo la iluminación.

   ks (Componente especular): Define cuán brillante o reflectante es el objeto. Si este valor es demasiado alto, puede causar que las vetas de madera no se distingan bien, ya que las reflexiones especulares pueden sobrescribir los detalles de la textura.

   shininess (Exponente especular): Controla el tamaño de los reflejos especulares. Valores altos generan reflejos pequeños y concentrados (efecto brillante), mientras que valores bajos producen reflejos más suaves y dispersos.
*/
   
   
   
}

// ****************************************************************************
// Clase MiEsferaE1

MiEsferaE1::MiEsferaE1(unsigned i, unsigned j)
{
   fila = i;
   col = j;

   agregar(new Esfera(20, 20));
}

bool MiEsferaE1::cuandoClick(const glm::vec3 & centro_wc) {
   std::cout << "Se ha seleccionado la esfera número " << col+1 << " de la fila número " << fila+1 << std::endl;
   return true; 
}


// -------------GrafoEsferaP5--------------------------------------------------------
GrafoEsferaP5::GrafoEsferaP5()
{
   const unsigned
      n_filas_esferas= 8,
      n_esferas_x_fila= 5 ;
   const float
      e = 0.4/n_esferas_x_fila ;
   
   agregar(scale(vec3(e,e,e)));

   for( unsigned i = 0 ; i < n_filas_esferas ; i++ )
   {
      NodoGrafoEscena * fila_esferas = new NodoGrafoEscena() ;
      for( unsigned j = 0 ; j < n_esferas_x_fila ; j++ )
      {
         // cout<<"ha llegado ";
         MiEsferaE1 * esfera = new MiEsferaE1(i,j) ;
         esfera->ponerIdentificador(i*n_esferas_x_fila+j+1);
         fila_esferas->agregar(translate(vec3(2.2, 0.0, 0.0)));
         fila_esferas->agregar( esfera );
      }
      agregar( fila_esferas );
      agregar(translate(vec3(0.0, 0.0, 5.0)));
   }
   
}

GrafoEsferasP5_2::GrafoEsferasP5_2()
{
   const unsigned
      n_filas_esferas= 8,
      n_esferas_x_fila = 5 ;
   const float
      e = 2.5/n_esferas_x_fila ;

   agregar(scale(vec3(e,e,e)));
   for( unsigned i = 0 ; i < n_filas_esferas ; i++ )
   {
      NodoGrafoEscena * fila_esferas = new NodoGrafoEscena() ;
      fila_esferas->agregar( translate(vec3( 3.0, 0.0, 0.0 )));
      for( unsigned j = 0 ; j < n_esferas_x_fila ; j++ )
      {
         MiEsferaE2 * esfera = new MiEsferaE2() ;
         esfera->ponerIdentificador(i*n_esferas_x_fila+j+1);
         fila_esferas->agregar(  translate(vec3( 2.5, 0.0, 0.0 )));
         fila_esferas->agregar( esfera );
      }
      agregar( fila_esferas );
      agregar(rotate(radians(360.0f/n_filas_esferas),vec3(0.0, 1.0, 0.0)));
   }
}

// ****************************************************************************
// Clase MiEsferaE2

MiEsferaE2::MiEsferaE2()
{
   ponerColor(glm::vec3(1.0,1.0,1.0));
   agregar(new Esfera(20, 20));
}

bool MiEsferaE2::cuandoClick(const glm::vec3 & centro_wc) {
   if (leerColor() == vec3(1.0,0.0,0.0))
      ponerColor({1.0,1.0,1.0});
   else
      ponerColor({1.0,0.0,0.0});

   return true; 
}


MiGrafo::MiGrafo()
{
   const unsigned
      n_esferas = 5 ;
   const float
      e = 2*M_PI/n_esferas ;

   agregar(scale(vec3(0.5,0.5,0.5)));
   for( unsigned i = 0 ; i < n_esferas ; i++ )
   {  
      float angulo = i*M_PI/(n_esferas-1);
      MiEsferaE2 * esfera = new MiEsferaE2() ;
      float trasl_x= sin(angulo)+1;
      float trasl_y= cos(angulo)+1;

      agregar(translate(vec3(trasl_x, trasl_y, 0.0 )));
      agregar(esfera);

      agregar(translate(vec3(0.0, 0.0, 0.0 ))); //QUIERO QUE VUELVA AL PUNTO DE ORIGEN PARA HACER LA TRASLACION DESDE EL (0,0,0)
   }

   
}


      





