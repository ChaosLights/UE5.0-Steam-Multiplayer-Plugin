// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Menu.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConstructResultDelegate);

UCLASS(BlueprintType, Blueprintable)
class MULTIPLAYERSESSIONS_API UMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	class UServerList* ServerListAccess;
	UPROPERTY(BlueprintAssignable)
	FOnConstructResultDelegate ConstructResult;
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumberOfPublicConnections = 4, FString MatchType = FString(TEXT("FreeForAll")), FString LobbyPath = FString(TEXT("/Game/ThirdPerson/Maps/Lobby")));
	UPROPERTY(BlueprintReadOnly)
	int32 NumOfResults = 0;
	UPROPERTY(BlueprintReadOnly)
	FString LastMatchType;
	UPROPERTY(BlueprintReadOnly)
	FString ServerAddress;
	UPROPERTY(BlueprintReadOnly)
	FString PlayerName;
	UPROPERTY(BlueprintReadOnly)
	int32 PlayerNum;
	UPROPERTY(BlueprintReadOnly)
	int32 NumOfPublicConnections;
	UPROPERTY(BlueprintReadOnly)
	float Ping;
	
protected:
	virtual bool Initialize() override;
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& Results, bool bWasSuccess);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void OnDestroySession(bool bWasSuccess);
	UFUNCTION()
	void OnStartSession(bool bWasSuccess);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UFUNCTION()
	void HostButtonClicked();
	UFUNCTION()
	void JoinButtonClicked();
	UFUNCTION()
	void MenuTearDown();
	int32 NumPublicConnections{4};
	FString MatchType{TEXT("")};
	UPROPERTY()
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
	FString PathToLobby{TEXT("")};
	IOnlineSessionPtr SessionInterface;
	
};
