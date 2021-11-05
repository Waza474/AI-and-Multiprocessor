// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAPAction.h"

GOAPAction::GOAPAction()
{
	RunningTime = 0;
}

GOAPAction::~GOAPAction()
{
}

void GOAPAction::DoReset()
{
	InRange = false;
	Target = nullptr;
	// This will call the child class function
	// Polymorphism magic :D
	Reset();
}

void GOAPAction::AddPrecondition(FString Name, bool State)
{
	Preconditions.Add(Name, State);
}

void GOAPAction::RemovePrecondition(FString Name)
{
	Preconditions.Remove(Name);
}

void GOAPAction::AddEffect(FString Name, bool State)
{
	Effects.Add(Name, State);
}

void GOAPAction::RemoveEffect(FString Name)
{
	Effects.Remove(Name);
}
