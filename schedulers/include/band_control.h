#ifndef BAND_CONTROL 
#define BAND_CONTROL 


#include "linked_list.h"
#include "constants.h"

typedef struct{
	int amount_moved;
	short last_side;
	short sign;
	int W;

	Node_t** left_list;
	Node_t** right_list;
	short type;
}side_controller_t;

short flip(short number){
	return 1-number;
}

int get_list_size(short side, side_controller_t* controller){
	if(side == 0) return get_length(*controller->left_list);
	else return get_length(*controller->right_list);
}

int get_list_size2(Node_t** list){
	return get_length(*list);
}

void init_controller(side_controller_t* controller, Node_t** left_list, Node_t** right_list, short type){
	controller->amount_moved = 0;
	controller->last_side = 0;
	controller->left_list = left_list;
	controller->right_list = right_list;
	controller->sign = 0;
	controller->type = type;
	controller->W = 2; //fixed
}
// 0 on left, 1 on right
short control_band(side_controller_t* controller){
	short type = controller->type;
		if(type == W_BAND){
			//printf("Using W_BAND\n");
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
			printf("Using SIGN\n");
			int new_side_length = get_list_size(controller->sign, controller); // Side from list to give
			if(new_side_length > 0) controller->last_side = controller->sign;		// If it has packages give on
			else controller->last_side = flip(controller->sign);							// If not, give another
			return controller->last_side;
		}
		else if(type == RANDOM_BAND){
			printf("Using RANDOM_BAND\n");
			int n = rand()%101;
			
			short dir = n > 50;
			int new_side_length = get_list_size(dir, controller); // Side from list to give
			
			if(new_side_length > 0) {
				printf("%d\n", dir);
				controller->last_side = dir; // If it has packages give on
			}
			else{
				controller->last_side = flip(dir);
				printf("%d\n", flip(dir)); // If not, give another try
			}
			return controller->last_side;
		}
		return -1;
}

#endif // BAND_CONTROL
