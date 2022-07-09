#include "AI/OPAG_TaskBase.h"
#include <BlueprintNodeHelpers.h>

//Method to print the Behavior Tree Node's property on its description (on the node itself)
FString UOPAG_TaskBase::GetStaticDescription() const
{
	FString NodeDescription;

	//TODO: Remove when finished with the porting!
	NodeDescription += TEXT("THIS IS A C++ NODE\n\n");

	//Inserts the name of the node
	NodeDescription += Super::GetStaticDescription();

	TArray<FProperty*> PropertiesToExclude;
	FString PropertyDescription = BlueprintNodeHelpers::CollectPropertyDescription(this, UBTTaskNode::StaticClass(), PropertiesToExclude);
	//If there re properties it adds them to the node's description
	if (PropertyDescription.Len())
	{
		NodeDescription += TEXT(":\n\n");
		NodeDescription += PropertyDescription;
	}

	return NodeDescription;
}