#include "control_luz.h"
#include "maquina_impl.h"
#include "BluePillHal.h"
#include "trace.h"

#define BOTON B12
#define PIN_LED C13


// Estados

static Resultado Estado_apagado(Maquina *contexto, Evento evento);
static Resultado Estado_encendido(Maquina *contexto, Evento evento);
static Resultado Estado_mudanza(Maquina *contexto, Evento evento);
static ControladorLuz *controlador = 0;

Maquina *ControladorLuz_init(ControladorLuz *self, TiempoMilisegundos tiempoLuz, TiempoMilisegundos tiempoBotonTotal)
{   controlador = self;
    Maquina_init(&self->maquina, Estado_apagado);
    self->tiempoLuz = tiempoLuz;
    self->tiempoBoton = tiempoBotonTotal;
    set_tick_handler(tick_handler);
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

void luzOn(void)
{
    BP_Pin_set(PIN_LED, 0);
}
void luzOff(void)
{
    BP_Pin_set(PIN_LED, 1);
}
/*
void setTimeoutLuz(ControladorLuz *controlador)
{
    controlador->TO_Luz = getTicks() + controlador->tiempoLuz;
}
void setTimeoutBoton(ControladorLuz *controlador)
{
    controlador->TO_Boton = getTicks() + controlador->tiempoBoton;
}
*/

void setTimeoutLuz(ControladorLuz *controlador)
{
    controlador->TO_Luz =  controlador->tiempoLuz;
}
void setTimeoutBoton(ControladorLuz *controlador)
{
    controlador->TO_Boton =  controlador->tiempoBoton;
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
/*
static void checkTIMEOUT(ControladorLuz *controlador)
{
    Maquina *maquina = (Maquina *)controlador;
    if (getTicks() == controlador->TO_Boton)
        Maquina_despacha(maquina, EV_TIMEOUT_BOTON);
    if (getTicks() == controlador->TO_Luz)
        Maquina_despacha(maquina, EV_TIMEOUT_LUZ);
}
*/
static void checkTIMEOUT (ControladorLuz *controlador)
{   if (controlador == 0) return;

    Maquina *maquina = (Maquina*)controlador;
    if (controlador->TO_Boton > 0 )
    {
        controlador->TO_Boton --;
    }
    else if (0 == controlador->TO_Boton )
    {
        Maquina_despacha(maquina, EV_TIMEOUT_BOTON);
    }
    if (controlador->TO_Luz > 0 )
    {
        controlador->TO_Luz --;
    }
    else if (0 == controlador->TO_Luz )
    {
        Maquina_despacha(maquina, EV_TIMEOUT_LUZ);
    }
}

void tick_handler (void)
{
    checkTIMEOUT((ControladorLuz *)controlador);
}
