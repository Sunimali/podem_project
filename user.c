#include "input.h"

/***************************************************************************************************************************
Lookup tables for gates
****************************************************************************************************************************/

int andGate[5][5] = {
//o   1 xv  d  db	
  {0, 0, 0, 0, 0},
  {0, 1, 2, 3, 4},
  {0, 2, 2, 2, 2},
  {0, 3, 2, 3, 0},
  {0, 4, 2, 0, 4}
};

int orGate[5][5] = {
//o   1 xv  d  db	
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
int tGat = 0;
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
		//printf("INPUTTTTTTT%d\n", Node[tmp->Id].Val); 
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
 int logicSimulateImpl(GATE * Node, GV gv, GV * fault){
	//printf("start LSI\n");
	if(dFrontier != NULL){
		FreeList(&dFrontier); //release the dfrontier list
	}
	int state = NEUTRAL;
	int itr ;
	//printf("return log\n");
	Node[gv.g].Val = gv.v ; // assign PI
	//printf("return log\n");
	//printf("primary INUPUT id: %d value:%d\n",gv->g, Node[gv->g].Val);
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
					//printf("nand id%d value%d\n", itr, Node[itr].Val);
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

			if(itr == fault->g){ // check whether ID is fault gate //fault activation
				//printf("fault gate id%d\n", itr);
				if(Node[itr].Val == 0 && fault->v == 1 ){
					Node[itr].Val = D;
					//printf("fault gate D\n");
				}else if(Node[itr].Val == 1 && fault->v == 0 ){
					Node[itr].Val = DB;
					//printf("fault gate DB\n");
				}else{
					;//printf("fault is not activated id%d fault%d\n", Node[itr].Val,  fault->v);
				}// }else if(Node[itr].Val != XV){ //fault is mask
				// 	state = FAILURE;
				// 	printf("fault gate MASK\n");
				// }else{
				// 	printf("fault gate value %d\n", Node[itr].Val);
				// }
			}

			//creating dfrontier
			if(checkDFrontier(itr, Node)){
				//printf("ADD dfrontier %d\n",itr);
				InsertEle(&(dFrontier), itr);//Add to dfrontier
			}


			if(Node[itr].Nfo == 0){   //find Fault Effect of PO
				if(Node[itr].Val == D || Node[itr].Val == DB ){ 
					//printf("PO IS d or db %d\n",itr);
					state =  SUCCESS;
					return state;
				}
			}	
		}

	}
	
	if(dFrontier == NULL && (Node[fault->g].Val == D || Node[fault->g].Val == DB )){ // check dfrontier is empty && fault is activated
				//printf("No dfrontier && fault mask\n");
				state = FAILURE;
	}
	//printf("return log\n");	
	return state;
 }

//end of simulateLogicImpl
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to podem
***************************************************************************************************/
int podem(GATE * Node, GV* fault, int tGatNum){
	//printf("start \n");
	tGat = tGatNum;

	//printf("start PDM\n");
	setDontcares(Node);
	//printf("done with set dont cares\n");
	int start = clock();
	int result = podemRecursion(Node, fault, start);
	//faultCount++;

	//if(result== SUCCESS){
		//PRINT VALUES AT PI
		//successFCount++;
		//printPI(Node);
		//printf("success id: %d val:%d\n",fault->g, fault->v);
		//printf("value: %d %d %d %d %d\n",Node[1].Val, Node[2].Val, Node[3].Val, Node[6].Val, Node[7].Val);
	//}else if(result== FAILURE){
		//print fault f in untestable
		//failedFCount++;
		//printf("fault is untestable%d\n", result);
	//}else{ //timeout
		//timeOutFCount++;
	//}

	//checkFaultCoverage( faultCount, successFCount, failedFCount, timeOutFCount);

}
//end of podem
/****************************************************************************************************************************/
/***************************************************************************************************
 Function to podemRecursion
***************************************************************************************************/
int i = 0;
int podemRecursion(GATE * Node, GV *fault, clock_t clockStart){
	i++;
	//printf("start PDR  %d \n",i);
	int result;
	clock_t end = clock();
	double duration = (double)(end - clockStart)/CLOCKS_PER_SEC;
	if(duration>1){
		state = TIMEOUT;
		return state;
	}
	GV pi;//=  malloc(sizeof(GV));
	GV obj; //=  malloc(sizeof(GV));
	
	obj = getObjective(Node, fault, obj); 	
	//printf("objective gate id: %d  val: %d",obj.g, obj.v);
	
	pi = backtrace(Node, obj); //if(i==129313){getchar();}
	
	//printf("backrace gate id: %d  val: %d\n",pi.g, pi.v);
	//printf("sta fasul id: %d  val: %d\n",fault->g, fault->v);
	state = logicSimulateImpl(Node,pi,fault);
	
	if(state == SUCCESS){
		return state;
	}else if(state == FAILURE){
		return state;
	}
	//printf("return log%d\n",i);  //if(i==138157){getchar();}
	//printf("return log%d\n",i);
	result = podemRecursion(Node, fault, clockStart);
	//if(i==129317){
	//printf("return log%d\n",i);	
	//}
	
	if(result == SUCCESS){
		state = SUCCESS;
		
		return state;
	}// }else if(result == FAILURE){
	// 	state = FAILURE;
	// 	return state;
	// }
	

	pi.v = !(pi.v) ;
	//printf("logic simulation2");
	state = logicSimulateImpl(Node, pi, fault);
	

	if(state == SUCCESS){
		return state;
	}else if(state == FAILURE){
		return state;
	}
	//printf("beend%d\n",i);
	result = podemRecursion(Node, fault, clockStart);
	//printf("1end%d\n",i);

	if(result == SUCCESS){
		state = SUCCESS;
		return state;
	}
	// }else if(result == FAILURE){
	// 	state = FAILURE;
	// 	return state;
	// }
	//printf("2end%d\n",i);
	//reset PI - BAD decision made ealier
	pi.v = XV;
	state = logicSimulateImpl(Node, pi, fault);
	if(state == SUCCESS){
		return state;
	}else if(state == FAILURE){
		return state;
	}
	state = FAILURE;
	//free(obj);
	//printf("end%d\n",i);
	return state;
	
}
//end of podemRecursion
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to getObjective
***************************************************************************************************/
GV getObjective(GATE * Node, GV *fault, GV obj){ //fault GV

	if(Node[fault->g].Val != D && Node[fault->g].Val != DB ){ // if fault is not exicted
		//printf("fault is not excited fault id%d val%d,sec val%d\n",fault->g,fault->v, Node[fault->g].Val);
		obj.v = !(fault->v);
		obj.g = fault->g;
		return obj;
	}
	
	int d = dFrontier->Id;//gate in d frontier
	//printf("get dfrontier%d\n",d);
	obj.g = findXFanIn(Node, d);
	obj.v = findNonControlVal(Node, d);//find non controlling value

	return obj;
	
}
//end of getObjective
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to backtrace
***************************************************************************************************/
GV backtrace(GATE * Node, GV gv){
	int g = gv.g;
	int v = gv.v;
	int numInvertions = 0;
	int i = g;

	if(Node[i].Type == NOR || Node[i].Type == NOT || Node[i].Type == NAND){// inverting type gate
		numInvertions ++;
	}

	while(Node[i].Nfi !=0){
		i = findXFanIn(Node, i);
		if(Node[i].Type == NOR || Node[i].Type == NOT || Node[i].Type == NAND){// inverting type gate
			numInvertions ++;
		}
	}
	if(numInvertions % 2 == 1){ // inverting gates count is odd
		v = !v;
	}

	gv.g= i;
	gv.v = v;
	 //printf("iddddd%d val%d\n",gv->g, gv->v);
	return gv;
}
//end of backtrace
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to findXFanIn
***************************************************************************************************/
int findXFanIn(GATE *Node, int i){
	LIST *tmp = Node[i].Fin;
	i = 0;
	while(tmp!=NULL){   
		if(Node[tmp->Id].Val == XV ){ // check value is x 
			i = tmp->Id;
			//printf("id is %d\n",i);
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
	//printf("start set dont cares\n");
	int i;
	for(i=1;i<=tGat;i++){ 
		Node[i].Val = XV;			
	}
	//printf("done set dont cares\n");
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
 Function to check DFrontier
***************************************************************************************************/
int checkDFrontier(int id, GATE *Node){
	//printf("value of date%d id%d\n", Node[id].Val, id);
	if(Node[id].Val != XV){ // if output is not x 
		//printf("value x\n");
		return 0;
	}
	LIST *Cur = Node[id].Fin;
	LIST *temp=NULL;
	//printf("value of input val%d\n", Node[10].Val);
	temp=Cur;
	while(temp!=NULL){
	if(Node[temp->Id].Val==D || Node[temp->Id].Val==DB){
		//printf("dfront\n");
		return 1;
	}
	temp=temp->Next; }
	return 0;
}
//end of checkDFrontier
/****************************************************************************************************************************/
/***************************************************************************************************
 Function to print PI
***************************************************************************************************/
void printPI(GATE* Node){
	int i;
	for(i=1;i<=tGat;i++){ 
		if(Node[i].Type !=0 && Node[i].Nfi == 0){
			printf("%d", Node[i].Val);
		}			
	}
	printf("\n");
}

//end of printPI
/****************************************************************************************************************************/

/***************************************************************************************************
 Function to check fault coverage
***************************************************************************************************/
void checkFaultCoverage(int totalf, int sf, int ff, int tf){
	printf("Total Fault count: %d\n", totalf);
	printf("Total Sucess Fault count: %d\n", sf);
	printf("Total Failed Fault count: %d\n", ff);
	printf("Total Time Out Fault count: %d\n", tf);
	printf("Total Sucess Fault coverage: %.2f%%\n", (double)(sf*100/totalf) );
}

//end of checkFaultCoverage
/****************************************************************************************************************************/
