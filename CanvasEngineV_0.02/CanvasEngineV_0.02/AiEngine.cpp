#include "AiEngine.h"
#include <iostream>
#include <list>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "Engine.h"

int Math::min(int a, int b)
{
	if (a < b)
		return a;
	else
		return b;
}

int Math::max(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}

int Math::abs(int a , int b)
{
		if (a > b)
		return (a-b);
	else
		return (b-a);
}

//////////////aiRealation/////////////////////

aiRelation::aiRelation(aiPerson* A, aiPerson* B)
{
	personA = A;
	personB = B;
}

Relation* aiRelation::generateRelation()
{
	Relation* outputRelation = new Relation;
	outputRelation->A = personA;
	outputRelation->B = personB;

	int randNum = rand()%6;
	outputRelation->relations = (ai_relation)randNum;
	randNum = rand()%7;
	outputRelation->friendship = (ai_friendship)randNum;
	outputRelation->heart = randNum * 20;
	
	int i = 0;
	int sum = 0;
	while (i < Math().min(personA->idNum,personB->idNum))
		sum += (aiSize-i-1);
	sum += Math().abs(personA->idNum,personB->idNum) - 1 + aiSize;
	outputRelation->id = sum;

	//print out
	std::cout << personA->name << " and " << personA->name <<" have a relation: " << outputRelation->relations;
	return outputRelation;
}

///////////////aiEvents///////////////////////

aiEvents::aiEvents(aiPerson* A,std::vector<aiPerson*> aiLists, ai_time time)
{
	aiList = aiLists;
	eventTime = time;
	personA = A;
	checked = false;
}

void aiEvents::generateEvent()
{
	if (personA->dailyEvents[eventTime] == NULL)
	{
		eventType = ai_schedule1[eventTime];

		if (eventType == EATING)
		{			
			Eating* eating = &Eating(personA,aiList, eventTime);
			eating->generateEating();
			for (std::vector<aiPerson*>::iterator it = eating->aiInvolved.begin(); it != eating->aiInvolved.end(); it++)
			{
				eating->personA = (*it);
				(*it)->dailyEvents[eventTime] = eating;
			}
		}
		else if (eventType == WORKING)
		{
			Working* working = &Working(personA,aiList, eventTime);
			working->generateWorking();
			for (std::vector<aiPerson*>::iterator it = working->aiInvolved.begin(); it != working->aiInvolved.end(); it++)
			{
				working->personA = (*it);
				(*it)->dailyEvents[eventTime] = working;
			}
		}
		else if (eventType == RESTING)
		{
			Resting* resting = &Resting(personA,aiList, eventTime);
			resting->generateResting();
			for (std::vector<aiPerson*>::iterator it = resting->aiInvolved.begin(); it != resting->aiInvolved.end(); it++)
			{
				resting->personA = (*it);
				(*it)->dailyEvents[eventTime] = resting;
			}
		}
		else if (eventType == FIGHTING)
		{	
			Fighting* fighting = &Fighting(personA,aiList, eventTime);
			fighting->generateFighting();
			for (std::vector<aiPerson*>::iterator it = fighting->aiInvolved.begin(); it != fighting->aiInvolved.end(); it++)
			{
				fighting->personA = (*it);
				(*it)->dailyEvents[eventTime] = fighting;
			}
		}
		else if (eventType == OTHER)
		{
			Other* other = &Other(personA,aiList, eventTime);
			other->generateOther();
			for (std::vector<aiPerson*>::iterator it = other->aiInvolved.begin(); it != other->aiInvolved.end(); it++)
			{
				other->personA = (*it);
				(*it)->dailyEvents[eventTime] = other;
			}
		}
	}
}

void aiEvents::eventCausedByBuff(int stages)
{
	aiDebuff* debuff = (personA)->debuff;
	int max = (personA)->debuff->ILL;
	int buffLabel = 0;

	//check which debuff dominates the next person
	if (debuff->SLEEPY > max)
	{
		max = debuff->SLEEPY;
		buffLabel = 1;
	}
	if (debuff->HUNGRY > max)
	{
		max = debuff->HUNGRY;
		buffLabel = 2;
	}
	if (debuff->TIRED > max)
	{
		max = debuff->TIRED;
		buffLabel = 3;
	}
	if (debuff->OVERWORKED > max)
	{
		max = debuff->OVERWORKED;
		buffLabel = 4;
	}
	if (debuff->BEHINDSCHEDULE > max)
	{
		max = debuff->BEHINDSCHEDULE;
		buffLabel = 5;
	}

	if (max > 70)
	{
		if (buffLabel = 0) //ILL
		{
			if (rand() % 5 == 0)
			{
				(personA)->dailyEvents[stages]->eventType = RESTING;
				updateEvent(stages, personA);
			}
		}
		else if (buffLabel = 1)  //SLEEPY
		{
			if (rand() % 5 == 0)
			{
				(personA)->dailyEvents[stages]->eventType = EATING;
				updateEvent(stages, personA);
			}
		}
		else if (buffLabel = 2)  //HUNGRY
		{
			if (rand() % 5 == 0)        //20% possibility to change its objective
			{
				(personA)->dailyEvents[stages]->eventType = RESTING;
				updateEvent(stages, personA);
			}
		}
		else if (buffLabel = 3) //TIRED
		{
			if (rand() % 5 == 0)
			{
				(personA)->dailyEvents[stages]->eventType = RESTING;
				updateEvent(stages, personA);
			}
			else if (rand() % 5 == 1)  
			{ 
				(personA)->dailyEvents[stages]->eventType = EATING;
				updateEvent(stages, personA);
			}
		}
		else if (buffLabel = 4) //OVERWORKED
		{
			if (rand() % 5 == 0)
			{
				(personA)->dailyEvents[stages]->eventType = RESTING;
				updateEvent(stages, personA);
			}
			else if (rand() % 5 == 1)  
			{ 
				(personA)->dailyEvents[stages]->eventType = EATING;
				updateEvent(stages, personA);
			}
		}
		else if (buffLabel = 5)  //BEHINDSCHEDULE
		{
			if (rand() % 5 == 0)
			{
				(personA)->dailyEvents[stages]->eventType = WORKING;
				updateEvent(stages, personA);
			}
		}
	}
}

void aiEvents::updateEvent(int stages, aiPerson* person)
{
	ai_event eventType = person->dailyEvents[stages]->eventType;

	if (eventType != ai_schedule1[stages])   //reduece the aiInvolved by 1
	{
		for (std::vector<aiPerson*>::iterator it = person->dailyEvents[stages]->aiInvolved.begin(); it != person->dailyEvents[stages]->aiInvolved.end(); it++)
		{
			if ((*it) == person)
			{
				person->dailyEvents[stages]->aiInvolved.erase(it);
				person->dailyEvents[stages]->possibility *= 0.75;
				break;
			}
		}

		if (eventType ==  WORKING)   //generate new event for the ai
		{
			Working(person,aiList, (ai_time)stages).generateWorkingInterruptive();
		}
		else if (eventType == EATING)
		{
			 Eating(person,aiList, (ai_time)stages).generateEatingInterruptive();
		}
		else if (eventType == RESTING)
		{
			Resting(person,aiList, (ai_time)stages).generateRestingInterruptive();
		}
		else if (eventType == FIGHTING)
		{
			Fighting(person,aiList, (ai_time)stages).generateFightingInterruptive();
		}
		else if (eventType == OTHER)
		{
			 Other(person,aiList, (ai_time)stages).generateOtherInterruptive();
		}
	}
}



//////////////children classes////////////////////////

Working::Working(aiPerson* A,std::vector<aiPerson*> aiLists, ai_time time):aiEvents(A,aiLists,time)
{
}

void Working::generateWorking()
{
	workingType = (ai_working)(rand()%5);
	for (std::vector<Relation*>::iterator it = personA->relationList.begin(); it != personA->relationList.end(); it++)
	{
		if ((*it)->relations == COLLEGUE)
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
	}
	if (workingType == MEETING)
	{
		aiInvolved = collegue;
		aiInvolved.push_back(personA);
	}
	if (workingType == MANUFACTURING)
	{
		int randNum = rand() % collegue.size();
		aiInvolved.push_back(collegue[randNum]);
		aiInvolved.push_back(personA);
	}
	if (workingType == GROUPWORKING)
	{
		aiInvolved = collegue;
		aiInvolved.push_back(personA);
	}	
	if (workingType == EXPERIMENTING)
	{
		int randNum = rand() % collegue.size();
		aiInvolved.push_back(collegue[randNum]);
		aiInvolved.push_back(personA);
	}
	if (workingType == CODING)
	{
		aiInvolved.push_back(personA);
	}
}

void Working::generateWorkingInterruptive()
{
	for (std::vector<Relation*>::iterator it = personA->relationList.begin(); it != personA->relationList.end(); it++)
	{
		if ((*it)->relations == COLLEGUE)
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
	}

	int count = 0;
	for (std::vector<aiPerson*>::iterator it = personA->dailyEvents[eventTime]->aiInvolved.begin(); it != personA->dailyEvents[eventTime]->aiInvolved.end(); it++)
	{
		if ((*it)->dailyEvents[eventTime]->eventType == WORKING)
		{
			Working* working = static_cast<Working*>((*it)->dailyEvents[eventTime]);
			if (working->workingType != CODING)
			{
				(*it)->dailyEvents[eventTime]->aiInvolved.push_back(personA);             
				personA->dailyEvents[eventTime] =(*it)->dailyEvents[eventTime];
				break;
			}
			count++;
		}
	}
	if (count == personA->dailyEvents[eventTime]->aiInvolved.size())
	{
		Working* working = new Working(personA,aiList,eventTime);
		working->workingType = CODING;
		personA->dailyEvents[eventTime] = working;
		personA->dailyEvents[eventTime]->aiInvolved.push_back(personA);
	}
}

Eating::Eating(aiPerson* A,std::vector<aiPerson*> aiLists, ai_time time):aiEvents(A,aiLists,time)
{
}

void Eating::generateEating()
{
	switch (eventTime)
	{
	case(EIGHTAM): eatingType = BREAKFAST;
		break;
	case(TWELVEAM): eatingType = LUNCH;
		break;
	case(SIXPM):eatingType = SUPPER;
		break;
	default:eatingType = SNACK;
		break;
	}

	for (std::vector<Relation*>::iterator it = personA->relationList.begin(); it != personA->relationList.end(); it++)
	{
		if ((*it)->relations == FAMILY && (eatingType == BREAKFAST || eatingType == SUPPER))
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
		else if ((*it)->relations == COLLEGUE && eatingType == LUNCH)
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
		else if (((*it)->relations == COLLEGUE || (*it)->relations == FRIENDS) && eatingType == SNACK)
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
	}

	if (eatingType == BREAKFAST)
	{
		aiInvolved = collegue;
		aiInvolved.push_back(personA);
	}
	else if(eatingType == LUNCH)
	{
		int randNum = rand()%collegue.size();
		aiInvolved.push_back(collegue[randNum]);
		aiInvolved.push_back(personA);
	}
	else if(eatingType == SUPPER)
	{
		aiInvolved = collegue;
		aiInvolved.push_back(personA);
	}
	else if	(eatingType == SNACK)
	{
		int randNum = rand()%collegue.size();
		aiInvolved.push_back(collegue[randNum]);
		aiInvolved.push_back(personA);
	}
}

void Eating::generateEatingInterruptive()
{
	for (std::vector<Relation*>::iterator it = personA->relationList.begin(); it != personA->relationList.end(); it++)
	{
		if ((*it)->relations == FAMILY || (*it)->relations == FRIENDS || (*it)->relations == COLLEGUE)
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
	}

	int count = 0;
	for (std::vector<aiPerson*>::iterator it = personA->dailyEvents[eventTime]->aiInvolved.begin(); it != personA->dailyEvents[eventTime]->aiInvolved.end(); it++)
	{
		if ((*it)->dailyEvents[eventTime]->eventType == EATING)
		{
			Eating* eating = static_cast<Eating*>((*it)->dailyEvents[eventTime]);
			
			(*it)->dailyEvents[eventTime]->aiInvolved.push_back(personA);             
			personA->dailyEvents[eventTime] =(*it)->dailyEvents[eventTime];
			break;
		}
		count++;
	}
	if ((count == personA->dailyEvents[eventTime]->aiInvolved.size()))
	{
		Eating* eating = new Eating(personA,aiList,eventTime);
		personA->dailyEvents[eventTime] = eating;
		personA->dailyEvents[eventTime]->aiInvolved.push_back(personA);

		switch (eventTime)
		{
		case(EIGHTAM): eating->eatingType = BREAKFAST;
			break;
		case(TWELVEAM): eating->eatingType = LUNCH;
			break;
		case(SIXPM):eating->eatingType = SUPPER;
			break;
		default: eating->eatingType = SNACK;
			break;
		}
	}
}

Fighting::Fighting(aiPerson* A,std::vector<aiPerson*> aiLists, ai_time time):aiEvents(A,aiLists,time)
{
}

void Fighting::generateFighting()
{
	switch (eventTime)
	{
	case(EIGHTAM): fightingType = ARGUE;
		break;
	case(SIXPM):fightingType = TEAMFIGHT;
		break;
	case(EIGHTPM):fightingType = ARGUE;
		break;
	case(TENPM):fightingType = ARGUE;
		break;
	default:fightingType = ARGUE;
		break;
	}

	for (std::vector<Relation*>::iterator it = personA->relationList.begin(); it != personA->relationList.end(); it++)
	{
		if ((*it)->relations == FAMILY && (fightingType == ARGUE))
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
		else if ((*it)->relations == COLLEGUE && fightingType == DEBATE)
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
		else if (((*it)->relations == FRIENDS || (*it)->relations == OPPONENT) && fightingType == TEAMFIGHT)
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
		else if ((*it)->relations == OPPONENT && fightingType == DUEL)
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
	}

	if (fightingType == TEAMFIGHT)
	{
		aiInvolved = collegue;
		aiInvolved.push_back(personA);
	}
	else if (fightingType == DEBATE)
	{
		int randNum = rand()%collegue.size();
		aiInvolved.push_back(collegue[randNum]);
		aiInvolved.push_back(personA);
	}
	else if (fightingType == DUEL)
	{
		aiInvolved = collegue;
		aiInvolved.push_back(personA);
	}
	else if (fightingType == ARGUE)
	{
		int randNum = rand()%collegue.size();
		aiInvolved.push_back(collegue[randNum]);
		aiInvolved.push_back(personA);
	}
}

void Fighting::generateFightingInterruptive()
{
	int randNum = rand() % personA->dailyEvents[eventTime]->aiInvolved.size();
	while (personA->dailyEvents[eventTime]->aiInvolved[randNum] == personA)
	{
		randNum = rand() % personA->dailyEvents[eventTime]->aiInvolved.size();
	}

	if (personA->dailyEvents[eventTime]->aiInvolved.size() > 1)
	{
		for (std::vector<aiPerson*>::iterator it = personA->dailyEvents[eventTime]->aiInvolved.begin(); it != personA->dailyEvents[eventTime]->aiInvolved.end(); it++)
		{
			if ((*it)==personA)
			{
				personA->dailyEvents[eventTime]->aiInvolved.erase(it);
				it = personA->dailyEvents[eventTime]->aiInvolved.begin();
			}
			if ((*it)==personA->dailyEvents[eventTime]->aiInvolved[randNum])
			{
				personA->dailyEvents[eventTime]->aiInvolved.erase(it);
				it = personA->dailyEvents[eventTime]->aiInvolved.begin();
			}
		}

		Fighting* fighting = new Fighting(personA, aiList, eventTime);
		fighting->fightingType = ARGUE;
		fighting->aiInvolved.push_back(personA);
		fighting->aiInvolved.push_back(personA->dailyEvents[eventTime]->aiInvolved[randNum]);
		personA->dailyEvents[eventTime] = fighting;
		personA->dailyEvents[eventTime]->aiInvolved[randNum]->dailyEvents[eventTime] = fighting;
	}
	else
	{
		Fighting* fighting = new Fighting(personA, aiList, eventTime);
		fighting->fightingType = ARGUE;
		fighting->aiInvolved.push_back(personA);
		personA->dailyEvents[eventTime] = fighting;
	}

}

Resting::Resting(aiPerson* A,std::vector<aiPerson*> aiLists, ai_time time):aiEvents(A,aiLists,time)
{
}

void Resting::generateResting()
{
	switch (eventTime)
	{
	case(EIGHTAM): restingType = OVERSLEEP;
		break;
	case(SIXPM):restingType = DRINKING;
		break;
	case(EIGHTPM):restingType = GAME;
		break;
	case(TENPM):restingType = SLEEP;
		break;
	default:restingType = NAP;
		break;
	}

	for (std::vector<Relation*>::iterator it = personA->relationList.begin(); it != personA->relationList.end(); it++)
	{
		if (((*it)->relations == FRIENDS || (*it)->relations == FAMILY) && (restingType == DRINKING || restingType == GAME))
		{
			if ((*it)->A == personA)
				collegue.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue.push_back((*it)->A);
		}
	}

	if (restingType == NAP || restingType == OVERSLEEP || restingType == SLEEP)
		collegue.push_back(personA);
	
	if (restingType == NAP)
		aiInvolved = collegue;
	else if (restingType == SLEEP)
		aiInvolved = collegue;
	else if (restingType == OVERSLEEP)
		aiInvolved = collegue;
	else if (restingType == GAME)
	{
		int randNum = rand()%collegue.size();
		aiInvolved.push_back(collegue[randNum]);
		aiInvolved.push_back(personA);
	}
	else if (restingType == DRINKING)
	{
		int randNum = rand()%collegue.size();
		aiInvolved.push_back(collegue[randNum]);
		aiInvolved.push_back(personA);
	}
}

void Resting::generateRestingInterruptive()
{
	std::vector<aiPerson*> collegue1;
	std::vector<aiPerson*> collegue2;
	std::vector<aiPerson*> collegue3;
	for (std::vector<Relation*>::iterator it = personA->relationList.begin(); it != personA->relationList.end(); it++)
	{
		if ((*it)->relations == FRIENDS)
		{
			if ((*it)->A == personA)
				collegue1.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue1.push_back((*it)->A);
		}
		if ((*it)->relations == COLLEGUE)
		{
			if ((*it)->A == personA)
				collegue2.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue2.push_back((*it)->A);
		}
		if ((*it)->relations == FAMILY)
		{
			if ((*it)->A == personA)
				collegue3.push_back((*it)->B);
			else if ((*it)->B == personA)
				collegue3.push_back((*it)->A);
		}
	}
	if (eventTime >= TENAM && eventTime <= SIXPM)
	{
		int count = 0;
		for (std::vector<aiPerson*>::iterator it = collegue2.begin(); it != collegue2.end(); it++)
		{
			if ((*it)->dailyEvents[eventTime]->eventType == RESTING)
			{
				Resting* resting = static_cast<Resting*>((*it)->dailyEvents[eventTime]);
				if (resting->restingType != NAP && resting->restingType != SLEEP && resting->restingType != OVERSLEEP)
				{
					(*it)->dailyEvents[eventTime]->aiInvolved.push_back(personA);             
					personA->dailyEvents[eventTime] =(*it)->dailyEvents[eventTime];
					break;
				}
			}
		}
		count++;
		if (count == personA->dailyEvents[eventTime]->aiInvolved.size())
		{
			Resting* resting = new Resting(personA, aiList, eventTime);
			resting->restingType = NAP;
			personA->dailyEvents[eventTime]->aiInvolved.push_back(personA);
		}
	}
	else
	{
		for (std::vector<aiPerson*>::iterator it = collegue3.begin(); it != collegue3.end(); it++)
		{
			if ((*it)->dailyEvents[eventTime]->eventType == RESTING)
			{
				(*it)->dailyEvents[eventTime]->aiInvolved.push_back(personA);             
				personA->dailyEvents[eventTime] =(*it)->dailyEvents[eventTime];
				break;
			}
		}
	}
}

Other::Other(aiPerson* A,std::vector<aiPerson*> aiLists, ai_time time):aiEvents(A,aiLists,time)
{
}

void Other::generateOther()
{
}

void Other::generateOtherInterruptive()
{
}
////////////////aiEvents effect generator//////////////////////

void aiEvents::generateEffect(int stages)
{
	if (personA->dailyEvents[stages]->checked == false)
	{
		double possibilities = 0;

		possibilities += generatePersonalityEffect(stages); 
		possibilities += generateFriendshipEffect(stages);    
		possibilities += generateLuckEffect(stages);
		possibilities += generateMoodEffect(stages);
		possibilities += generateBuffEffect(stages);

		personA->dailyEvents[stages]->possibility = possibilities/5;

		if (personA->dailyEvents[stages]->possibility < 60)
		{
			personA->dailyEvents[stages]->completed = false;
			personA->dailyEvents[stages]->checked = true;
		}
		else
		{
			personA->dailyEvents[stages]->completed = true;
			personA->dailyEvents[stages]->checked = true;
		}
	}
}

double aiEvents::generatePersonalityEffect(int stages)
{
	double possibilities = 0;

	for (std::vector<aiPerson*>::iterator it = aiInvolved.begin(); it != aiInvolved.end(); it++)
	{
		int personality = (*it)->aiPersonality;
		//ai_event eventType = personA->dailyEvents[stages]->eventType;
		double possibilit1 = 100;

		////////////////////////////////////add mood intot he personality effect generator/////////

		if (personality & HARDWORKING  ||  personality & OPTIMISTIC)
		{
			if (eventType == WORKING)
				possibilit1 *= 1.25;
		}
		if (personality & PESSIMISTIC)
		{
			if (eventType == WORKING)
				possibilit1 *= 0.75;
		}
		if (personality & LAZY)
		{
			if (eventType == WORKING)
				possibilit1  *= 0.75;
			else if (eventType == RESTING)
				possibilit1 *= 1.28;
		}
		if (personality & KIND || personality & TENDER)
		{
			if (eventType == FIGHTING)
				possibilit1  *= 0.75;
			else
				possibilit1 *= 1.125;
		}
		if (personality & RASH || personality & HOSTILE)
		{
			if (eventType == FIGHTING)
				possibilit1 *= 1.25;
			else if (eventType == WORKING || eventType == RESTING)
				possibilit1 *= 0.875;
		}
		else
			possibilit1 *= 0.90;
		possibilities += possibilit1;
	}
	return possibilities/aiInvolved.size();
}

double aiEvents::generateFriendshipEffect(int stages)
{
	int count = 0;
	double possibilities = 0;
	for (std::vector<aiPerson*>::iterator it = aiInvolved.begin(); it != aiInvolved.end(); it++)
	{
		for (std::vector<aiPerson*>::iterator iter = (it+1); iter != aiInvolved.end(); iter++)
		{
			ai_friendship friendship;
			
			for (std::vector<Relation*>::iterator itRelation = (*it)->relationList.begin(); itRelation != (*it)->relationList.end(); itRelation++)
			{
				if (((*itRelation)->A == (*it) && (*itRelation)->B == (*iter)) || ((*itRelation)->B == (*it) && (*itRelation)->A == (*iter)))
					friendship = (*itRelation)->friendship;
			}

			double possibility = 100;

			if(friendship > JUSTSOSO)  //relation is better than JUSTSOSO
			{
				if (eventType != FIGHTING)
					possibility *= (1+0.05*(friendship-3));
				else
					possibility *= (1-0.05*(friendship-3));
			}
			else if (friendship == JUSTSOSO)  //relation is stranger
			{
				if (rand()%2 == 0)
				{
					friendship = NOTBAD;  //the relation is enhanced
					//npcRelation->heart = 20 * friendship;   
				}
				else
				{
					(*it)->aiMood = UNHAPPY;   //mood changed
					possibility *= (1+0.05*(friendship-3));
					if (rand()%2 == 0)
					{
						friendship = (ai_friendship)(friendship-1); 
						//npcRelation->heart = 20 * friendship; 
					}
				}
			}
			else   //relation is worse than JUSTSOSO
			{
				if (eventType != FIGHTING)
				{
					if (rand()%5 == 0)     //there is a 20% possibility that the coefficent wont decrease rapidly
					{
						possibility *= (1+0.05*(friendship-3));
						friendship = (ai_friendship)(friendship+1);
						//npcRelation->heart = 20 * npcRelation->relations; 
					}
					else
					{
						(*it)->aiMood = ANGRY; 
						possibility *= (1+0.075*(friendship-3));

						if (rand()%3 == 0 && (friendship != OPPONENT))
						{
							friendship = (ai_friendship)(friendship-1);
							//npcRelation->heart = 20 * npcRelation->relations; 
						}
					}
				}
				else  //if the event is fighting
				{
					(*it)->aiMood = OUTRAGEOUS;   //mood changed
					possibility *= (1 - 0.05*(friendship-3));

					if (rand()%3 == 0 && (friendship != OPPONENT))
					{
						friendship = (ai_friendship)(friendship-1);
						//npcRelation->heart = 20 * npcRelation->relations; 
					}
				}
			}
			count++;
			possibilities += possibility;
		}
	}
	return possibilities/count;
}

double aiEvents::generateMoodEffect(int stages)
{
	double possibilities = 0;
	for (std::vector<aiPerson*>::iterator it = aiInvolved.begin(); it != aiInvolved.end(); it++)
	{
		ai_mood &npcMood = (*it)->aiMood;
		double possibility = 100;

		if (npcMood < COMPOSED && npcMood > DESPARATE)   //0.05
			possibility *= (1 + 0.05*(npcMood-8));
		if (npcMood >= COMPOSED && npcMood < CONFIDENT)   //0.025
			possibility *= (1 + 0.025*(npcMood-8));
		if (npcMood == OUTRAGEOUS|| npcMood == ANGRY || npcMood == DESPARATE)   //0.05
		{
			if (eventType == FIGHTING)
				possibility *= (1 - 0.05*(npcMood-8));
			else
				possibility *= (1 + 0.05*(npcMood-8));
		}
		if (npcMood == CRAZY || npcMood == CONFIDENT)
		{
			if (eventType == WORKING)
				possibility *= (1 - 0.075*(npcMood-8));
			else
				possibility *= (1 + 0.075*(npcMood-8));
		}
		possibilities += possibility;
	}
	return possibilities/aiInvolved.size();
}

double aiEvents::generateLuckEffect(int stages)
{
	return 100;
}

double aiEvents::generateBuffEffect(int stages)
{
	double possibilities = 0;
	for (std::vector<aiPerson*>::iterator it = aiInvolved.begin(); it != aiInvolved.end(); it++)
	{
		aiDebuff* debuff = (*it)->debuff;
		int possibility = 100;

		//buff effect//
		if (debuff->ILL > 50)
		{
			possibility *= 0.5;
		}
		if (debuff->SLEEPY > 50)
		{
			possibility *= 0.75;
		}
		if (debuff->HUNGRY > 50)
		{
			if (eventType != EATING)
				possibility *= 0.75;
			else
				possibility *= 1.5;
		}
		if (debuff->TIRED > 50)
		{
			if (eventType != RESTING)
				possibility *= 0.75;
			else
				possibility *= 1.5;
		}
		if (debuff->OVERWORKED > 50)
		{
			if (eventType == WORKING)
				possibility *= 0.5;
			else if (eventType == RESTING)
				possibility *= 1.25;
			else
				possibility *= 0.875;
		}
		if (debuff->BEHINDSCHEDULE > 50)
		{
			if (eventType == WORKING)
				possibility *= 1.5;
		}
		possibilities+=possibility;
	}
	return possibilities/aiInvolved.size();
}

/////////////////////aiPerson////////////////////////////////

aiPerson::aiPerson()
{
	this->name = ai_name(rand()%8);
	occupation = (ai_occupation)(rand()%12+1);

	int i = 0;
	while (i < 3)
	{
		if (rand()%3 != 0)
			aiPersonality = aiPersonality|(ai_personality)(rand()%4 + 2*i);
		i++;
	}

	aiMood = COMPOSED;
	full =  new int[8];
	
	dailyEvents = std::vector<aiEvents*>(8);

	debuff->BEHINDSCHEDULE = 40;
	debuff->HUNGRY = 40;
	debuff->ILL = 40;
	debuff->OVERWORKED = 40;
	debuff->SLEEPY = 40;
	debuff->TIRED = 40;
}

void aiPerson::updateBuff(int stages)
{
	//generate buff according to the previous event result
	if (stages != 0)
	{
		if (dailyEvents[stages-1]->completed)
		{
			if (debuff->BEHINDSCHEDULE > 50)
			{
				debuff->BEHINDSCHEDULE -= 10;
				debuff->TIRED += 10;           //finish an event on time when PROCASTINATING casued TIRED
			}
			if (stages != 1)
			{
				if (dailyEvents[stages-2]->eventType == WORKING && dailyEvents[stages-1]->eventType == WORKING)
					debuff->OVERWORKED += 10;     // have working 2 in a row cause OVERWORKED
			}
			if (dailyEvents[stages-1]->eventType == RESTING)
				debuff->TIRED -= 10;             // clear TIRED if RESTING is completed
			if (dailyEvents[stages-1]->eventType == FIGHTING)
				debuff->ILL += 10;          //hurt by FIGHTING
			if (dailyEvents[stages-1]->eventType == EATING)
				debuff->HUNGRY -= 10;
		}
		else
		{
			if (dailyEvents[stages-1]->eventType == WORKING)
				debuff->BEHINDSCHEDULE += 10;
			if (dailyEvents[stages-1]->eventType == RESTING)
				debuff->TIRED += 10;             
			if (dailyEvents[stages-1]->eventType == FIGHTING)
				debuff->ILL += 10;          
			if (dailyEvents[stages-1]->eventType == EATING)
				debuff->HUNGRY += 10;
		}
	}
}


std::vector<Relation*> aiPerson::generateRelation(std::vector<aiPerson*> npc)
{
	int i = 0;
	while (i < aiSize)
	{
		if(this->idNum < i)
		{
			Relation* relation = (aiRelation(this,npc[i]).generateRelation());
			this->relationList.push_back(relation);
			npc[i]->relationList.push_back(relation);
		}
		i++;
	}
	return relationList;
}

Schedule* aiPerson::generateSchedule(std::vector<aiPerson*> npc)
{
	for(int i = 0; i <= TENPM; i++)
	{
		if (dailyEvents[i] == NULL)
		{
			aiEvents(this,npc,(ai_time)i).generateEvent();
		}
	}

	aiSchedule->eventList = dailyEvents;
	aiSchedule->id = this->idNum;
	aiSchedule->person = this;
	return aiSchedule;
}

////////////////////aiEngine//////////////////////////////////
aiEngine::aiEngine()
{
}

bool aiEngine::initialize()
{
	ai_schedule1.push_back(EATING);
	ai_schedule1.push_back(WORKING);
	ai_schedule1.push_back(EATING);
	ai_schedule1.push_back(WORKING);
	ai_schedule1.push_back(WORKING);
	ai_schedule1.push_back(EATING);
	ai_schedule1.push_back(RESTING);
	ai_schedule1.push_back(RESTING);


	int i = 0;
	//aiList = std::vector<aiPerson*>(aiSize);
	while (i < aiSize)
	{
		aiPerson npcA = aiPerson();
		npcA.idNum = i;
		aiList.push_back(&npcA);
	}

	for (std::vector<aiPerson*>::iterator it = aiList.begin(); it != aiList.end(); it++)
		relationList.push_back((*it)->generateRelation(aiList));

	for (std::vector<aiPerson*>::iterator it = aiList.begin(); it != aiList.end(); it++)
		scheduleListPre.push_back((*it)->generateSchedule(aiList));

	return true;
}

bool aiEngine::simulate()
{

	while (true)
	{

		for (std::vector<aiPerson*>::iterator it = aiList.begin(); it != aiList.end(); it++)
		{
			for (int i = 0; i < 8; i++)
			{
				aiEvents((*it), aiList, (ai_time)i).generateEffect(i);
				(*it)->updateBuff(i);
				aiEvents((*it), aiList, (ai_time)i).eventCausedByBuff(i);
			}
		}
	}
	return true;
}

bool aiEngine::end()
{
	return true;
}
