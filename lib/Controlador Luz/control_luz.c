#include "control_luz.h"
#include "maquina_impl.h"
#include "BluePillHal.h"
#include "trace.h"

// Estados

static Resultado Estado_apagado(Maquina *contexto, Evento evento);
static Resultado Estado_encendido(Maquina *contexto, Evento evento);
static Resultado Estado_mudanza(Maquina *contexto, Evento evento);

Maquina *ControladorLuz_init(ControladorLuz *self, TiempoMilisegundos tiempoLuz, TiempoMilisegundos tiempoBotonTotal)
{
    Maquina_init(&self->maquina, Estado_apagado);
    self->tiempoLuz = tiempoLuz;
    self->tiempoBoton = tiempoBotonTotal;
    return &(self->maquina);
}

static Resultado Estado_apagado(Maquina *contexto, Evento evento)
{
    Resultado resultado = {0};
    ControladorLuz *self = (ControladorLuz *)contexto; // Por que pone este Cast?
    switch (evento)
    {
    case EV_BOTON:
        TRACE_ENTER(0);
        BP_Pin_set(A0, 1);
        self->contadorPulsaciones = 1;
        luzOn();
        setTimeoutBoton(self); // No lo puede interpretar como un puntero a Controlador Luz?
        setTimeoutLuz(self);
        resultado.codigo = RES_TRANSICION;
        resultado.param = Estado_encendido;
        TRACE_EXIT(0);
        break;
    default:
        resultado.codigo = RES_IGNORADO;
    }
    return resultado;
}

static Resultado Estado_encendido(Maquina *contexto, Evento evento)
{
    ControladorLuz *self = (ControladorLuz *)contexto;
    Resultado resultado = {0};
    switch (evento)
    {
    case EV_BOTON:
        TRACE_ENTER(1);
        self->contadorPulsaciones++;
        if (self->contadorPulsaciones >= 3)
        {
            resultado.codigo = RES_TRANSICION;
            resultado.param = Estado_mudanza;
            self->contadorPulsaciones = 0;
        }
        TRACE_EXIT(1);
        break;
    case EV_TIMEOUT_BOTON:
        TRACE_ENTER(2);
        self->contadorPulsaciones = 0;
        resultado.codigo = RES_PROCESADO;
        TRACE_EXIT(2);
        break;
    case EV_TIMEOUT_LUZ:
        TRACE_ENTER(3);
        luzOff();
        resultado.codigo = RES_TRANSICION;
        resultado.param = Estado_apagado;
        TRACE_EXIT(3);
        break;
    default:
        resultado.codigo = RES_IGNORADO;
    }
    return resultado;
}

static Resultado Estado_mudanza(Maquina *contexto, Evento evento)
{
    ControladorLuz *self = (ControladorLuz *)contexto;
    Resultado resultado = {0};
    switch (evento)
    {
    case EV_BOTON:
        TRACE_ENTER(4);
        self->contadorPulsaciones++;
        setTimeoutBoton(self);
        if (self->contadorPulsaciones == 1)
        {
            // setTimeoutBoton(self);
        }
        else if (self->contadorPulsaciones >= 3)
        {
            luzOff();
            resultado.codigo = RES_TRANSICION;
            resultado.param = Estado_apagado;
        }
        TRACE_EXIT(4);
        break;
    case EV_TIMEOUT_BOTON:
        TRACE_ENTER(5);
        self->contadorPulsaciones = 0;
        resultado.codigo = RES_PROCESADO;
        TRACE_EXIT(5);
        break;
    default:
        resultado.codigo = RES_IGNORADO;
    }
    return resultado;
}
