#ifndef OPTIONS_C_
#define OPTIONS_C_

#include "path.c"
#include "menudata.c"

typedef struct {
    char hstDosPath[HSTDOS_PATH_MAXLENGTH];
    char tempPath[HSTDOS_PATH_MAXLENGTH];
    char backText[HSTDOS_TITLE_MAXLENGTH];
    int onlyExecutableFiles;
    int titleBackgroundColor;
    int titleTextColor;
    int menuBackgroundColor;
    int menuTextColor;
    int selectedBackgroundColor;
    int selectedTextColor;
    int shadowBackgroundColor;
    int shadowTextColor;
} HstDosOptions;

void initOptions(HstDosOptions *options)
{
    memset(options->hstDosPath, 0, HSTDOS_PATH_MAXLENGTH);
    memset(options->tempPath, 0, HSTDOS_PATH_MAXLENGTH);
    memset(options->backText, 0, HSTDOS_TITLE_MAXLENGTH);
    
    // default only executable files
    options->onlyExecutableFiles = 1;

    // default back text
    strcpy(options->backText, "< Back >");

    // default colors
    options->titleBackgroundColor = 1;
    options->titleTextColor = 15;
    options->menuBackgroundColor = 0;
    options->menuTextColor = 7;
    options->selectedBackgroundColor = 5;
    options->selectedTextColor = 15;
    options->shadowBackgroundColor = 0;
    options->shadowTextColor = 8;

    // set temp path from environment variable
    strncat(options->tempPath, getenv("TEMP"), HSTDOS_PATH_MAXLENGTH);

    // default temp path, if environment variable is not set
    if (options->tempPath == NULL || options->tempPath[0] == '\0')
    {
        strcpy(options->tempPath, "C:\\");
    }

    // default back text
    strcpy(options->backText, "Back");
}

void readOptions(HstDosOptions *options)
{
    int i;
    IniSection section;
    char hstDosIniPath[255] = {0};

    clearIniSection(&section);
    strcpy(section.name, "hstdos");

    combinePath(hstDosIniPath, options->hstDosPath, "hstdos.ini");

	if (!readSectionFromIniFile(&section, hstDosIniPath))
    {
        return;
    }

    for (i = 0; i < section.count; i++)
    {
        if (stricmp(section.properties[i].name, "backText") == 0 && section.properties[i].value[0] != '\0')
        {
            options->backText[0] = '\0';
            strncat(options->backText, section.properties[i].value, HSTDOS_INI_VALUE_MAXLENGTH);
            continue;
        }

        if (stricmp(section.properties[i].name, "onlyExecutableFiles") == 0 && section.properties[i].value[0] != '\0')
        {
            options->onlyExecutableFiles = atoi(section.properties[i].value);
            continue;
        }

        if (stricmp(section.properties[i].name, "titleBackgroundColor") == 0 && section.properties[i].value[0] != '\0')
        {
            options->titleBackgroundColor = atoi(section.properties[i].value);
            continue;
        }

        if (stricmp(section.properties[i].name, "titleTextColor") == 0 && section.properties[i].value[0] != '\0')
        {
            options->titleTextColor = atoi(section.properties[i].value);
            continue;
        }

        if (stricmp(section.properties[i].name, "menuBackgroundColor") == 0 && section.properties[i].value[0] != '\0')
        {
            options->menuBackgroundColor = atoi(section.properties[i].value);
            continue;
        }

        if (stricmp(section.properties[i].name, "menuTextColor") == 0 && section.properties[i].value[0] != '\0')
        {
            options->menuTextColor = atoi(section.properties[i].value);
            continue;
        }

        if (stricmp(section.properties[i].name, "selectedBackgroundColor") == 0 && section.properties[i].value[0] != '\0')
        {
            options->selectedBackgroundColor = atoi(section.properties[i].value);
            continue;
        }

        if (stricmp(section.properties[i].name, "selectedTextColor") == 0 && section.properties[i].value[0] != '\0')
        {
            options->selectedTextColor = atoi(section.properties[i].value);
            continue;
        }

        if (stricmp(section.properties[i].name, "shadowBackgroundColor") == 0 && section.properties[i].value[0] != '\0')
        {
            options->shadowBackgroundColor = atoi(section.properties[i].value);
            continue;
        }

        if (stricmp(section.properties[i].name, "shadowTextColor") == 0 && section.properties[i].value[0] != '\0')
        {
            options->shadowTextColor = atoi(section.properties[i].value);
            continue;
        }
    }
}

#endif