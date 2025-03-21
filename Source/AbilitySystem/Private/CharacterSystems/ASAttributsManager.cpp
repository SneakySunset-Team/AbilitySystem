#include "AbilitySystem/Public/CharacterSystems/ASAttributsManager.h"
#include "NiagaraCommon.h"
#include "Animation/AnimNode_Inertialization.h"

UASAttributsManager::UASAttributsManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UASAttributsManager::BeginPlay()
{
	Super::BeginPlay();
}

void UASAttributsManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UASAttributsManager::EditStat(EStat InStat, float InValue)
{
	Attributs->Stats[InStat] -= InValue;
}

