#include "control_luz.h"
#include "maquina_impl.h"


// Estados

static Resultado Estado_apagado(Maquina *contexto, Evento evento);
static Resultado Estado_encendido(Maquina *contexto, Evento evento);

Maquina * ControladorLuz_init(ControladorLuz *self,TiempoMilisegundos tiempoLuz, TiempoMilisegundos tiempoBotonTotal){
    Maquina_init(&self->maquina,Estado_apagado);
    self->tiempoLuz = tiempoLuz;
    self->tiempoBoton  = tiempoBotonTotal/2;
}

static Resultado Estado_apagado(Maquina *contexto, Evento evento){
    Resultado resultado = {0};
    ControladorLuz *self = (ControladorLuz*) contexto; //Por que pone este Cast?
    switch(evento){
    case EV_RESET:

        resultado.codigo = RES_PROCESADO;
    break; case EV_BOTON:
        setTimeout(self->tiempoOn,contexto);
        resultado.codigo = RES_TRANSICION;
        resultado.param = Estado_encendido;
    break; default:
        resultado.codigo = RES_IGNORADO;
    }
    return resultado;
}

static Resultado Estado_encendido(Maquina *contexto, Evento evento){
    (void)contexto;
    Resultado resultado = {0};
    switch(evento){
    break; case EV_TIMEOUT:
        luzOff();
        resultado.codigo = RES_TRANSICION;
        resultado.param = Estado_apagado;
    break; default:
        resultado.codigo = RES_IGNORADO;
    }
    return resultado;
}