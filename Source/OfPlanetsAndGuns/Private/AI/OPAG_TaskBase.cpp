#include "AI/OPAG_TaskBase.h"
#include <BlueprintNodeHelpers.h>

//ATM this class is only used to print a node properties on its description in the Behavior Tree.

//Method to print the Behavior Tree Node's properties on its description (on the node itself in the tree)
FString UOPAG_TaskBase::GetStaticDescription() const
{
	FString NodeDescription;

	//TODO: Remove when finished with the porting!
	NodeDescription += TEXT("THIS IS A C++ NODE\n\n");

	//Inserts the name of the node in the description
	NodeDescription += Super::GetStaticDescription();

	//This should be used to exclude the CDO properties from the node's description.
	//ATM it doesn't seem necessary, so it's kept empty.
	const TArray<FProperty*> PropertiesToExclude;
	//Gets all the class's properties
	const FString PropertyDescription = BlueprintNodeHelpers::CollectPropertyDescription(this, UBTTaskNode::StaticClass(), PropertiesToExclude);
	//If there are properties they get added to the node's description
	if (PropertyDescription.Len())
	{
		NodeDescription += TEXT(":\n\n");
		NodeDescription += PropertyDescription;
	}

	return NodeDescription;
}