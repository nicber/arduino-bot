#pragma once
#include "core_componente.h"
#include "motor.h"
#include "sensor_luz.h"
#include "movimiento.h"
#include "sensor_ultrason.h"

extern core::cont_comp<hw::sensor::sensor_luz, hw::sensor::sensores_ultrasonicos, movimiento::movimiento, hw::motores> componentes;