// Fill out your copyright notice in the Description page of Project Settings.


#include "LichAnimInstance.h"
#include "LichSkillActor.h"
#include "Kismet/GameplayStatics.h"
#include "FaceTheSunCharacter.h"
#include "Materials/MaterialInterface.h"
#include "Engine/World.h"
#include "LichEnemy.h"

void ULichAnimInstance::AnimNotify_LichSkill()
{
    auto Lich = Cast<ALichEnemy>(TryGetPawnOwner());
    TArray<AActor*> outAct;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(),AActor::StaticClass(),outAct);
    for(auto i : outAct)
    {
        AFaceTheSunCharacter* ch = Cast<AFaceTheSunCharacter>(i);
        if(ch)
        {
            Lich->SkillLocation = ch->GetActorLocation();
            Lich->SkillRotator = ch->GetActorRotation();
            UGameplayStatics::SpawnDecalAtLocation(GetWorld(),DecalMaterial,FVector(500.f,500.f,500.f),ch->GetActorLocation(),ch->GetActorRotation(),3.f);
        }
    }
}

void ULichAnimInstance::AnimNotify_LichDeath()
{
    auto Lich = Cast<ALichEnemy>(TryGetPawnOwner());
    Lich->Destroy();
}

