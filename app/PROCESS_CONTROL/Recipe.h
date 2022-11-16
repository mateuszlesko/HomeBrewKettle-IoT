#ifndef PROCESS_CONTROL_RECIPE_STAGE
#define PROCESS_CONTROL_RECIPE_STAGE

#define MAX_RECIPES_STAGES 8;


struct RecipeProcedure{
    int MashId;
    RecipeStage* stageList;
};

struct RecipeStage
{
    int TemperatureC;
    int HoldingTime;
};
#endif
