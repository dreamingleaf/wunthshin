// Fill out your copyright notice in the Description page of Project Settings.


#include "wunthshin/Widgets/ItemNotify/WG_WSItemNotify.h"

void UWG_WSItemNotify::SetParentItem(AA_WSItem* InParent) 
{ 
	ParentItem = InParent;
	FetchFromParentItem(InParent);
}
