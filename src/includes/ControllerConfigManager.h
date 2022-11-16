#include "dtos/ControllerConfig.h"

extern const char *configurationFilename;
extern ControllerConfig config;

void initConfigFile();
void saveConfiguration();
void printFile(const char *filename);