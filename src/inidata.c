#ifndef INIDATA_C_
#define INIDATA_C_

typedef struct {
    char* name;
    char* value;
} Property;

typedef struct
{
    int count;
    int size;
    Property* array;
} PropertiesArray;

typedef struct {
    char* name;
    PropertiesArray* properties;
} Section;

typedef struct {
    int count;
    int size;
    Section* array;
} SectionsArray;

typedef struct {
    SectionsArray* sections;
} IniData;

Property* initIniProperty()
{
    Property* property;
    property = calloc(1, sizeof(*property));
    if (property == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    property->name = calloc(50, sizeof(char));
    if (property->name == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    strcpy(property->name, "");
    property->value = calloc(255, sizeof(char));
    if (property->value == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    strcpy(property->value, "");
    return property;
}

PropertiesArray* initIniProperties()
{
    PropertiesArray* properties;
    properties = calloc(1, sizeof(*properties));
    if (properties == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    properties->count = 0;
    properties->size = 10;
    properties->array = calloc(properties->size, sizeof(*properties->array));
    if (properties->array == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    return properties;
}

Section* initIniSection()
{
    Section* section;
    section = calloc(1, sizeof(*section));
    if (section == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    section->name = calloc(255, sizeof(char));
    if (section->name == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    strcpy(section->name, "");
    section->properties = initIniProperties();
    return section;
}

SectionsArray* initIniSections()
{
    SectionsArray* sections;
    sections = calloc(1, sizeof(*sections));
    if (sections == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    sections->count = 0;
    sections->size = 10;
    sections->array = calloc(sections->size, sizeof(*sections->array));
    if (sections->array == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    return sections;
}

IniData* initIniData()
{
    IniData* iniData;
    iniData = calloc(1, sizeof(*iniData));
    if (iniData == NULL)
    {
        printf("Couldn't allocate memory\n");
        exit(1);
    }
    iniData->sections = initIniSections();
    return iniData;
}

void expandIniProperties(PropertiesArray* properties)
{
    properties->size *= 2;
    properties->array = realloc(properties->array, properties->size * sizeof(*properties->array));
}

void addIniProperty(PropertiesArray* properties, Property* property)
{
    // expand properties array, if count is equal to size
    if (properties->count >= properties->size)
    {
        expandIniProperties(properties);
    }
    
    // add property to properties array
    properties->array[properties->count++] = *property;
}

void expandIniSections(SectionsArray* sections)
{
    sections->size *= 2;
    sections->array = realloc(sections->array, sections->size * sizeof(*sections->array));
}

void addIniSection(SectionsArray* sections, Section* section)
{
    // expand sections array, if count is equal to size
    if (sections->count >= sections->size)
    {
        expandIniSections(sections);
    }
    
    // add section to sections array
    sections->array[sections->count++] = *section;
}

void freeIniProperty(Property* property)
{
    if (property == NULL)
    {
        return;
    }
    if (property->name != NULL)
    {
        free(property->name);
    }
    if (property->value != NULL)
    {
        free(property->value);
    }
}

void freeIniProperties(PropertiesArray* properties)
{
    int i;
    if (properties == NULL)
    {
        return;
    }
    if (properties->array != NULL)
    {
        for(i = 0; i < properties->count; i++)
        {
            freeIniProperty(&properties->array[i]);
        }    
        free(properties->array);
    }
}

void freeIniSection(Section* section)
{
    if (section == NULL)
    {
        return;
    }
    if (section->name != NULL)
    {
        free(section->name);
    }
    freeIniProperties(section->properties);
    free(section->properties);
}

void freeIniSections(SectionsArray* sections)
{
    int i;
    if (sections == NULL)
    {
        return;
    }
    if (sections->array != NULL)
    {
        for(i = 0; i < sections->count; i++)
        {
            freeIniSection(&sections->array[i]);
        }    
        free(sections->array);
    }
}

void freeIniData(IniData* iniData)
{
    if (iniData == NULL)
    {
        return;
    }
    freeIniSections(iniData->sections);
    free(iniData->sections);
}

#endif