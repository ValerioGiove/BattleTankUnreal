// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSPeed)
{
	
	if (!Barrel) { return; }

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	UE_LOG(LogTemp, Warning, TEXT("%s barrel set %s"), *GetOwner()->GetName(), *StartLocation.ToString())
	UE_LOG(LogTemp, Warning, TEXT("%s hit location to %s"), *GetOwner()->GetName(), *HitLocation.ToString())
	
	//Compute outspeed
	bool TrialPath = UGameplayStatics::SuggestProjectileVelocity(
			this,
			OutLaunchVelocity,
			StartLocation,
			HitLocation,
			LaunchSPeed,
			ESuggestProjVelocityTraceOption::DoNotTrace
		);


	if(TrialPath)
	{
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();

		UE_LOG(LogTemp, Warning, TEXT("aiming at %s"), *AimDirection.ToString())
		MoveBarrelTowards(AimDirection);

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("suggestvelocityfail"))
	}
	
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel * BarrelToSet)
{
	Barrel = BarrelToSet;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(5);
}

