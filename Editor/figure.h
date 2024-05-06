#pragma once
class figure
{
public:
	virtual int getCenterX() const = 0;

	virtual int getCenterY() const = 0;

	virtual void setCenterX(int newCX) const = 0;

	virtual void setCenterY(int newCY) const = 0;

	virtual void setRadX(int newRadX) const = 0;

	virtual void setRadY(int newRadY) const = 0;

	virtual void drawFigure(HDC hDc)const =0;

};



class circle : public figure
{
public:
	circle(int centerX,int centerY,int radX,int radY)
	{
		X = centerX;
		Y = centerY;
		rX = radX;
		rY = radY;
	}

	int getRadX()
	{
		return rX;
	}

	int getRadY()
	{
		return rY;
	}

	void setRadX(int newRadX) const override
	{
		rY = newRadX;
	}

	void setRadY(int newRadY) const override
	{
		rX = newRadY;
	}

	void setCenterX(int newCX) const override
	{
		X = newCX;
	}

	void setCenterY(int newCY) const override
	{
		Y = newCY;
	}

	int getCenterX() const override
	{
		return X;
	}

	int getCenterY() const override
	{
		return Y;
	}

	void drawFigure(HDC hDc) const override
	{

		int x = 0, y = abs(rX);
		int a2 = rY * rY;
		int b2 = rX * rX;
		int delta = 4 * b2 * ((x + 1) * (x + 1)) + a2 * ((2 * y - 1) * (2 * y - 1)) - 4 * a2 * b2;

		while (a2 * (2 * y - 1) > 2 * b2 * (x + 1))
		{
			SetPixel(hDc, X + x, Y + y, color); 
			SetPixel(hDc, X - x, Y + y, color);
			SetPixel(hDc, X + x, Y - y, color);
			SetPixel(hDc, X - x, Y - y, color);

			if (delta < 0)
			{
				delta += 4 * b2 * (2 * x + 3);
			}
			else
			{
				delta = delta - 8 * a2 * (y - 1) + 4 * b2 * (2 * x + 3);
				y--;
			}

			x++;
		}

		delta = b2 * ((2 * x + 1) * (2 * x + 1)) + 4 * a2 * ((y + 1) * (y + 1)) - 4 * a2 * b2;

		while (y + 1 != 0)
		{
			SetPixel(hDc, X + x, Y + y, color);
			SetPixel(hDc, X - x, Y + y, color);
			SetPixel(hDc, X + x, Y - y, color);
			SetPixel(hDc, X - x, Y - y, color);

			if (delta < 0)
			{
				delta += 4 * a2 * (2 * y + 3);
			}
			else
			{
				delta = delta - 8 * b2 * (x + 1) + 4 * a2 * (2 * y + 3);
				x++;
			}

			y--;
		}

	}

private:
	mutable int rX;
	mutable int rY;
	COLORREF color = RGB(255, 0, 0);
	mutable int X;
	mutable int Y;

};


class rectangle : public figure
{
public:
	rectangle(int centerX, int centerY, int width, int height)
	{
		X = centerX;
		Y = centerY;
		w = width;
		h = height;

	};
	
	int getCenterX() const override
{
	return X;
}

	int getCenterY() const override
{
	return Y;
}

	void setRadX(int newRadX) const override
	{
		w = newRadX*2;
	}

	void setRadY(int newRadY) const override
	{
		h = newRadY*2;
	}

	void setCenterX(int newCX) const override
	{
		X = newCX;
	}

	void setCenterY(int newCY) const override
	{
		Y = newCY;
	}

	int getWidth()
	{
		return w;
	}

	int getHeight()
	{
		return h;
	}

	void drawFigure(HDC hDc) const override
	{
		int x0 = int(X - w / 2);
		int x1 = int(X + w / 2);
		int y0 = int(Y - h / 2);
		int y1 = int(Y + h / 2);

		if (x0 > x1)
		{
			int buf = x0;
			x0 = x1;
			x1 = buf;
		}

		if (y0 > y1)
		{
			int buf = y0;
			y0 = y1;
			y1 = buf;
		}

		
		for (int x = x0; x <= x1; ++x) {
			SetPixel(hDc, x, y0,color);
		}

		for (int x = x0; x <= x1; ++x) {
			SetPixel(hDc, x, y1, color);
		}

		for (int y = y0; y <= y1; ++y) {
			SetPixel(hDc, x0, y,color);
		}

		for (int y = y0; y <= y1; ++y) {
			SetPixel(hDc, x1, y,color);
		}



	}

private:
	COLORREF color = RGB(255, 0, 0);
	mutable int X;
	mutable int Y;
	mutable int w;
	mutable int h;

};


class triangle : public figure
{
public:
	triangle(int centerX, int centerY, int length, int height)
	{
		X = centerX;
		Y = centerY;
		l = length;
		h = height;

	}

	int getCenterX() const override
	{
		return X;
	}

	int getCenterY() const override
	{
		return Y;
	}

	void setRadX(int newRadX) const override
	{
		l = newRadX;
	}

	void setRadY(int newRadY) const override
	{
		h = newRadY;
	}

	void setCenterX(int newCX) const override
	{
		X = newCX;
	}

	void setCenterY(int newCY) const override
	{
		Y = newCY;
	}

	void drawFigure(HDC hDc) const override
	{
		int x0 = X;
		int x1 = int(X - l / 2);
		int x2 = int(X + l / 2);
		int y0 = int(Y - h / 2);
		int y1 = int(Y + h / 2);
		int y2 = int(Y + h / 2);
		


		float bufX = x1;
		float bufY = y1;

		int dX = x0 -x1 ;
		int dY = y0- y1 ;
		
		int steps = max(abs(dX), abs(dY));

		float xIncrement = dX / (float)steps;
		float yIncrement = dY / (float)steps;

		for (int i = 0; i < steps; i++)
		{
			SetPixel(hDc, bufX, bufY, color);
			bufX += xIncrement;
			bufY += yIncrement;
		}


		dX = x2 - x0;
		dY = y2 - y0;

		steps = max(abs(dX), abs(dY));

		xIncrement = dX / (float)steps;
		yIncrement = dY / (float)steps;

		for (int i = 0; i < steps; i++)
		{
			SetPixel(hDc, bufX, bufY, color);
			bufX += xIncrement;
			bufY += yIncrement;
		}
		

		if (x2 < x1)
		{
			int buf = x2;
			x2 = x1;
			x1 = buf;
		}
		for (int x = x1; x <= x2; ++x) {
			SetPixel(hDc, x, y1, color);
		}
	}

private:
	COLORREF color = RGB(255, 0, 0);
	mutable int X;
	mutable int Y;
	mutable int l;
	mutable int h;

};



