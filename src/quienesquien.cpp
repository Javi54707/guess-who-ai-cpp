#include "../include/quienesquien.h"
#include <sstream>
#include <iostream>
#include <iterator>
#include <math.h>
#include <algorithm>

QuienEsQuien::QuienEsQuien(){
}

QuienEsQuien::QuienEsQuien(const QuienEsQuien &quienEsQuien){
}

QuienEsQuien& QuienEsQuien::operator= (const QuienEsQuien &quienEsQuien){
     return *this;
}

QuienEsQuien::~QuienEsQuien(){
     this->limpiar() ;
}

void QuienEsQuien::limpiar(){
     this->personajes.clear();
     this->atributos.clear();
     this->personajes_images.clear();
     for(vector<vector<bool>>::iterator it_tablero = this->tablero.begin();
         it_tablero != this->tablero.end();
         it_tablero++){
          (*it_tablero).clear();
     }
     this->tablero.clear();
     this->arbol.clear() ;
}

template <typename T>
std::ostream& operator<<  (std::ostream& out, const std::vector<T>& v) {
     if ( !v.empty() ) {
          out <<  '[' ;
          std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
          out <<  "\b\b]";
     }

     return out;
}

void QuienEsQuien::mostrar_estructuras_leidas(){
     cout <<  "personajes: " <<  (this->personajes) <<  endl;
     cout <<  "atributos: " <<  (this->atributos) <<  endl;
     cout <<  "tablero:    " <<  endl;
     // Escribe la cabecera del tablero
     for(vector<string>::iterator it_atributos = this->atributos.begin() ;
         it_atributos != this->atributos.end() ;
         it_atributos++){
          cout <<  *it_atributos << "\t";
     }
     cout << endl;

     int indice_personaje = 0;
     for(vector<vector<bool>>::iterator it_tablero_atributos = tablero.begin();
         it_tablero_atributos!= tablero.end();
         it_tablero_atributos++){
          string personaje = this->personajes[indice_personaje];
          int indice_atributo = 0;
          for(vector<bool>::iterator it_atributos_personaje = (*it_tablero_atributos).begin();
              it_atributos_personaje != (*it_tablero_atributos).end() ;
              it_atributos_personaje++){
               cout <<  *it_atributos_personaje<< "\t";
               indice_atributo++;
          }
          cout <<  personaje <<  endl;
          indice_personaje++;
     }
}

/**
  * @brief Devuelve una copia de la cadena original sin las subcadenas no deseadas.
  *
  * @param cadena_original Cadena de la que se eliminan las subcadenas no deseadas.
  * @param cadena_a_eliminar Subcadena que se busca y se elimina.
  *
  * @return Copia de la cadena original sin las subcadenas no deseadas.
  */


string limpiar_string(string cadena_original,string cadena_a_eliminar){
     string linea(cadena_original);
     while(linea.find_first_of(cadena_a_eliminar) != std::string::npos){
          linea.erase(linea.find_first_of(cadena_a_eliminar),cadena_a_eliminar.length());
     }

     return linea;
}

istream& operator >>  (istream& is, QuienEsQuien &quienEsQuien) {
     quienEsQuien.limpiar();
     if(is.good()){ //Lee la linea con los nombres de las columnas
          string linea;
          getline(is, linea, '\n');
          linea = limpiar_string(linea,"\r");
          bool salir=false;
          while(!salir && linea.find('\t') != string::npos ){
               string atributo = linea.substr(0,linea.find('\t'));
               quienEsQuien.atributos.push_back(atributo);
               linea = linea.erase(0,linea.find('\t')+1);
               if (linea.substr(0,linea.find('\t'))=="Nombre personaje")
                   salir=true;
          }
     }
     //LEE los atributos para cada personaje
     while( is.good()) {
          string linea;
          getline(is, linea, '\n');
          linea = limpiar_string(linea,"\r");

          //Si la linea contiene algo extrae el personaje.
          //Si no la ignora.
          if(linea != ""){;
               vector<bool> atributos_personaje;
               int indice_atributo=0;
               bool salir=false;
               while(!salir && linea.find('\t') != string::npos){
                   string valor = linea.substr(0,linea.find('\t'));
                   if (valor == "0" || valor == "1"){
                    bool valor_atributo = valor == "1";
                    atributos_personaje.push_back(valor_atributo);
                    linea = linea.erase(0,linea.find('\t')+1) ;
                    indice_atributo++;
                   }
                   else{
                       salir=true;
                   }
               }
               string nombre_personaje;
               string imagen_personaje;
               if (linea.find('\t') == string::npos){
                nombre_personaje = linea;
                imagen_personaje=""; //no tiene imagen
               }
               else{
                nombre_personaje=linea.substr(0,linea.find('\t'));
                linea = linea.erase(0,linea.find('\t')+1) ;
                imagen_personaje=linea;

               }
               quienEsQuien.personajes.push_back(nombre_personaje);
               quienEsQuien.tablero.push_back(atributos_personaje);
               quienEsQuien.personajes_images.push_back(imagen_personaje);
          }
     }

     quienEsQuien.mostrar_estructuras_leidas();
     quienEsQuien.arbol = quienEsQuien.crear_arbol();
     quienEsQuien.tg=nullptr; //puntero al tablero grafico
     quienEsQuien.con=nullptr; //puntero a la consola
     if (quienEsQuien.modo_graph){
        int nper = quienEsQuien.personajes.size();
        int nrows,ncols=3;
        if (nper<8)
           nrows = nper/ncols;
        else{
            ncols=4;
            nrows = nper/ncols;
        }
        //se crea un tablero grafico
        quienEsQuien.tg = new TableroGrafico(400,400,nrows,ncols);
        //ponemos en el tablero grafico las im�genes
        for (int i=0;i<nper;i++){

            quienEsQuien.tg->putImagen(i/ncols,i%ncols,quienEsQuien.personajes_images[i].c_str());
        }
        //creamos la consola
        quienEsQuien.con=new Consola();
     }
     return is;
}

ostream& operator << (ostream& os, const QuienEsQuien &quienEsQuien){
     //Escribimos la cabecera, que contiene los atributos y al final una columna para el nombre

     cout<<"NUmero de Atributos "<<quienEsQuien.atributos.size()<<endl;
     for(vector<string>::const_iterator it_atributos = quienEsQuien.atributos.begin();
         it_atributos != quienEsQuien.atributos.end();
         it_atributos++){
          os << *it_atributos << "\t";
     }

     os << "Nombre personaje" << endl;
     
     //Rellenamos con ceros y unos cada linea y al final ponemos el nombre del personaje.
     for(int indice_personaje=0;indice_personaje<quienEsQuien.personajes.size();indice_personaje++){
          for(int indice_atributo=0;indice_atributo<quienEsQuien.atributos.size();indice_atributo++){
               os << quienEsQuien.tablero[indice_personaje][indice_atributo] << "\t";
          }
          os << quienEsQuien.personajes[indice_personaje] << endl;
     }
     
     return os;
}

/**
* @brief Convierte un numero a un vector de bool que corresponde
*        con su representadon en binario con un numero de digitos
*              f i j o.
*
* @param n Numero a convertir en binario.
* @param digitos Numero de digitos de la representaddn binaria.
*
* @return Vector de booleanos con la representadon en binario de @e n
*      con el numero de elementos especificado por @e digitos.
*/
vector<bool> convertir_a_vector_bool(int n, int digitos) {
     vector<bool> ret;
     while(n) {
          if (n&1)
               ret.push_back(true);
          else
               ret.push_back(false);

          n>>=1;
     }

     while(ret.size()<digitos)
          ret.push_back(false);

     reverse(ret.begin(),ret.end());
     return ret;
}

/**
* @brief Esta es una propuesta de cabecera de la funcion para crear el arbol.
*/
bintree<Pregunta> QuienEsQuien::crear_arbol( vector<string> atributos, int indice_atributo, vector<string> personajes,
                                             vector<bool> personajes_restantes, vector<vector<bool>> tablero){
     if (personajes.size() == 1) {
          // Si solo queda un personaje, es la hoja
          Pregunta pregunta_final(personajes[0],1);
          bintree<Pregunta> arbol_final(pregunta_final);
          return arbol_final;
     }

     // Si aún tenemos más de un personaje, dividimos el árbol
     int atributo_actual = indice_atributo % atributos.size();
     vector<bool> valores_atributo;
     for (int i = 0; i < personajes.size(); i++) {
          valores_atributo.push_back(tablero[i][atributo_actual]);
     }

     // Dividir personajes en función de su valor en el atributo
     vector<string> personajes_izquierda, personajes_derecha;
     vector<bool> personajes_izquierda_restantes, personajes_derecha_restantes;
     for (int i = 0; i < personajes.size(); i++) {
          if (valores_atributo[i]) {
               personajes_izquierda.push_back(personajes[i]);
               personajes_izquierda_restantes.push_back(personajes_restantes[i]);
          } else {
               personajes_derecha.push_back(personajes[i]);
               personajes_derecha_restantes.push_back(personajes_restantes[i]);
          }
     }

     // Creamos los nodos de la izquierda y derecha
     Pregunta pregunta_actual(atributos[atributo_actual]);
     bintree<Pregunta> arbol(pregunta_actual);
     arbol.insert_left(arbol.root(), crear_arbol(atributos, indice_atributo + 1, personajes_izquierda, personajes_izquierda_restantes, tablero));
     arbol.insert_right(arbol.root(), crear_arbol(atributos, indice_atributo + 1, personajes_derecha, personajes_derecha_restantes, tablero));

     return arbol;
}

bintree<Pregunta> QuienEsQuien::crear_arbol(){
     int indice_atributo = 0;
     vector<bool> personajes_restantes;
     for(vector<string>::iterator it = personajes.begin();it!=personajes.end();it++){
          personajes_restantes.push_back(true);
     }

     return crear_arbol( this->atributos, indice_atributo, this->personajes,
                         personajes_restantes, this->tablero);
}

void QuienEsQuien::usar_arbol(bintree<Pregunta> arbol_nuevo){
     arbol = arbol_nuevo;
}

void QuienEsQuien::iniciar_juego(){
     Ventana v(tg, con, "WisW");

     bintree<Pregunta>::node jugada_actual = arbol.root();
     set<string> personajes_activos(personajes.begin(), personajes.end());
     string respuesta;

     while (!jugada_actual.null() && personajes_activos.size() > 1) {
          cout << "Pregunta: " << *jugada_actual << " (s/n): ";
          cin >> respuesta;

          if (respuesta == "s") {
               jugada_actual = jugada_actual.left();
          } else if (respuesta == "n") {
               jugada_actual = jugada_actual.right();
          } else {
               cout << "Respuesta no válida. Use 's' o 'n'." << endl;
          }

          personajes_activos = informacion_jugada(jugada_actual);
          if (modo_graph) ocultar_personajes_graph(personajes_activos);
     }

     if (personajes_activos.size() == 1) {
          cout << "Tu personaje es: " << *personajes_activos.begin() << endl;
     } else {
          cout << "No he podido adivinar tu personaje." << endl;
     }

     if (modo_graph) {
          char c;
          do {
               con->WriteText("Pulsa 0 para salir.");
               c = con->ReadChar();
          } while (c != '0');
     }

     v.cerrar();
}	

set<string> QuienEsQuien::informacion_jugada(bintree<Pregunta>::node jugada_actual){
     set<string> personajes_levantados;
     // Recorremos el tablero y encontramos los personajes que coinciden con la pregunta
     for (int i = 0; i < personajes.size(); i++) {
          if (jugada_actual.null()) {
               break;
          }
          bool coincide = false;
          if (jugada_actual.left().null() && jugada_actual.right().null()) {
               // Si es una hoja, agregamos el personaje
               personajes_levantados.insert(personajes[i]);
          } else {
               bool atributo = tablero[i][jugada_actual->getAtributoIndex()];
               if ((atributo && !jugada_actual.left().null()) || (!atributo && !jugada_actual.right().null())) {
                    personajes_levantados.insert(personajes[i]);
               }
          }
     }
     return personajes_levantados;
}

void escribir_esquema_arbol(ostream& ss, const bintree <Pregunta>& a,
                            bintree<Pregunta>::node n, string& pre){
     if (n.null())
          ss <<  pre <<  "-- x" <<  endl;
     else {
          ss <<  pre <<  "-- " <<  (*n) <<  endl;
          if ( !n.right().null() || !n.left().null()) {// Si no es una hoja
               pre += "   |";
               escribir_esquema_arbol(ss,a, n.right(), pre);
               pre.replace(pre.size()-4, 4, "    ");
               escribir_esquema_arbol (ss,a, n.left(), pre);
               pre.erase(pre.size()-4, 4);
          }
     }
}

void QuienEsQuien::escribir_arbol_completo() const{
     string pre = "";
     escribir_esquema_arbol(cout,this->arbol,this->arbol.root(),pre);
}

void QuienEsQuien::eliminar_nodos_redundantes(){
// TODO :)
}

float QuienEsQuien::profundidad_promedio_hojas(){
     int suma_profundidades = 0;
     int contador_hojas = 0;

     function<void(bintree<Pregunta>::node, int)> calcular_profundidad = [&](bintree<Pregunta>::node n, int profundidad) {
          if (n.null()) return;

          if (n.left().null() && n.right().null()) { // Es hoja
               suma_profundidades += profundidad;
               contador_hojas++;
          } else {
               calcular_profundidad(n.left(), profundidad + 1);
               calcular_profundidad(n.right(), profundidad + 1);
          }
     };

     calcular_profundidad(arbol.root(), 0);

     if (contador_hojas == 0) return 0; // Evitar división por cero
     return static_cast<float>(suma_profundidades) / contador_hojas;
}

void QuienEsQuien::aniadir_personaje(const string &nombre_personaje, const vector<bool> &atributos) {
     // Añadir el personaje a la lista de personajes
     personajes.push_back(nombre_personaje);

     // Añadir las respuestas de los atributos a la tabla de atributos
     tablero.push_back(atributos);

     // Añadir el personaje a la lista de personajes restantes
     vector<bool> personajes_restantes(personajes.size(), true);
     personajes_restantes[personajes.size() - 1] = true;

     // Regenerar el árbol con el nuevo personaje
     arbol = crear_arbol(atributos_posibles, 0, personajes, personajes_restantes, tablero);
}

void QuienEsQuien::eliminar_personaje(const string &nombre_personaje) {
     // Buscar el índice del personaje a eliminar
     auto it = find(personajes.begin(), personajes.end(), nombre_personaje);
     if (it != personajes.end()) {
          int index = distance(personajes.begin(), it);

          // Eliminar el personaje de la lista y de la tabla
          personajes.erase(it);
          tablero.erase(tablero.begin() + index);

          // Regenerar el árbol de decisiones
          arbol = crear_arbol(atributos_posibles, 0, personajes, personajes_restantes, tablero);
     } else {
          cout << "El personaje no existe." << endl;
     }
}

vector<string> QuienEsQuien::preguntas_formuladas() {
     vector<string> preguntas;
     // Recorrer el árbol de decisiones y extraer las preguntas
     obtener_preguntas_recursivo(arbol.root(), preguntas);
     return preguntas;
}

void QuienEsQuien::obtener_preguntas_recursivo(bintree<Pregunta>::node nodo, vector<string> &preguntas) {
     if (nodo.null()) return;

     // Si el nodo es una pregunta, la agregamos a la lista
     if (!nodo.left().null() && !nodo.right().null()) {
          preguntas.push_back(nodo->getPregunta());
     }

     // Recursión para los nodos izquierdo y derecho
     obtener_preguntas_recursivo(nodo.left(), preguntas);
     obtener_preguntas_recursivo(nodo.right(), preguntas);
}

/**
* @brief Genera numero enteros positivos aleatorios en el rango [min,max].
**/
int generaEntero(int min, int max){
     int tam= max-min;
     return ((rand( )%tam)+min) ;
}

void QuienEsQuien::tablero_aleatorio(int numero_de_personajes){
     srand(0);
     this->limpiar();
     float log_2_numero_de_personajes = log(numero_de_personajes)/log(2);
     int numero_de_atributos = ceil(log_2_numero_de_personajes);

     cout <<  "Peticion para generar " <<  numero_de_personajes << " personajes ";
     cout <<  "con " << numero_de_atributos <<  " atributos" << endl;
     cout <<  "Paso 1: generar " <<  pow(2, numero_de_atributos) <<  " personajes" << endl;

     //Fase 1: completar el tablero con todos los personajes posibles
     //Complete el tablero y los nombres de personajes a la vez
     for(int indice_personaje=0;indice_personaje< pow(2,numero_de_atributos);indice_personaje++){
          vector<bool> atributos_personaje =
                                    convertir_a_vector_bool(indice_personaje,numero_de_atributos);
          string nombre_personaje = "Personaje_"+to_string(indice_personaje);

          this->personajes.push_back(nombre_personaje);
          this->tablero.push_back(atributos_personaje);
     }

     // Completo los nombres de los atributos.
     for( int indice_atributo=0;indice_atributo<numero_de_atributos;indice_atributo++){
          string nombre_atributo = "Atributo_"+to_string(indice_atributo);
          this->atributos.push_back(nombre_atributo);
     }
     cout <<  "Paso 2: eliminar " << (pow(2,numero_de_atributos)-numero_de_personajes) <<  "personajes" << endl;

     //Fase 2. Borrar personajes aleatoriamente hasta que quedan solo los que hemos pedido.
     while(personajes.size()>numero_de_personajes){
          int personaje_a_eliminar = generaEntero(0,personajes.size());
          personajes.erase(personajes.begin()+personaje_a_eliminar);
          tablero.erase(tablero.begin()+personaje_a_eliminar);
     }
}
void QuienEsQuien::ocultar_personajes_graph(const set<string> &personajes_activos){
    //ocultamos los personajes
    int idx=0;
    int ncols=tg->getNcols();
    for (auto it=personajes.begin();it!=personajes.end();++it,idx++){
        if ( personajes_activos.find(*it)==personajes_activos.end())
            tg->putImagen(idx/ncols,idx%ncols,imagen_ocultar.c_str());
    }
}


void QuienEsQuien::setImagenOcultar(const char * n){
    imagen_ocultar=n;
}

void QuienEsQuien::setModoGraph(bool m){
    modo_graph=m;
}


