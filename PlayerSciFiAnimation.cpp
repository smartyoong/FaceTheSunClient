// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSciFiAnimation.h"
#include "FaceTheSunCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerSciFiAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	/*¼Óµµ*/
	auto Pawn = Cast<AFaceTheSunCharacter>(TryGetPawnOwner());
	if (::IsValid(Pawn))
	{
		AFaceTheSunCharacter* Character = Cast<AFaceTheSunCharacter>(Pawn);
		if (Character)
		{
			speed = FVector::DotProduct(Character->GetVelocity(), Character->GetActorForwardVector());
			bIsInAir = Character->GetCharacterMovement()->IsFalling();
			bIsCrouch = Character->GetCharacterMovement()->IsCrouching();
			Direction = FVector::DotProduct(Character->GetActorRightVector(), Character->GetVelocity());
			PlayerPitch = UKismetMathLibrary::NormalizedDeltaRotator(Character->GetActorRotation(), Character->GetBaseAimRotation()).Pitch;
		}
	}
}