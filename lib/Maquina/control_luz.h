#ifndef CONTROL_LUZ_H
#define CONTROL_LUZ_H
#include "maquina.h"

enum EventoControladorLuz{
    EV_BOTON_PULSADO = EV_BOTON,
    EV_TIMEOUT_LUZ,
    EV_TIMEOUT_BOTON
};

typedef unsigned TiempoMilisegundos;

typedef struct ControladorLuz{
    Maquina maquina;
    TiempoMilisegundos tiempoLuz;
    TiempoMilisegundos tiempoBoton;
    TiempoMilisegundos TO_Luz;
    TiempoMilisegundos TO_Boton;
}ControladorLuz;

/**
 * @brief Inicializa un controlador de luz de escalera (máquina de estado)
 * Se debe emplear el objeto Maquina retornado para invocar los métodos
 * de despacho y proceso de eventos
 * 
 * @param self Este objeto
 * @param tiempoOn Tiempo que permanece encendida la luz
 * @return Maquina* Máquina de estado del controlador
 */
Maquina * ControladorLuz_init(ControladorLuz *self,TiempoMilisegundos tiempoLuz, TiempoMilisegundos tiempoBoton);

/* IMPLEMENTAR */
void luzOn(void);

/* IMPLEMENTAR */
void luzOff(void);

/* IMPLEMENTAR */
bool setTimeoutLuz(Maquina *maquina);
bool setTimeoutBoton(Maquina *maquina);

#endif
