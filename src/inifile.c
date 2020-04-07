#include <stdio.h>
#include <stdlib.h>
#include "string.c"
#include "inidata.c"

#define LINEMAXLENGTH 1024

int isEmpty(char* line)
{
    return strlen(line) == 0;
}

int isComment(char* line)
{
    return line[0] == ':' || line[0] == '#';
}

int isSection(char* line)
{
    return line[0] == '[';
}

Section* parseIniSection(char* line)
{
    Section* section;
    char delim[] = "[]";
    char *ptr;
    ptr = strtok(line, delim);

    if (ptr == NULL || ptr[0] == '\0')
    {
        return NULL;
    }

    section = initIniSection();
    strcpy(section->name, ptr);
    return section;
}

Property* parseIniProperty(char* line)
{
    Property* property;
    char* delimiterPointer;
    int delimiterIndex;
    int valueLength;

    delimiterPointer = strchr(line, '=');
    delimiterIndex = (int)(delimiterPointer - line);

    // return null, if line doesn't contain '=' delimiter
    if (delimiterIndex <= 0)
    {
        return NULL;
    }

    property = initIniProperty();
    strncpy(property->name, line, delimiterIndex);
    property->name[delimiterIndex] = '\0';
    valueLength = strlen(line) - delimiterIndex - 1;
    strncpy(property->value, &line[delimiterIndex+1], valueLength);
    property->value[valueLength] = '\0';
    return property;
}

IniData* readIniFile(char* path)
{
    FILE* filePointer;
    char line[LINEMAXLENGTH];
    int i;
    Property* property = NULL;
    Section* section = NULL;
    IniData* iniData = NULL;

    filePointer = fopen(path, "r");

    if (filePointer == NULL)
    {
        return NULL;
    }

    iniData = initIniData();

    // read lines from file
    while(fgets(line, LINEMAXLENGTH, filePointer))
    {
        // trim leading and tailing whitespaces
        trim(line);

        // skip line, if it's empty and a comment
        if (isEmpty(line) || isComment(line))
        {
            continue;
        }

        // parse section
        if (isSection(line))
        {
            section = parseIniSection(line);

            // add section, if it's not null
            if (section != NULL)
            {
                addIniSection(iniData->sections, section);
            }

            continue;
        }

        // skip line, if section is null
        if (section == NULL)
        {
            continue;
        }

        // parse property
        property = parseIniProperty(line);

        // skip property, if it's null
        if (property == NULL)
        {
            continue;
        }

        // add property to section
        addIniProperty(section->properties, property);
    }

    fclose(filePointer);

    return iniData;
}