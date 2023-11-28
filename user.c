#include "input.h"

/***************************************************************************************************************************
Lookup tables for gates
****************************************************************************************************************************/
int andGate[5][5] = {
  {0, 0, 0, 0, 0},
  {0, 1, 2, 3, 4},
  {0, 2, 2, 2, 2},
  {0, 3, 2, 3, 0},
  {0, 4, 2, 0, 4}
};

int orGate[5][5] = {
  {0, 1, 2, 3, 4},
  {1, 1, 1, 1, 1},
  {2, 1, 2, 2, 2},
  {3, 1, 2, 3, 1},
  {4, 1, 2, 1, 4}
};

int notGate[5] = {1, 0 , 2, 4, 3};
int lineNumber = 0;
LIST* dFrontier = NULL;
int state = NEUTRAL;
/****************************************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************
 Function to read the test patterns(.pattern files)
***************************************************************************************************/
int ReadPattern(FILE * pattern, int patternList[Mpt][Mpi], int nPi){

	char line[Mlin];
	char input[5] = "";

	while (fgets(line, sizeof(line), pattern) && lineNumber < Mpt) {
		int index = 0;

		while(index != nPi){
			sprintf(input, "%.*s\n", 1 , &line[index]);
			//printf("test%d ", atoi(input));
			patternList[lineNumber][index] = atoi(input);
			index ++;
		}
		printf("\n");
		lineNumber++;
	}
}


void printPatternList(int patternList[Mpt][Mpi], int nPi){
	int patternNum;
	for(patternNum = 0; patternNum < lineNumber; patternNum++ ){
		int inputNum;
		for(inputNum = 0; inputNum < nPi; inputNum++ ){
			printf("%d", patternList[patternNum][inputNum]);
		}
		printf("\n");
	}
}

//end of ReadPattern 
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to simulate logic
***************************************************************************************************/
 void simulateLogic(GATE * Node, int patternList[Mpt][Mpi], int tGat, FILE * res){
	int testNum;
	//iterating through test cases
	for(testNum = 0; testNum < lineNumber; testNum++){
		int itr ;
		int curPatternIndex = 0;
		for(itr = 0; itr <= tGat; itr++){ 
			if(Node[itr].Type != 0){  //not an invalid gate
				switch(Node[itr].Type){
					case 1: // type input
						Node[itr].Val = patternList[testNum][curPatternIndex];
						curPatternIndex ++;
						break;
					case 2: // type  from
						Node[itr].Val = Node[Node[itr].Fin->Id].Val; 
						break;
					case 3: // type  buff
						Node[itr].Val = Node[Node[itr].Fin->Id].Val;
						break;
					case 4: // type not
						Node[itr].Val = notGate[Node[Node[itr].Fin->Id].Val];
						break;
					case 5: //type and
						Node[itr].Val = andOperation(Node, (Node[itr].Fin));
						break;
					case 6: //type Nand
						Node[itr].Val = notGate[andOperation(Node, (Node[itr].Fin))];
						break;
					case 7: // type or
						Node[itr].Val = orOperation(Node, (Node[itr].Fin));
						break;
					case 8: // type Nor
						Node[itr].Val = notGate[orOperation(Node, (Node[itr].Fin))];
						break;
					case 9: // type xor
						Node[itr].Val = xorOperation(Node, (Node[itr].Fin));
						break;
					case 10: // type xNor
						Node[itr].Val = notGate[xorOperation(Node, (Node[itr].Fin))];
						break;
				}
				if(Node[itr].Nfo == 0){
					fprintf(res, "%d" , Node[itr].Val);
				}
			}

		}
		fprintf(res, "\n");
	}
 }

//end of simulateLogic
/****************************************************************************************************************************/


/***************************************************************************************************
 Function to andOperation
***************************************************************************************************/
int andOperation(GATE * Node, LIST *Cur){
	LIST *tmp=Cur;
	int output=1;

	while(tmp!=NULL){   
		output = andGate[output][Node[tmp->Id].Val]; // check output by two by two
		if(output == 0){ // if one pair give out as 0 then stop
			break;
		}
		tmp = tmp->Next; 
	} 
	return output;
}
//end of andOperation
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to orOperation
***************************************************************************************************/
int orOperation(GATE * Node, LIST *Cur){
	LIST *tmp=Cur;
	int output=0;


	while(tmp!=NULL){   
		output = orGate[output][Node[tmp->Id].Val]; // check output by two by two
		if(output == 1){ // if one pair give out as 1 then stop
			break;
		}
		tmp = tmp->Next; 
	} 
	return output;
}
//end of orOperation
/****************************************************************************************************************************/


/***************************************************************************************************
 Function to xorOperation
***************************************************************************************************/
int xorOperation(GATE * Node, LIST *Cur){
	LIST *tmp=Cur;
	int output=1;
 
	if (Node[tmp->Next->Id].Val == Node[tmp->Id].Val){ // check input value is equal or not
		output = 0;
	}
		
	return output;
}
//end of xorOperation
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to simulate logic Impl
***************************************************************************************************/
 int logicSimulateImpl(GATE * Node, GV *gv){
	FreeList(dFrontier); //release the dfrontier list
	int state = NEUTRAL;
	int itr ;
	for(itr = 0; itr <= tGat; itr++){ 
		if(Node[itr].Type != 0){  //not an invalid gate
			switch(Node[itr].Type){
				case 1: // type input
					break;
				case 2: // type  from
					Node[itr].Val = Node[Node[itr].Fin->Id].Val; 
					break;
				case 3: // type  buff
					Node[itr].Val = Node[Node[itr].Fin->Id].Val;
					break;
				case 4: // type not
					Node[itr].Val = notGate[Node[Node[itr].Fin->Id].Val];
					break;
				case 5: //type and
					Node[itr].Val = andOperation(Node, (Node[itr].Fin));
					break;
				case 6: //type Nand
					Node[itr].Val = notGate[andOperation(Node, (Node[itr].Fin))];
					break;
				case 7: // type or
					Node[itr].Val = orOperation(Node, (Node[itr].Fin));
					break;
				case 8: // type Nor
					Node[itr].Val = notGate[orOperation(Node, (Node[itr].Fin))];
					break;
				case 9: // type xor
					Node[itr].Val = xorOperation(Node, (Node[itr].Fin));
					break;
				case 10: // type xNor
					Node[itr].Val = notGate[xorOperation(Node, (Node[itr].Fin))];
					break;
			}

			if(itr == gv->id){ // check whether ID is fault gate //fault activation
				if(Node[itr].Val == 1 && gv->Val = 0 ){
					Node[itr].Val = D;
				}else if(Node[itr].Val == 0 && gv->Val = 1 ){
					Node[itr].Val = DB;
				}else if(Node[itr].Val != X){ //fault is mask
					state = FAILURE;
				}
			}

			//creating dfrontier
			if(checkDFrontier(Node[itr].Fin, Node)){
				InsertEle(&(dFrontier), id);//Add to dfrontier
			}

			if(dFrontier == NULL && (Node[gv->g]->Val == D || Node[gv->g]->Val == DB )){ // check dfrontier is empty && fault is activated
				state = FAILURE;
			}

			if(Node[itr].Fot == 0){   //find Fault Effect of PO
				if(Node[gv->g]->Val != D || Node[gv->g]->Val != DB ){ 
					return SUCCESS;
				}
			}	

		}

	}	
	return state;
 }

//end of simulateLogicImpl
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to podem
***************************************************************************************************/
int podem(GATE * Node, GV* fault){
	setDontcares(Node);

	int result = podemRecursion(Node, fault);

	if(result== SUCCESS){
		//PRINT VALUES AT PI
	}else{
		//print fault f in untestable
	}

}
//end of podem
/****************************************************************************************************************************/
/***************************************************************************************************
 Function to podemRecursion
***************************************************************************************************/
int podemRecursion(GATE * Node, GV *gv){

	int result;	
	gv = getObjective(Node, gv);
	gv = backtrace(Node, gv);
	state = logicSimulateImpl(Node,gv);

	result = podemRecursion(Node, gv);

	if(result == SUCCESS){
		state = SUCCESS;
		return state;
	}else if(result == FAILURE){
		state = FAILURE;
		return state;
	}

	gv->v = !(gv->v) ;
	state = logicSimulateImpl(gv);
	
	result = podemRecursion(Node, gv);

	if(result == SUCCESS){
		state = SUCCESS;
		return state;
	}else if(result == FAILURE){
		state = FAILURE;
		return state;
	}

	//reset PI - BAD decision made ealier
	gv->v = X;
	state = logicSimulateImpl(gv);

	state = FAILURE;
	return state;
	
}
//end of podemRecursion
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to getObjective
***************************************************************************************************/
*GV getObjective(GATE * Node, GV *gv){ //fault GV

	if(Node[gv->g]->Val != D || Node[gv->g]->Val != DB ){ // if fault is not exicted
		gv->v = !(gv->v);
		return gv;
	}
	
	int d = dFrontier->Id;//gate in d frontier
	gv->g = findXFanIn(Node, d);
	gv->v = findNonControlVal(Node, d);//find non controlling value

	return gv;
	
}
//end of getObjective
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to backtrace
***************************************************************************************************/
*GV backtrace(GATE * Node, GV *gv){
	int g = gv->g;
	int v = gv->v;
	int numInvertions = 0;
	int i = g;

	while(Node[i].Nfi !=0){
		i = findXFanIn(Node, i);
		if(Node[i].Type == NOR || Node[i].Type == NOT || Node[i].Type == NAND){// inverting type gate
			numInvertions ++;
		}
	}
	if(numInvertions % 2 == 1){ // inverting gates count is odd
		v = !v;
	}

	gv->g= i;
	gv->v = v;
return gv;
}
//end of backtrace
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to findXFanIn
***************************************************************************************************/
int findXFanIn(GATE *Node, int i){
	LIST *tmp = Node[i]->Fin;
	i = 0;
	while(tmp!=NULL){   
		if([Node[tmp->Id].Val == X ){ // check value is x 
			i = tmp->Id;
			break;
		}
		tmp = tmp->Next; 
	} 
	return i;
}
//end of findXFanIn
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to setDontcares
***************************************************************************************************/
void setDontcares(GATE *Node){
	int i;
	for(i=1;i<=tGat;i++){ 
		Node[i].Val = X;			
	}
}
//end of findXFanIn
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to findNonControlVal
***************************************************************************************************/
int findNonControlVal(GATE *Node, int gId){
	if(Node[gId].Type==AND || Node[gId].Type==NAND) 	return 1;
	else  if(Node[gId].Type==OR || Node[gId].Type==NOR) return 0;
	else return -1;
}
//end of findNonControlVal
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to find Fault Effect of PO
***************************************************************************************************/
//int findFaultEffPO(GATE *Node, GV* gv){
	//int i;
	//for(i=1;i<=tGat;i++){ 
	// 	if(Node[id].Fot == 0){
	// 		if(Node[gv->g]->Val != D || Node[gv->g]->Val != DB ){ 
	// 			return SUCCESS;
	// 		}
	// 	}			
	// //}
// 	return FAILURE;
// }
//end of findFaultEffPO
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to check DFrontier
***************************************************************************************************/
int checkDFrontier(LIST *Cur, GATE *Node){
	LIST *temp=NULL;
	
	temp=Cur;
	while(temp!=NULL){
	if(Node[temp->Id].Val==D || Node[temp->Id].Val==DB) return 1;
	temp=temp->Next; }
	return 0;
}
//end of checkDFrontier
/****************************************************************************************************************************/