// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSciFiAnimation.h"
#include "FaceTheSunCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TP_WeaponComponent.h"

void UPlayerSciFiAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	/*속도*/
	auto Pawn = Cast<AFaceTheSunCharacter>(TryGetPawnOwner());
	if (::IsValid(Pawn))
	{
		AFaceTheSunCharacter* Character = Cast<AFaceTheSunCharacter>(Pawn);
		if (Character)
		{
			speed = FVector::DotProduct(Character->GetVelocity(), Character->GetActorForwardVector());
			bIsInAir = Character->GetCharacterMovement()->IsFalling();
			//bIsCrouch = Character->GetCharacterMovement()->IsCrouching();
			Direction = FVector::DotProduct(Character->GetActorRightVector(), Character->GetVelocity());
			PlayerPitch = UKismetMathLibrary::NormalizedDeltaRotator(Character->GetActorRotation(), Character->GetBaseAimRotation()).Pitch;
		}
	}
}

//커스텀 애니메이션 노티파이
void UPlayerSciFiAnimation::AnimNotify_EndReload()
{
	auto Pawn = Cast<AFaceTheSunCharacter>(TryGetPawnOwner());
	if (::IsValid(Pawn))
	{
		AFaceTheSunCharacter* Character = Cast<AFaceTheSunCharacter>(Pawn);
		if (Character)
		{
			Character->Gun->EndReload();
		}
	}
}

void UPlayerSciFiAnimation::AnimNotify_EndReload1P()
{
	auto Pawn = Cast<AFaceTheSunCharacter>(TryGetPawnOwner());
	if (::IsValid(Pawn))
	{
		AFaceTheSunCharacter* Character = Cast<AFaceTheSunCharacter>(Pawn);
		if (Character)
		{
			Character->Gun1P->EndReload1P();
		}
	}
}