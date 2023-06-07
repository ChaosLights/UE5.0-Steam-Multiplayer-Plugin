// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UEntryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;
	virtual bool Initialize() override;
	UPROPERTY(BlueprintReadWrite)
	FString ServerAddress;
private:
	UFUNCTION()
	void JoinButtonClicked();
	
};
