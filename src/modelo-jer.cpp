// Nombre: María,  Apellidos: Cribillés Pérez, Titulación: GIM
// email: mariacribilles@correo.ugr.es, DNI o pasaporte: 75573410X

#include "grafo-escena.h"
#include "malla-ind.h"
#include "ig-aux.h"
#include "malla-revol.h"
#include "modelo-jer.h"


using namespace std;
using namespace glm;

// -----------------------------------------------------------------------------------------------
// Clase Malla Torre

MallaTorrePuente::MallaTorrePuente(unsigned n)
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

//    //los vertices color RGB igual a las componentes XYZ
//    for(float i=0; i<vertices.size(); i++) {
//       col_ver.push_back(vec3(108.0, 58.0, 42.0));
//    } 
    // for (size_t i = 0; i < vertices.size(); i++) {
    //     col_ver.push_back(vec3(108.0 / 255.0, 58.0 / 255.0, 42.0 / 255.0)); //marron
    // }
   

}



// Clase Cubo24
CuboPuente::CuboPuente ()
:  MallaInd( "Cubo24" )
{
   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0,  1.0, -1.0 }, // 1
         {  1.0,  1.0, -1.0 }, // 2

         {  1.0, -1.0, -1.0 }, // 3
         {  1.0, -1.0, -1.0 }, // 4
         {  1.0,  1.0, -1.0 }, // 5

         {  1.0,  1.0,  1.0 }, // 6
         {  1.0, -1.0,  1.0 }, // 7
         {  1.0, -1.0,  1.0 }, // 8

         {  1.0,  1.0,  1.0 }, // 9
         { -1.0,  1.0,  1.0 }, // 10
         { -1.0, -1.0,  1.0 }, // 11

         { -1.0, -1.0,  1.0 }, // 12
         { -1.0,  1.0,  1.0 }, // 13
         { -1.0,  1.0, -1.0 }, // 14

         { -1.0, -1.0, -1.0 }, // 15
         { -1.0, -1.0, -1.0 }, // 16
         { -1.0, -1.0,  1.0 }, // 17

         {  1.0, -1.0,  1.0 }, // 18
         {  1.0, -1.0, -1.0 }, // 19
         {  1.0,  1.0, -1.0 }, // 20

         { -1.0,  1.0, -1.0 }, // 21
         { -1.0,  1.0,  1.0 }, // 22
         {  1.0,  1.0,  1.0 }  // 23
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

// -----------------------------------------------------------------------------
//Clase Cilindro (hueco)
CilindroPuente::CilindroPuente(const int num_verts_per, const unsigned nperfiles){

   //perfil original (base centro en el origen, radio y altura 1)
   std::vector<glm::vec3> perfil = std::vector<glm::vec3>();   //creamos e inicializamos perfil

   for(int i=0; i<num_verts_per; i++){
      float altura = i*(1.0/(num_verts_per-1));
      perfil.push_back(glm::vec3(1.0,altura,0.0));
   }
   inicializar(perfil,nperfiles);

//    for (size_t i = 0; i < vertices.size(); i++) {
//         col_ver.push_back(vec3(255.0/255.0, 255.0/255.0, 191.9/255.0));  // beys
//     }
    //cilindro_der->ponerColor({245.0,245.0,220.0});
}


PuenteElevadizo::PuenteElevadizo(){
    
    
    //Todo el puente elevadizo en general con todos los componentes---------------------------------------------------------
    ponerNombre("PuenteElevadizo");
    unsigned ident = 1;
    NodoGrafoEscena *puente_elevadizo = new NodoGrafoEscena();
    puente_elevadizo->ponerIdentificador(0);


    //Soporte: 4 torres de mallas---------------------------------------------------------------------------------------
    NodoGrafoEscena *soporte = new NodoGrafoEscena();

    soporte->ponerNombre("Soporte con torres");
    //soporte->ponerIdentificador(ident);
    //ident++;
    

    for (int i = 0; i < 4; i++) {   //soporte=4 torres
        NodoGrafoEscena *torre = new NodoGrafoEscena();
        torre->ponerNombre("Torre " + std::to_string(i + 1));   //enumero torres
        torre->ponerIdentificador(ident);
        ident++;
        torre->agregar(translate(vec3((i % 2 == 0 ? -2 : 2), 0.0, (i < 2 ? -2 : 2))));
        torre->agregar(scale(vec3(0.5, 1.0, 0.5))); //reduce el ancho x, aumento la altura (y) en 2 y reduzco profundidad en z
        TexturaXY *textura_soporte=new TexturaXY("rojometalico.jpeg");
        Material* material_soporte = new Material(textura_soporte, 0.75, 0.65, 0.25, 50.0);
        material_soporte->ponerNombre("MaterialSoporte");
        torre->agregar(material_soporte);
        torre->agregar(new MallaTorrePuente(4));  //malla torre de malla-ind con 3 pisos
        soporte->agregar(torre);
    }

    agregar(soporte);

    
   


    // Cilindros---------------------------------------------------------------------------------------
    NodoGrafoEscena *cilindros = new NodoGrafoEscena();

    //cilindro izquierda
    NodoGrafoEscena *cilindro_izq = new NodoGrafoEscena();
    cilindro_izq->ponerNombre("Cilindro Izquierdo");
    cilindro_izq->ponerIdentificador(ident);
    ident++;

    
    cilindro_izq->agregar(translate(vec3(-2.0, 3.0, 2.0)));
    unsigned ind_rot_cilindro_izq= cilindro_izq->agregar(rotate(float(M_PI/2),vec3{1.0,0.0,0.0}));  
    cilindro_izq->agregar(scale(vec3(0.2, 1.0, 0.2)));
    
    //cilindro_izq->ponerColor({245.0,245.0,220.0});
    //TexturaXY *textura_cilindro=new TexturaXY("texturacilindro.jpeg"); el cilindro lo dejo como plano, sin textura
    //Material* material_cilindro = new Material(textura_cilindro, 0.75, 0.65, 0.25, 50.0);
    //cilindro_izq->agregar(material_cilindro);
    cilindro_izq->agregar(new CilindroPuente(20, 20));
    agregar(cilindro_izq);

    //cilindro derecha
    NodoGrafoEscena *cilindro_der = new NodoGrafoEscena();
    cilindro_der->ponerNombre("Cilindro Derecho");
    cilindro_der->ponerIdentificador(ident);
    ident++;

    
    cilindro_der->agregar(translate(vec3(2.0, 3.0, 2.0)));
    unsigned ind_rot_cilindro_der= cilindro_der->agregar(rotate(float(M_PI/2),vec3{1.0,0.0,0.0}));
    cilindro_der->agregar(scale(vec3(0.2, 1.0, 0.2)));
    // TexturaXY *textura_cilindro=new TexturaXY("texturacilindro.jpeg");
    // Material* material_cilindro = new Material(textura_cilindro, 0.75, 0.65, 0.25, 50.0);
    //cilindro_der->agregar(material_cilindro);
    cilindro_der->agregar(new CilindroPuente(20, 20));
    agregar(cilindro_der);


    cilindros->agregar(cilindro_izq);
    cilindros->agregar(cilindro_der);




    // Puentes---------------------------------------------------------------------------------------
    NodoGrafoEscena *puentes = new NodoGrafoEscena();
    Textura *textura_puente=new Textura("piedrapuente.jpeg");
    Material* material_puente = new Material(textura_puente, 0.7, 0.6, 0.3, 50.0);
    material_puente->ponerNombre("MaterialPuente");
   

    NodoGrafoEscena *puente_izq = new NodoGrafoEscena();
    puente_izq->ponerNombre("Puente Izquierdo");
    puente_izq->ponerIdentificador(ident++);
   

    //rotacion en el eje x (angulo inicial 0)
    unsigned ind_rot_puente_izq = puente_izq->agregar(rotate(0.0f, vec3(1.0, 0.0, 0.0)));
    puente_izq->agregar(translate(vec3(-0.875, 2.0, 0.25)));
    puente_izq->agregar(scale(vec3(1.125, 0.1, 1.75)));  
    puente_izq->agregar(material_puente);  
    puente_izq->agregar(new CuboPuente());
    agregar(puente_izq);

    NodoGrafoEscena *puente_der = new NodoGrafoEscena();
    puente_der->ponerNombre("Puente Derecho");
    puente_der->ponerIdentificador(ident++);
    

    unsigned ind_rot_puente_der = puente_der->agregar(rotate(0.0f, vec3(1.0, 0.0, 0.0)));
    puente_der->agregar(translate(vec3(1.375, 2.0, 0.25)));
    puente_der->agregar(scale(vec3(1.125, 0.1, 1.75)));
    puente_der->agregar(material_puente);
    puente_der->agregar(new CuboPuente());
    agregar(puente_der);


    puentes->agregar(puente_izq);
    puentes->agregar(puente_der);


    //Cabina ----------------------------------------------------------------------
    NodoGrafoEscena *cabina = new NodoGrafoEscena();
    TexturaXZ *textura_ascensor=new TexturaXZ("textura_ascensor.jpeg");
    Material* material_ascensor = new Material(textura_ascensor, 0.5, 0.6, 0.3, 50.0);
    material_ascensor->ponerNombre("MaterialAscensor");

    cabina->ponerNombre("Cabina de seguridad");
    cabina->ponerIdentificador(ident++);

    unsigned ind_tras_cabina= cabina->agregar(translate(vec3(2.25, 3.0, -2.35)));
    cabina->agregar(scale(vec3(0.4, 0.25, 0.35)));   
    cabina->agregar(material_ascensor);
    cabina->agregar(new CuboPuente());
    agregar(cabina);


    // Punteros a las matrices de rotación
    puente_elevadizo->agregar(soporte);
    puente_elevadizo->agregar(cilindros);
    puente_elevadizo->agregar(puentes);
    puente_elevadizo->agregar(cabina);

    agregar(puente_elevadizo);
    
    pm_rot_puente_izq = puente_izq->leerPtrMatriz(ind_rot_puente_izq);
    pm_rot_puente_der = puente_der->leerPtrMatriz(ind_rot_puente_der);

    pm_rot_cilindro_izq= cilindro_izq->leerPtrMatriz(ind_rot_cilindro_izq);
    pm_rot_cilindro_der= cilindro_der->leerPtrMatriz(ind_rot_cilindro_der);


    pm_tras_cabina= cabina->leerPtrMatriz(ind_tras_cabina);
    

}

unsigned PuenteElevadizo::leerNumParametros() const {
    return 6;
}

//declaro pivotes para 
float pivote_pizq_x = -2.0f; 
float pivote_pizq_y = 2.0f;                    
float pivote_pizq_z = 0.0f;                   

float pivote_pder_x = 2.5f; 
float pivote_pder_y = 2.0f;                    
float pivote_pder_z = 0.0f;  

//punto inicial cilindros
// vec3 punto_inicial_cilindro_izq=vec3(-2.0, 3.0, 2.0);
// vec3 punto_inicial_cilindro_der=vec3(2.0, 3.0, 2.0);


// //angulo en el que hemos trasladado el cilindro
// float tita = M_PI/2;

// //matriz de rotacion en el eje x
// std::vector<std::vector<float>> matriz_rotacion = {{
//         {1.0f, 0.0f, 0.0f},
//         {0.0f, cos(tita), -sin(tita)},
//         {0.0f, sin(tita), cos(tita)}
// }};

// //traslado el punto cil izq
// glm::vec3 punto_trasladado_cilindro_izq = {
//     matriz_rotacion[0][0]*punto_inicial_cilindro_izq[0] + matriz_rotacion[0][1]*punto_inicial_cilindro_izq[1] + matriz_rotacion[0][2]*punto_inicial_cilindro_izq[2],
//     matriz_rotacion[1][0]*punto_inicial_cilindro_izq[0] + matriz_rotacion[1][1]*punto_inicial_cilindro_izq[1] + matriz_rotacion[1][2]*punto_inicial_cilindro_izq[2],
//     matriz_rotacion[2][0]*punto_inicial_cilindro_izq[0] + matriz_rotacion[2][1]*punto_inicial_cilindro_izq[1] + matriz_rotacion[2][2]*punto_inicial_cilindro_izq[2]
// };        

// //traslado el punto cil der
// glm::vec3 punto_trasladado_cilindro_der = {
//     matriz_rotacion[0][0]*punto_inicial_cilindro_der[0] + matriz_rotacion[0][1]*punto_inicial_cilindro_der[1] + matriz_rotacion[0][2]*punto_inicial_cilindro_der[2],
//     matriz_rotacion[1][0]*punto_inicial_cilindro_der[0] + matriz_rotacion[1][1]*punto_inicial_cilindro_der[1] + matriz_rotacion[1][2]*punto_inicial_cilindro_der[2],
//     matriz_rotacion[2][0]*punto_inicial_cilindro_der[0] + matriz_rotacion[2][1]*punto_inicial_cilindro_der[1] + matriz_rotacion[2][2]*punto_inicial_cilindro_der[2]
// }; 

//punto inicial cabina
vec3 punto_inicial_cabina=vec3(2.25, 3.0, -2.35);


            
void PuenteElevadizo::actualizarEstadoParametro(const unsigned iParam, const float t_sec) {
    switch (iParam) {
        case 0:  //subir/bajar rotar puente izquierdo
            //rotate(ángulo_en_radianes, eje_de_rotación)
            

            *pm_rot_puente_izq = translate(vec3(pivote_pizq_x, pivote_pizq_y, pivote_pizq_z)) * 
                    rotate(float(M_PI / 4 * fabs(sin( 2*M_PI * t_sec / 8))), vec3(0.0, 0.0, 1.0)) *  //al poner el valor absoluto el seno solo es positivo y no baja el puente, solo sube
                    translate(vec3(-pivote_pizq_x, -pivote_pizq_y, -pivote_pizq_z));
            break;
        case 1: //subir/bajar rotar puente derecho
            //quiero que rote en el eje z
            *pm_rot_puente_der = translate(vec3(pivote_pder_x, pivote_pder_y, pivote_pder_z)) * 
                    rotate(float(M_PI / 4 * -fabs(sin(- 2*M_PI * t_sec / 8))), vec3(0.0, 0.0, 1.0)) *  
                    translate(vec3(-pivote_pder_x, -pivote_pder_y, -pivote_pder_z));
            break;
        case 2: //rotar cilindro izquierda
            // *pm_rot_cilindro_izq=translate(vec3(punto_trasladado_cilindro_izq))*
            //     rotate(float(2*M_PI * t_sec / 8), vec3(0.0, 0.0, 1.0))*
            //     translate(vec3(-punto_trasladado_cilindro_izq));

            // *pm_rot_cilindro_izq = 
            //     translate(vec3(-2.0, 3.0, 2.0)) *  // Posición original
            //     rotate(float(2 * M_PI * t_sec / 4), vec3(0.0, 0.0, 1.0)) * // Rotación en su eje local
            //     translate(vec3(2.0, -3.0, -2.0));  // Inversión de la posición


            *pm_rot_cilindro_izq=rotate(float(2*M_PI * t_sec / 16), vec3(0.0, 0.0, 1.0))* (*pm_rot_cilindro_izq);
            break;
        case 3:  //rotar cilindro derecha
            *pm_rot_cilindro_der=rotate(float(2*M_PI * t_sec / 16), vec3(0.0, 0.0, 1.0))* (*pm_rot_cilindro_der);
            // *pm_rot_cilindro_der=translate(vec3(punto_trasladado_cilindro_der))*
            //     rotate(float(2*M_PI * t_sec / 8), vec3(0.0, 0.0, 1.0))*
            //     translate(vec3(-punto_trasladado_cilindro_der));

            // *pm_rot_cilindro_der = 
            //     translate(vec3(2.0, 3.0, 2.0)) *  // Posición original
            //     rotate(float(2 * M_PI * t_sec / 4), vec3(0.0, 1.0, 0.0)) * // Rotación en su eje local
            //     translate(vec3(-2.0, -3.0, -2.0));  // Inversión de la posición

            break;
        case 4: //cambiar de altura puente izquierdo
            //*pm_tras_puente_izq=translate(vec3(pivote_pizq_x, pivote_pizq_y+t_sec, pivote_pizq_z));
            //bool estoy_subiendo
            // if (t_sec%15==0)
                *pm_tras_cabina=translate(vec3(punto_inicial_cabina))*
                    translate(vec3(0.0, 0.75*sin(t_sec), 0.0));
            break;
        case 5: //cambiar de altura puente derecho
            //*pm_tras_puente_der=translate(vec3(pivote_pder_x, pivote_pder_y+t_sec, pivote_pder_z));
            break;

    }
}


