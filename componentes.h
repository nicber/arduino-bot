#pragma once
#include "core_componente.h"
#include "motor.h"
#include "sensor_luz.h"
#include "movimiento.h"

extern core::cont_comp<hw::sensor::sensor_luz, movimiento::movimiento, hw::motores> componentes;