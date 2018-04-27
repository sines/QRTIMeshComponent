// Fill out your copyright notice in the Description page of Project Settings.

#include "QuVRAssetFactoryAnimation.h"
#include "Runtime/Engine/Classes/Animation/SkeletalMeshActor.h"
#include "Editor/UnrealEd/Classes/ActorFactories/ActorFactoryAnimationAsset.h"
#include "Runtime/AssetRegistry/Public/IAssetRegistry.h"
#include "Runtime/AssetRegistry/Public/AssetRegistryModule.h"
#include "Runtime/Engine/Classes/Engine/DestructibleMesh.h"
#include "Runtime/Engine/Classes/Animation/AnimBlueprint.h"
#include "Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "AssetData.h"
#define LOCTEXT_NAMESPACE "QuVRAssetFactoryAnimation"

/**
* Find am alignment transform for the specified actor rotation, given a model-space axis to align, and a world space normal to align to.
* This function attempts to find a 'natural' looking rotation by rotating around a local pitch axis, and a world Z. Rotating in this way
* should retain the roll around the model space axis, removing rotation artifacts introduced by a simpler quaternion rotation.
*/
FQuat FindActorAlignmentRotation(const FQuat& InActorRotation, const FVector& InModelAxis, const FVector& InWorldNormal)
{
	FVector TransformedModelAxis = InActorRotation.RotateVector(InModelAxis);

	const auto InverseActorRotation = InActorRotation.Inverse();
	const auto DestNormalModelSpace = InverseActorRotation.RotateVector(InWorldNormal);

	FQuat DeltaRotation = FQuat::Identity;

	const float VectorDot = InWorldNormal | TransformedModelAxis;
	if (1.f - FMath::Abs(VectorDot) <= KINDA_SMALL_NUMBER)
	{
		if (VectorDot < 0.f)
		{
			// Anti-parallel
			return InActorRotation * FQuat::FindBetween(InModelAxis, DestNormalModelSpace);
		}
	}
	else
	{
		const FVector Z(0.f, 0.f, 1.f);

		// Find a reference axis to measure the relative pitch rotations between the source axis, and the destination axis.
		FVector PitchReferenceAxis = InverseActorRotation.RotateVector(Z);
		if (FMath::Abs(FVector::DotProduct(InModelAxis, PitchReferenceAxis)) > 0.7f)
		{
			PitchReferenceAxis = DestNormalModelSpace;
		}

		// Find a local 'pitch' axis to rotate around
		const FVector OrthoPitchAxis = FVector::CrossProduct(PitchReferenceAxis, InModelAxis);
		const float Pitch = FMath::Acos(PitchReferenceAxis | DestNormalModelSpace) - FMath::Acos(PitchReferenceAxis | InModelAxis);//FMath::Asin(OrthoPitchAxis.Size());

		DeltaRotation = FQuat(OrthoPitchAxis.GetSafeNormal(), Pitch);
		DeltaRotation.Normalize();

		// Transform the model axis with this new pitch rotation to see if there is any need for yaw
		TransformedModelAxis = (InActorRotation * DeltaRotation).RotateVector(InModelAxis);

		const float ParallelDotThreshold = 0.98f; // roughly 11.4 degrees (!)
		if (!FVector::Coincident(InWorldNormal, TransformedModelAxis, ParallelDotThreshold))
		{
			const float Yaw = FMath::Atan2(InWorldNormal.X, InWorldNormal.Y) - FMath::Atan2(TransformedModelAxis.X, TransformedModelAxis.Y);

			// Rotation axis for yaw is the Z axis in world space
			const FVector WorldYawAxis = (InActorRotation * DeltaRotation).Inverse().RotateVector(Z);
			DeltaRotation *= FQuat(WorldYawAxis, -Yaw);
		}
	}

	return InActorRotation * DeltaRotation;
}


/*-----------------------------------------------------------------------------
UQuVRAssetFactoryAnimation
-----------------------------------------------------------------------------*/
UQuVRAssetFactoryAnimation::UQuVRAssetFactoryAnimation(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisplayName = LOCTEXT("SingleAnimSkeletalDisplayName", "Single Animation Skeletal");
	NewActorClass = ASkeletalMeshActor::StaticClass();
}


bool UQuVRAssetFactoryAnimation::CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg)
{
	if (!AssetData.IsValid() ||
		(!AssetData.GetClass()->IsChildOf(UAnimSequenceBase::StaticClass())))
	{
		OutErrorMsg = NSLOCTEXT("CanCreateActor", "NoAnimData", "A valid anim data must be specified.");
		return false;
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	if (AssetData.GetClass()->IsChildOf(UAnimSequenceBase::StaticClass()))
	{
		const FString SkeletonPath = AssetData.GetTagValueRef<FString>("Skeleton");
		if (SkeletonPath.IsEmpty())
		{
			OutErrorMsg = NSLOCTEXT("CanCreateActor", "NoSkeleton", "UAnimationAssets must have a valid Skeleton.");
			return false;
		}

		FAssetData SkeletonData = AssetRegistry.GetAssetByObjectPath(*SkeletonPath);

		if (!SkeletonData.IsValid())
		{
			OutErrorMsg = NSLOCTEXT("CanCreateActor", "NoSkeleton", "UAnimationAssets must have a valid Skeleton.");
			return false;
		}

		// skeleton should be loaded by this time. If not, we have problem
		// so I'm changing this to load directly not using tags and values
		USkeleton* Skeleton = Cast<USkeleton>(SkeletonData.GetAsset());
		if (Skeleton)
		{
			USkeletalMesh * PreviewMesh = Skeleton->GetPreviewMesh(true);
			if (PreviewMesh)
			{
				return true;
			}
			else
			{
				OutErrorMsg = NSLOCTEXT("CanCreateActor", "UAnimationAssetNoSkeleton", "UAnimationAssets must have a valid Skeleton with a valid preview skeletal mesh.");
				return false;
			}
		}
		else
		{
			OutErrorMsg = NSLOCTEXT("CanCreateActor", "NoSkeleton", "UAnimationAssets must have a valid Skeleton.");
			return false;
		}
	}

	FAssetData SkeletalMeshData;

	if (!SkeletalMeshData.IsValid())
	{
		OutErrorMsg = NSLOCTEXT("CanCreateActor", "NoSkeletalMeshAss", "No valid skeletal mesh was found associated with the animation sequence.");
		return false;
	}

	// Check to see if it's actually a DestructibleMesh, in which case we won't use this factory
	if (SkeletalMeshData.GetClass()->IsChildOf(UDestructibleMesh::StaticClass()))
	{
		OutErrorMsg = NSLOCTEXT("CanCreateActor", "NoDestructibleMesh", "The animation sequence must not have a DestructibleMesh associated with it.");
		return false;
	}

	return true;
}

USkeletalMesh* UQuVRAssetFactoryAnimation::GetSkeletalMeshFromAsset(UObject* Asset) const
{
	USkeletalMesh* SkeletalMesh = NULL;

	if (UAnimSequenceBase* AnimationAsset = Cast<UAnimSequenceBase>(Asset))
	{
		// base it on preview skeletal mesh, just to have something
		SkeletalMesh = AnimationAsset->GetSkeleton() ? AnimationAsset->GetSkeleton()->GetAssetPreviewMesh(AnimationAsset) : nullptr;
	}
	else if (UAnimBlueprint* AnimBlueprint = Cast<UAnimBlueprint>(Asset))
	{
		SkeletalMesh = AnimBlueprint->TargetSkeleton ? AnimBlueprint->TargetSkeleton->GetAssetPreviewMesh(AnimBlueprint) : nullptr;
	}

	// Check to see if it's actually a DestructibleMesh, in which case we won't use this factory
	if (SkeletalMesh != NULL && SkeletalMesh->IsA(UDestructibleMesh::StaticClass()))
	{
		SkeletalMesh = NULL;
	}

	check(SkeletalMesh != NULL);
	return SkeletalMesh;
}

void UQuVRAssetFactoryAnimation::PostSpawnActor(UObject* Asset, AActor* NewActor)
{
	USkeletalMesh* SkeletalMesh = GetSkeletalMeshFromAsset(Asset);
	UAnimBlueprint* AnimBlueprint = Cast<UAnimBlueprint>(Asset);
	ASkeletalMeshActor* NewSMActor = CastChecked<ASkeletalMeshActor>(NewActor);

	Super::PostSpawnActor(Asset, NewActor);
	// Term Component
	NewSMActor->GetSkeletalMeshComponent()->UnregisterComponent();

	// Change properties
	NewSMActor->GetSkeletalMeshComponent()->SkeletalMesh = SkeletalMesh;
	if (NewSMActor->GetWorld()->IsGameWorld())
	{
		NewSMActor->ReplicatedMesh = SkeletalMesh;
	}

	// Init Component
	NewSMActor->GetSkeletalMeshComponent()->RegisterComponent();
	if (AnimBlueprint)
	{
		NewSMActor->GetSkeletalMeshComponent()->SetAnimInstanceClass(AnimBlueprint->GeneratedClass);
	}

	UAnimationAsset* AnimationAsset = Cast<UAnimationAsset>(Asset);
	USkeletalMeshComponent* NewSASComponent = (NewSMActor->GetSkeletalMeshComponent());
	NewSMActor->AddActorLocalOffset(FVector(0, 0, 0.1));
	if (NewSASComponent)
	{
		if (AnimationAsset)
		{
			NewSASComponent->SetAnimationMode(EAnimationMode::Type::AnimationSingleNode);
			NewSASComponent->AnimationData.AnimToPlay = AnimationAsset;
			NewSASComponent->AnimationData.bSavedLooping = 0;
			NewSASComponent->AnimationData.bSavedPlaying = 0;
			// set runtime data
			NewSASComponent->SetAnimation(AnimationAsset);

			if (UAnimSequenceBase* AnimSeq = Cast<UAnimSequenceBase>(AnimationAsset))
			{
				//If we have a negative play rate, default initial position to sequence end
				if (AnimSeq->RateScale < 0.f)
				{
					NewSASComponent->AnimationData.SavedPosition = AnimSeq->SequenceLength;
					NewSASComponent->SetPosition(AnimSeq->SequenceLength, false);
				}
			}

		}
	}
}

void UQuVRAssetFactoryAnimation::PostCreateBlueprint(UObject* Asset, AActor* CDO)
{
	Super::PostCreateBlueprint(Asset, CDO);

	if (Asset != NULL && CDO != NULL)
	{
		// Set
		USkeletalMesh* SkeletalMesh = GetSkeletalMeshFromAsset(Asset);
		UAnimBlueprint* AnimBlueprint = Cast<UAnimBlueprint>(Asset);

		ASkeletalMeshActor* SkeletalMeshActor = CastChecked<ASkeletalMeshActor>(CDO);
		SkeletalMeshActor->GetSkeletalMeshComponent()->SkeletalMesh = SkeletalMesh;
		if (SkeletalMeshActor->GetSkeletalMeshComponent()->AnimClass == AnimBlueprint->GetBlueprintClass())
		{
			SkeletalMeshActor->GetSkeletalMeshComponent()->SetAnimInstanceClass(AnimBlueprint->GeneratedClass);
		}

		// SetAnimationAsset
		UAnimationAsset* AnimationAsset = Cast<UAnimationAsset>(Asset);
		USkeletalMeshComponent* SkeletalComponent = (SkeletalMeshActor->GetSkeletalMeshComponent());
		if (AnimationAsset)
		{
			SkeletalComponent->SetAnimationMode(EAnimationMode::Type::AnimationSingleNode);
			SkeletalComponent->SetAnimation(AnimationAsset);
		}
	}
}

FQuat UQuVRAssetFactoryAnimation::AlignObjectToSurfaceNormal(const FVector& InSurfaceNormal, const FQuat& ActorRotation) const
{
	return FindActorAlignmentRotation(ActorRotation, FVector(0.f, 0.f, 1.f), InSurfaceNormal);
};

#undef LOCTEXT_NAMESPACE