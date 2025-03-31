#include "ASGameMode.h"


void AASGameMode::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle OnTimerHandle_250ms;
	FTimerHandle OnTimerHandle_500ms;
	FTimerHandle OnTimerHandle_1000ms;
	GetWorld()->GetTimerManager().SetTimer(OnTimerHandle_250ms, this, &AASGameMode::OnTimer_250ms, .25f, true);	
	GetWorld()->GetTimerManager().SetTimer(OnTimerHandle_500ms, this, &AASGameMode::OnTimer_500ms, .5f, true);	
	GetWorld()->GetTimerManager().SetTimer(OnTimerHandle_1000ms, this, &AASGameMode::OnTimer_1000ms, 1, true);	
}

void AASGameMode::OnTimer_250ms()
{
	OnTimerEvent_250ms.Broadcast();
}

void AASGameMode::OnTimer_500ms()
{
	OnTimerEvent_500ms.Broadcast();
}

void AASGameMode::OnTimer_1000ms()
{
	OnTimerEvent_1000ms.Broadcast();
}
