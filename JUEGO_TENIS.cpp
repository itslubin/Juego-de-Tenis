
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

// Se declaran las constantes
const int LIM_INF_HAB = 1, LIM_SUP_HAB = 4, LIM_INF_VEL = 1, LIM_SUP_VEL = 4, ANCHO_PISTA = 7, LARGO_PISTA = 3, JUEGOS_SET = 3, DIM_ARRAY_GOLPES = ANCHO_PISTA + 2, DIM_ARRAY_TENISTAS = 10;
const bool MODO_DEBUG = false, JUEGO_ALEATORIO = true, DETALLES = true;

typedef enum {NADIE, TENISTA1, TENISTA2} tTenista;
typedef enum {NADA, QUINCE, TREINTA, CUARENTA, VENTAJA} tPuntosJuego;
typedef int tConteoGolpes[DIM_ARRAY_GOLPES];
typedef double tConteoProbabilidad[DIM_ARRAY_GOLPES];
struct tDatosPartido {
    int posicion;
    tPuntosJuego puntos = NADA;
    int juegos = 0;
    int golpes_ganadores = 0;
    tConteoGolpes golpes;
};
struct tDatosTenista {
    string iniciales;
    int habilidad;
    int velocidad;
    tDatosPartido datos_partido;
    int partidos_ganados = 0;
    int partidos_perdidos = 0;
};

typedef tDatosTenista tArray[DIM_ARRAY_TENISTAS];

struct tListaTenistas {
    tArray arrayT;
    int cont = 0;
};

// Se declaran los prototipos (para decir que la funcion existe), luego se llaman en el main y las implementamos después del main

int menu();
int introducirDato(int LIM_INF, int LIM_SUP);
tTenista quienSaca(tTenista jugador1, tTenista jugador2);
int golpeoBola(int posicion_tenista, int habilidad);
int correTenista(int posicion_tenista, int velocidad, int posicion_bola);
string puntosAstring(tPuntosJuego puntuacion);
string toString(tTenista tenista_enum, tDatosTenista datos_tenista1, tDatosTenista datos_tenista2);
tDatosTenista toDatoTenista(tTenista tenista_enum, tDatosTenista tenista1, tDatosTenista tenista2);
tTenista jugadorRival(tTenista jugador);
tTenista actualizarMarcador(tTenista ganador_punto, tDatosTenista& datos_tenista1, tDatosTenista& datos_tenista2);
void pintarPeloteo(tDatosTenista datos_tenista1, tDatosTenista datos_tenista2, tTenista bola_jugador, int pos_bola);
void pintarMarcador(const tDatosTenista& datos_tenista1, const tDatosTenista& datos_tenista2, tTenista servicio_para);
tTenista hayGanadorSet(tDatosTenista datos_tenista1, tDatosTenista datos_tenista2);
void mostrarEstadisticas(string iniciales, int& golpes_ganados, tConteoGolpes golpes);
tTenista lance(tTenista& jugadorActivo, tDatosTenista& datos_tenista1, tDatosTenista& datos_tenista2, int habilidad, int velocidad, int& posicion_tenista, int& posicion_bola);
tTenista jugarPunto(tTenista& jugadorActivo, tTenista servicio, int posicion_inicial, tDatosTenista& datos_tenista1, tDatosTenista& datos_tenista2, int habilidad, int velocidad, int posicion_tenista, int posicion_bola);
tTenista jugarJuego(tTenista& jugadorActivo, tTenista servicio, int posicion_inicial, tDatosTenista& datos_tenista1, tDatosTenista& datos_tenista2, int habilidad, int velocidad, int posicion_tenista, int posicion_bola);


bool cargar(tListaTenistas& listaT);
void guardar(const tListaTenistas& listaT);
void mostrar(const tListaTenistas& listaT);
void eliminarTenista(tListaTenistas& listaT, string iniciales);
void mostrarIniciales(const tListaTenistas& listaT);
int buscarIniciales(const tListaTenistas& listaT, string ini);
void introducirTenista(tListaTenistas& listaT);

tTenista jugarPartido(tDatosTenista& tenista1, tDatosTenista& tenista2);
void jugarTorneo(tListaTenistas& listaT, int indT1, int indT2, int indT3, int indT4);
void seleccionarTop4(const tListaTenistas& listaT, int& indT1, int& indT2, int& indT3, int& indT4);


int main() {
    // se declaran las variables (
    bool exito;
    int op, pos, pos1, pos2, pos3, pos4;
    string ini;
    tTenista ganador_punto = NADIE, ganador_juego = NADIE, ganador_set = NADIE, jugadorActivo, servicio, ganador_partido;
    tDatosTenista datos_tenista1, datos_tenista2, datos_tenista3, datos_tenista4;
    tListaTenistas listaT;
    ofstream archivo;

    // Editar la funcion de introducirTenista par que me añada un nuevo tenista en la lista
    cout << "---------------------" << endl << "SIMULADOR DE TENIS V3" << endl << "---------------------" << endl << endl;
    exito = cargar(listaT);
    if (!exito) {
        cout << "ERROR: No se ha podido cargar la lista!" << endl;
        // Me crea un archivo en caso de que exista uno
        archivo.open("tenistas.txt");
    }
    else {
        op = -1;
        while (op != 0) {
            op = menu();
            switch (op) {
                case 0:
                {
                    guardar(listaT);
                }
                    break;
                case 1:
                {
                    mostrar(listaT);
                }
                    break;
                case 2: 
                {
                    introducirTenista(listaT);
                }
                    break;
                case 3: 
                {
                    cout << "Iniciales de los tenistas: ";
                    mostrarIniciales(listaT);
                    cout << endl << "Introduce las iniciales del tenista a eliminar: ";
                    cin >> ini;
                    eliminarTenista(listaT, ini);
                }
                    break;
                case 4:
                {
                    cout << "Iniciales de los tenistas: ";
                    mostrarIniciales(listaT);
                    cout << endl;
                    // El usuario introduce los datos de los tenistas que son transferidos a variables locales del main()
                    do {
                        cout << "Introduce las iniciales del tenista 1 : ";
                        cin >> datos_tenista1.iniciales;
                        pos = buscarIniciales(listaT, datos_tenista1.iniciales);
                        if (pos == -1) {
                            cout << "     ERROR: No existe ningun tenista con esas iniciales" << endl;
                        }
                    } while (pos == -1);
                    do {
                        cout << "Introduce las iniciales del tenista 2 : ";
                        cin >> datos_tenista2.iniciales;
                        pos = buscarIniciales(listaT, datos_tenista2.iniciales);
                        if (pos == -1) {
                            cout << "     ERROR: No existe ningun tenista con esas iniciales" << endl;
                        }
                        if (datos_tenista2.iniciales == datos_tenista1.iniciales) {
                            cout << "     ERROR: Tenista repetido. Elija otro" << endl;
                        }
                    } while ((pos == -1) || datos_tenista2.iniciales == datos_tenista1.iniciales);

                    // paso todos los datos de los tenistas a mis variables locales, primero busco donde estan en mi lista y luego los paso
                    pos1 = buscarIniciales(listaT, datos_tenista1.iniciales);
                    datos_tenista1 = listaT.arrayT[pos1];
                    pos2 = buscarIniciales(listaT, datos_tenista2.iniciales);
                    datos_tenista2 = listaT.arrayT[pos2];

                    cout << endl << "Comienza el partido..." << endl << endl;

                    // se juega el partido
                    ganador_partido = jugarPartido(datos_tenista1, datos_tenista2);
                    cout << endl;
                
                    // actualizo el historico de los partidos ganados y perdidos

                    listaT.arrayT[pos1].partidos_ganados = datos_tenista1.partidos_ganados;
                    listaT.arrayT[pos1].partidos_perdidos = datos_tenista1.partidos_perdidos;
                    listaT.arrayT[pos2].partidos_ganados = datos_tenista2.partidos_ganados;
                    listaT.arrayT[pos2].partidos_perdidos = datos_tenista2.partidos_perdidos;
                }
                    break;
                case 5: 
                {
                    cout << "Iniciales de los tenistas: ";
                    mostrarIniciales(listaT);
                    cout << endl;
                    // El usuario introduce los datos de los tenistas que son transferidos a variables locales del main()
                    do {
                        cout << "Introduce las iniciales del tenista 1 : ";
                        cin >> datos_tenista1.iniciales;
                        pos = buscarIniciales(listaT, datos_tenista1.iniciales);
                        if (pos == -1) {
                            cout << "     ERROR: No existe ningun tenista con esas iniciales" << endl;
                        }
                    } while (pos == -1);
                    do {
                        cout << "Introduce las iniciales del tenista 2 : ";
                        cin >> datos_tenista2.iniciales;
                        pos = buscarIniciales(listaT, datos_tenista2.iniciales);
                        if (pos == -1) {
                            cout << "     ERROR: No existe ningun tenista con esas iniciales" << endl;
                        }
                        if (datos_tenista2.iniciales == datos_tenista1.iniciales) {
                            cout << "     ERROR: Tenista repetido. Elija otro" << endl;
                        }
                    } while ((pos == -1) || datos_tenista2.iniciales == datos_tenista1.iniciales);
                    do {
                        cout << "Introduce las iniciales del tenista 3 : ";
                        cin >> datos_tenista3.iniciales;
                        pos = buscarIniciales(listaT, datos_tenista3.iniciales);
                        if (pos == -1) {
                            cout << "     ERROR: No existe ningun tenista con esas iniciales" << endl;
                        }
                        if (datos_tenista3.iniciales == datos_tenista1.iniciales || datos_tenista3.iniciales == datos_tenista2.iniciales) {
                            cout << "     ERROR: Tenista repetido. Elija otro" << endl;
                        }
                    } while ((pos == -1) || datos_tenista3.iniciales == datos_tenista1.iniciales || datos_tenista3.iniciales == datos_tenista2.iniciales);
                    do {
                        cout << "Introduce las iniciales del tenista 4 : ";
                        cin >> datos_tenista4.iniciales;
                        pos = buscarIniciales(listaT, datos_tenista4.iniciales);
                        if (pos == -1) {
                            cout << "     ERROR: No existe ningun tenista con esas iniciales" << endl;
                        }
                        if (datos_tenista4.iniciales == datos_tenista1.iniciales || datos_tenista4.iniciales == datos_tenista2.iniciales || datos_tenista4.iniciales == datos_tenista3.iniciales) {
                            cout << "     ERROR: Tenista repetido. Elija otro" << endl;
                        }
                    } while ((pos == -1) || datos_tenista4.iniciales == datos_tenista1.iniciales || datos_tenista4.iniciales == datos_tenista2.iniciales || datos_tenista4.iniciales == datos_tenista3.iniciales);

                    // busco las posiciones en las que estan los tenistas que ha seleccionado el usuario
                    pos1 = buscarIniciales(listaT, datos_tenista1.iniciales);
                    pos2 = buscarIniciales(listaT, datos_tenista2.iniciales);
                    pos3 = buscarIniciales(listaT, datos_tenista3.iniciales);
                    pos4 = buscarIniciales(listaT, datos_tenista4.iniciales);

                    // dentro del torneo se pasan los datos de los tenistas de la lista a sus variables locales (las de la funcion) y se juega un torneo
                    jugarTorneo(listaT, pos1, pos2, pos3, pos4);
                }
                break;
                case 6: 
                {
                    seleccionarTop4(listaT, pos1, pos2, pos3, pos4);
                    jugarTorneo(listaT, pos1, pos2, pos3, pos4);
                }
                break;
                default:
                {
                    cout << "ERROR: Opcion no valida" << endl << endl;
                }
            }
            
            system("pause");
            cout << endl;
        }
    }

    return 0;
}

/*Carga todos los datos del archivo tenista.txt al programa*/
bool cargar(tListaTenistas& listaT) {
    // NOTA: tambien iria con un tipo tDatosTenista jugador como variable local
    ifstream archivo;
    bool ok;
    archivo.open("tenistas.txt");
    if (archivo.is_open()) {
        // me carga el los datos de la lista en el programa mientras que no sea fin de linea
        archivo >> listaT.arrayT[listaT.cont].iniciales;
        // mientras que tengas algo que leer 
        while (!archivo.eof() && listaT.cont < DIM_ARRAY_TENISTAS) {
            archivo >> listaT.arrayT[listaT.cont].habilidad >> listaT.arrayT[listaT.cont].velocidad >> listaT.arrayT[listaT.cont].partidos_ganados >> listaT.arrayT[listaT.cont].partidos_perdidos;
            listaT.cont++;
            archivo >> listaT.arrayT[listaT.cont].iniciales;
        }
        ok = true;
        archivo.close();
    }
    else {
        ok = false;
    }
    return ok;
}

/*Es un procedimiento que muestra los tenistas de la lista dada*/
void mostrar(const tListaTenistas& listaT) {

    cout << "  INI  HAB  VEL   PG   PP" << endl;
    for (int i = 0; i < listaT.cont; i++) {
        cout << "  " << listaT.arrayT[i].iniciales << "  " << right << setw(3) << listaT.arrayT[i].habilidad << "  " << right << setw(3) << listaT.arrayT[i].velocidad << "  " << right << setw(3) << listaT.arrayT[i].partidos_ganados << "  " << right << setw(3) << listaT.arrayT[i].partidos_perdidos << endl;
    }
    cout << endl;
}

/*Es un procedimiento que recibe una lista de tenistas y me registra un nuevo tenista*/
void introducirTenista(tListaTenistas& listaT) {
    // se declaran las variables 
    tDatosTenista jugador;
    bool igual = false;
    if (listaT.cont < DIM_ARRAY_TENISTAS) {
        cout << "Datos del jugador" << endl;
        do {
            cout << "   - Introduce las iniciales del jugador (3 letras): ";
            cin >> jugador.iniciales;
            for (int i = 0; i < listaT.cont; i++) {
                if (listaT.arrayT[i].iniciales == jugador.iniciales) {
                    igual = true;
                }
                else {
                    igual = false;
                }
            }
        } while (jugador.iniciales.size() != 3 || igual);

        cout << "   - Introduce la habilidad del jugador (entre " << LIM_INF_HAB << " y " << LIM_SUP_HAB << "): ";
        // dentro de esta funcion llamamos a introducirDato()
        jugador.habilidad = introducirDato(LIM_INF_HAB, LIM_SUP_HAB);

        cout << "   - Introduce la velocidad del jugador (entre " << LIM_INF_VEL << " y " << LIM_SUP_VEL << "): ";
        jugador.velocidad = introducirDato(LIM_INF_VEL, LIM_SUP_VEL);

        cout << endl;

        // metemos el tenista en la lista 

        listaT.arrayT[listaT.cont] = jugador;
        listaT.cont++;

        cout << "Nuevo tenista creado" << endl << endl;
    }

    else {
        cout << "ERROR: No caben mas tenistas en la lista" << endl;
    }

}

/*Recibe un dato y analiza si este dato está dentro del rango, si lo está me devuelve un dato*/
int introducirDato(int LIM_INF, int LIM_SUP) {
    int dato;
    cin >> dato;
    while (dato < LIM_INF || dato > LIM_SUP) {
        cout << "Introduce un valor entre el rango pedido : ";
        cin >> dato;
    }
    return dato;
}

/*Recibe una lista de tenistas y me elimina un tenista que ha escogido el usuario*/
void eliminarTenista(tListaTenistas& listaT, string iniciales) {
    int pos;
    pos = buscarIniciales(listaT, iniciales);
    if (pos != -1) {
        for (int i = pos; i < listaT.cont - 1; i++) {
            listaT.arrayT[i] = listaT.arrayT[i + 1];
        }
        listaT.cont--;
        cout << "      Tenista eliminado corrrectamente" << endl << endl;
    }
    else {
        cout << endl << "      ERROR: El tenista introducido no existe" << endl << endl;
    }
}

/*La funcion recibe una lista de tenistas y busca las iniciales de un tenista concreto, devuelve asi la posicion en la cual se encuentra dicho tenista en el array/lista*/
int buscarIniciales(const tListaTenistas& listaT, string ini) {
    int pos = 0;
    bool encontrado = false;
    while (pos < listaT.cont && !encontrado) {
        if (listaT.arrayT[pos].iniciales == ini) {
            encontrado = true;
        }

        else {
            pos++;
        }
    }

    if (!encontrado) {
        pos = -1;
    }

    return pos;
}

/*Recibe una lista de tenistas y muestra por pantalla las iniciales de los tenistas*/
void mostrarIniciales(const tListaTenistas& listaT) {
    for (int i = 0; i < listaT.cont; i++) {
        cout << " " << listaT.arrayT[i].iniciales << " -";
    }
    cout << endl;
}

/*Encapsula en main del tenis de la v2, es decir, me juega un set entre 2 tenistas*/
tTenista jugarPartido(tDatosTenista& tenista1, tDatosTenista& tenista2) {

    // Se inicializan los datos del tenista activo
    int habilidad = 0, velocidad = 0, posicion_tenista = 0, posicion_inicial, posicion_bola;
    tTenista ganador_punto = NADIE, ganador_juego = NADIE, ganador_set = NADIE, jugadorActivo, servicio;

    // No hace falta inicializar los datos del partido ya que en antes del main ya los he inicializado

    // si es par
    if (ANCHO_PISTA % 2 == 0) {
        posicion_inicial = ANCHO_PISTA / 2;
    }
    // si es impar
    else {
        posicion_inicial = ANCHO_PISTA / 2 + 1;
    }

    tenista1.datos_partido.posicion = posicion_inicial;
    tenista2.datos_partido.posicion = posicion_inicial;
    posicion_bola = posicion_inicial;

    jugadorActivo = quienSaca(TENISTA1, TENISTA2);
    servicio = jugadorActivo;

    // se juegan muchos juegos, es decir, un set
    while (ganador_set == NADIE) {

        // pongo inicialmente a 0 todos los golpes y como esta dentro del bucle solo me cuenta los de cada juego 
        for (int i = 0; i < DIM_ARRAY_GOLPES; i++) {
            tenista1.datos_partido.golpes[i] = 0;
            tenista2.datos_partido.golpes[i] = 0;
        }

        tenista1.datos_partido.golpes_ganadores = 0;
        tenista2.datos_partido.golpes_ganadores = 0;

        //aqui se juegan muchos puntos, es decir, un punto

        ganador_juego = jugarJuego(jugadorActivo, servicio, posicion_inicial, tenista1, tenista2, habilidad, velocidad, posicion_tenista, posicion_bola);

        // cambiamos a que ahora el servicio al acabar un juego sea el otro quien saque en cada PUNTO
        servicio = jugadorRival(servicio);
        ganador_set = hayGanadorSet(tenista1, tenista2);
        if (ganador_set != NADIE) {
            cout << "    " << tenista1.iniciales << "  " << tenista1.datos_partido.juegos << endl;
            cout << "    " << tenista2.iniciales << "  " << tenista2.datos_partido.juegos << endl;
            cout << endl << toString(ganador_set, tenista1, tenista2) << " se hizo con el partido" << endl;
        }
    }

    // se actualiza el historico de los tenistas dentro del subprograma

    if (ganador_set == TENISTA1) {
        tenista1.partidos_ganados++;
        tenista2.partidos_perdidos++;
    }
    else {
        tenista2.partidos_ganados++;
        tenista1.partidos_perdidos++;
    }

    return ganador_set;
}

/*Recibe los indices de 4 tenistas dentro de la lista de tenistas y me juega un torneo entre ellos,
Los dos primeros juegan un set, luegos los dos siguientes otro y por ultimo se libra el ultimo set entre los dos ganadores*/
void jugarTorneo(tListaTenistas& listaT, int pos1, int pos2, int pos3, int pos4) {
    int pos, pos_ga1, pos_ga2;
    tTenista ganador_partido1, ganador_partido2, ganador_torneo;
    tDatosTenista datos_tenista1, datos_tenista2, datos_tenista3, datos_tenista4, datos_ganador1, datos_ganador2;

    // paso todos los datos de los tenistas a mis variables locales
    datos_tenista1 = listaT.arrayT[pos1];
    datos_tenista2 = listaT.arrayT[pos2];
    datos_tenista3 = listaT.arrayT[pos3];
    datos_tenista4 = listaT.arrayT[pos4];

    // COMIENZA EL TORNEO
    cout << endl;
    cout << "* Primera semifinal: " << datos_tenista1.iniciales << " vs " << datos_tenista4.iniciales << " *" << endl;
    cout << "* Segunda semifinal: " << datos_tenista2.iniciales << " vs " << datos_tenista3.iniciales << " *" << endl << endl;
    cout << "* COMIENZA EL TORNEO *" << endl << endl;
    system("pause");

    // PRIMERA SEMIFINAL

    cout << endl << "* Primera semifinal: " << datos_tenista1.iniciales << " vs " << datos_tenista4.iniciales << " *" << endl << endl;
    cout << "Comienza el partido..." << endl << endl;
    ganador_partido1 = jugarPartido(datos_tenista1, datos_tenista4);
    cout << endl << "* CAMPEON PRIMERA SEMIFINAL: " << toString(ganador_partido1, datos_tenista1, datos_tenista4) << " *" << endl << endl;

    // actualizo el historial de los tenistas de la primera partida
    listaT.arrayT[pos1].partidos_ganados = datos_tenista1.partidos_ganados;
    listaT.arrayT[pos1].partidos_perdidos = datos_tenista1.partidos_perdidos;
    listaT.arrayT[pos4].partidos_ganados = datos_tenista4.partidos_ganados;
    listaT.arrayT[pos4].partidos_perdidos = datos_tenista4.partidos_perdidos;

    system("pause");

    // SEGUNDA SEMIFINAL

    cout << endl << "* Segunda semifinal: " << datos_tenista2.iniciales << " vs " << datos_tenista3.iniciales << " *" << endl << endl;
    cout << "Comienza el partido..." << endl << endl;
    ganador_partido2 = jugarPartido(datos_tenista2, datos_tenista3);
    cout << endl << "* CAMPEON SEGUNDA SEMIFINAL: " << toString(ganador_partido2, datos_tenista2, datos_tenista3) << " *" << endl << endl;

    // actualizo el historial de los tenistas de la segunda partida
    listaT.arrayT[pos2].partidos_ganados = datos_tenista2.partidos_ganados;
    listaT.arrayT[pos2].partidos_perdidos = datos_tenista2.partidos_perdidos;
    listaT.arrayT[pos3].partidos_ganados = datos_tenista3.partidos_ganados;
    listaT.arrayT[pos3].partidos_perdidos = datos_tenista3.partidos_perdidos;

    system("pause");
    datos_ganador1 = toDatoTenista(ganador_partido1, datos_tenista1, datos_tenista4);
    datos_ganador2 = toDatoTenista(ganador_partido2, datos_tenista2, datos_tenista3);
    cout << endl << "* Gran final: " << datos_ganador1.iniciales << " vs " << datos_ganador2.iniciales << " *" << endl << endl;
    cout << "Comienza el partido..." << endl << endl;
    ganador_torneo = jugarPartido(datos_ganador1, datos_ganador2);
    cout << endl << "* CAMPEON DEL TORNEO: " << toString(ganador_torneo, datos_ganador1, datos_ganador2) << " *" << endl << endl;

    // actualizo el historial de los tenistas del TORNEO
    pos_ga1 = buscarIniciales(listaT, datos_ganador1.iniciales);
    pos_ga2 = buscarIniciales(listaT, datos_ganador2.iniciales);
    listaT.arrayT[pos_ga1].partidos_ganados = datos_ganador1.partidos_ganados;
    listaT.arrayT[pos_ga1].partidos_perdidos = datos_ganador1.partidos_perdidos;
    listaT.arrayT[pos_ga2].partidos_ganados = datos_ganador2.partidos_ganados;
    listaT.arrayT[pos_ga2].partidos_perdidos = datos_ganador2.partidos_perdidos;

}

/*Recibe un dato de tipo tTenista y dos tenistas, y me devuelve un tDatoTenista*/
tDatosTenista toDatoTenista(tTenista tenista_enum, tDatosTenista tenista1, tDatosTenista tenista2) {
    tDatosTenista tenista;
    if (tenista_enum == TENISTA1) {
        tenista = tenista1;
    }
    else {
        tenista = tenista2;
    }
    return tenista;
}

/*Se trata de un procedimiento que me selecciona a los 4 jugadores con mayor numero de partidos ganados de la lista*/
void seleccionarTop4(const tListaTenistas& listaT, int& indT1, int& indT2, int& indT3, int& indT4) {
    double max = 0;
    for (int i = 0; i < listaT.cont; i++) {
        if (listaT.arrayT[i].partidos_ganados > max) {
            max = listaT.arrayT[i].partidos_ganados;
            indT1 = i;
        }
    }
    max = 0;
    for (int i = 0; i < listaT.cont; i++) {
        if ((listaT.arrayT[i].partidos_ganados > max) && (i != indT1)) {
            max = listaT.arrayT[i].partidos_ganados;
            indT2 = i;
        }
    }
    max = 0;
    for (int i = 0; i < listaT.cont; i++) {
        if ((listaT.arrayT[i].partidos_ganados > max) && (i != indT1) && (i != indT2)) {
            max = listaT.arrayT[i].partidos_ganados;
            indT3 = i;
        }
    }
    max = 0;
    for (int i = 0; i < listaT.cont; i++) {
        if ((listaT.arrayT[i].partidos_ganados > max) && (i != indT1) && (i != indT2) && (i != indT3)) {
            max = listaT.arrayT[i].partidos_ganados;
            indT4 = i;
        }
    }
    
}

/*Me guarda los datos registrados durante el programa en el ARCHIVO de la lista de tenistas*/
void guardar(const tListaTenistas& listaT) {
    ofstream archivo;
    archivo.open("tenistas.txt");
    if (archivo.is_open()) {
        for (int i = 0; i < listaT.cont; i++) {
            archivo << listaT.arrayT[i].iniciales << " " << listaT.arrayT[i].habilidad << " " << listaT.arrayT[i].velocidad << " " << listaT.arrayT[i].partidos_ganados << " " << listaT.arrayT[i].partidos_perdidos << endl;
        }
        archivo.close();
    }
    else {
        cout << "ERROR: El archivo no se pudo abrir" << endl;
    }
}

/*El usuario escoje una opcion de las disponibles y la funcion me devulve la opcion escogida*/
int menu() {
    int op;
    cout << "Elija una de las siguientes opciones: " << endl;
    cout << "<1> - Ver datos tenistas" << endl;
    cout << "<2> - Nuevo tenista" << endl;
    cout << "<3> - Eliminar tenista" << endl;
    cout << "<4> - Jugar Partido" << endl;
    cout << "<5> - Torneo semifinales/final" << endl;
    cout << "<6> - Torneo top-4" << endl;
    cout << "<0> - Salir" << endl << endl;
    cout << "Opcion: ";
    cin >> op;
    cout << endl;


    return op;
}

/*Recibe diferentes datos; esta funcion unicamente me hace un lance, haya o no un ganadador del lance, esta se PARA; 
llama a las funciones golpeoBola() y correTenista() para calcular la posicion de la bola y del tenista; me devuelve el ganador del punto*/
tTenista lance(tTenista& jugadorActivo, tDatosTenista& datos_tenista1, tDatosTenista& datos_tenista2, int habilidad, int velocidad, int& posicion_tenista, int& posicion_bola) {
    
    tTenista ganador_punto = NADIE;
    if (DETALLES) {
        cout << "Golpea " << toString(jugadorActivo, datos_tenista1, datos_tenista2) << endl;
    }
    
    // declaramos posicion_bola asignando el valor de la funcion Y LO QUE HACE la funcion, asi cada vez que se la llame me hace tanto los COUT como los CALCULOS
    posicion_bola = golpeoBola(posicion_tenista, habilidad);

    if (jugadorActivo == TENISTA1) {
        datos_tenista1.datos_partido.golpes[posicion_bola]++;
    }
    if (jugadorActivo == TENISTA2) {
        datos_tenista2.datos_partido.golpes[posicion_bola]++;
    }

    if (MODO_DEBUG) {
        cout << toString(jugadorActivo, datos_tenista1, datos_tenista2) << " dispara hacia la calle " << posicion_bola << endl;
    }

    // si no se ha terminado el punto cambia de jugador 
    // ahora el jugador activo es el que corre despues del que golpea
    
    if (jugadorActivo == TENISTA1) {
        jugadorActivo = TENISTA2;
        velocidad = datos_tenista2.velocidad;
        habilidad = datos_tenista2.habilidad;
        posicion_tenista = datos_tenista2.datos_partido.posicion;
    }

    else {
        jugadorActivo = TENISTA1;
        velocidad = datos_tenista1.velocidad;
        habilidad = datos_tenista1.habilidad;
        posicion_tenista = datos_tenista1.datos_partido.posicion;
    }

    if (posicion_bola == ANCHO_PISTA + 1 or posicion_bola == 0) {
        if (DETALLES) {
            pintarPeloteo(datos_tenista1, datos_tenista2, jugadorActivo, posicion_bola);
            cout << endl << endl;
        }
        // En vez de sumar puntos vamos ha indicar directamente quien ha ganado el punto *Anotación para actualizarMarcador()
        ganador_punto = jugadorActivo;
        if (MODO_DEBUG) {
            system("pause");
        }
        if (DETALLES) {
            cout << "El ganador del punto es " << toString(ganador_punto, datos_tenista1, datos_tenista2) << endl << endl;
        }

        // esto es punto para el que ahora le tocaría correr
    }

    if (ganador_punto == NADIE) {

        // los nombres de las variables ahora pueden ser diferentes, lo que importa es el orden
        posicion_tenista = correTenista(posicion_tenista, velocidad, posicion_bola);

        // actualización de las posiciones (hay que hacerlo siempre)
        if (jugadorActivo == TENISTA1) {
            datos_tenista1.datos_partido.posicion = posicion_tenista;
        }

        else {
            datos_tenista2.datos_partido.posicion = posicion_tenista;
        }

        if (posicion_tenista != posicion_bola) {

            if (jugadorActivo == TENISTA1) {
                datos_tenista2.datos_partido.golpes_ganadores++;
            }
            if (jugadorActivo == TENISTA2) {
                datos_tenista1.datos_partido.golpes_ganadores++;
            }

            // si el tenista no llega a la bola pues se acaba el punto

            if (MODO_DEBUG) {
                cout << toString(jugadorActivo, datos_tenista1, datos_tenista2) << " no ha llegado a la posicion de la bola y se ha terminado el punto." << endl;
            }
            if (DETALLES) {
                pintarPeloteo(datos_tenista1, datos_tenista2, jugadorActivo, posicion_bola);
                cout << endl << endl;
            }
            ganador_punto = jugadorRival(jugadorActivo);

            if (MODO_DEBUG) {
                system("pause");
            }
            if (DETALLES) {
                cout << "El ganador del punto es " << toString(ganador_punto, datos_tenista1, datos_tenista2) << endl << endl;
            }

        }
        
        // si el jugador llega a la bola y no HA HABIDO ganador del punto, entonces se termina el lance y se vuelve a repetir el bucle dentro de la funcion jugarPunto()
        else {
            if (MODO_DEBUG) {
                cout << toString(jugadorActivo, datos_tenista1, datos_tenista2) << " ha llegado a la bola" << endl;
            }
        }
    }
    return ganador_punto;
}

/*Recibe los datos correspondientes, hace varios lances hasta que HAYA un ganador del punto (ej. 00 - 15), llama así a la funcion lance(), me devuelve el ganador del punto*/
tTenista jugarPunto(tTenista& jugadorActivo, tTenista servicio, int posicion_inicial, tDatosTenista& datos_tenista1, tDatosTenista& datos_tenista2, int habilidad,  int velocidad, int posicion_tenista, int posicion_bola) {
    tTenista ganador_punto;

    ganador_punto = NADIE;

    datos_tenista1.datos_partido.posicion = posicion_inicial;
    datos_tenista2.datos_partido.posicion = posicion_inicial;
    posicion_tenista = posicion_inicial;
    posicion_bola = posicion_inicial;

    // Para que al inicio de cada juego sea el otro quien saque en todos los comienzos
    jugadorActivo = servicio;

    // Al empezar un nuevo punto dentro del bucle de jugarJuego, como no sabemos quien acabó el punto anterior, hay que volver a resignar la velocidad y habilidad del jugadorActivo ya que el que saca cada punto es siempre el mismo, pero el que acaba el punto (correTenista) no tiene por qué coincidir con que el que saca
    if (jugadorActivo == TENISTA1) {
        velocidad = datos_tenista1.velocidad;
        habilidad = datos_tenista1.habilidad;
    }

    if (jugadorActivo == TENISTA2) {
        velocidad = datos_tenista2.velocidad;
        habilidad = datos_tenista2.habilidad;
    }
    if (DETALLES) {
        pintarMarcador(datos_tenista1, datos_tenista2, jugadorActivo);
        cout << endl;
    }

    while (ganador_punto == NADIE) {

        if (DETALLES) {
            pintarPeloteo(datos_tenista1, datos_tenista2, jugadorActivo, posicion_bola);
            cout << endl << endl;
        }
        
        if (MODO_DEBUG) {
            system("pause");
        }
        
    // cada vez que se llama a la funcion lance, hay que actualizar el jugadorActivo por que el que golpea no tiene por que coincidir con el correTenista que acaba la partida
    if (jugadorActivo == TENISTA1) {
        velocidad = datos_tenista1.velocidad;
        habilidad = datos_tenista1.habilidad;
    }

    if (jugadorActivo == TENISTA2) {
        velocidad = datos_tenista2.velocidad;
        habilidad = datos_tenista2.habilidad;
    }
    
        // mientras que no haya un ganador del punto (es decir en un lance nadie ha perdido y el correTenista llega a la bola) se sigue haciendo lances hasta que alguien pierda
        ganador_punto = lance(jugadorActivo, datos_tenista1, datos_tenista2, habilidad, velocidad, posicion_tenista, posicion_bola);
    }

    return ganador_punto;
}

/* Recibe los datos correspondientes, me hace varios puntos dentro del un juego, llamando así a la funcion jugarPunto(), me devuelve el ganador del Juego*/
tTenista jugarJuego(tTenista& jugadorActivo, tTenista servicio, int posicion_inicial, tDatosTenista& datos_tenista1, tDatosTenista& datos_tenista2, int habilidad, int velocidad, int posicion_tenista, int posicion_bola) {
    
    // declaramos de nuevo ganador_punto como NADIE para que no almacene basura
    tTenista ganador_juego = NADIE, ganador_punto = NADIE;
    datos_tenista1.datos_partido.puntos = NADA, datos_tenista2.datos_partido.puntos = NADA;
    
    if (DETALLES) {
        cout << "Sevicio para " << toString(servicio, datos_tenista1, datos_tenista2) << endl << endl;
    }
    
    // mientras no haya un ganador del juego
    while (ganador_juego == NADIE) {
        // se vuelve a declarar para el segundo bucle con el while de finJuego porque si no finPunto ahora sería true y el programa se pararía
	    
	    // mientras no haya ganador del punto se seguira ejecuntado esto 
        ganador_punto = jugarPunto(jugadorActivo, servicio, posicion_inicial, datos_tenista1, datos_tenista2, habilidad, velocidad, posicion_tenista, posicion_bola);
                
                // si se ha terminado el punto actualizamos el marcador con el ganador del punto y luego al volverse a ejecutar el bucle me muestra el marcador actualizado al inicio de la partida nueva
        	    ganador_juego = actualizarMarcador(ganador_punto, datos_tenista1, datos_tenista2);
                
                // si hay ganador del punto no quiero que me muestre aun las estadisticas
                if (ganador_juego != NADIE) {
                    if (DETALLES) {
                        cout << endl;
                        mostrarEstadisticas(datos_tenista1.iniciales, datos_tenista1.datos_partido.golpes_ganadores, datos_tenista1.datos_partido.golpes);
                        cout << endl << endl;
                        mostrarEstadisticas(datos_tenista2.iniciales, datos_tenista2.datos_partido.golpes_ganadores, datos_tenista2.datos_partido.golpes);
                        cout << endl << endl;
                    }
                }
        	    
                if (ganador_juego == TENISTA1) {
                    cout << "El ganador del juego es " << toString(TENISTA1, datos_tenista1, datos_tenista2)  << endl << endl;
                }
                else if (ganador_juego == TENISTA2) {
                    cout << "El ganador del juego es " << toString(TENISTA2, datos_tenista1, datos_tenista2) << endl << endl;
                }
                
                // si no hay ganador del juego aun, para que no se me ejecute el bucle de seguido, hacemos pulsar tecla para continuar
                else if (MODO_DEBUG) {
                     system("pause");
                }
	}
	
	return ganador_juego;
}

/*Recibe los datos de las iniciales, los golpes y los golpes ganados y al ser un procedimiento, me muestra las estadisticas de ambos jugadores*/
void mostrarEstadisticas(string iniciales, int& golpes_ganados, tConteoGolpes golpes) {

    // usamos golpes_totales para hacer la media entre todos los golpes y golpes_fuera para contabilizar los golpes no forzados
    int golpes_totales = 0, golpes_fuera = 0;
    tConteoProbabilidad probabilidad_bola;

    // no hace falta inicializar las probabilidades porque se sobreescriben cada vez 

    // calculo los golpes totales
    for (int i = 0; i < DIM_ARRAY_GOLPES; i++) {
        golpes_totales = golpes_totales + golpes[i];
    }

    golpes_fuera = golpes[0] + golpes[ANCHO_PISTA + 1];

    // calculo la probabilidad en cada, aquí se sobreescribe
    for (int i = 0; i < DIM_ARRAY_GOLPES; i++) {
        probabilidad_bola[i] = (golpes[i] / double(golpes_totales)) * 100;
    }

    cout << "Estadisticas de " << iniciales << endl;
    cout << "   Golpes totales: " << golpes_totales << endl;
    cout << "   Golpes ganadores: " << golpes_ganados << endl;
    cout << "   Errores no forzados: " << golpes_fuera << endl;
    cout << "   Distribucion de los golpes en la pista" << endl;
    cout << "      Calle ";
    for (int i = 0; i < DIM_ARRAY_GOLPES; i++) {
        cout << "     " << i;
    }
    cout << endl;
    cout << "          % ";

    // se puede cambiar esto quitando el array, cambiando la condicion de igualdad con mayor o igual que 10 y menor o igual que 9.9 y poniendo el calculo directamente en el if 
    for (int i = 0; i < DIM_ARRAY_GOLPES; i++) {
        cout << setw(6) << fixed << setprecision(1) << probabilidad_bola[i];
    }
}

/*Recibe un tenista de tipo tTenista y me imprime las iniciales correspondientes*/
string toString(tTenista tenista_enum, tDatosTenista datos_tenista1, tDatosTenista datos_tenista2) {
    string tenista_string;
    switch(tenista_enum){
        
        case TENISTA1:
        {
            tenista_string = datos_tenista1.iniciales;
            break;
        }
        case TENISTA2:
        {
            tenista_string = datos_tenista2.iniciales;
            break;
        }
    }
    return tenista_string;
}

/*Recibe la puntuacion. Indica el valor de cada punto obtenido en el juego. Nos devuelve ese string(valor) como resultado*/
string puntosAstring(tPuntosJuego puntuacion) {
    string resultado;
    switch (puntuacion) {
    case NADA:
    {
        resultado = "00";
        break;
    }
    case QUINCE:
    {
        resultado = "15";
        break;
    }
    case TREINTA:
    {
        resultado = "30";
        break;
    }
    case CUARENTA:
    {
        resultado = "40";
        break;
    }
    case VENTAJA:
    {
        resultado = "AD";
        break;
    }
    default: 
    cout << "ERROR" << endl;
    }
    return resultado;
}

/*Recibe los datos de las puntuaciones y juegos, sabiendo ya quien gana el punto, sumamos los puntos o declaramos el ganador del juego (con el objetivo de no poner el enum: GANA)*/
tTenista actualizarMarcador(tTenista ganador_punto, tDatosTenista& datos_tenista1, tDatosTenista& datos_tenista2) {
    tTenista ganador_juego = NADIE;
    // si alguien ha ganado el juego
    if (ganador_punto == TENISTA1) {
        // si se ha acabado la partida sabiendo el ganador del punto, partimos de la situacion anterior a cuando se suman puntos ya que no hace falta porque ya se sabe cuando alguien gana antes de sumarlos (declarado en el main en sus respectivos lugares)
        if ((datos_tenista1.datos_partido.puntos == VENTAJA and datos_tenista2.datos_partido.puntos == CUARENTA) or (datos_tenista1.datos_partido.puntos == CUARENTA and datos_tenista2.datos_partido.puntos < CUARENTA)) {
            datos_tenista1.datos_partido.juegos++;
            ganador_juego = TENISTA1;
        }
        // si se sigue jugando porque no es fin de juego aun
        else {
            datos_tenista1.datos_partido.puntos = tPuntosJuego(datos_tenista1.datos_partido.puntos + 1);
        }
    }
    
    if (ganador_punto == TENISTA2) {
        if ((datos_tenista2.datos_partido.puntos == VENTAJA and datos_tenista1.datos_partido.puntos == CUARENTA) or (datos_tenista2.datos_partido.puntos == CUARENTA and datos_tenista1.datos_partido.puntos < CUARENTA)) {
            datos_tenista2.datos_partido.juegos++;
            ganador_juego = TENISTA2;
        }
        else {
            datos_tenista2.datos_partido.puntos = tPuntosJuego(datos_tenista2.datos_partido.puntos + 1);
        }
    }
    
    if ((datos_tenista1.datos_partido.puntos == VENTAJA) && (datos_tenista2.datos_partido.puntos == VENTAJA)) {
        datos_tenista1.datos_partido.puntos = CUARENTA;
        datos_tenista2.datos_partido.puntos = CUARENTA;
    }
    
    return ganador_juego;
}

/*Es un procedimiento que me dibuja el marcador, recibe los datos correspondientes para pintarlo*/
void pintarMarcador(const tDatosTenista& datos_tenista1, const tDatosTenista& datos_tenista2, tTenista servicio_para) {
    cout << "    " << datos_tenista1.iniciales << "  " << datos_tenista1.datos_partido.juegos << " : " << puntosAstring(datos_tenista1.datos_partido.puntos) << " ";
    if (servicio_para == TENISTA1) {
        cout << "*";
    }
    cout << endl;
    cout << "    " << datos_tenista2.iniciales << "  " << datos_tenista2.datos_partido.juegos << " : " << puntosAstring(datos_tenista2.datos_partido.puntos) << " ";

    if (servicio_para == TENISTA2) {
        cout << "*";
    }
    cout << endl;
}

/*Recibe los datos de la partida de tenis y pinta las diferentes situaciones del juego*/
void pintarPeloteo(tDatosTenista datos_tenista1, tDatosTenista datos_tenista2, tTenista bola_jugador, int pos_bola) {

    /*INICIALES      */
    cout << " ";
    // como el bucle se hace desde el 0 me tiene que pintar hasta ANCHO_PISTA - 1 veces
    for (int i = 0; i < ANCHO_PISTA; i++) {
        // como la i comienza en 0, el nombre se pone donde este el tenista en ese momento menos 1
        if (i == datos_tenista1.datos_partido.posicion - 1) {
            cout << datos_tenista1.iniciales;
        }
        // si no esta el jugador ahi, pongo espacio
        else {
            cout << "  ";
        }
    }

    /* - - - - - */
    cout << endl << "  ";

    // el bucle me lo tiene que repetir hasta ANCHO_PISTA - 1 veces para rellenar la fila de la pista
    for (int i = 0; i < ANCHO_PISTA; i++) {
        cout << "- ";
    }
    /* | | |O| x LARGO_PISTA*/
    // me repite el bucle LARGO_PISTA veces
    for (int i = 0; i < LARGO_PISTA; i++) {
        // en la posicion 0, me tiene que escupir la linea de pista en la que que puede estar la pos_bola o no
        if (i == 0) {
            cout << endl;
            /* | | |O| x ANCHO_PISTA*/
            for (int i = 0; i < ANCHO_PISTA + 2; i++) {
                if (i == pos_bola && bola_jugador == TENISTA1) {
                    cout << "o";
                }
                else {
                    cout << " ";
                }
                if (i != ANCHO_PISTA + 1) {
                    cout << "|";
                }
            }
        }
        // para que no me ejecute este codigo siempre (ya que si me imprime la linea de pista con la bola no quiero que se imprima esta tambien) en el caso que ser distinto que no me lo imprima
        if (i != 0) {
            cout << endl;
            for (int i = 0; i < ANCHO_PISTA + 1; i++) {
                cout << " |";
            }
        }
    }

    /*--1-2-3-4-5-6-7-....-*/
    cout << endl << "-";
    for (int i = 0; i < ANCHO_PISTA; i++) {
        // como la i la declaro en 0, los numeros de la pista son i + 1
        cout << "-" << i + 1;
    }
    cout << "--";

    // A PARTIR DE AQUÍ ES EL MISMO CODIGO PERO AL REVES!!!!

    /* | | |O| x LARGO_PISTA*/
    for (int i = 0; i < LARGO_PISTA; i++) {
        // me imprime las lineas de la pista en el resto de casos
        if (i != LARGO_PISTA - 1) {
            cout << endl;

            for (int i = 0; i < ANCHO_PISTA + 1; i++) {
                cout << " |";
            }
        }
        // en este caso como la linea de la pista que se imprime la ponemos al final del todo, solo me imprime la linea donde esta la bola en cuando sea LARGO_PISTA - 1
        if (i == LARGO_PISTA - 1) {
            cout << endl;
            for (int i = 0; i < ANCHO_PISTA + 2; i++) {
                if (i == pos_bola && bola_jugador == TENISTA2) {
                    cout << "o";
                }
                else {
                    cout << " ";
                }
                if (i != ANCHO_PISTA + 1) {
                    cout << "|";
                }
            }
        }
    }

    /* - - - - - */
    cout << endl << "  ";
    for (int i = 0; i < ANCHO_PISTA; i++) {
        cout << "- ";
    }

    /*       INICIALES*/
    cout << endl << " ";
    for (int i = 0; i < ANCHO_PISTA; i++) {
        if (i == datos_tenista2.datos_partido.posicion - 1) {
            cout << datos_tenista2.iniciales;
        }
        else {
            cout << "  ";
        }
    }
}

/*Recibe el nombre de ambos jugadores. Nos indica mediante un numero aleatorio cual de los dos jugadores empieza la partida.
Devuelve el nombre del jugador que empieza.*/
tTenista quienSaca(tTenista jugador1, tTenista jugador2) {

	tTenista jug_saca;

	int num;
	srand((unsigned)time(0));
	num = rand() % 2;

	if (num == 0) {
		jug_saca = jugador1;
	}
	else {
		jug_saca = jugador2;
	}
	return jug_saca;
}

/*Recibe los valores de los juegos de ambos jugadores, me calcula quien es el ganador del set y me devuelve el mismo*/
tTenista hayGanadorSet(tDatosTenista datos_tenista1, tDatosTenista datos_tenista2) {
    tTenista ganador_set = NADIE;
    if ((datos_tenista1.datos_partido.juegos >= datos_tenista2.datos_partido.juegos + 2) && (datos_tenista1.datos_partido.juegos >= JUEGOS_SET)) {
        ganador_set = TENISTA1;
    }
    if ((datos_tenista2.datos_partido.juegos >= datos_tenista1.datos_partido.juegos + 2) && (datos_tenista2.datos_partido.juegos >= JUEGOS_SET)) {
        ganador_set = TENISTA2;
    }
    // NO entiendo por qué aquí al quitar el else el valor de ganador_set cambia
    return ganador_set;
}

/*Recibe un jugador y me dice quien es el jugador rival, lo uso para cambiar el servicio al final de cada Juego*/
tTenista jugadorRival(tTenista jugador) {
    tTenista rival;
    if (jugador == TENISTA1) {
        rival = TENISTA2;
    }
    else {
        rival = TENISTA1;
    }
    return rival;
}

/* Recibe la posicion del destino y su habilidad. Si el golpeador puede o no mandar la bola al destino deseado depende de su habilidad. 
Devuelve la posicion a donde llega finalmente la bola*/
int golpeoBola(int posicion_tenista, int habilidad) {
    int posicion_bola;
    // si es modo teclado
    if (JUEGO_ALEATORIO == false) {
       cout << "Introduce la calle donde quieres mandar la bola (entre 1 y " << ANCHO_PISTA << ") : " << endl;
       cin >> posicion_bola;
    }
    else {
        posicion_bola = rand() % ANCHO_PISTA + 1;
    }
    if (MODO_DEBUG) {
     cout << "El jugador desea mandar la bola a la calle " << posicion_bola << endl;
    }
     
    int distancia_a_destino = abs(posicion_bola - posicion_tenista);
    if (distancia_a_destino > habilidad) {
        int probabilidad_acierto;
        probabilidad_acierto = 100 - ((distancia_a_destino - habilidad) / (double)((ANCHO_PISTA - 1) - LIM_INF_HAB)) * 100;
    
        int resultado = rand() % 100;
        
        if (MODO_DEBUG) {
        cout << "Un tiro complicado con una probabilidad de " << probabilidad_acierto << " y un resultado de " << resultado << " por lo que";
        }
        if (resultado >= probabilidad_acierto) {
            if (MODO_DEBUG) {

            cout << " la bola no ha llegado a su destino" << endl;
            }
            if (rand () % 2 == 1) {
                ++posicion_bola;
            }
            else {
                --posicion_bola;
            }
        }
        else {
            if (MODO_DEBUG) {
            cout << " consigue mandar la bola a la calle deseada " << endl;
            } 
        }
    }
    
    return posicion_bola;
}

/*Recibe la posicion del tenista, su velocidad y la posicion de la bola. 
 Indica asi, si el corredor llega o no a la calle deseada en base a su habilidad. Devuelve la posición hasta donde corre el tenista.*/ 
int correTenista (int posicion_tenista, int velocidad, int posicion_bola) {
    int distancia = abs(posicion_bola - posicion_tenista);
    if (velocidad < distancia){
        if (posicion_tenista < posicion_bola) {
            posicion_tenista = posicion_tenista + velocidad;
        }
        else {
            posicion_tenista = posicion_tenista - velocidad;
        }
    }
    else {
        posicion_tenista = posicion_bola;
    }
    
    return posicion_tenista;
}

