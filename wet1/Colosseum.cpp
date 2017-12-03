//
//  Colosseum.cpp
//  wet1
//
//  Created by Ben on 26/11/2017.
//  Copyright © 2017 Ben. All rights reserved.
//

#include "Colosseum.hpp"
#include "Gladiator.hpp"
using std::bad_alloc;

/* Description:   Adds a new trainer.
 * Input:         trainerID - The ID of the trainer to add.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - if trainerID <= 0.
 *                FAILURE - If trainerID is already in the Colosseum.
 *                SUCCESS - Otherwise.
 */
StatusType Colosseum::AddTrainer(int trainerID) {
	if (trainerID <= 0) return INVALID_INPUT;
	if (trainers.exists(trainerID)) return FAILURE;
	try {
		trainers.addTrainer(trainerID);
	}
	catch (const bad_alloc& e) {
		return ALLOCATION_ERROR;
	};
	return SUCCESS;
}


/* Description:   Adds a new gladiator to the system.
 * Input:		  gladiatorID - ID of the gladiator to add.
 *                trainerID - The ID of the gladiator's trainer
 *                level - The gladiator's level
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - if gladiatorID <=0, or if trainerID <=0,
 *				  or if level <= 0
 *                FAILURE - If gladiatorID is already in the Colosseum,
 *				  or trainerID isn't in the Colosseum.
 *                SUCCESS - Otherwise.
 */
StatusType Colosseum::BuyGladiator(int gladiatorID, int trainerID, int level) {
	if ((gladiatorID<=0) || (trainerID <=0) || (level<=0)) return INVALID_INPUT;
	PointingGladiator dummy(gladiatorID,level,NULL);
	if ( gladiatorIdTree.exist(dummy) || (!trainers.exists(trainerID)) )
		return FAILURE;
	try {
		
		Trainer* trainerP = trainers.findTrainer(trainerID);
		PointingGladiator* gladForIdTree = new PointingGladiator(gladiatorID,
															  level, trainerP);
		Gladiator* gladForLevelTree = new Gladiator(gladiatorID, level);
		trainers.addGladiator(trainerID, gladiatorID, level);
		gladiatorIdTree.insert(gladForIdTree);
		gladiatorLevelTree.insert(gladForLevelTree);
		
	}
	catch (const bad_alloc& e) {
		return ALLOCATION_ERROR;
	}
	return SUCCESS;
}



/* Description:   Removes an existing gladiator.
 * Input:		  gladiatorID - The ID of the gladiator to remove.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If gladiatorID <= 0.
 *                FAILURE - If gladiatorID isn't in the Colosseum.
 *                SUCCESS - Otherwise.
 */

StatusType Colosseum::FreeGladiator(int gladiatorID) {
	if (gladiatorID <= 0) return INVALID_INPUT;
	PointingGladiator dummy(gladiatorID, 100, NULL);
	if (!gladiatorIdTree.exist(dummy)) return FAILURE;
	PointingGladiator* gladiatorToFree = gladiatorIdTree.find(dummy);
	Gladiator dummy2(gladiatorID, gladiatorToFree->level);
	gladiatorIdTree.remove(dummy);
	gladiatorLevelTree.remove(dummy2);
	trainers.removeGladiator(gladiatorToFree->owner->id, gladiatorID,
							 gladiatorToFree->level);
	return SUCCESS;
}


/* Description:   Increases the level of a gladiator.
 * Input:         gladiatorID - The ID of the gladiator.
 *		          levelIncrease - The increase in level.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If gladiatorID<=0, or if levelIncrease<=0
 *                FAILURE - If gladiatorID isn't in the Colosseum.
 *                SUCCESS - Otherwise.
 */
StatusType Colosseum::LevelUp(int gladiatorID, int levelIncrease){
	if (gladiatorID<=0 || levelIncrease <= 0 ) return INVALID_INPUT;
	PointingGladiator dummy(gladiatorID, 100, NULL);
	if (!gladiatorIdTree.exist(dummy)) return FAILURE;
	try {
		PointingGladiator* gladiatorToFree = gladiatorIdTree.find(dummy);
		int newGladiatorLevel = levelIncrease + gladiatorToFree->level;
		Trainer* pTrainer = gladiatorToFree->owner;
		
		FreeGladiator(gladiatorID);

		//todo make names nicer
		Gladiator* gladiatorToAddToTrainer = new Gladiator(gladiatorID,
												 newGladiatorLevel);
		Gladiator* gladiatorToAddToLevelTree = new Gladiator(gladiatorID, newGladiatorLevel);
		
		PointingGladiator* gladiatorToAddToIDTree = new PointingGladiator(gladiatorID, newGladiatorLevel, pTrainer);
		
		
		pTrainer->gladiators.insert(gladiatorToAddToTrainer);
		gladiatorIdTree.insert(gladiatorToAddToIDTree);
		gladiatorLevelTree.insert(gladiatorToAddToLevelTree);
		
		
	}
	catch (const bad_alloc& e) {
		return ALLOCATION_ERROR;
	}
	
	return SUCCESS;
}


StatusType Colosseum::UpgradeGladiator(int gladiatorID, int upgradedID){
	return SUCCESS;
}

/* Description:   Returns the gladiator with the highest level from trainerID
 * 			If trainerID < 0, returns the top gladiator in the entire DS.
 * Input:         DS - A pointer to the data structure.
 *                trainerID - The trainer that we'd like to get the data for.
 * Output:        gladiatorID - A pointer to a variable that should be updated
 *					to the ID of the top gladiator.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL, or if gladiatorID == NULL, or
 *					if trainerID == 0.
 *                SUCCESS - Otherwise.
 */
StatusType Colosseum::GetTopGladiator(int trainerID, int *gladiatorID){
	if (!gladiatorID || trainerID == 0) return INVALID_INPUT;
	if (trainerID > 0 && !trainers.exists(trainerID)) return FAILURE;
	
	if (trainerID > 0) {
		Trainer* trainer = trainers.findTrainer(trainerID);
		*gladiatorID = trainer->gladiators.getTop()->id;
		return SUCCESS;
	}
	
	*gladiatorID = gladiatorLevelTree.getTop()->id;
	return SUCCESS;
}


StatusType Colosseum::GetAllGladiatorsByLevel(int trainerID, int **gladiators,
								   int *numOfGladiator){
	return SUCCESS;
}


StatusType Colosseum	::UpdateLevels(int stimulantCode, int stimulantFactor){
	return SUCCESS;
}
