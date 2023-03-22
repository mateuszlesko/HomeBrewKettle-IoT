#include "json_helper.h"

void deserialize_json_to_mashing_recipe(char* json, T_MashingProcess *p_mashing, int* temperatures, int* durations)
{
    cJSON *root2 = cJSON_Parse(json);
	p_mashing->mashing_id = cJSON_GetObjectItem(root2,"MID")->valueint;
    p_mashing->recipe_id = cJSON_GetObjectItem(root2,"RID")->valueint;
    
    p_mashing->num_stages = cJSON_GetObjectItem(root2,"PC")->valueint;
      
    cJSON *temperatures_json= cJSON_GetObjectItem(root2,"MTpL");
    
    for (int i = 0; i < p_mashing->num_stages; i++) 
    {
        temperatures[i] = cJSON_GetArrayItem(temperatures_json, i)->valueint;
	}
    cJSON *time_durations_json = cJSON_GetObjectItem(root2,"MTmL");
    for(int i = 0; i < p_mashing->num_stages; i++)
    {
        durations[i] = cJSON_GetArrayItem(time_durations_json, i)->valueint;
    }
    cJSON_Delete(root2);
}

void deserialize_json_to_remote_control(char* json, T_RemoteControl *p_rc){
    cJSON *root2 = cJSON_Parse(json);
    p_rc->control_signals = cJSON_GetObjectItem(root2,"cs")->valueint;
    cJSON_Delete(root2);
}