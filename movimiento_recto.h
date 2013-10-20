#pragma once
#include "movimiento.h"

namespace movimiento
{
class mov_recto : public sub_mov
{
protected:
    vel_mot actualizar_imp(util::micros_t) override;
public:
	mov_recto(double);
};
}