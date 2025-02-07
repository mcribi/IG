// Nombre: María,  Apellidos: Cribillés Pérez, Titulación: GIM
// email: mariacribilles@correo.ugr.es, DNI o pasaporte: 75573410X// *********************************************************************

// **
// ** Asignatura: INFORMÁTICA GRÁFICA
// ** 
// ** Mallas indexadas (implementación). Clase 'MallaInd' y derivadas.
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Implementación de las clases 
// **        + MallaInd: malla indexada de triángulos (derivada de Objeto3D)
// **        + MallaPLY: malla indexada de triángulos, leída de un PLY (derivada de MallaInd)
// **        + algunas clases derivadas de MallaInd.
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
#include "aplicacion-ig.h"
#include "malla-ind.h"   // declaración de 'ContextoVis'
#include "lector-ply.h"
#include "seleccion.h"   // para 'ColorDesdeIdent' 


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
   // .... promediando las normales de las caras

   //recorremos la tabla caras que hay en la malla
   for (int i = 0; i < nt; i++) { //recorremos cada triangulo de la malla
   //obtenemos las coordenadas de los tres vertices de cada cara (siempre en orden coherente)
      vec3 p = vertices[triangulos[i][0]];
      vec3 q = vertices[triangulos[i][1]];
      vec3 r = vertices[triangulos[i][2]];

      //lados del triangulo (aristas de la cara)
      vec3 a = q - p;
      vec3 b = r - p;

      //Producto vectorial de las aristas, vector perpendicular a la cara
      vec3 m_c = cross(a, b);

      //si el vector es nuelo enotnces la normal del triangulo tambien es nula
      vec3 n_c;
      if (length(m_c) != 0.0)
         n_c = normalize(m_c); //normalizamos m_c
      else
         n_c = vec3(0.0, 0.0, 0.0); //triangulos degenerados: dos o mas vertices estan en la misma posicion-> el producto vectorial produce un vector de longitud nula-> error al normalizar

      nor_tri.push_back(n_c);
   }

}


// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcularNormales()
{
   using namespace glm ;
   // COMPLETAR: en la práctica 4: calculo de las normales de la malla
   // se debe invocar en primer lugar 'calcularNormalesTriangulos'
   // ....... Normales de los vértices

   calcularNormalesTriangulos(); //calculamos las normales de las caras

   nor_ver = std::vector<vec3>(vertices.size(), vec3(0.0, 0.0, 0.0));

   //recorremos los triangulos de la malla para sumar las normales de los triangulos a los vertices
   for (int i = 0; i < triangulos.size(); i++) {
      for (int j = 0; j < 3; j++) {
         unsigned indice_vertice = triangulos[i][j];

         nor_ver[indice_vertice] = nor_ver[indice_vertice] + nor_tri[i];
      }
   }

   //normalizamos 
   for (int i = 0; i < nor_ver.size(); i++)
      if (length(nor_ver[i]) != 0.0)
         nor_ver[i] = normalize(nor_ver[i]);


}


// --------------------------------------------------------------------------------------------

void MallaInd::visualizarGL( )
{
   // comprobar algunas precondiciones básicas
   using namespace std ;
   assert( aplicacionIG != nullptr );
   Cauce * cauce = aplicacionIG->cauce ; assert( cauce != nullptr );
   CError();

   // si la malla no vértices o no tiene triángulos, imprimir advertencia y salir.
   if ( triangulos.size() == 0 || vertices.size() == 0 )
   {  cout << "advertencia: intentando dibujar malla vacía '" << leerNombre() << "'" << endl << flush ;
      return ;
   }

   // COMPLETAR: práctica 1: cambiar color del cauce
   //
   // Si el objeto tiene un color asignado (se comprueba con 'tieneColor')
   //    - hacer push del color actual del cauce
   //    - fijar el color en el cauce usando el color del objeto (se lee con 'leerColor()')
   if(tieneColor()){
      cauce->pushColor();
      cauce->fijarColor(leerColor());
   }

   // COMPLETAR: práctica 1: crear el descriptor de VAO, si no está creado
   //  Si el puntero 'dvao' es nulo, crear el descriptor de VAO
   //   * en primer lugar se crea el descriptor de VAO, con su constructor 
   //     (se le pasa como parámetro la tabla de posiciones y el número de atributos del cauce). 
   //   * se añade el descriptor de VBO con la tabla de índices (la tabla de triángulos),
   //   * finalmente se añaden al VAO los descriptores VBOs con tablas de atributos 
   //     que no estén vacías
   //  Si el VAO ya está creado, (dvao no nulo), no hay que hacer nada.
   //
   if (dvao==nullptr){  //si el descriptor de VAO no está creado
      dvao= new DescrVAO(numero_atributos_cauce, new DescrVBOAtribs(ind_atrib_posiciones,vertices));   
      dvao->agregar(new DescrVBOInds(triangulos));    //crear y añadir el VBO de indices con la tabla de triangulos

      //Para cada atributo , crear el descriptor de VBO y añadir al VAO
      if(col_ver.size() > 0)  //color
         dvao->agregar(new DescrVBOAtribs(ind_atrib_colores,col_ver));
      if(nor_ver.size() > 0)  //normales
         dvao->agregar(new DescrVBOAtribs(ind_atrib_normales,nor_ver));
      //if(nor_tri.size() > 0)
      //   dvao->agregar(new DescrVBOAtribs(ind_atrib_,nor_tri)); //esto es para otra práctica (lo ha dicho el profe)
      if(cc_tt_ver.size() > 0)   //textura
         dvao->agregar(new DescrVBOAtribs(ind_atrib_coord_text, cc_tt_ver));
      
   }
   
   

   // COMPLETAR: práctica 1: visualizar el VAO usando el método 'draw' de 'DescrVAO'
   dvao->draw(GL_TRIANGLES);

   // COMPLETAR: práctica 1: restaurar color anterior del cauce 
   //
   // Si el objeto tiene un color asignado (se comprueba con 'tieneColor')
   //    - hacer 'pop' del color actual del cauce

   if (tieneColor()){
      cauce->popColor();
   }

}


// -----------------------------------------------------------------------------
// Visualizar el objeto con OpenGL
// usa las tablas de normales, colores y coordenadas de textura, si no están vacías.
      
void MallaInd::visualizarGeomGL( )
{
   // Comprobar que el descriptor de VAO ya está creado (se asume porque es llamada despues de visualizarGL)
   // (es decir, este método únicamente se podrá invocar después de que 
   // se haya llamado a 'visualizaGL')
   
   assert( dvao != nullptr );

   // COMPLETAR: práctica 1: visualizar únicamente la geometría del objeto 
   // Como solo quiere visualizar la geometría, primero se desactivan los atributos, se pinta y se vuelven a activar los atributos
   //    1. Desactivar todas las tablas de atributos del VAO (que no estén vacías)
   if(col_ver.size() > 0)  //color
      dvao->habilitarAtrib(ind_atrib_colores,0);

   if(nor_ver.size() > 0)  //normales
      dvao->habilitarAtrib(ind_atrib_normales,0);

   if(cc_tt_ver.size() > 0) //texturas
      dvao->habilitarAtrib(ind_atrib_coord_text,0);

   //    2. Dibujar la malla (únicamente visualizará los triángulos), se usa el método 'draw' del VAO (dvao)
   dvao->draw(GL_TRIANGLES);

   //    3. Volver a activar todos los atributos para los cuales la tabla no esté vacía
   if(col_ver.size() > 0)  //color
      dvao->habilitarAtrib(ind_atrib_colores,1);

   if(nor_ver.size() > 0)  //normales
      dvao->habilitarAtrib(ind_atrib_normales,1);

   if(cc_tt_ver.size() > 0) //texturas
      dvao->habilitarAtrib(ind_atrib_coord_text,1);


}

// -----------------------------------------------------------------------------
// Visualizar las normales del objeto, si no tiene tabla de normales imprime 
// advertencia y no hace nada.

void MallaInd::visualizarNormalesGL(  )
{
   using namespace std ;
   assert( aplicacionIG != nullptr );
   Cauce * cauce = aplicacionIG->cauce ; assert( cauce != nullptr );

   if ( nor_ver.size() == 0 )
   {
      cout << "Advertencia: intentando dibujar normales de una malla que no tiene tabla (" << leerNombre() << ")." << endl ;
      return ;
   }  

   if( nor_ver.size() != vertices.size() )
   {
      cout << "Error visu. normales: tabla de normales no vacía y de tamaño distinto a la de vértices." << endl ;
      cout << "Nombre del objeto        : " << leerNombre() << endl ;
      cout << "Tamaño tabla vértices    : " << vertices.size() << endl ;
      cout << "Tamaño tabla de normales : " << nor_ver.size() << endl ;
      exit(1);
   }
   CError();

   // COMPLETAR: práctica 4: visualizar las normales del objeto MallaInd
   // 
   // *1* Si el puntero al descriptor de VAO de normales ('dvao_normales') es nulo, 
   //    debemos de crear dicho descriptor, con estos pasos:
   //
   //       * Para cada posición 'v_i' de un vértice en el vector 'vertices':
   //             - Leer la correspondiente normal 'n_i' del vector de normales ('nor_ver').
   //             - Añadir 'v_i' al vector 'segmentos_normales'.
   //             - Añadir 'v_i+a*n_i' al vector 'segmentos_normales'.
   //
   //       * Crear el objeto descriptor del VAO de normales, para ello se usa el vector 
   //          'segmentos_normales' y se tiene en cuenta que esa descriptor únicamente gestiona 
   //          una tabla de atributos de vértices (la de posiciones, ya que las otras no se 
   //          necesitan).
   // 
   // *2* Visualizar el VAO de normales, usando el método 'draw' del descriptor, con el 
   //       tipo de primitiva 'GL_LINES'.

   //  ..........

   if (dvao_normales == nullptr) { //si es nelo, lo creamos el descriptor 
      for (int i = 0; i < vertices.size(); i++) { //para cada posicion vi de un vertice
         segmentos_normales.push_back(vertices[i]); //leemos la normal y la añadimos 
         segmentos_normales.push_back(vertices[i] + (0.5f * nor_ver[i])); //cojo a=0.5
      }
      //crear objeto descriptor del VAO de normales
      dvao_normales = new DescrVAO(numero_atributos_cauce, new DescrVBOAtribs(ind_atrib_posiciones, segmentos_normales));
   }
   dvao_normales->draw(GL_LINES); //visualizar normales
}

// -----------------------------------------------------------------------------
// visualizar el objeto en 'modo seleccion', es decir, sin iluminación y con los colores 
// basados en los identificadores de los objetos
void MallaInd::visualizarModoSeleccionGL() 
{

   using namespace std ;
   assert( aplicacionIG != nullptr );
   Cauce * cauce = aplicacionIG->cauce ; assert( cauce != nullptr );

   // COMPLETAR: práctica 5: visualizar la malla en modo selección 
   //
   // Se debe escribir código para visualizar únicamente la geometría, pero usando el color 
   // obtenido a partir del identificador. El código da estos pasos:
   // 
   // 1. Leer el identificador del objeto (con 'leerIdentificador'). Si el objeto tiene 
   //    identificador (es decir, si su identificador no es -1)
   //       + Hacer push del color del cauce, con 'pushColor'.
   //       + Fijar el color del cauce (con 'fijarColor') usando un color obtenido a 
   //         partir del identificador (con 'ColorDesdeIdent'). 
   // 2. Invocar 'visualizarGeomGL' para visualizar la geometría.
   // 3. Si tiene identificador: hacer pop del color, con 'popColor'.
   //
   int ident = leerIdentificador();
   if (ident != -1) {
      cauce->pushColor();
      cauce->fijarColor(ColorDesdeIdent(ident));
   }

   visualizarGeomGL();

   if (ident != -1)  
      cauce->popColor();

}


// ****************************************************************************
// Clase 'MallaPLY'

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre( std::string("malla leída del archivo '") + nombre_arch + "'" );

   // COMPLETAR: práctica 2: leer archivo PLY e inicializar la malla
   LeerPLY(nombre_arch, vertices, triangulos);


   // COMPLETAR: práctica 4: invocar  a 'calcularNormales' para el cálculo de normales
   // .................
   calcularNormales();

}

// ****************************************************************************
// Clase 'Cubo

Cubo::Cubo()
:  MallaInd( "cubo 8 vértices" )
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

   calcularNormales();

}

// -----------------------------------------------------------------------------------------------
// Clase TETRAEDRO

Tetraedro::Tetraedro()
:  MallaInd( "tetraedro 4 vértices irregular de altura 2" )
{

   vertices =
      {  { -1.0, 0.0, 0.0 }, // 0
         { 0.0, -1.0, 0.0 }, // 1
         { 1.0, 0.0, 0.0 }, // 2
         { 0.0, 0.5, +2.0 } // 3
      } ;



   triangulos =
      {  {0,1,3}, {0,3,2}, 
         {1,2,3}, {0,1,2}
      } ;

      col_ver = { 
      {0.0, 0.0, 0.0}, // 0
      {0.0, 0.0, 1.0}, // 1
      {0.0, 1.0, 0.0}, // 2
      {0.0, 1.0, 1.0}, // 3
      }  ; 

   calcularNormales();

}

// -----------------------------------------------------------------------------------------------
// Clase CuboColores

CuboColores::CuboColores()
:  MallaInd( "Cubo de colores" )
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

      col_ver = { //si la X es -1->R=0, si la Y es -1-> G=0, si la Zes -1->B=0
      {0.0, 0.0, 0.0}, // 0
      {0.0, 0.0, 1.0}, // 1
      {0.0, 1.0, 0.0}, // 2
      {0.0, 1.0, 1.0}, // 3
      {1.0, 0.0, 0.0}, // 4
      {1.0, 0.0, 1.0}, // 5
      {1.0, 1.0, 0.0}, // 6
      {1.0, 1.0, 1.0}, // 7
      };  

   
   calcularNormales();
}

// -----------------------------------------------------------------------------------------------
// Clase Estrella 4 picos

Estrella4picos::Estrella4picos()
:  MallaInd( "Estrella 4 picos" )
{

   vertices =
      {  { 0.5, 0.5, 0.0 }, // centro de la estrella 0
         { 0.5, 0.0, 0.0 }, // 1
         { 1.0, 0.5, 0.0 }, // 2
         { 0.5, 1.0, 0.0 }, // 3
         { 0.0, 0.5, 0.0 },  // 4
         { 0.6, 0.4, 0.0 }, // 5
         { 0.6, 0.6, 0.0 }, //6
         { 0.4, 0.6, 0.0 }, //7
         { 0.4, 0.4, 0.0 } //8
      } ;


   triangulos =
      {  
         {0,1,5}, {0,5,2}, 
         {0,6,2}, {0,6,3}, 

         {0,7,3}, {0,4,7}, 
         {0,4,8}, {0,8,1} 
      } ;

   col_ver = { 
      { 0.5, 0.5, 0.0 }, // centro de la estrella 0
      { 0.5, 0.0, 0.0 }, // 1
      { 1.0, 0.5, 0.0 }, // 2
      { 0.5, 1.0, 0.0 }, // 3
      { 0.0, 0.5, 0.0 },  // 4
      { 0.6, 0.4, 0.0 }, // 5
      { 0.6, 0.6, 0.0 }, //6
      { 0.4, 0.6, 0.0 }, //7
      { 0.4, 0.4, 0.0 } //8
   };  

   

}

// -----------------------------------------------------------------------------------------------
// Clase EstrellaZ

EstrellaZ::EstrellaZ(unsigned n)
:  MallaInd( "Estrella Z" )
{
   vertices ={  
      { 0.5, 0.5, 0.0 }, // centro
   } ;

   for (unsigned i=0; i<n; i++) {
      float angulo = i*2*M_PI/n;
      float desfase = M_PI/n; //vértice intermedio a la mitad del ángulo entre dos picos
      vertices.push_back({0.5+0.5*cos(angulo), 0.5+0.5*sin(angulo), 0.0}); //los picos
      vertices.push_back({0.5+0.25*cos(angulo+desfase), 0.5+0.25*sin(angulo+desfase), 0.0}); //los puntos unen los picos
   }//la distancia al centro de los picos es 0.5 y en los mini-picos de 0.25
   
   for (unsigned i=1; i<2*n; i++) {
      triangulos.push_back({0,i,i+1});
   }
   triangulos.push_back({0,2*n,1});


   col_ver.push_back({1.0,1.0,1.0});   //el centro es blanco
   for (unsigned i=1; i<vertices.size(); i++) { //los demas colores son las coordenadas del propio punto
      glm::vec3 actual = vertices[i];
      col_ver.push_back({actual.x, actual.y, actual.z});
   } 
}

// ****************************************************************************
// Clase CasaX

CasaX::CasaX()
:  MallaInd( "Casa X")
{
   //mas alargada en el eje X que en el eje Z
   vertices =
      {  { -0.5 , -0.25 , -0.25 }, // 0
         { -0.5 , -0.25 , +0.25 }, // 1
         { -0.5 , +0.25 , -0.25 }, // 2
         { -0.5 , +0.25 , +0.25 }, // 3
         { +0.5 , -0.25 , -0.25 }, // 4
         { +0.5 , -0.25 , +0.25 }, // 5
         { +0.5 , +0.25 , -0.25 }, // 6
         { +0.5 , +0.25 , +0.25 }, // 7

         //tejado:
         { +0.5 , +0.5 , 0.0 }, // 8
         { -0.5 , +0.5 , 0.0 }, // 9

      } ;



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         //{0,5,1}, {0,4,5}, // Y- quitamos triangulos de la base
         //{2,3,7}, {2,7,6}, // Y+ (+2) quitamos triangulos de la tapa

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3},  // Z+ (+1)

         //triangulos tejado:
         {8,6,7}, {2,9,3}, 
         {2,9,8}, {8,6,2}, {8,3,9}, 
         {8,7,3} 

      } ;

   //cada vertice color RGB igual a las componentes XYZ
   for(int i=0; i<vertices.size(); i++) {
      col_ver.push_back(vertices[i]);
   }
   

}

// ****************************************************************************
// Clase Malla Triangulo

MallaTriangulo::MallaTriangulo()
:  MallaInd( "Malla Triangulo")
{
   //un unico triangulo con 3 vertices, perpendicular en Z
   vertices =
      {  { -0.5 , 0.0 , 0.0 }, // 0
         { 0.5 , 0.0 , 0.0 }, // 1
         { 0.0 , sqrt(2) , 0.0 } // 2

      } ;

   triangulos ={{0,1,2}};
}

// ****************************************************************************
// Clase Malla Cuadrado

MallaCuadrado::MallaCuadrado()
:  MallaInd( "Malla Cuadrado")
{
   //cuadrado: dos triangulos, 4 vertices, perpendicular en Z, lado 2 unidades, centro en el origen
   vertices =
      {  { 1.0 , -1.0 , 0.0 }, //0
         { 1.0 , 1.0 , 0.0 }, //1
         { -1.0 , 1.0 , 0.0 }, //2
         { -1.0 , -1.0 , 0.0 } //3

      } ;

   triangulos ={{0,1,2}, {0,3,2}};
}

// ****************************************************************************
// Clase Malla Piramide L

MallaPiramideL::MallaPiramideL()
:  MallaInd( "Malla Piramide L")
{
   //base en forma de L
   //7 vertices (6 base + pico), 6 triangulos adyacentes + triangulos minimo base (4)
   vertices =
      {  { 0.0 , 1.0 , 0.0 }, //pico

         //base
         { 1.0 , 0.0 , -1.0 }, //1
         { 1.0 , 0.0 , 1.0 }, //2
         { 0.0 , 0.0 , 1.0 }, //3
         { 0.0 , 0.0 , 0.0 }, //4
         { -1.0 , 0.0 , 0.0 }, //5
         { -1.0 , 0.0 , -1.0 } //6

      } ;

   triangulos ={
      //base
      {1,5,6}, {1,4,5}, {1,4,3}, {1,2,3},

      //caras
      {0,2,3}, {0,4,3}, {0,4,5}, {0,5,6}, {0,1,6}, {0,1,2}

      };
}

// -----------------------------------------------------------------------------------------------
// Clase PiramideEstrellaZ

PiramideEstrellaZ::PiramideEstrellaZ(unsigned n)
:  MallaInd( "Piramide Estrella Z" )
{
   vertices ={  
      { 0.5, 0.5, 0.5 }, //apice de la piramide 1
      { 0.5, 0.5, 0.0 } // centro de la base 0

   } ;

   //base es estrellaZ
   for (unsigned i=0; i<n; i++) {   
      float angulo = i*2*M_PI/n;
      float desfase = M_PI/n; //vértice intermedio a la mitad del ángulo entre dos picos
      vertices.push_back({0.5+0.5*cos(angulo), 0.5+0.5*sin(angulo), 0.0}); //los picos
      vertices.push_back({0.5+0.25*cos(angulo+desfase), 0.5+0.25*sin(angulo+desfase), 0.0}); //los puntos unen los picos
   }
   


   for (unsigned i=2; i<=2*n; i++) {
      triangulos.push_back({1,i,i+1});
      triangulos.push_back({0,1,i});
   }

   //para cerrar la pirmide
   triangulos.push_back({0,1,(2*n)+1});   //con el apice

   triangulos.push_back({1,(2*n)+1,2}); //en la base




   col_ver.push_back({1.0,1.0,1.0});   //el centro de la base es blanco
   col_ver.push_back({1.0,1.0,1.0});   //el apice tambien es blanco

   //los demas vertices color RGB igual a las componentes XYZ
   for(int i=2; i<vertices.size(); i++) {
      col_ver.push_back(vertices[i]);
   }

   
   

}

// -----------------------------------------------------------------------------------------------
// Clase RejillaY

RejillaY::RejillaY(unsigned n, unsigned m)
:  MallaInd( "Rejilla Y" )
{   
   for (float i=0; i<n; i++){
      for (float j=0; j<m; j++){
         vertices.push_back({i/(n-1), 0, j/(m-1)});
      }
   }
    
   for(float i=0; i<n-1; i++){
      for(float j=0; j<m-1; j++){
         triangulos.push_back({i*m+j,i*m+j+1,(i+1)*m+j});
         triangulos.push_back({i*m+j+1,(i+1)*m+j+1,(i+1)*m+j});
      }//la i hace referencia a la fila actual y la (i+1) a la siguiente fila. La i se multiplica pq es la fila q va 0,4,8
   }  //la j hace referencia a la columna actual y la (j+1) a la siguiente columna (la j se suma pq va de una en una)

   //los vertices color RGB igual a las componentes XYZ
   for(float i=0; i<vertices.size(); i++) {
      col_ver.push_back(vertices[i]);
   } 

}

// -----------------------------------------------------------------------------------------------
// Clase Malla Torre

MallaTorre::MallaTorre(unsigned n)
:  MallaInd( "Malla Torre" )
{   //malla indexada con 4(n + 1) vértices y 8n triángulos
   //no tiene tapa ni base
   //pongo que cada planta tiene 1 de altura

   for (int i=0; i<=n; i++){
      vertices.push_back({0, i, 0});   //0+i*4
      vertices.push_back({0, i, 1});   //1+i*4
      vertices.push_back({1, i, 1});   //2+i*4
      vertices.push_back({1, i, 0});   //3+i*4 
   }

   for (int i=0; i<=4*n; i++){
      if((i+1)%4==0){   //la ultima cara de cada piso va diferente
         triangulos.push_back({i, i+4, i+1});
         triangulos.push_back({i, i-3, 1+i});
      }else{
         if (i!=4*n){   //si no es la ultima iteracion (para que no una con la planta baja)
            triangulos.push_back({i, i+4, i+5});
            triangulos.push_back({i, i+1, i+5});
         }
      }
   }

   //los vertices color RGB igual a las componentes XYZ
   for(float i=0; i<vertices.size(); i++) {
      col_ver.push_back(vertices[i]);
   } 

}


// -----------------------------------------------------------------------------------------------
// Clase Cubo24
Cubo24::Cubo24 ()
:  MallaInd( "Cubo24" )
{
   vertices =
      {  { 0.0, 0.0, 0.0 }, // 0
         { 0.0, 1.0, 0.0 }, // 1
         { 1.0, 1.0, 0.0 }, // 2

         { 1.0, 0.0, 0.0 }, // 3
         { 1.0, 0.0, 0.0 }, // 4
         { 1.0, 1.0, 0.0 }, // 5

         { 1.0, 1.0, 1.0 }, // 6
         { 1.0, 0.0, 1.0 }, // 7
         { 1.0, 0.0, 1.0 }, // 8

         { 1.0, 1.0, 1.0 }, // 9
         { 0.0, 1.0, 1.0 }, // 10
         { 0.0, 0.0, 1.0 }, // 11

         { 0.0, 0.0, 1.0 }, // 12
         { 0.0, 1.0, 1.0 }, // 13
         { 0.0, 1.0, 0.0 }, // 14

         { 0.0, 0.0, 0.0 }, // 15
         { 0.0, 0.0, 0.0 }, // 16
         { 0.0, 0.0, 1.0 }, // 17

         { 1.0, 0.0, 1.0 }, // 18
         { 1.0, 0.0, 0.0 }, // 19
         { 1.0, 1.0, 0.0 }, // 20

         { 0.0, 1.0, 0.0 }, // 21
         { 0.0, 1.0, 1.0 }, // 22
         { 1.0, 1.0, 1.0 } // 23

         
      } ;



   triangulos =
      {  {0,1,2}, {0,2,3}, 
         {4,5,6}, {4,6,7},

         {8,9,11}, {9,10,11}, 
         {12,13,14}, {12,14,15},

         {16,17,18}, {16,18,19}, 
         {20,21,22}, {22,20,23}  
      } ;

      /*
      Coordenadas de textura (u,v):
         u: Mapea la posición horizontal.
         v: Mapea la posición vertical, invertida respecto al eje Y porque las texturas suelen tener su origen en la esquina superior izquierda.*/
      
      cc_tt_ver = { //la v se invierte 
      {0.0, 1.0-0.0}, //0
      {0.0, 1.0-1.0}, //1
      {1.0, 1.0-1.0}, //2
      {1.0, 1.0-0.0}, //3

      {0.0, 1.0-0.0}, //
      {0.0, 1.0-1.0},
      {1.0, 1.0-1.0},
      {1.0, 1.0-0.0},

      {0.0, 1.0-0.0},
      {0.0, 1.0-1.0},
      {1.0, 1.0-1.0},
      {1.0, 1.0-0.0},

      {0.0, 1.0-0.0},
      {0.0, 1.0-1.0},
      {1.0, 1.0-1.0},
      {1.0, 1.0-0.0},

      {0.0, 1.0-0.0},
      {0.0, 1.0-1.0},
      {1.0, 1.0-1.0},
      {1.0, 1.0-0.0},

      {0.0, 1.0-0.0},
      {0.0, 1.0-1.0},
      {1.0, 1.0-1.0},
      {1.0, 1.0-0.0}
   };


   calcularNormales();

}

//CUBO24 DE LADO 1 CENTRADO EN EL ORIGEN
// vertices =
//       {  { -0.5, -0.5, -0.5 }, // 0
//          { -0.5,  0.5, -0.5 }, // 1
//          {  0.5,  0.5, -0.5 }, // 2

//          {  0.5, -0.5, -0.5 }, // 3
//          {  0.5, -0.5, -0.5 }, // 4
//          {  0.5,  0.5, -0.5 }, // 5

//          {  0.5,  0.5,  0.5 }, // 6
//          {  0.5, -0.5,  0.5 }, // 7
//          {  0.5, -0.5,  0.5 }, // 8

//          {  0.5,  0.5,  0.5 }, // 9
//          { -0.5,  0.5,  0.5 }, // 10
//          { -0.5, -0.5,  0.5 }, // 11

//          { -0.5, -0.5,  0.5 }, // 12
//          { -0.5,  0.5,  0.5 }, // 13
//          { -0.5,  0.5, -0.5 }, // 14

//          { -0.5, -0.5, -0.5 }, // 15
//          { -0.5, -0.5, -0.5 }, // 16
//          { -0.5, -0.5,  0.5 }, // 17

//          {  0.5, -0.5,  0.5 }, // 18
//          {  0.5, -0.5, -0.5 }, // 19
//          {  0.5,  0.5, -0.5 }, // 20

//          { -0.5,  0.5, -0.5 }, // 21
//          { -0.5,  0.5,  0.5 }, // 22
//          {  0.5,  0.5,  0.5 }  // 23
//       } ;


MallaDiscoP4::MallaDiscoP4()
{
   ponerColor({1.0, 1.0, 1.0});
   const unsigned ni = 23, nj = 31 ;

   for( unsigned i= 0 ; i < ni ; i++ )
   for( unsigned j= 0 ; j < nj ; j++ )
   {
      const float
         fi = float(i)/float(ni-1), //punto inicial hasta un giro completo de 2pi
         fj = float(j)/float(nj-1), //del centro a su perimetro
         ai = 2.0*M_PI*fi,
         x = fj * cos( ai ),
         y = fj * sin( ai ),
         z = 0.0 ;
      vertices.push_back({ x, y, z });

      //EJERCICIO ADICIONAL 1 PR4
      //coordenadas de textura (u,v): tiene ue estar en el rango [0,1] pero el disco redondo (x,y) esta entre el rango [-1,1]
      //queremos mapear la textura en el disco, tenemos que mapear las dos coordenadas porque ambas se mueven desde el -1 al 1
      //Para mapear el rango inicial [a,b] a [c,d] se utiliza: 
      //valor_mapeado = c+( ((x-a)*(d-c)) / b-a  )
      //para que se vean los cuadrados mas grandes: aplicamos una escala de 0.5 (dividimos entre 2 y se hace mas grande la textura)
      //cc_tt_ver.push_back({float(0.5*x+0.5), float(0.5*y+0.5)});
      //cc_tt_ver.push_back({0.5 * (1.0 + fj * cos(2.0 * M_PI * fi)), 0.5 * (1.0 + fj * sin(2.0 * M_PI * fi))});

      //EJERCICIO ADICIONAL 2 PR4
      cc_tt_ver.push_back({fi, fj});
      //o bien: normalizando el angulo ai al rango [0,1]
      //cc_tt_ver.push_back({fj, ai / (2 * M_PI)});


  }
   for( unsigned i= 0 ; i < ni-1 ; i++ )
   for( unsigned j= 0 ; j < nj-1 ; j++ )
   {
      triangulos.push_back({ i*nj+j, i*nj+(j+1), (i+1)*nj+(j+1) });
      triangulos.push_back({ i*nj+j, (i+1)*nj+(j+1), (i+1)*nj+j });
   }
}

Piramide::Piramide(){
   vertices =
      {  
         //primera cara
         { 0.0, -1.0, 0.0 }, // 0
         {-1.0, 1.0, 0.0 }, // 1
         { 0.0, 0.0, 1.0 }, //pico 2

         //segunda cara
         { -1.0, 1.0, 0.0 }, // 3
         { 1.0, 1.0, 0.0 },  //4
         { 0.0, 0.0, 1.0 }, //pico 5

         //tercera cara
         { 1.0, 1.0, 0.0 }, // 6
         { 0.0, -1.0, 0.0 },  //7
         { 0.0, 0.0, 1.0 }, //pico 8

         //cuarta cara
         {-1.0, 1.0, 0.0 },  //9
         { 1.0, 1.0, 0.0 },  //10
         { 0.0, -1.0, 0.0 } // 11

      } ;


   triangulos =
      {  
         {0, 1,2}, {3,4,5}, 
         {6,7,8}, {9,10,11}
      } ;

   cc_tt_ver = { 
      { 1.0, 1.0}, //  0
      { 0.0, 1.0 }, // 1
      { 0.5, 0.0 }, // pico 2

      { 1.0, 1.0}, //  3
      { 0.0, 1.0 }, // 4
      { 0.5, 0.0 }, // 5

      { 1.0, 1.0}, //  6
      { 0.0, 1.0 }, // 7
      { 0.5, 0.0 }, // 8

      { 1.0, 1.0}, //  9
      { 0.0, 1.0 }, // 10
      { 0.5, 0.0 }// 11

   };  
}


MallaMiEjercicio2::MallaMiEjercicio2(){
   vertices =
      {  
         //CUBO
         { 0.0, 0.0, 0.0 }, // 0
         { 0.0, 1.0, 0.0 }, // 1
         { 1.0, 1.0, 0.0 }, // 2

         { 1.0, 0.0, 0.0 }, // 3
         { 1.0, 0.0, 0.0 }, // 4
         { 1.0, 1.0, 0.0 }, // 5

         { 1.0, 1.0, 1.0 }, // 6
         { 1.0, 0.0, 1.0 }, // 7
         { 1.0, 0.0, 1.0 }, // 8

         { 1.0, 1.0, 1.0 }, // 9
         { 0.0, 1.0, 1.0 }, // 10
         { 0.0, 0.0, 1.0 }, // 11

         { 0.0, 0.0, 1.0 }, // 12
         { 0.0, 1.0, 1.0 }, // 13
         { 0.0, 1.0, 0.0 }, // 14

         { 0.0, 0.0, 0.0 }, // 15
         // { 0.0, 0.0, 0.0 }, // 16
         // { 0.0, 0.0, 1.0 }, // 17

         //estos son para la tapa y la base que no la queremos:
         // { 1.0, 0.0, 1.0 }, // 18
         // { 1.0, 0.0, 0.0 }, // 19
         // { 1.0, 1.0, 0.0 }, // 20

         // { 0.0, 1.0, 0.0 }, // 21
         // { 0.0, 1.0, 1.0 }, // 22
         // { 1.0, 1.0, 1.0 } // 23

         //tejado:
         { 0.0, 1.0, 0.0 }, // 16
         { 1.0, 1.0, 0.0 }, // 17
         { 0.5, 1.5, 0.5 }, // 18 pico

         { 1.0, 1.0, 0.0 }, // 19
         { 1.0, 1.0, 1.0 }, // 20
         { 0.5, 1.5, 0.5 }, // 21 pico

         { 1.0, 1.0, 1.0 }, // 22
         { 0.0, 1.0, 1.0 }, // 23
         { 0.5, 1.5, 0.5 }, // 24 pico
   
         { 0.0, 1.0, 1.0 }, // 25
         { 0.0, 1.0, 0.0 }, // 26
         { 0.5, 1.5, 0.5 }, // 27 pico

         //para cerrar la parte de abajo:
         { 0.0, 0.0, 0.0 }, // 28
         { 1.0, 0.0, 0.0 }, // 29
         { 0.0, 0.0, 1.0 }, // 30
         { 1.0, 0.0, 1.0 } // 31
         
      } ;



   triangulos =
      {  {0,1,2}, {0,2,3}, 
         {4,5,6}, {4,6,7},

         {8,9,11}, {9,10,11}, 
         {12,13,14}, {12,14,15},

         {16,17,18}, {19,20,21}, 
         {22,23,24}, {25,26,27},  

         //parte de abajo de la casa (sin textura):
         {28,29,30}, {30,29,31} 
      } ;

      /*
      Coordenadas de textura (u,v):
         u: Mapea la posición horizontal.
         v: Mapea la posición vertical, invertida respecto al eje Y porque las texturas suelen tener su origen en la esquina superior izquierda.*/
      
      cc_tt_ver = { //la v se invierte 
      {0.0, 1.0-0.0}, //0
      {0.0, 1.0-0.5}, //1
      {0.25, 1.0-0.5}, //2
      {0.25, 1.0-0.0}, //3

      {0.25, 1.0-0.0}, //4
      {0.25, 1.0-0.5}, //5
      {0.5, 1.0-0.5}, //6
      {0.5, 1.0-0.0}, //7

      {0.5, 1.0-0.0}, //8
      {0.5, 1.0-0.5}, //9
      {0.75, 1.0-0.5}, //10
      {0.75, 1.0-0.0}, //11

      {0.75, 1.0-0.0}, //12
      {0.75, 1.0-0.5}, //13
      {1.0, 1.0-0.5}, //14
      {1.0, 1.0-0.0}, //15

   //    // {0.0, 1.0-0.0},
   //    // {0.0, 1.0-1.0}
   //    // {1.0, 1.0-1.0},
   //    // {1.0, 1.0-0.0},

   //    // {0.0, 1.0-0.0},
   //    // {0.0, 1.0-1.0},
   //    // {1.0, 1.0-1.0},
   //    // {1.0, 1.0-0.0}

   //textura de la piramide
   {0.0, 1.0-0.5}, //16
   {0.25, 1.0-0.5}, //17
   {0.2, 1.0-1.0}, //18 pico

   {0.0, 1.0-0.5}, //19
   {0.25, 1.0-0.5}, //20
   {0.2, 1.0-1.0}, //21 pico

   {0.0, 1.0-0.5}, //22
   {0.25, 1.0-0.5}, //23
   {0.2, 1.0-1.0}, //24 pico

   {0.0, 1.0-0.5}, //25
   {0.25, 1.0-0.5}, //26
   {0.2, 1.0-1.0}, //27 pico

   {0.0, 1.0-0.0}, //28
   {0.0, 1.0-0.0}, //29
   {0.0, 1.0-0.0}, //30
   {0.0, 1.0-0.0} //31





   };
}


