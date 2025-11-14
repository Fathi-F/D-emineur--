#include "HowTo.h"

#include <SFML/Graphics/Font.h>
#include <SFML/Graphics/RectangleShape.h>
#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/Text.h>
#include <SFML/Graphics/Texture.h>

sfText* CreateText(const char* text, const char* fontFilePath, const int size)
{
    // Use sfText_create() to create a Text and contains it in a sfText pointer
    sfText* newText = sfText_create();
    // Create its font with sfFont_createFromFile(fontFilePath) contains it in a sfFont pointer, 
    // where fontFilePath is the path to find the .ttf file in your project
    sfFont* newFont = sfFont_createFromFile(fontFilePath);
    if (!newFont) // Check if it found the font file to be sure
        return NULL;

    // Use sfText_setString(newText, text) to set the text you want, 
    // where newText is your sfText pointer and text is "your text" (char[] or const char*)
    sfText_setString(newText, text);
    // Assign the font to your sfText with sfText_setFont(newText, newFont), 
    // where newText is your sfText pointer and newFont is your sfFont pointer
    sfText_setFont(newText, newFont);
    // Finally set the size of your text with sfText_setCharacterSize(newText, size),
    // where newText is your sfText pointer and size is a integer (int)
    sfText_setCharacterSize(newText, size);

    return newText;
}

sfRectangleShape* CreateRectangle(sfVector2f size, sfVector2f position, sfColor color)
{
    // Use sfRectangleShape_create() to create a Rectangle and contains it in a sfRectangleShape pointer
    sfRectangleShape* newShape = sfRectangleShape_create();
    // Set its size with sfRectangleShape_setSize(newShape, size),
    // where newShape is your sfRectangleShape pointer and size is a sfVector2f
    sfRectangleShape_setSize(newShape, size);
    // Set its position with sfRectangleShape_setPosition(newShape, position),
    // where newShape is your sfRectangleShape pointer and position is a sfVector2f
    sfRectangleShape_setPosition(newShape, position);
    // Finally set its color with sfRectangleShape_setFillColor(newShape, color),
    // where newShape is your sfRectangleShape pointer and color is a sfColor
    sfRectangleShape_setFillColor(newShape, color);
    
    return newShape;
}

sfSprite* CreateSprite(const char* textureFilePath)
{
    // Use sfSprite_create() to create a Sprite and contains it in a sfSprite pointer
    sfSprite* newSprite = sfSprite_create();
    // Create its texture with sfTexture_createFromFile(textureFilePath, NULL)
    // where textureFilePath is the path to find the .png or .jpg file in your project
    sfTexture* newTexture = sfTexture_createFromFile(textureFilePath, NULL);
    if (!newTexture) // Check if it found the texture file to be sure
        return NULL;

    // Finally assign the texture to your sfSprite with sfSprite_setTexture(newSprite, newTexture, NULL),
    // where newSprite is your sfSprite pointer and newTexture is your sfTexture pointer
    sfSprite_setTexture(newSprite, newTexture, NULL);
    
    return newSprite;
}

bool IsIntersecting(sfRectangleShape* rect, sfSprite* sprite)
{
    // Get the global bounds of both the rectangle and the sprite
    // Usually you just have to write the type of element and _getGlobalBounds(element) to get its global bounds
    sfFloatRect r = sfRectangleShape_getGlobalBounds(rect);
    sfFloatRect s = sfSprite_getGlobalBounds(sprite);

    // Use sfFloatRect_intersects to check if they are intersecting
    // It returns true if they are intersecting, false otherwise
    if (sfFloatRect_intersects(&r, &s, NULL))
    {
        // The rectangle and the sprite are intersecting
        return true;
    }
    
    return false;
}

bool IsContained(sfVector2i point, sfRectangleShape* rect)
{
    // Get the global bounds of the rectangle
    // Usually you just have to write the type of element and _getGlobalBounds(element) to get its global bounds
    sfFloatRect bounds = sfRectangleShape_getGlobalBounds(rect);

    // Use sfFloatRect_contains to check if the point is contained in the rectangle
    // It returns true if the point is contained, false otherwise
    if (sfFloatRect_contains(&bounds, point.x, point.y))
    {
        // The point is contained in the rectangle
        return true;
    }
    
    return false;
}
