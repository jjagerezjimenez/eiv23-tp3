#define BOTON B12
#define PIN_LED C13
#define TIEMPO_LUZ_ENCENDIDA 60000
#define TIEMPO_PARA_PRESIONAR 1000
#include <BluePillHal.h>
#include "control_luz.h"

void luzOn(void)
{
    BP_Pin_set(PIN_LED, 1);
}
void luzOff(void)
{
    BP_Pin_set(PIN_LED, 0);
}
void setTimeoutLuz(ControladorLuz *controlador)
{
    controlador->TO_Luz = getTicks() + controlador->tiempoLuz;
}
void setTimeoutBoton(ControladorLuz *controlador)
{
    controlador->TO_Boton = getTicks() + controlador->tiempoBoton;
}
bool botonPresionado()
{
    static TiempoMilisegundos ultimoCambio = 0;
    static bool valorAnterior;
    bool valorActual = BP_Pin_read(BOTON);
    bool Presionado = false;

    if (!valorActual && valorAnterior && (getTicks() - ultimoCambio) >= 15)
    {
        Presionado = true;
        ultimoCambio = getTicks();
        valorAnterior = valorActual;
    }
    return Presionado;
}

int main(void)
{
    BP_inicio();
    BP_Pin_mode(BOTON, IN_PULL_UP);
    BP_Pin_mode(PIN_LED, OUT_2MHz);
    static ControladorLuz implControlador;
    Maquina *controlador = ControladorLuz_init(&implControlador, TIEMPO_LUZ_ENCENDIDA, TIEMPO_PARA_PRESIONAR);
    setControlador((ControladorLuz *)controlador);
    while (1)
    {
        Maquina_procesa(controlador);
        if (botonPresionado())
        {
            Maquina_despacha(controlador, EV_BOTON);
        }
    }
}
