#include "Weapons/Modules/SpecializedModules/Accessory/OPAG_AccessoryThrowable.h"
#include "Weapons/Throwables/OPAG_Throwable.h"

AOPAG_AccessoryThrowable::AOPAG_AccessoryThrowable()
{

}

void AOPAG_AccessoryThrowable::BeginPlay()
{
	Super::BeginPlay();
}

void AOPAG_AccessoryThrowable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TSubclassOf<AOPAG_Throwable> AOPAG_AccessoryThrowable::GetThrowable() const
{
	return Throwable;
}