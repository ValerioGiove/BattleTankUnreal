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
		GetControlledTank()->AimAt(HitLocation);
		//UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"), *HitLocation.ToString())
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	//Find crosshair in pixel adapting to screensize
	int32 ViewportX, ViewportY;
	GetViewportSize(ViewportX, ViewportY);
	auto ScreenLocation = FVector2D(ViewportX * CrossHairXLocation, ViewportY * CrossHairYLocation);

	FVector LookDirection;
	if (GetLookDirecton(ScreenLocation, LookDirection))
	{
		//Line direction and hit coord
		GetLookVectorHitLocation(LookDirection, HitLocation);
		//UE_LOG(LogTemp, Warning, TEXT("HitLocation: %s"), *HitLocation.ToString())
	}

	return true;
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);

	if (GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECollisionChannel::ECC_Visibility)
		)
	{
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0);
	return false;

}

bool ATankPlayerController::GetLookDirecton(FVector2D ScreenLocation, FVector& LookDirection) const
{
	//Deprojecting
	FVector CameraWorldProjection;//Discard
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldProjection, LookDirection);

}