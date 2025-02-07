// Nombre: María,  Apellidos: Cribillés Pérez, Titulación: GIM
// email: mariacribilles@correo.ugr.es, DNI o pasaporte: 75573410X

#include "examen-ec-p123.h"

using namespace std;
using namespace glm;

P1Malla::P1Malla()
:  MallaInd("cubo 8 vétices a color")
{
   vertices =
      {  { -1.0, 0.0, +1.0 }, // 0
         { -2.0, 0.0, +1.0 }, // 1
         { 1.0, 0.0, +1.0 }, // 2
         { 2.0, 0.0, +1.0 }, // 3
         { -1.0, 0.0, -1.0 }, // 4
         { -2.0, 0.0, -1.0 }, // 5
         { +1.0, 0.0, -1.0 }, // 6
         { +2.0, 0.0, -1.0 }, // 7
         { +0.0, 1.0, -1.0}, // 8 //los picos
         { +0.0, 1.0, +1.0}, //9
      } ;



   triangulos =
      {  {0,5,4},{0,1,5},{0,9,4},{4,8,9},{2,6,9},{8,6,9},{2,6,3},{6,3,7}     } ;

    col_ver = {
      {0.3, 0.3, 0.3}, // 0
      {0.3, 0.3, 0.3}, // 1
      {0.3, 0.3, 0.3}, // 2
      {0.3, 0.3, 0.3}, // 3
      {0.3, 0.3, 0.3}, // 4
      {0.3, 0.3, 0.3}, // 5
      {0.3, 0.3, 0.3}, // 6
      {0.3, 0.3, 0.3}, // 7
      {1.0, 1.0, 1.0},
      {1.0, 1.0, 1.0},
   }; 
}

P2MallaTriangulada::P2MallaTriangulada(unsigned n){
   
   //double desp = 0.0;
   double desp = 1.0;

   //vertices
   for (float i=0; i<5*n; i++) {
      vertices.push_back({-2.0, 0.0, desp*i*(-1)});
      vertices.push_back({-1.0, 0.0, desp*i*(-1)});
      vertices.push_back({0.0, 1.0, desp*i*(-1)});//el apice
      vertices.push_back({1.0, 0.0, desp*i*(-1)});
      vertices.push_back({2.0, 0.0, desp*i*(-1)});
      
   }

   //triangulos
   // for(int i=1; i<n; i++){
   //    for (unsigned j=0; j<5*n; j++) {
         
         
   //       //int k = n+j;
   //       if(1+i*4==j and j!=0){   //la ultima cara de cada piso va diferente
   //          triangulos.push_back({j, j+4, j+5});
   //       }else{
   //          if (j!=5*n){   //si no es la ultima iteracion (para que no una con la planta baja)
   //             triangulos.push_back({j, j+1, j+5});
   //             triangulos.push_back({j+1, j+5, j+6});
   //          }
   //       }
         
   //    }
   // }
   for (unsigned j=0; j<5*n; j++) {
      int contador=0;
      if(j%4){ //cada fila aumento
         contador++;
      }
      //int k = n+j;
      if((j-4)%4==0 and j!=0){   //la ultima cara de cada piso va diferente
         triangulos.push_back({j, j+4, j+5});
      }else{
         if (j!=5*n){   //si no es la ultima iteracion (para que no una con la planta baja)
            triangulos.push_back({j, j+1, j+5});
            triangulos.push_back({j+1, j+5, j+6});
         }
      }
      
   }

   // triangulos.push_back({0,1, 5});
   // triangulos.push_back({1,5, 6});

   
   
}

GrafoCubosVertical::GrafoCubosVertical(unsigned h, unsigned w)
{
   
   NodoGrafoEscena *cubo1 = new NodoGrafoEscena();
   //unsigned ind_rot_cubo1=cubo1->agregar(rotate(float(0.0), vec3({1.0,0.0,0.0})));
   unsigned ind_altura_patas = cubo1->agregar(scale(vec3(1.0,1.0,1.0)));
   unsigned ind_tras_vert =cubo1-> agregar(translate(vec3(0.0,0.0,0.0)));
   
   cubo1->agregar(scale(vec3(1.0,h/1.0,w/1.0)));
   cubo1->agregar(new Cubo());
   agregar(cubo1);


   NodoGrafoEscena *cubo2 = new NodoGrafoEscena();
   unsigned ind_rot_cubo2=cubo2->agregar(rotate(float(0.0), vec3({1.0,0.0,0.0})));
   cubo2->agregar(translate(vec3(0.0,h+2.0,w-1.0)));
   cubo2->agregar(scale(vec3(1.0,h/1.0,w/1.0)));
   cubo2->agregar(new Cubo());
   agregar(cubo2);

   pm_altura_patas = cubo1->leerPtrMatriz(ind_altura_patas);
   pm_tras_vert = cubo1->leerPtrMatriz(ind_tras_vert);
   pm_rot_cubo2=cubo2->leerPtrMatriz(ind_rot_cubo2);
}

unsigned GrafoCubosVertical::leerNumParametros() const {
    return 2;
}

void GrafoCubosVertical::actualizarEstadoParametro(const unsigned iParam, const float t_sec) {
   
    float v;
    switch (iParam)
    {
    case 0:
         v = 1.0+0.5*sin(M_PI*(1/1.3)*t_sec);
        *pm_tras_vert = scale(vec3(1.0,v,1.0));
        *pm_altura_patas = translate(vec3(0.0,-0.5 * (v - 1),0.0));
        break;
    case 1:
    
      alpha = M_PI / 4 * -fabs(sin(- 2*M_PI * t_sec / 8));
        *pm_rot_cubo2 =  (translate(vec3(1.0,2.0,0.0))) * 
                        (rotate(float(alpha), vec3(0.0,0.0,1.0)))*
                        (translate(vec3(-1.0,-2.0,0.0)));
        break;
    default:
        break;
    }


}


