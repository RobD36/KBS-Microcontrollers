#include "items.h"

Item::Item(ItemType type, int x, int y, int size)
{
  this->size = size;
  this->type = type;
  this->x = x;
  this->y = y;

  switch (type)
  {
  case STONE:
    this->value = size;
    break;
  case GOLD:
    this->value = size * 5;
    break;
  case DIAMOND:
    this->value = size * 30;
    break;
  case RAT:
    this->value = 5;
    break;
  default:
    // Handle unknown item type
    break;
  }
};

Item::Item()
{
  this->size = 0;
  this->type = STONE;
  this->x = 0;
  this->y = 0;
  this->value = 0;
}
