// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryWidget.h"

#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

bool UEntryWidget::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	if(JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}
	return true;
}

void UEntryWidget::JoinButtonClicked()
{
	JoinButton->SetIsEnabled(false);
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr SessionInterface;
	if(Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if(SessionInterface.IsValid())
		{
			SessionInterface->GetResolvedConnectString(NAME_GameSession, ServerAddress);
		}
	}
	PlayerController->ClientTravel(ServerAddress, ETravelType::TRAVEL_Absolute);
}
