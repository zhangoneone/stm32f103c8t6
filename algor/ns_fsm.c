#include "ns_fsm.h"

static void fsm_add_state(Fsm_t *fsm,Fsm_State_t *state) {
	fsm->state = state;
}
void fsm_set_cur_state(Fsm_t *fsm, int id, void *action_param) {
	Fsm_State_t *start = fsm->state->next_state;
	if (fsm->state->state_id == id) {
		fsm->cur_state = fsm->state; 
		if(fsm->cur_state->action!=NULL)
			fsm->cur_state->action(action_param); 
		return;
	}
	while (start != fsm->state) {
		if (start->state_id == id) {
			fsm->cur_state = start; 
			if (fsm->cur_state->action != NULL)
				fsm->cur_state->action(action_param);
			return;
		}
		else start = start->next_state;
	}
	if (start == fsm->state) {
		fsm->cur_state = fsm->state;
		if (fsm->cur_state->action != NULL)
			fsm->cur_state->action(action_param);
	}
}
Fsm_State_t *fsm_get_cur_state(Fsm_t *fsm) {
	return fsm->cur_state;
}
void fsm_input_event(Fsm_t *fsm, Fsm_Event_t event, void *action_param) {
	if (fsm->cur_state->transfer_event == event) {
		fsm->cur_state = fsm->cur_state->next_state;
		if (fsm->cur_state->action != NULL)
			fsm->cur_state->action(action_param);
		return;
	}
	else return;
}
void fsm_start(Fsm_t *fsm, Fsm_State_t *start_state, Fsm_State_t *cur_state, void *action_param) {
	fsm_add_state(fsm, start_state);
	fsm_set_cur_state(fsm, cur_state->state_id,action_param);
}

void fsm_test() {
	fsm->start(fsm, &released, &released, 0);
	show_info("当前状态:%s\r\n", fsm->cur_state->state_name);
	fsm_input_event(fsm, unstable_low_lov, 0);
	show_info("发生事件%s,当前状态:%s\r\n", tostring(unstable_low_lov), fsm->cur_state->state_name);
	fsm_input_event(fsm, low_vol,0);
	show_info("发生事件%s,当前状态:%s\r\n", tostring(low_vol), fsm->cur_state->state_name);
	fsm_input_event(fsm, unstable_high_lov, 0);
	show_info("发生事件%s,当前状态:%s\r\n", tostring(unstable_high_lov), fsm->cur_state->state_name);
	fsm_input_event(fsm, high_vol, 0);
	show_info("发生事件%s,当前状态:%s\r\n", tostring(high_vol), fsm->cur_state->state_name);
}
