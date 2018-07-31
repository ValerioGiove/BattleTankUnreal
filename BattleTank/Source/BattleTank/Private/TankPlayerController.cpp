// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto PossesedTank = GetControlledTank();

	if (!PossesedTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller not possessing a tank"))
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Player Controller possessing: %s"),*PossesedTank->GetName())
	}
	
}



ATank * ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) { return; }

	FVector HitLocation; //Out Parameter
	if (GetSightRayHitLocation(HitLocation))
	{
		UE_LOG(LogTemp, Warning, TEXT("HitLocation: %s"), *HitLocation.ToString())
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	//Find crosshair in pixel adapting to screensize
	int32 ViewportX, ViewportY;
	GetViewportSize(ViewportX, ViewportY);
	auto ScreenLocation = FVector2D(ViewportX * CrossHairXLocation, ViewportY * CrossHairYLocation);

	//Deprojecting
	FVector CameraWorldProjection;
	FVector WorldDirection;

	if (DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldProjection, WorldDirection))
	{
		UE_LOG(LogTemp, Warning, TEXT("HitLocation: %s"), *WorldDirection.ToString())
	}

	return true;
}
