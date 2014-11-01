/**  This file is part of Witch Blast.
  *
  *  Witch Blast is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  Witch Blast is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with Witch Blast.  If not, see <http://www.gnu.org/licenses/>.
  */

#ifndef __MYTOOLS
#define __MYTOOLS

#include <stdlib.h>
#include <math.h>

const float PI = 3.141592;

/** Vector utility, used in physics
 ** Default constructor creates a random Vector */
class Vector2D
{
public:
	float x, y;
	Vector2D(float amplitude = 1.0f)
	{
		float v = (float)rand() / (float)RAND_MAX;
		v *= 6.283f;
		x = cosf(v) * amplitude;
        y = sinf(v) * amplitude;
    }

	Vector2D(float m_x, float m_y) { x = m_x; y = m_y; }

	float distance2(Vector2D vector)
	{
	    return ( (vector.x - x) * (vector.x - x) + (vector.y - y) * (vector.y - y) );
	}

	float norm()
	{
	  return sqrt(x * x + y * y);
	}

  float distance2(Vector2D vector, float repeatZone)
	{
	    float result = (vector.x - x) * (vector.x - x) + (vector.y - y) * (vector.y - y);

	    float d2 = (vector.x - ( x + repeatZone) ) * (vector.x - ( x + repeatZone)) + (vector.y - y) * (vector.y - y);
	    if (d2 < result) result = d2;

	    d2 = ( (vector.x + repeatZone) - x) * ((vector.x + repeatZone) - x) + (vector.y - y) * (vector.y - y);
	    if (d2 < result) result = d2;

	    return result;
	}

	float angleTo(Vector2D vector)
	{
	  float angle;
	  if (vector.y - y < 0.001f && vector.y - y > -0.001f)
    {
      if (vector.x < x) angle = PI * 0.5;
      else angle = -PI * 0.5;
    }
    else
    {
      float tan = (vector.x - x) / (vector.y - y);
      angle = atan(tan);
    }

    return angle;
	}

	Vector2D vectorTo(Vector2D vector, float amplitude)
	{
	  float angle = angleTo(vector);
	  Vector2D result;

	  if (vector.y > y)
	  {
	    result.x = sin(angle) * amplitude;
	    result.y = cos(angle) * amplitude;
	  }
	  else
	  {
	    result.x = -sin(angle) * amplitude;
	    result.y = -cos(angle) * amplitude;
	  }

    if (result.x != result.x)
      return (Vector2D(amplitude));

	  return result;
	}

	Vector2D vectorNearlyTo(Vector2D vector, float amplitude, float diffAngle)
	{
	  float angle = angleTo(vector);
	  float randomFloat = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	  angle = angle - diffAngle * 0.5f + diffAngle * randomFloat;
	  Vector2D result;

	  if (vector.y > y)
	  {
	    result.x = sin(angle) * amplitude;
	    result.y = cos(angle) * amplitude;
	  }
	  else
	  {
	    result.x = -sin(angle) * amplitude;
	    result.y = -cos(angle) * amplitude;
	  }

    if (result.x != result.x)
      return (Vector2D(amplitude));

	  return result;
	}
};

class IntCoord
{
public:
	int x, y;
    IntCoord(int x, int y) { this->x = x; this->y = y; }
};

#endif
