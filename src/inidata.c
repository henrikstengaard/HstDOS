#ifndef INIDATA_C_
#define INIDATA_C_

#define HSTDOS_INI_PROPERTIES_MAXCOUNT 20
#define HSTDOS_INI_VALUE_MAXLENGTH 100
#define HSTDOS_INI_NAME_MAXLENGTH 50

typedef struct {
    char name[HSTDOS_INI_NAME_MAXLENGTH];
    char value[HSTDOS_INI_VALUE_MAXLENGTH];
} IniProperty;

typedef struct
{
    char name[HSTDOS_INI_NAME_MAXLENGTH];
    int count;
    IniProperty properties[HSTDOS_INI_PROPERTIES_MAXCOUNT];
} IniSection;

void clearIniSection(IniSection *section)
{
    int i;
    for (i = 0; i < HSTDOS_INI_PROPERTIES_MAXCOUNT; i++)
    {
		memset(section->properties[i].name, 0, HSTDOS_INI_NAME_MAXLENGTH);
		memset(section->properties[i].value, 0, HSTDOS_INI_VALUE_MAXLENGTH);
    }
    section->count = 0;
    memset(section->name, 0, HSTDOS_INI_NAME_MAXLENGTH);
}

#endif