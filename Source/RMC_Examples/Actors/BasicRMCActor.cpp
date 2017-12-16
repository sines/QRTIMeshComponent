// Copyright 2016 Chris Conway (Koderz). All Rights Reserved.

#include "RMC_Examples.h"
#include "BasicRMCActor.h"
#include "RuntimeMeshLibrary.h"


void ABasicRMCActor::OnConstruction(const FTransform& Transform)
{
	TArray<FVector> Vertices;
	TArray<FVector> Normals;
	TArray<FRuntimeMeshTangent> Tangents;
	TArray<FVector2D> TextureCoordinates;
	TArray<int32> Triangles;

//	URuntimeMeshLibrary::CreateBoxMesh(FVector(100, 100, 100), Vertices, Triangles, Normals, TextureCoordinates, Tangents);
	FScaleMatrix fScale(FVector(1,1,1));
	FMatrix InMatrix = FMatrix::Identity;
	FMatrix AxisRotation = FMatrix::Identity;
	AxisRotation = FRotationMatrix::MakeFromYZ(FVector(0, 1, 0), FVector(0, 0, 1));

	FVector vectorBase = FVector(0,0,0);
	URuntimeMeshLibrary::BuildCylinderMesh(vectorBase, FVector(1,0,0), FVector(0,1,0), FVector(0,0,1), 2,60,16,Vertices, Triangles);
	RuntimeMesh->CreateMeshSection(0, Vertices, Triangles, Normals, TextureCoordinates, TArray<FColor>(), Tangents, true, EUpdateFrequency::Infrequent);

	// Create the mesh section specifying collision
	float Angle = FMath::DegreesToRadians(PI * 5);
	float scale = 10.0f;
	float XOffset = 1.0;
	int32 numsides = 32;
	TArray<FVector> ConeVertices;
	TArray<FVector> ConeNormals;
	TArray<int32> ConeTriangles;
	URuntimeMeshLibrary::BuildConeVerts(Angle, Angle, scale, XOffset, numsides, ConeVertices, ConeTriangles);
	RuntimeMesh->CreateMeshSection(1, ConeVertices, ConeTriangles, ConeNormals, TextureCoordinates, TArray<FColor>(), Tangents, true, EUpdateFrequency::Infrequent);
	
}

void ABasicRMCActor::Destroyed()
{
	delete QuVRCoordinateAxis;
	QuVRCoordinateAxis = 0;
}


void ABasicRMCActor::CreateXXX()
{
	QuVRCoordinateAxis = new FQuVRCoordinateAxis();
}
void ABasicRMCActor::SetXXXType(EAxisList::Type xxxtype)
{
	QuVRCoordinateAxis->SetCurrentAxis(xxxtype);
}

void ABasicRMCActor::GetXXX(const FVector& InLocation, const FVector2D& InMousePosition, FVector& OutDrag, FRotator& OutRotation, FVector& OutScale)
{

	UWorld* world = GetWorld();
	check(world);
	APlayerCameraManager* cameraManager = world->GetFirstPlayerController()->PlayerCameraManager;
	ULocalPlayer* localPlayer = world->GetFirstLocalPlayerFromController();
	FSceneViewFamilyContext viewFamily(
		FSceneViewFamily::ConstructionValues(
			localPlayer->ViewportClient->Viewport,
			world->Scene,
			localPlayer->ViewportClient->EngineShowFlags).SetRealtimeUpdate(true));
	FVector ViewLocation;
	FRotator ViewRotation;
	FSceneView* SceneView = localPlayer->CalcSceneView(&viewFamily, ViewLocation, ViewRotation, localPlayer->ViewportClient->Viewport);
	
	//calculate mouse position
	check(localPlayer->ViewportClient->Viewport);
	//////////////////////////////////////////////////////////////////////////
	QuVRCoordinateAxis->AbsoluteTranslationConvertMouseMovementToAxisMovement(SceneView, InLocation, InMousePosition, OutDrag, OutRotation, OutScale);
}
