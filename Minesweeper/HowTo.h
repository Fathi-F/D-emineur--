#pragma once

#include <SFML/Graphics/Types.h>
#include <SFML/Graphics/Color.h>
#include <SFML/System/Vector2.h>
#include <stdbool.h>

/// <summary>
/// Here an example of how you create a Text in SFML
/// </summary>
/// <param name="text">The text you want SFML to write on screen</param>
/// <param name="fontFilePath">The path where your font file .ttf is</param>
/// <param name="size">The desired size of your text</param>
/// <returns>THIS IS AN EXAMPLE DON'T USE THIS FUNCTION</returns>
sfText* CreateText(const char* text, const char* fontFilePath, const int size);

/// <summary>
/// Here an example of how you create a Rectangle in SFML
/// </summary>
/// <param name="size">The desired size of your rectangle in sfVector2f format ({ width, height })</param>
/// <param name="position">The initial position of your rectangle in sfVector2f format ({ width, height }). BE CAREFUL ! the origin of any element is at its top left by default.</param>
/// <param name="color">The color to fill your rectangle with in sfColor format (usually create with sfColor_fromRGB( r, g, b))</param>
/// <returns>THIS IS AN EXAMPLE DON'T USE THIS FUNCTION</returns>
sfRectangleShape* CreateRectangle(sfVector2f size, sfVector2f position, sfColor color);

/// <summary>
/// Here an example of how you create a Sprite in SFML
/// </summary>
/// <param name="textureFilePath">The path where your file .png or .jpg is</param>
/// <returns>THIS IS AN EXAMPLE DON'T USE THIS FUNCTION</returns>
sfSprite* CreateSprite(const char* textureFilePath);

/// <summary>
/// Here an example of how you check if a rectangle collides with a sprite in SFML (or any other graphical element)
/// </summary>
/// <param name="rect">A rectangle that you would like to check if it collides with some other element (it can be any kind of graphical element)</param>
/// <param name="sprite">A sprite that you would like to check if it collides with some other element (it can be any kind of graphical element)</param>
/// <returns>THIS IS AN EXAMPLE DON'T USE THIS FUNCTION</returns>
bool IsIntersecting(sfRectangleShape* rect, sfSprite* sprite);

/// <summary>
/// Here an example of how you check if a point is contained in a rectangle in SFML
/// </summary>
/// <param name="point">The point you want to check if it is contained in an element, in sfVector2i format ({ x, y }). </param>
/// <param name="rect">The rectangle (it can be any kind of graphical element) you want to check if it contains the point, in sfRectangleShape* format.</param>
/// <returns>THIS IS AN EXAMPLE DON'T USE THIS FUNCTION</returns>
bool IsContained(sfVector2i point, sfRectangleShape* rect);