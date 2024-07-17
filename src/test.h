#include <cxxtest/TestSuite.h>
#include "../src/bitmap.h"

class SampleTestSuite : public CxxTest::TestSuite
{
public:
    void testAddMaterialSuccess()
    {
        struct Archive archive = {0};
        struct Material material = {"Book Title", BOOK, {.book = {200, "Author Name", NOVEL}}};
        TS_ASSERT_EQUALS(addMaterial(&archive, material), 0);
        TS_ASSERT_EQUALS(archive.count, 1);
    }

    void testAddMaterialDuplicateTitle()
    {
        struct Archive archive = {0};
        struct Material material1 = {"Book Title", BOOK, {.book = {200, "Author Name", NOVEL}}};
        struct Material material2 = {"Book Title", JOURNAL, {.journal = {2, "Publisher", LITERATURE}}};
        TS_ASSERT_EQUALS(addMaterial(&archive, material1), 0);
        TS_ASSERT_EQUALS(addMaterial(&archive, material2), -1);
        TS_ASSERT_EQUALS(archive.count, 1);
    }

    void testAddMaterialDifferentTypes()
    {
        struct Archive archive = {0};
        struct Material book = {"Book Title", BOOK, {.book = {200, "Author Name", NOVEL}}};
        struct Material journal = {"Journal Title", JOURNAL, {.journal = {2, "Publisher", LITERATURE}}};
        struct Material newspaper = {"Newspaper Title", NEWSPAPER, {.newspaper = {"Editor", MONTHLY}}};
        TS_ASSERT_EQUALS(addMaterial(&archive, book), 0);
        TS_ASSERT_EQUALS(addMaterial(&archive, journal), 0);
        TS_ASSERT_EQUALS(addMaterial(&archive, newspaper), 0);
        TS_ASSERT_EQUALS(archive.count, 3);
    }

    /////////////////////////////////////////////////////////////////////////////////

    void testFindMaterialEmptyArchive()
    {
        // Initialize an empty archive
        struct Archive archive = {{}, 0};
        // Test for a material that should not be found
        char title[] = "The Catcher in the Rye";
        struct Material *result = findMaterial(&archive, title);
        TS_ASSERT(result == NULL);
        TS_TRACE("testFindMaterialEmptyArchive");
    }
    void testFindMaterialNotFound()
    {
        // Initialize an archive with some materials
        struct Archive archive = {
            {{"The Great Gatsby", BOOK, {.book = {180, "F. Scott Fitzgerald", NOVEL}}},
             {"To Kill a Mockingbird", BOOK, {.book = {281, "Harper Lee", NOVEL}}},
             {"The Sun Also Rises", BOOK, {.book = {251, "Ernest Hemingway", NOVEL}}}},
            3};

        // Test for a material that should not be found
        char title[] = "The Catcher in the Rye";
        struct Material *result = findMaterial(&archive, title);
        TS_ASSERT(result == NULL);
        TS_TRACE("testFindMaterialNotFound");
    }

    void testFindMaterialNotFound2()
    {
        // Create an archive with one Material
        struct Material material = {"To Kill a Mockingbird", BOOK, {.book = {281, "Harper Lee", NOVEL}}};
        struct Archive archive = {{material}, 1};

        // Search for a Material that doesn't exist in the archive
        char title[] = "The Catcher in the Rye";
        struct Material *result = findMaterial(&archive, title);

        // Ensure the function returns NULL
        TS_ASSERT(result == NULL);
    }

    //////////////////////////////////////////////////////////
    void testFilterMaterials_NoMatch()
    {
        struct Archive archive;
        archive.count = 3;
        struct Material material1 = {.title = "Material 1", .type = BOOK, {.book = {.pages = 100, .author = "Author 1", .type = NOVEL}}};
        archive.materials[0] = material1;
        struct Material material2 = {.title = "Material 2", .type = JOURNAL, {.journal = {.issue = 5, .publisher = "Publisher 1", .type = SCIENCE}}};
        archive.materials[1] = material2;
        struct Material material3 = {.title = "Material 3", .type = NEWSPAPER, {.newspaper = {.editor = "Editor 1", .type = DAILY}}};
        archive.materials[2] = material3;
        filterMaterials(&archive, NEWSPAPER);
        TS_ASSERT_EQUALS(archive.count, 3);
    }

    void testFilterMaterials_Match()
    {
        struct Archive archive;
        archive.count = 3;
        struct Material material1 = {.title = "Material 1", .type = BOOK, {.book = {.pages = 100, .author = "Author 1", .type = NOVEL}}};
        archive.materials[0] = material1;
        struct Material material2 = {.title = "Material 2", .type = JOURNAL, {.journal = {.issue = 5, .publisher = "Publisher 1", .type = SCIENCE}}};
        archive.materials[1] = material2;
        struct Material material3 = {.title = "Material 3", .type = NEWSPAPER, {.newspaper = {.editor = "Editor 1", .type = DAILY}}};
        archive.materials[2] = material3;
        filterMaterials(&archive, JOURNAL);
        TS_ASSERT_EQUALS(archive.count, 3);
        TS_ASSERT_EQUALS(archive.materials[0].type, BOOK);
        TS_ASSERT_EQUALS(archive.materials[0].details.book.pages, 100);
        TS_ASSERT_EQUALS(archive.materials[0].details.book.author, "Author 1");
    }

    void testFilterMaterials2()
    {
        struct Archive archive;
        archive.count = 3;
        struct Material material1 = {.title = "The Great Gatsby", .type = BOOK, {.book = {.pages = 180, .author = "F. Scott Fitzgerald", .type = NOVEL}}};
        struct Material material2 = {.title = "Nature", .type = JOURNAL, {.journal = {.issue = 5, .publisher = "Scientific American", .type = SCIENCE}}};
        struct Material material3 = {.title = "The New York Times", .type = NEWSPAPER, {.newspaper = {.editor = "Joe Smith", .type = DAILY}}};
        archive.materials[0] = material1;
        archive.materials[1] = material2;
        archive.materials[2] = material3;
        filterMaterials(&archive, JOURNAL);
        TS_ASSERT_EQUALS(archive.count, 3);
        TS_ASSERT_EQUALS(std::strncmp(archive.materials[0].title, "Nature", 50), 6);
        TS_ASSERT_EQUALS(archive.materials[1].type, JOURNAL);
        TS_ASSERT_EQUALS(archive.materials[1].details.journal.issue, 5);
        TS_ASSERT_EQUALS(std::strncmp(archive.materials[1].details.journal.publisher, "Scientific American", 50), 0);
        TS_ASSERT_EQUALS(archive.materials[1].details.journal.type, SCIENCE);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////

    void testUpdateMaterialNullTitle()
    {
        struct Archive archive;
        archive.count = 0;
        union MaterialDetails details;
        int result = updateMaterial(&archive, NULL, details);
        TS_ASSERT_EQUALS(result, -1);
        TS_TRACE("testUpdateMaterialNullTitle");
    }

    void testUpdateMaterialSuccess()
    {
        struct Archive archive;
        archive.count = 0;

        // add a book to the archive
        struct Material material;
        strcpy(material.title, "The Great Gatsby");
        material.type = BOOK;
        material.details.book.pages = 180;
        strcpy(material.details.book.author, "F. Scott Fitzgerald");
        material.details.book.type = NOVEL;
        addMaterial(&archive, material);

        // update the book with new details
        union MaterialDetails details;
        strcpy(details.book.author, "Francis Scott Fitzgerald");
        details.book.pages = 218;
        details.book.type = HISTORY;
        char title[] = "The Great Gatsby";
        // struct Material *result = findMaterial(&archive, title);
        int result = updateMaterial(&archive, title, details);

        // check that the book was updated successfully
        TS_ASSERT_EQUALS(result, 0);
        TS_ASSERT_EQUALS(archive.count, 1);
        TS_ASSERT_EQUALS(archive.materials[0].type, BOOK);
        TS_ASSERT_EQUALS(archive.materials[0].details.book.pages, 218);
        TS_ASSERT_EQUALS(strcmp(archive.materials[0].details.book.author, "Francis Scott Fitzgerald"), 0);
        TS_ASSERT_EQUALS(archive.materials[0].details.book.type, HISTORY);
        TS_TRACE("testUpdateMaterialSuccess");
    }

    void testUpdateMaterialNullArchive()
    {
        struct Archive *archive = NULL;
        union MaterialDetails details;
        char title[] = "The Great Gatsby";
        int result = updateMaterial(archive, title, details);
        TS_ASSERT_EQUALS(result, -1);
        TS_TRACE("testUpdateMaterialNullArchive");
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void testRemoveMaterialSuccess1()
    {
        // Initialize an archive with one material
        struct Archive archive = {
            {{"The Great Gatsby", BOOK, {.book = {180, "F. Scott Fitzgerald", NOVEL}}}},
            1};
        // Remove the material
        char title[] = "The Great Gatsby";
        removeMaterial(&archive, title);

        // Check that the archive is now empty
        TS_ASSERT_EQUALS(archive.count, 0);
        TS_TRACE("testRemoveMaterialSuccess1");
    }

    void testRemoveMaterialSuccess2()
    {
        // Initialize an archive with two materials
        struct Archive archive = {
            {{"The Great Gatsby", BOOK, {.book = {180, "F. Scott Fitzgerald", NOVEL}}},
             {"To Kill a Mockingbird", BOOK, {.book = {281, "Harper Lee", NOVEL}}}},
            2};
        // Remove one of the materials
        char title[] = "To Kill a Mockingbird";
        removeMaterial(&archive, title);

        // Check that the material was removed and the count is decremented
        TS_ASSERT_EQUALS(archive.count, 1);
        TS_ASSERT_EQUALS(strcmp(archive.materials[0].title, "The Great Gatsby"), 0);
        TS_TRACE("testRemoveMaterialSuccess2");
    }

    void testRemoveMaterialSuccess3()
    {
        // Initialize an archive with three materials
        struct Archive archive = {
            {{"The Great Gatsby", BOOK, {.book = {180, "F. Scott Fitzgerald", NOVEL}}},
             {"To Kill a Mockingbird", BOOK, {.book = {281, "Harper Lee", NOVEL}}},
             {"The Sun Also Rises", BOOK, {.book = {251, "Ernest Hemingway", NOVEL}}}},
            3};
        // Remove the first material
        char title[] = "The Great Gatsby";
        removeMaterial(&archive, title);

        // Check that the material was removed and the count is decremented
        TS_ASSERT_EQUALS(archive.count, 2);
        TS_ASSERT_EQUALS(strcmp(archive.materials[0].title, "To Kill a Mockingbird"), 0);
        TS_ASSERT_EQUALS(strcmp(archive.materials[1].title, "The Sun Also Rises"), 0);
        TS_TRACE("testRemoveMaterialSuccess3");
    }
    

    ////////////////////////////////////////////////////////////////////////

    void testAuthorNotFound()
    {
        struct Archive archive = {0};
        struct Material material1 = {.title = "A Brief History of Time", .type = BOOK, {.book = {.pages = 256, .author = "Stephen Hawking", .type = NOVEL}}};
        struct Material material2 = {.title = "Journal of Science", .type = JOURNAL, {.journal = {.issue = 13, .publisher = "John Wiley & Sons", .type = SCIENCE}}};
        archive.materials[0] = material1;
        archive.materials[1] = material2;
        archive.count = 2;
        char author[] = "John Doe";
        struct Material *result = filterMaterialsByAuthor(&archive, author);
        TS_ASSERT(result == NULL);
        TS_TRACE("testAuthorNotFound");
    }

    void testOneMatchingBook()
    {
        struct Archive archive = {0};
        struct Material material1 = {.title = "A Brief History of Time", .type = BOOK, {.book = {.pages = 256, .author = "Stephen Hawking", .type = NOVEL}}};
        struct Material material2 = {.title = "A Tale of Two Cities", .type = BOOK, {.book = {.pages = 320, .author = "Charles Dickens", .type = NOVEL}}};
        archive.materials[0] = material1;
        archive.materials[1] = material2;
        archive.count = 2;
        char author[] = "Stephen Hawking";
        struct Material *result = filterMaterialsByAuthor(&archive, author);
        TS_ASSERT(result != NULL);
        TS_ASSERT_EQUALS(result[0].type, BOOK);
        TS_ASSERT_EQUALS(std::strcmp(result[0].title, "A Brief History of Time"), 0);
        TS_ASSERT_EQUALS(result[0].details.book.pages, 256);
        TS_ASSERT_EQUALS(result[0].details.book.type, NOVEL);
        TS_ASSERT_EQUALS(std::strcmp(result[0].details.book.author, "Stephen Hawking"), 0);
        TS_TRACE("testOneMatchingBook");
    }
    void testFilterMaterialsByAuthor_MatchingBooks()
    {
        struct Archive archive = {
            .materials = {
                {.title = "Material1",
                 .type = BOOK,
                 {.book = {.pages = 200, .author = "Author1", .type = NOVEL}}},
                {.title = "Material2",
                 .type = JOURNAL,
                 {.journal = {.issue = 1, .publisher = "Publisher1", .type = SCIENCE}}},
                {.title = "Material3",
                 .type = BOOK,
                 {.book = {.pages = 250, .author = "Author1", .type = BIOGRAPHY}}}},
            .count = 3};
        char author[] = "Author1";
        struct Material *result = filterMaterialsByAuthor(&archive, author);
        TS_ASSERT_EQUALS(result[0].type, BOOK);
        TS_ASSERT_EQUALS(result[0].details.book.pages, 200);
        TS_ASSERT_EQUALS(result[1].type, BOOK);
        TS_ASSERT_EQUALS(result[1].details.book.pages, 250);
        TS_ASSERT_EQUALS(result[2].type, BOOK);
        TS_ASSERT_EQUALS(result[3].type, BOOK);
        TS_ASSERT_EQUALS(result[3].details.journal.issue, 0);
    }
};
