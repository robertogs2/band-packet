#ifndef BAND_CONTROL 
#define BAND_CONTROL 


#include "linked_list.h"
#include "constants.h"
#include "lpthread.h"

enum control_type{
  W_BAND,
  SIGN_BAND,
  RANDOM_BAND
};


typedef struct{
	int amount_moved;
	short last_side;
	short sign;
	int W;

	Node_t** left_list;
	Node_t** right_list;
	short type;
}side_controller_t;

/*
flip: Flips from 0 to 1 or 1 to 0
short number: Input number
*/

short flip(short number){
	return 1-number;
}

/*
get_list_size: Returns amount of packages in one side of a band
short side: Side of the band
side_controller_t* controller: Controller pointer for the band
*/

int get_list_size(short side, side_controller_t* controller){
	if(side == 0) return get_length(*controller->left_list);
	else return get_length(*controller->right_list);
}

/*
get_list_size: Returns amount of packages in one side of a band
short side: Side of the band
side_controller_t* controller: Controller pointer for the band
*/

int get_list_size2(Node_t** list){
	return get_length(*list);
}

/*
sign_changer: Changes the sign of the band given some time
void* side_controller_void: Void pointer for the controller
*/

int sign_changer(void* side_controller_void){
	side_controller_t* side_controller = (side_controller_t*) side_controller_void;
	while(1){
		usleep(1000*(side_controller)->W);
		(side_controller)->sign = flip((side_controller)->sign);
	}
}

/*
init_controller: Initialices the controller for the band
side_controller_t* controller: Controller pointer
Node_t** left_list: Pointer to left list
Node_t** right_list: Pointer to right list
short type: Type of controller
int W: Parameter for the controller
*/


void init_controller(side_controller_t* controller, Node_t** left_list, Node_t** right_list, short type, int W){
	controller->amount_moved = 0;
	controller->last_side = 0;
	controller->left_list = left_list;
	controller->right_list = right_list;
	controller->sign = 0;
	controller->type = type;
	controller->W = W; //fixed

	// Thread to change sign
	if(type == SIGN_BAND){
		printf("%p\n", controller);
		lpthread_t* thread = (lpthread_t*) malloc(sizeof(lpthread_t));
		Lthread_create(thread, NULL, &sign_changer, (void*) (controller));
	}
	
}
// 0 on left, 1 on right
short control_band(side_controller_t* controller){
	short type = controller->type;
		if(type == W_BAND){
			if(controller->amount_moved >= controller->W-1){ // Reached limit

				controller->last_side = flip(controller->last_side); // Changes side
				int new_side_length = get_list_size(controller->last_side, controller);
				if(new_side_length == 0){
					controller->last_side = flip(controller->last_side); // New side has no packages
					controller->amount_moved++; // Keep going to enter here everytime
				}
				else{ // No problem with new side
					controller->amount_moved = 0;
				}
			}
			else if(get_list_size(controller->last_side, controller) > 0){ // Packages remain in the side
				controller->amount_moved++;
			}
			else{ // Reset the side as there are none there
				controller->amount_moved = 0;
				controller->last_side=flip(controller->last_side);
			}
			return controller->last_side;
		}
		else if(type == SIGN_BAND){
			int new_side_length = get_list_size(controller->sign, controller); // Side from list to give
			if(new_side_length > 0) controller->last_side = controller->sign;		// If it has packages give on
			else controller->last_side = flip(controller->sign);							// If not, give another
			return controller->last_side;
		}
		else if(type == RANDOM_BAND){
			int n = rand()%101;
			
			short dir = n > 50;
			int new_side_length = get_list_size(dir, controller); // Side from list to give
			
			if(new_side_length > 0) {
				controller->last_side = dir; // If it has packages give on
			}
			else{
				controller->last_side = flip(dir);
			}
			return controller->last_side;
		}
		return -1;
}

#endif // BAND_CONTROL
