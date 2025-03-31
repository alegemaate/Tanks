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
  if (xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1) {
    return true;
  }
  return false;
}

bool collisionBottom(float yMin1, float yMax1, float yMax2) {
  if (yMax1 > yMax2 && yMin1 < yMax2) {
    return true;
  }
  return false;
}

bool collisionTop(float yMin1, float yMax1, float yMin2) {
  if (yMax1 > yMin2 && yMin1 < yMin2) {
    return true;
  }
  return false;
}

bool collisionLeft(float xMin1, float xMax1, float xMin2) {
  if (xMax1 > xMin2 && xMin1 < xMin2) {
    return true;
  }
  return false;
}

bool collisionRight(float xMin1, float xMax1, float xMax2) {
  if (xMax1 > xMax2 && xMin1 < xMax2) {
    return true;
  }
  return false;
}

// Finds angle of point 2 relative to point 1
float find_angle(float x_1, float y_1, float x_2, float y_2) {
  return atan2f(y_1 - y_2, x_1 - x_2);
}

// Finds distance between 2 points
float find_distance(float x_1, float y_1, float x_2, float y_2) {
  return hypotf(x_1 - x_2, y_1 - y_2);
}

// Convert radians to degrees
float rad_to_deg(float radians) {
  return radians * 180.0f / M_PI;
}
