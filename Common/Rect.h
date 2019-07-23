#ifndef RECT_H_INCLUDED
#define RECT_H_INCLUDED

#include <math.h>

namespace CommonCode
{

class Rect
{
	public:

		int x;
		int y;

		int width;
		int height;

		Rect()
		{
			x = 0;
			y = 0;
			width = 0;
			height = 0;
		}

		Rect(int x, int y, int width, int height)
		{
			this->x = x;
			this->y = y;

			this->width = width;
			this->height = height;
		}

		bool Contains(int x, int y)
		{
			if (x >= this->x && x <= Right()
				&& y >= this->y && y <= Bottom())
			{
				return true;
			}
			return false;
		}

		bool Equals(Rect rect)
		{
			bool left = this->x == rect.x;
			bool top = this->y == rect.y;
			bool width = this->width == rect.width;
			bool height = this->height == rect.height;

			return left && top && width && height;
		}

		void Inflate(int size)
		{
			x -= size;
			y -= size;
			width += size;
			height += size;
		}

		void Deflate(int size)
		{
			x += size;
			y += size;
			width -= size;
			height -= size;
		}

		bool IsDifferent(Rect right)
		{
			return x != right.x || y != right.y
				|| width != right.width || height != right.height;
		}

		bool Intersects(Rect rect)
		{
			return rect.x <= this->Right()
				&& rect.Right() >= this->x
				&& rect.y <= this->Bottom()
				&& rect.Bottom() >= this->y;
		}

		/// <summary>
		/// Will adjust the current rectangle to fit inside the rectangle given
		/// </summary>
		void Fit(Rect rect)
		{
			if (x < rect.x)
			{
				x = rect.x;
			}
			if (y < rect.y)
			{
				y = rect.y;
			}
			if (Right() > rect.Right())
			{
				width = rect.width - x;
			}
			if (Bottom() > rect.Bottom())
			{
				height = rect.height - y;
			}
		}


		int Right()
		{
			return x + width;
		}

		int Bottom()
		{
			return y + height;
		}

};

class Rectf
{
public:

	float x;
	float y;

	float width;
	float height;

	Rectf()
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}

	Rectf(float x, float y, float width, float height)
	{
		this->x = x;
		this->y = y;

		this->width = width;
		this->height = height;
	}

	bool Contains(float x, float y)
	{
		if (x >= this->x && x <= Right()
			&& y >= this->y && y <= Bottom())
		{
			return true;
		}
		return false;
	}

	bool Contains(Rectf rect)
	{
		if (x <= rect.x && y <= rect.y
			&& this->Right() > rect.Right() && this->Bottom() > rect.Bottom())
		{
			return true;
		}
		return false;
	}

	bool Equals(Rectf rect)
	{
		bool left = this->x == rect.x;
		bool top = this->y == rect.y;
		bool width = this->width == rect.width;
		bool height = this->height == rect.height;

		return left && top && width && height;
	}

	void Inflate(float size)
	{
		x -= size;
		y -= size;
		width += size;
		height += size;
	}

	void Deflate(float size)
	{
		x += size;
		y += size;
		width -= size;
		height -= size;
	}

	bool IsDifferent(Rectf right)
	{
		return !this->Equals(x, right.x) || !this->Equals(y, right.y)
			|| !this->Equals(width, right.width) || !this->Equals(height, right.height);
	}

	bool Intersects(Rectf rect)
	{
		return rect.x <= this->Right() 
			&& rect.Right() >= this->x 
			&& rect.y <= this->Bottom() 
			&& rect.Bottom() >= this->y;
	}

	/// <summary>
	/// Will adjust the current rectangle to fit inside the rectangle given
	/// </summary>
	void Fit(Rectf rect)
	{
		if (x < rect.x)
		{
			x = rect.x;
		}
		if (y < rect.y)
		{
			y = rect.y;
		}
		if (Right() > rect.Right())
		{
			width = rect.width - x;
		}
		if (Bottom() > rect.Bottom())
		{
			height = rect.height - y;
		}
	}


	float Right()
	{
		return x + width;
	}

	float Bottom()
	{
		return y + height;
	}

	private:

		bool Equals(float left, float right, float epsilon = 0.001f)
		{
			return fabs(left - right) < epsilon;
		}
};

}

#endif // RECT_H_INCLUDED
