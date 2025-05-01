#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include <iomanip>
#include "Sequence.h"
#include "GreenRayWindow.h"

namespace sa
{

extern const double PI;

class Histogram
{
public:
	bool doGaussianBuilding;

	long width, height;
	long indentLeft, indentRight;
	long indentTop, indentBottom;
	long numberSize, labelSize, graphLabelSize, arrowSize;
	long axisWidth, hatchLength, hatchWidth, lineWidth, gaussianWidth;
	long numberIndentX, numberIndentY, verticalNumberCorrection;
	long precisionX, precisionY;
	long originX, originY;
	long xLabelIndent, yLabelIndent;
	long graphLabelPosX, graphLabelPosY;
	long gaussianSegmentCount;

	double leftBound, rightBound;
	double topBound, bottomBound;
	double scaleDivX, scaleDivY;
	double scaleX, scaleY;

	sf::Color lineColor, fillColor, gaussianColor;
	sf::Font &font;
	sf::RectangleShape xAxis, yAxis;
	sf::CircleShape xArrow, yArrow;
	std::vector <sf::RectangleShape> hatchesX, hatchesY;
	std::vector <sf::Text> numbersX, numbersY;
	std::vector <sf::RectangleShape> boxes;
	std::vector <sf::RectangleShape> gaussian;
	sf::Text xLabel, yLabel, graphLabel, zeroLabel;
	sf::String xLabelString, yLabelString, graphLabelString;
public:
	Histogram(long w, long h, sf::Font &f);

	void setLeftIndent(long li);
	void setRightIndent(long ri);
	void setTopIndent(long ti);
	void setBottomIndent(long bi);
	void setLabelSize(long ts);
	void setArrowSize(long as);
	void setNumberSize(long ns);
	void setGraphLabelSize(long gls);
	void setAxisWidth(long aw);
	void setHatchWidth(long hw);
	void setHatchLength(long hl);
	void setLineWidth(long lw);
	void setXLabel(sf::String xl);
	void setYLabel(sf::String yl);
	void setLeftBound(double lb);
	void setRightBound(double rb);
	void setBottomBound(double bb);
	void setTopBound(double tb);
	void setScaleDivision(double sdX, double sdY);
	void setLineColor(sf::Color lc);
	void setFillColor(sf::Color fc);
	void setNumberIndent(long niX, long niY);
	void setPrecision(long px, long py);
	void setVerticalNumberCorrection(long vnc);
	void setGraphLabel(sf::String gl);
	void setGraphLabelPos(long x, long y);
	void setSize(long w, long h);
	void setGaussianWidth(long gw);
	void setGaussianColor(sf::Color gc);
	void setGaussianSegmentCount(long gsc);
	void setGaussianBuilding(bool gb);

	void drawAxes();
	void drawHistogram(sa::Sequence &s);
	void drawGaussian(sa::Sequence &s, long sc);
	void clear();

	friend gr::Window &operator <<(gr::Window &w, Histogram &h);
	friend std::wofstream &operator <<(std::wofstream &s, Histogram &h);
	friend std::wifstream &operator >>(std::wifstream &s, Histogram &h);
};

std::string str(double, long);
std::wstring formatted(const std::wstring&);
std::wstring disformatted(const std::wstring&);

}