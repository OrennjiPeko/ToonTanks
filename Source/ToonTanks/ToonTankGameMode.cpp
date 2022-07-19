// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTankGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Turret.h"
#include"ToonTanksPlayerController.h"

void AToonTankGameMode::BeginPlay()
{
	Super::BeginPlay();
	HandleGameStart();
}


void AToonTankGameMode::ActorDied(AActor* DeadActor) 
{
	if (DeadActor == Tank) 
	{
		Tank->TankHandleDestruction();
		if (ToonTanksPlayerController) 
		{
			ToonTanksPlayerController->SetPlayerEnabledState(false);
		}
		GameOver(false);
	}
	else if (ATurret* DestoryedTurret = Cast<ATurret>(DeadActor)) 
	{
		DestoryedTurret->TurretHandleDestruction();
		--TargetTurrets;
		if (TargetTurrets == 0) 
		{
			GameOver(true);
		}
	}

	
}

void AToonTankGameMode::HandleGameStart() 
{
	TargetTurrets = GetTargetTurrtCount();
	Tank = Cast <ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();

	if (ToonTanksPlayerController) {
		ToonTanksPlayerController->SetPlayerEnabledState(false);
		FTimerHandle PlayerEnableTimerHandle;
		FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
			ToonTanksPlayerController,
			&AToonTanksPlayerController::SetPlayerEnabledState,
			true
		);

		GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle,
			PlayerEnableTimerDelegate,
			StartDelay,
			false
		);
	}

}

 int32 AToonTankGameMode::GetTargetTurrtCount()
{
	 TArray<AActor*> Turrets;
	 UGameplayStatics::GetAllActorsOfClass(this, ATurret::StaticClass(), Turrets);
	 return Turrets.Num();
}

