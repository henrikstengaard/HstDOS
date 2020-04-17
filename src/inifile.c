#ifndef INIFILE_C_
#define INIFILE_C_

#include "string.c"
#include "inidata.c"

#define LINEMAXLENGTH 1024

char isEmpty(char* line)
{
    return strlen(line) == 0;
}

char isComment(char* line)
{
    return line[0] == ':' || line[0] == '#';
}

char isSection(char* line)
{
    return line[0] == '[';
}

char isSectionNameEqual(char *sectionName, char* line)
{
    char delim[] = "[]";
    char *ptr;

    if (line[0] != '[')
    {
        return 0;
    }
    
    ptr = strtok(line, delim);

    return ptr != NULL || ptr[0] != '\0' && stricmp(sectionName, ptr);
}

char parseIniProperty(IniProperty* property, char* line)
{
    char* delimiterPointer;
    int delimiterIndex;
    int valueLength;

    delimiterPointer = strchr(line, '=');
    delimiterIndex = (int)(delimiterPointer - line);

    // return false, if line doesn't contain '=' delimiter
    if (delimiterIndex <= 0)
    {
        return 0;
    }

    property->name[0] = '\0';
    strncat(
        property->name,
        line,
        delimiterIndex > HSTDOS_INI_NAME_MAXLENGTH ? HSTDOS_INI_NAME_MAXLENGTH : delimiterIndex);
    property->name[delimiterIndex] = '\0';
    property->value[0] = '\0';
    strncat(
        property->value,
        &line[delimiterIndex+1],
        valueLength > HSTDOS_INI_VALUE_MAXLENGTH ? HSTDOS_INI_VALUE_MAXLENGTH : valueLength);

    return 1;
}

char readSectionFromIniFile(IniSection *section, char *path)
{
    FILE *filePointer;
    char line[LINEMAXLENGTH];
    char inSection = 0;

    filePointer = fopen(path, "r");

    if (filePointer == NULL)
    {
        return 0;
    }

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
            if (inSection)
            {
                break;
            }

            inSection = isSectionNameEqual(section->name, line);

            continue;
        }

        // skip line, if not in section
        if (!inSection)
        {
            continue;
        }

        // parse property
        if (parseIniProperty(&section->properties[section->count], line))
        {
            section->count++;
        }
    }

    fclose(filePointer);

    return 1;
}

#endif