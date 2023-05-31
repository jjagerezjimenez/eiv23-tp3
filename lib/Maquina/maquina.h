#ifndef MAQUINA_H
#define MAQUINA_H
#include <stdbool.h>
#define MAX_EV_COLA 16 /* debe ser potencia de 2*/

typedef struct Maquina Maquina;
typedef unsigned Evento; //donde lo define?
typedef struct Resultado Resultado; // Donde lo define?
typedef Resultado (*Estado)(Maquina* contexto, Evento evento);


enum EventoSistema{
    /**
     * @brief Significa que no hay eventos disponibles
     */
    EV_NULO,
    /**
     * @brief Reinicia la máquina de estado
     * 
     */
    EV_RESET,
    /**
     * @brief Primer evento disponible para la aplicación
     * 
     */
    EV_USUARIO
};


/**
 * @brief Máquina de estado
 */
struct Maquina{
    struct {
        Evento eventos[MAX_EV_COLA];
        /**
         * @brief lecturas % MAX_EV_COLA : indice del proximo elemento a leer
         * SI lecturas != escrituras
         */
        unsigned lecturas;
        /**
         * @brief escrituras % MAX_EV_COLA : indice del proximo espacio libre 
         * SI (escrituras - lecturas) < MAX_EV_COLA
         */
        unsigned escrituras;
    }cola;
    Estado estadoInicial;
    Estado estado;
};


/**
 * @brief Despacha un evento para posterior procesamiento
 * 
 * @param self Este objeto
 * @param evento Evento a despachar
 * @return true Evento despachado
 * @return false Falla al despachar evento
 */
bool Maquina_despacha(Maquina *self, Evento evento);
/**
 * @brief Procesa un evento disponible
 * 
 * @param self Este objeto
 * @return true Evento procesado
 * @return false No había eventos disponibles
 */
bool Maquina_procesa(Maquina *self);

#endif
