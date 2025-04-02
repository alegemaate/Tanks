#include "tools.h"

#include <cmath>

// Collision
bool collisionAny(float xMin1,
                  float xMax1,
                  float xMin2,
                  float xMax2,
                  float yMin1,
                  float yMax1,
                  float yMin2,
                  float yMax2) {
  return xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1;
}

bool collisionBottom(float yMin1, float yMax1, float yMax2) {
  return yMax1 > yMax2 && yMin1 < yMax2;
}

bool collisionTop(float yMin1, float yMax1, float yMin2) {
  return yMax1 > yMin2 && yMin1 < yMin2;
}

bool collisionLeft(float xMin1, float xMax1, float xMin2) {
  return xMax1 > xMin2 && xMin1 < xMin2;
}

bool collisionRight(float xMin1, float xMax1, float xMax2) {
  return xMax1 > xMax2 && xMin1 < xMax2;
}

// Convert radians to degrees
float rad_to_deg(float radians) {
  return radians * 180.0F / 3.14159265358979323846F;
}
