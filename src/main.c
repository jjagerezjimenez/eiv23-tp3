#include <BluePillHal.h>
#include "control_luz.h"
#include "trace.h"

#define BOTON B12
#define PIN_LED C13
#define TIEMPO_LUZ_ENCENDIDA 60000
#define TIEMPO_PARA_PRESIONAR 1000

void luzOn(void)
{
    BP_Pin_set(PIN_LED, 0);
}
void luzOff(void)
{
    BP_Pin_set(PIN_LED, 1);
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

    if (!valorActual && valorAnterior && (getTicks() - ultimoCambio) >= 30)
    {
        Presionado = true;
        ultimoCambio = getTicks();
    }
    valorAnterior = valorActual;
    return Presionado;
}

static bool procesaAntirreboteBoton(void)
{
    enum
    {
        TOPE_CUENTA = 5
    };

    static TiempoMilisegundos t0 = 0;
    static bool estado = false;
    static uint8_t contador = 0;

    bool const estadoPin = BP_Pin_read(BOTON);
    TiempoMilisegundos const t = getTicks();

    if ((t - t0) >= 1)
    {
        t0 = t;
        if (estadoPin && contador < TOPE_CUENTA)
        {
            ++contador;
            if (contador == TOPE_CUENTA)
            {
                estado = true;
            }
        }
        else if (!estadoPin && contador > 0)
        {
            --contador;
            if (contador == 0)
            {
                estado = false;
            }
        }
    }
    return estado;
}

static void checkTIMEOUT(ControladorLuz *controlador)
{
    Maquina *maquina = (Maquina *)controlador;
    if (getTicks() == controlador->TO_Boton)
        Maquina_despacha(maquina, EV_TIMEOUT_BOTON);
    if (getTicks() == controlador->TO_Luz)
        Maquina_despacha(maquina, EV_TIMEOUT_LUZ);
}

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
        checkTIMEOUT((ControladorLuz *)controlador);
        Maquina_procesa(controlador);
        bool const nuevoEstadoBoton = procesaAntirreboteBoton();
        if (!nuevoEstadoBoton && estadoBoton)
        {
            Maquina_despacha(controlador, EV_BOTON);
        }
        estadoBoton = nuevoEstadoBoton;
    }
}
