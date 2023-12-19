#include "items.h"


Item::Item(ItemType type, int x, int y, int size)
{
  this->size = size;
  this->type = type;
  this->x = x;
  this->y = y;

    if (type == STONE)
  {
    this->value = size;
  }
    else if (type == GOLD)
  {
    this->value = size * 5;
  } 
    else if (type == DIAMOND) 
  {
    this->value = size * 30;
  }
};

Item::~Item()
{
  size = 0;
}

