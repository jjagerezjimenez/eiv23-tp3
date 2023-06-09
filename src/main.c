#include <stdint.h>
#include <BluePillHal.h>
#include "trace.h"

#define BOTON B12
#define PIN_LED C13
#define TIEMPO_LUZ_ENCENDIDA 60000
#define TIEMPO_PARA_PRESIONAR 1000


int main(void)
{
    BP_inicio();
    BP_Pin_mode(BOTON, IN_PULL_UP);
    BP_Pin_mode(PIN_LED, OUT_2MHz);
    TRACE_INIT();
    luzOff();
    static ControladorLuz implControlador;
    Maquina *controlador = ControladorLuz_init(&implControlador, TIEMPO_LUZ_ENCENDIDA, TIEMPO_PARA_PRESIONAR);
    bool estadoBoton = procesaAntirreboteBoton();
    while (1)
    {

        Maquina_procesa(controlador);
        bool const nuevoEstadoBoton = procesaAntirreboteBoton();
        if (!nuevoEstadoBoton && estadoBoton)
        {
            Maquina_despacha(controlador, EV_BOTON);
        }
        estadoBoton = nuevoEstadoBoton;
    }
}
