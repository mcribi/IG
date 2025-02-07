// Nombre: María,  Apellidos: Cribillés Pérez, Titulación: GIM
// email: mariacribilles@correo.ugr.es, DNI o pasaporte: 75573410X

// *********************************************************************
// **
// ** Asignatura: INFORMÁTICA GRÁFICA
// ** 
// ** Mallas indexadas creadas por revolución de un perfil (implementación). Clase 'MallaRevol' y derivadas.
// ** Copyright (C) 2016-2024 Carlos Ureña
// **
// ** Implementación de las clases 
// **    + MallaRevol: malla indexada de triángulos obtenida por 
// **      revolución de un perfil (derivada de MallaInd)
// **    + MallaRevolPLY: malla indexada de triángulos, obtenida 
// **      por revolución de un perfil leído de un PLY (derivada de MallaRevol)
// **    + algunas clases derivadas de MallaRevol
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
#include "lector-ply.h"
#include "malla-revol.h"

using namespace std ;

// *****************************************************************************




// Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
// a partir de un perfil y el número de copias que queremos de dicho perfil.
void MallaRevol::inicializar
(
   const std::vector<glm::vec3> & perfil,     // tabla de vértices del perfil original
   const unsigned               num_copias  // número de copias del perfil
)
{
   using namespace glm ;
   
   // COMPLETAR: práctica 2: implementar algoritmo de creación de malla de revolución
   //
   // Escribir el algoritmo de creación de una malla indexada por revolución de un 
   // perfil, según se describe en el guion de prácticas.


   //tabla vertices
   for (int i=0; i<num_copias; i++){
      for (int j=0; j<perfil.size(); j++){
         float angulo=(2*M_PI*i)/(num_copias-1);

         //Queremos rotar en el eje de la y. Matriz de rotación 3x3 del eje y:
         std::vector<std::vector<float>> matriz_rotacion = 
         {  {cos(angulo), 0.0, sin(angulo)},
            {0.0 ,1.0, 0},
            {-sin(angulo), 0.0, cos(angulo)},
         };

         //multiplicamos 
         glm::vec3 nuevo_vertice = {
            matriz_rotacion[0][0]*perfil[j][0] + matriz_rotacion[0][1]*perfil[j][1] + matriz_rotacion[0][2]*perfil[j][2],
            matriz_rotacion[1][0]*perfil[j][0] + matriz_rotacion[1][1]*perfil[j][1] + matriz_rotacion[1][2]*perfil[j][2],
            matriz_rotacion[2][0]*perfil[j][0] + matriz_rotacion[2][1]*perfil[j][1] + matriz_rotacion[2][2]*perfil[j][2],
         };

         vertices.push_back(nuevo_vertice);
         
      }
   }
   
   
   //tabla de triangulos
   int k=0; 
   for (int i=0; i<num_copias-1; i++){
      for (int j=0; j<perfil.size()-1; j++){
         k=i*perfil.size()+j;

         triangulos.push_back({k, k + perfil.size(), k + perfil.size() + 1});
         triangulos.push_back({k, k + perfil.size() + 1, k + 1});
      }
   }

   //Práctica 4: calcular las normales
   vector<vec3> normales_aristas;

   //Primero calculamos las normales de las aristas
   for (int i = 0; i < perfil.size()-1; i++) { //recorremos todo el perfil
      float v_1 = (perfil[i+1] - perfil[i])[0]; //dos vertices consecutivos
      float v_2 = (perfil[i+1] - perfil[i])[1];
      vec3 m_i(vec3(v_2, -v_1, 0.0f)); //normal de las aristas

      if (length(m_i) != 0.0) //si no es nulo
         m_i = normalize(m_i);  //normalizamos para que el vector m_i tenga longitud unidad
      
      normales_aristas.push_back(m_i); //vector de normales, una por cada segmento del perfil
   }

   //Vértices
   vector<vec3> normales_vertices;

   normales_vertices.push_back(normales_aristas[0]); //en los extremos, el vertices sol tiene una arista asociada por lo que esa normal es tambien la del vertices
   for (int i = 1; i < perfil.size()-1; i++) {
      normales_vertices.push_back(normalize(normales_aristas[i-1] + normales_aristas[i])); //suma normalizada
   } //promedio de dos normales de aristas consecutivos

   normales_vertices.push_back(normales_aristas[perfil.size() - 2]); //es un extremo

   //Calculo de coordenadas de textura: d_i y v_i
   vector<float> d, t, suma;
   float suma_total;

   for (int i = 0; i < perfil.size()-1; i++) {
      d.push_back(sqrt(length(perfil[i+1] - perfil[i]))); //distancia entre dos vertices consecutivos 
   }

   //Dividendo
   suma.push_back(0.0f);
   for (int i = 1; i < perfil.size(); i++) {
      suma.push_back(suma[i-1] + d[i-1]);
   }

   //divisor
   suma_total = suma[perfil.size()-1];
   t.push_back(0.0f);
   for (int i = 1; i < perfil.size(); i++)
      t.push_back(suma[i] / suma_total); //definicion de t_i: es la distancia, medida a lo largo del perfil, entre el vértie 0 y el vértice i (normalizada de forma que t_n-1=1)

}

// -----------------------------------------------------------------------------
// constructor, a partir de un archivo PLY

MallaRevolPLY::MallaRevolPLY
(
   const std::string & nombre_arch,
   const unsigned      nperfiles
)
{
   ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));
   // COMPLETAR: práctica 2: crear la malla de revolución
   // Leer los vértice del perfil desde un PLY, después llamar a 'inicializar'
   std::vector<glm::vec3> perfil;
   LeerVerticesPLY(nombre_arch, perfil);  
   inicializar(perfil,nperfiles);


}

// clases mallas indexadas por revolución de un perfil generado proceduralmente
// -----------------------------------------------------------------------------
//Clase Cilindro (hueco)
Cilindro::Cilindro(const int num_verts_per, const unsigned nperfiles){

   //perfil original (base centro en el origen, radio y altura 1)
   std::vector<glm::vec3> perfil = std::vector<glm::vec3>();   //creamos e inicializamos perfil

   for(int i=0; i<num_verts_per; i++){
      float altura = i*(1.0/(num_verts_per-1));
      perfil.push_back(glm::vec3(1.0,altura,0.0));
   }
   inicializar(perfil,nperfiles);
}

// -----------------------------------------------------------------------------
//Clase Cilindro relleno
CilindroRelleno::CilindroRelleno(const int num_verts_per, const unsigned nperfiles){

   //perfil original (base centro en el origen, radio y altura 1)
   std::vector<glm::vec3> perfil = std::vector<glm::vec3>();   //creamos e inicializamos perfil


   for(int i=0; i<num_verts_per; i++){
      float altura = i*(1.0/(num_verts_per-1));
      perfil.push_back(glm::vec3(1.0,altura,0.0));
      perfil.push_back(glm::vec3(0.0,altura,0.0));
   }
   inicializar(perfil,nperfiles);
}


// -----------------------------------------------------------------------------
//Clase Cono
Cono::Cono(const int num_verts_per, const unsigned nperfiles){
   // la base tiene el centro en el origen, el radio y altura son 1
   std::vector<glm::vec3> perfil = std::vector<glm::vec3>();   //creamos e inicializamos perfil

   for(int i=0; i<num_verts_per; i++){
      float x= 1.0-i*(1.0/(num_verts_per-1));
      float altura = i*(1.0/(num_verts_per-1));
      perfil.push_back(glm::vec3(x,altura,0.0));
   }
   inicializar(perfil,nperfiles);

}

// -----------------------------------------------------------------------------
//Clase Cono Relleno
ConoRelleno::ConoRelleno(const int num_verts_per, const unsigned nperfiles){
   // la base tiene el centro en el origen, el radio y altura son 1
   std::vector<glm::vec3> perfil = std::vector<glm::vec3>();   //creamos e inicializamos perfil

   for(int i=0; i<num_verts_per; i++){
      float x= 1.0-i*(1.0/(num_verts_per-1));
      float altura = i*(1.0/(num_verts_per-1));
      perfil.push_back(glm::vec3(x,altura,0.0));
      perfil.push_back(glm::vec3(0.0,altura,0.0));
   }
   inicializar(perfil,nperfiles);

}


// -----------------------------------------------------------------------------
//Clase Esfera
Esfera::Esfera(const int num_verts_per, const unsigned nperfiles){

   // La esfera tiene el centro en el origen, el radio es la unidad -> ec de la circunferencia es x^2+y^2=1
   std::vector<glm::vec3> perfil = std::vector<glm::vec3>();   //creamos e inicializamos perfil

   for (float i=0; i<num_verts_per; i++) {
      float angulo = i*M_PI/(num_verts_per-1);
      perfil.push_back({sin(angulo), cos(angulo), 0.0});
   }

   inicializar(perfil,nperfiles);

}




