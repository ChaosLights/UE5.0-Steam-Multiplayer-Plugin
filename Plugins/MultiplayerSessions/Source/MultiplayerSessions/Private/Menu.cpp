// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "ServerList.h"

void UMenu::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;
	PathToLobby = LobbyPath;
	UWorld* World = GetWorld();
	if(World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if(PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}
	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}
	if(MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
		MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}
}

bool UMenu::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	if(HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &UMenu::HostButtonClicked);
	}
	if(JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UMenu::JoinButtonClicked);
	}
	return true;
}

void UMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
	MenuTearDown();
}

void UMenu::OnCreateSession(bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		UWorld* World = GetWorld();
		if(World)
		{
			World->ServerTravel(PathToLobby+"?listen");
		}
	}
	else
	{
		HostButton->SetIsEnabled(true);
	}
}

void UMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& Results, bool bWasSuccess)
{
	if(MultiplayerSessionsSubsystem == nullptr)
	{
		return;
	}
	NumOfResults = Results.Num();
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if(Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
	}
	NumOfPublicConnections = NumPublicConnections;
	for(auto Result : Results)
	{
		if(ServerListAccess->bShouldExit)
		{
			ServerListAccess->bShouldExit = true;
			JoinButton->SetIsEnabled(true);
			return;
		}
		FString SettingValue;
		Result.Session.SessionSettings.Get("MatchType", SettingValue);
		if(SettingValue == MatchType)
		{
			ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			if(SessionInterface.IsValid())
			{
				SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result);
				APlayerState* PlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<APlayerState>();
				Ping = PlayerState->GetPingInMilliseconds();
				SessionInterface->GetResolvedConnectString(NAME_GameSession, ServerAddress);
				PlayerNum = SessionInterface->GetNamedSession(NAME_GameSession)->RegisteredPlayers.Num();
			}
			PlayerName = Result.Session.OwningUserName;
			LastMatchType = SettingValue;
			ConstructResult.Broadcast();
		}
	}
	JoinButton->SetIsEnabled(true);
	if(!bWasSuccess || Results.Num() == 0)
	{
		JoinButton->SetIsEnabled(true);
	}
}

void UMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	FString Address;
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Join Success")));
	if(Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if(SessionInterface.IsValid())
		{
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);
		}
	}
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	if(Result != EOnJoinSessionCompleteResult::Success)
	{
		JoinButton->SetIsEnabled(true);
	}
}

void UMenu::OnDestroySession(bool bWasSuccess)
{
}

void UMenu::OnStartSession(bool bWasSuccess)
{
}

void UMenu::HostButtonClicked()
{
	HostButton->SetIsEnabled(false);
	if(MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->CreateSession(NumPublicConnections, MatchType);
	}
	
}

void UMenu::JoinButtonClicked()
{
	NumOfResults = 0;
	JoinButton->SetIsEnabled(false);
	if(MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->FindSessions(10000);
	}
}
void UMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World= GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if(PlayerController)
	{
		FInputModeGameOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->SetShowMouseCursor(false);
	}
}