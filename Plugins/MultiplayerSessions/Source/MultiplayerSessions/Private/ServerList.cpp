// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerList.h"
#include "Components/Button.h"


bool UServerList::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	if(BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UServerList::BackClicked);
	}
	return true;
}

void UServerList::BackClicked()
{
	bShouldExit = true;
}
