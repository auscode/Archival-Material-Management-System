#include "bitmap.h"

/*
This function adds a new material to an archive by checking if there is enough space in the archive and 
if the material title does not already exist in the archive. If the material can be added, it is added 
to the archive and the function returns 0. If the archive count is greater than and equal to 100 or the material title already exists, 
the function returns -1 to indicate failure.
*/
int addMaterial(struct Archive *archive, struct Material material)
{
}

/*
This function searches for a Material with a given title in an Archive. 
It first checks if the Archive is empty, and if so, returns NULL. If the Archive contains Materials, 
the function iterates over each Material in the Archive and compares its title with the desired title. 
If it finds a Material with a matching title, it returns a pointer to that Material. 
If no Material with the desired title is found, the function returns NULL.
*/
struct Material *findMaterial(struct Archive *archive, char *title)
{
}

/*
This function filters materials of a specified type from an Archive struct and stores them in a new Archive struct. 
It loops through each material in the original archive and checks if it matches the specified type. 
If a match is found, it copies the material to the filtered archive, including type-specific details and common fields. 
If no materials of the specified type are found, it returns early. The filteredArchive struct is initialized with a 
count of 0 and its materials array is updated as materials are added to it.
*/
void filterMaterials(struct Archive *archive, enum MaterialType type)
{
}

/*
This function updates the details of a material (book, journal, or newspaper) stored in an archive based on its title. 
It first checks if the archive and title are not null, then finds the material with the given title. 
If the material is found, it updates its details based on its type (book, journal, or newspaper). 
If the type of the material is invalid, the function returns an error code. If the update is successful, it returns a success code.
*/
int updateMaterial(struct Archive *archive, char *title, union MaterialDetails details)
{
}

/*
This function removes a material from an archive by title. It searches for the material with the given title in the archive, 
and if found, removes it by shifting all subsequent materials down by one index. The count of materials in the archive is 
decremented, and the memory used by the last material in the archive is cleared. If the material is not found, the function does nothing.
*/
void removeMaterial(struct Archive *archive, char *title)
{
}

/*
This function filters materials (books, journals, and newspapers) stored in an archive by the provided author name. 
It returns an array of pointers to the materials that match the author name. If the archive or author name is NULL, 
or the author name is an empty string, the function returns NULL. The matching materials are stored in a temporary 
array of structs, which is then copied into a newly allocated array of structs before being returned.
*/
struct Material *filterMaterialsByAuthor(struct Archive *archive, char *author)
{
}
