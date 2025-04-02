#include "Effects/ASLingeringEffect.h"

#include "ASGameMode.h"
#include "CharacterSystems/ASAttributsManager.h"

void UASLingeringEffect::ApplyEffect(UASAttributsManager* InTargetAttributsManager)
{
	if (!CanApplyEffect(InTargetAttributsManager))
	{
		return;
	}
	
	TargetAttributManager = InTargetAttributsManager;
	TargetAttributManager->AddLingeringEffect(this);
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UASLingeringEffect::OnTimerEnded, Duration == -1 ? 999999 : Duration, false);

	AASGameMode* GameMode = Cast<AASGameMode>(GetWorld()->GetAuthGameMode());
	switch (TickRate)
	{
	case EASTickRate::Quarter_s:
		GameMode->OnTimerEvent_250ms.AddDynamic(this, &UASLingeringEffect::ApplyDot);
		break;
	case EASTickRate::Half_s:
		GameMode->OnTimerEvent_500ms.AddDynamic(this, &UASLingeringEffect::ApplyDot);
		break;
	case EASTickRate::Full_s:
		GameMode->OnTimerEvent_1000ms.AddDynamic(this, &UASLingeringEffect::ApplyDot);
		break;
	}
}

void UASLingeringEffect::ApplyDot()
{
	Super::ApplyEffect(TargetAttributManager);
}

void UASLingeringEffect::OnTimerEnded()
{
	TargetAttributManager->RemoveLingeringEffect(this);
	AASGameMode* GameMode = Cast<AASGameMode>(GetWorld()->GetAuthGameMode());
	switch (TickRate)
	{
	case EASTickRate::Quarter_s:
		GameMode->OnTimerEvent_250ms.RemoveDynamic(this, &UASLingeringEffect::ApplyDot);
		break;
	case EASTickRate::Half_s:
		GameMode->OnTimerEvent_500ms.RemoveDynamic(this, &UASLingeringEffect::ApplyDot);
		break;
	case EASTickRate::Full_s:
		GameMode->OnTimerEvent_1000ms.RemoveDynamic(this, &UASLingeringEffect::ApplyDot);
		break;
	}
}
