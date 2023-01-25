#pragma once

#include "EntityMesh.h"
#include "physics.h"


enum CarType : uint8 { CAR1, CAR2, CAR3, CAR4, CAR5, CAR6, TRUCK1, TRUCK2, TRUCK3, BUS1, BUS2, BUS3, COUNT };

class Car : public EntityMesh
{
public:
	int roadpos;

	//Contenedor de las strings de el enum
	static const std::string enum_string[13];

	CarType type;

	Physics physics/*int cartype*/;

	Car(CarType cartype);


	void update();
};

