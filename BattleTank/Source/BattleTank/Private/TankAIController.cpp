// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerTank = GetPlayerTank();

	if (!PlayerTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Controller not found player tank"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Controller found player tank: %s"), *PlayerTank->GetName())
	}

}

void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (GetPlayerTank())
	{
		//Aim at player
		GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());
	}
}

ATank * ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank * ATankAIController::GetPlayerTank() const
{
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn) { return nullptr; }
	else
	{
		return Cast<ATank>(PlayerPawn);
	}
}
