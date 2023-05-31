#include "control_luz.h"
#include "maquina_impl.h"

int contadorPulsaciones = 0 ;

// Estados

static Resultado Estado_apagado(Maquina *contexto, Evento evento);
static Resultado Estado_encendido(Maquina *contexto, Evento evento);

Maquina * ControladorLuz_init(ControladorLuz *self,TiempoMilisegundos tiempoLuz, TiempoMilisegundos tiempoBotonTotal){
    Maquina_init(&self->maquina,Estado_apagado);
    self->tiempoLuz = tiempoLuz;
    self->tiempoBoton  = tiempoBotonTotal/2;
}

static Resultado Estado_apagado(Maquina * contexto, Evento evento){
    Resultado resultado = {0};
    ControladorLuz *self = (ControladorLuz*) contexto; //Por que pone este Cast?
    switch(evento){
    case EV_RESET:

        resultado.codigo = RES_PROCESADO;
    break; case EV_BOTON:
        contadorPulsaciones++;
        
        setTimeoutBoton((ControladorLuz *) contexto); //No lo puede interpretar como un puntero a Controlador Luz?
        setTimeoutLuz  ((ControladorLuz *) contexto);
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
        case EV_BOTON:
            contadorPulsaciones++;
            if (contadorPulsaciones == 3) {
                resultado.codigo = RES_TRANSICION;
                resultado.param = Estado_Mudanza;
            }
            break;
        case EV_TIMEOUT_BOTON:
            contadorPulsaciones = 0;
            break;
    break; case EV_TIMEOUT_LUZ:
        if (!ignorarTIMEOUTluz) luzOff();
        resultado.codigo = RES_TRANSICION;
        resultado.param = Estado_apagado;
    break; default:
        resultado.codigo = RES_IGNORADO;
    }
    return resultado;
}

static Resultado Estado_Mudanza(Maquina *context, evento evento){

}