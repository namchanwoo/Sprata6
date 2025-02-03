
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatingActor.generated.h"

class UWidgetComponent;


UENUM(BlueprintType)
enum class EActorBehavior : uint8
{
	Disappear        UMETA(DisplayName="Disappear"),    
	MeshChange       UMETA(DisplayName="MeshChange"),   
	RotationChange   UMETA(DisplayName="RotationChange")
};

UCLASS()
class SPRATABASE_API ARotatingActor : public AActor
{
	GENERATED_BODY()

public:
	ARotatingActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	void UpdateRotationSpeed();
	
	void UpdateWidgetDisplay();
	
	UFUNCTION()
	void OnCountdownTick();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Platformer|Components", meta=(AllowPrivateAccess=true))
	USceneComponent* DefaultScene;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Platformer|Components", meta=(AllowPrivateAccess=true))
	UWidgetComponent* WidgetComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Platformer|Components", meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* MeshComp;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Platformer|RotationSettings", meta=(AllowPrivateAccess=true))
	float RotationSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Platformer|BehaviorSettings", meta=(AllowPrivateAccess=true))
	EActorBehavior BehaviorMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Platformer|BehaviorSettings", meta=(AllowPrivateAccess=true))
	float CountdownTime;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Platformer|MeshChangeSettings", meta=(AllowPrivateAccess=true))
	UStaticMesh* NewMesh;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Platformer|MeshChangeSettings", meta=(AllowPrivateAccess=true))
	UMaterialInterface* NewMaterial;

private:
	FTimerHandle TimerHandle_Countdown;
	
	FTimerHandle TimerHandle_RotationDisplay;
};
