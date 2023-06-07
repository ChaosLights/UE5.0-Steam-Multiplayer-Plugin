// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "ServerList.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBackClickedDelegate);
UCLASS()
class MULTIPLAYERSESSIONS_API UServerList : public UUserWidget
{
	GENERATED_BODY()
public:
	FOnBackClickedDelegate OnBackClicked;
	virtual bool Initialize() override;
	void BackClicked();
	bool bShouldExit = false;
	UPROPERTY(BlueprintReadWrite)
	UButton* BackButton;
};
