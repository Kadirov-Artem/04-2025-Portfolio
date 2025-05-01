#include "Histogram.h"

/* Histogram */

const double sa::PI = 3.1415926535897;

sa::Histogram::Histogram(long awidth, long aheight, sf::Font &afont):
	width(awidth), height(aheight), indentLeft(50), indentRight(20), indentTop(20),
	indentBottom(50), labelSize(16), leftBound(-10), rightBound(10), bottomBound(-10),
	topBound(10), scaleDivX(1), scaleDivY(1), axisWidth(1), hatchWidth(1), lineWidth(1),
	hatchLength(7), arrowSize(10), font(afont), gaussianSegmentCount(100),
	numberIndentX(7), numberIndentY(3), precisionX(2), precisionY(2),
	verticalNumberCorrection(3), lineColor(0, 0, 0), fillColor(255, 64, 64), numberSize(16),
	xLabelString(""), yLabelString(""), xLabelIndent(5), yLabelIndent(10), 
	graphLabelPosX(awidth / 2), graphLabelPosY(aheight / 2), graphLabelSize(16),
	graphLabelString(""), gaussianColor(0, 0, 0), gaussianWidth(1),
	doGaussianBuilding(false)
{}

void sa::Histogram::setLeftIndent(long aleftIndent)
{indentLeft = aleftIndent;}

void sa::Histogram::setRightIndent(long arightIndent)
{indentRight = arightIndent;}

void sa::Histogram::setTopIndent(long atopIndent)
{indentTop = atopIndent;}

void sa::Histogram::setBottomIndent(long abottomIndent)
{indentBottom = abottomIndent;}

void sa::Histogram::setLabelSize(long alabelSize)
{labelSize = alabelSize;}

void sa::Histogram::setGraphLabelSize(long gls)
{graphLabelSize = gls;}

void sa::Histogram::setXLabel(sf::String xText)
{xLabelString = xText;}

void sa::Histogram::setYLabel(sf::String yText)
{yLabelString = yText;}

void sa::Histogram::setLeftBound(double lb)
{leftBound = lb;}

void sa::Histogram::setRightBound(double rb)
{rightBound = rb;}

void sa::Histogram::setBottomBound(double bb)
{bottomBound = bb;}

void sa::Histogram::setTopBound(double tb)
{topBound = tb;}

void sa::Histogram::setScaleDivision(double sdX, double sdY)
{
	scaleDivX = sdX;
	scaleDivY = sdY;
}

void sa::Histogram::setAxisWidth(long aw)
{axisWidth = aw;}

void sa::Histogram::setHatchWidth(long hw)
{hatchWidth = hw;}

void sa::Histogram::setLineWidth(long lw)
{lineWidth = lw;}

void sa::Histogram::setLineColor(sf::Color lc)
{lineColor = lc;}

void sa::Histogram::setFillColor(sf::Color fc)
{fillColor = fc;}

void sa::Histogram::setHatchLength(long hl)
{hatchLength = hl;}

void sa::Histogram::setArrowSize(long as)
{arrowSize = as;}

void sa::Histogram::setNumberIndent(long niX, long niY)
{
	numberIndentX = niX;
	numberIndentY = niY;
}

void sa::Histogram::setPrecision(long px, long py)
{
	precisionX = px;
	precisionY = py;
}

void sa::Histogram::setNumberSize(long ns)
{numberSize = ns;}

void sa::Histogram::setGraphLabel(sf::String gl)
{graphLabelString = gl;}

void sa::Histogram::setVerticalNumberCorrection(long vnc)
{verticalNumberCorrection = vnc;}

void sa::Histogram::setGraphLabelPos(long x, long y)
{
	graphLabelPosX = x;
	graphLabelPosY = y;
}

void sa::Histogram::setSize(long w, long h)
{
	width = w;
	height = h;
}

void sa::Histogram::setGaussianWidth(long gw)
{gaussianWidth = gw;}

void sa::Histogram::setGaussianColor(sf::Color gc)
{gaussianColor = gc;}

void sa::Histogram::setGaussianSegmentCount(long gsc)
{gaussianSegmentCount = gsc;}

void sa::Histogram::setGaussianBuilding(bool gb)
{doGaussianBuilding = gb;}

void sa::Histogram::drawAxes()
{
	/* Delete previous hatches and numbers */
	hatchesX.clear();
	hatchesY.clear();
	numbersX.clear();
	numbersY.clear();

	/* Calculate scales (pixels per unit) */
	scaleX = (width - (indentLeft + indentRight)) / (rightBound - leftBound);
	scaleY = (height - (indentBottom + indentTop)) / (topBound - bottomBound);

	/* Find the plot's origin (pxs) */
	originX = indentLeft + (scaleX * -leftBound);
	originY = indentTop + (scaleY * topBound);

	/* X-axis setup */
	xAxis.setSize(sf::Vector2f(width - (indentLeft + indentRight), axisWidth));
	xAxis.setOrigin(sf::Vector2f(0, axisWidth / 2));
	xAxis.setOutlineThickness(0);
	xAxis.setFillColor(sf::Color(0, 0, 0));
	xAxis.setPosition(sf::Vector2f(indentLeft, originY));
	xAxis.setPosition(sf::Vector2f(indentLeft, originY));

	/* Y-axis setup */
	yAxis.setSize(sf::Vector2f(axisWidth, height - (indentBottom + indentTop)));
	yAxis.setOrigin(sf::Vector2f(axisWidth / 2, 0));
	yAxis.setOutlineThickness(0);
	yAxis.setFillColor(sf::Color(0, 0, 0));
	yAxis.setPosition(sf::Vector2f(originX, indentTop));
	
	/* X-arrow setup */
	xArrow.setPointCount(3);
	xArrow.setRadius(arrowSize);
	xArrow.setOrigin(sf::Vector2f(arrowSize, arrowSize));
	xArrow.setOutlineThickness(0);
	xArrow.setFillColor(sf::Color(0, 0, 0));
	xArrow.setRotation(90);
	xArrow.setScale(0.5, 1);
	xArrow.setPosition(sf::Vector2f(width - indentRight, originY));

	/* Y-arrow setup */
	yArrow.setPointCount(3);
	yArrow.setRadius(arrowSize);
	yArrow.setOrigin(sf::Vector2f(arrowSize, arrowSize));
	yArrow.setOutlineThickness(0);
	yArrow.setFillColor(sf::Color(0, 0, 0));
	yArrow.setScale(0.5, 1);
	yArrow.setPosition(sf::Vector2f(originX, indentTop));

	/* Horizontal hatches and numbers setup */
	for (double x = leftBound; x <= rightBound - scaleDivX / 4; x += scaleDivX)
	{
		long hatchPosX = indentLeft + (x - leftBound) * scaleX;

		sf::RectangleShape hatch;
		hatch.setSize(sf::Vector2f(hatchWidth, hatchLength));
		hatch.setOrigin(sf::Vector2f(hatchWidth / 2, hatchLength / 2));
		hatch.setOutlineThickness(0);
		hatch.setFillColor(sf::Color(0, 0, 0));
		hatch.setPosition(hatchPosX, originY);

		hatchesX.push_back(hatch);

		if (abs(x) > scaleDivX / 2)
		{
			sf::Text number;
			number.setCharacterSize(numberSize);
			number.setFillColor(sf::Color(0, 0, 0));
			number.setFont(font);
			number.setString(str(x, precisionX));
			sf::FloatRect gbounds = number.getGlobalBounds();
			number.setOrigin(gbounds.width / 2, 0);
			number.setPosition(hatchPosX, originY + numberIndentY);

			numbersX.push_back(number);
		}
	}

	/* Vertical hatches and numbers setup */
	for (double y = bottomBound; y <= topBound - scaleDivY / 4; y += scaleDivY)
	{
		long hatchPosY = indentTop + (topBound - y) * scaleY;

		sf::RectangleShape hatch;
		hatch.setSize(sf::Vector2f(hatchLength, hatchWidth));
		hatch.setOrigin(sf::Vector2f(hatchLength / 2, hatchWidth / 2));
		hatch.setOutlineThickness(0);
		hatch.setFillColor(sf::Color(0, 0, 0));
		hatch.setPosition(originX, hatchPosY);
		
		hatchesY.push_back(hatch);

		if (abs(y) > scaleDivY / 2)
		{
			sf::Text number;
			number.setCharacterSize(numberSize);
			number.setFillColor(sf::Color(0, 0, 0));
			number.setFont(font);
			number.setString(str(y, precisionY));
			sf::FloatRect gbounds = number.getGlobalBounds();
			number.setOrigin(gbounds.width, gbounds.height / 2);
			number.setPosition(originX - numberIndentX,
							   hatchPosY - verticalNumberCorrection);

			numbersY.push_back(number);
		}
	}

	/* Zero setup */
	zeroLabel.setCharacterSize(numberSize);
	zeroLabel.setFillColor(sf::Color(0, 0, 0));
	zeroLabel.setFont(font);
	zeroLabel.setString("0");
	sf::FloatRect gbounds = zeroLabel.getGlobalBounds();
	zeroLabel.setOrigin(gbounds.width, 0);
	zeroLabel.setPosition(originX - numberIndentX, originY + numberIndentY);

	/* If the horizontal axis is out of bounds */
	if (originY < indentTop)
	{
		xAxis.move(0, indentTop - originY);
		xArrow.move(0, indentTop - originY);

		for (std::vector <sf::RectangleShape>::iterator iter = hatchesX.begin();
			 iter != hatchesX.end(); iter++)
		{
			iter->move(sf::Vector2f(0, indentTop - originY));
		}

		for (std::vector <sf::Text>::iterator iter = numbersX.begin();
			 iter != numbersX.end(); iter++)
		{
			iter->move(sf::Vector2f(0, indentTop - originY));
		}
	}
	else if (originY > height - indentBottom)
	{
		xAxis.move(0, (height - indentBottom) - originY);
		xArrow.move(0, (height - indentBottom) - originY);

		for (std::vector <sf::RectangleShape>::iterator iter = hatchesX.begin();
			 iter != hatchesX.end(); iter++)
		{
			iter->move(sf::Vector2f(0, (height - indentBottom) - originY));
		}

		for (std::vector <sf::Text>::iterator iter = numbersX.begin();
			 iter != numbersX.end(); iter++)
		{
			iter->move(sf::Vector2f(0, (height - indentBottom) - originY));
		}
	}

	/* If the vertical axis is out of bounds */
	if (originX < indentLeft)
	{
		yAxis.move(indentLeft - originX, 0);
		yArrow.move(indentLeft - originX, 0);

		for (std::vector <sf::RectangleShape>::iterator iter = hatchesY.begin();
			 iter != hatchesY.end(); iter++)
		{
			iter->move(sf::Vector2f(indentLeft - originX, 0));
		}

		for (std::vector <sf::Text>::iterator iter = numbersY.begin();
			 iter != numbersY.end(); iter++)
		{
			iter->move(sf::Vector2f(indentLeft - originX, 0));
		}
	}
	else if (originX > width - indentRight)
	{
		yAxis.move((width - indentRight) - originX, 0);
		yArrow.move((width - indentRight) - originX, 0);

		for (std::vector <sf::RectangleShape>::iterator iter = hatchesY.begin();
			 iter != hatchesY.end(); iter++)
		{
			iter->move(sf::Vector2f((width - indentRight) - originX, 0));
		}

		for (std::vector <sf::Text>::iterator iter = numbersY.begin();
			 iter != numbersY.end(); iter++)
		{
			iter->move(sf::Vector2f((width - indentRight) - originX, 0));
		}
	}

	/* X-label setup */
	xLabel.setCharacterSize(labelSize);
	xLabel.setFillColor(sf::Color(0, 0, 0));
	xLabel.setFont(font);
	xLabel.setString(xLabelString);
	gbounds = xLabel.getGlobalBounds();
	xLabel.setOrigin(gbounds.width, 0);

	/* Get x-numbers' height */
	if (numbersX.size())
	{
		gbounds = numbersX[0].getGlobalBounds();
		xLabel.setPosition(width - indentRight,
						   xAxis.getPosition().y + numberIndentY + gbounds.height + xLabelIndent);
	}
	else
	{
		xLabel.setPosition(width - indentRight,
						   xAxis.getPosition().y + xLabelIndent);
	}

	/* Y-label setup */
	yLabel.setRotation(0);
	yLabel.setCharacterSize(labelSize);
	yLabel.setFillColor(sf::Color(0, 0, 0));
	yLabel.setFont(font);
	yLabel.setString(yLabelString);
	gbounds = yLabel.getGlobalBounds();
	yLabel.setOrigin(gbounds.width, gbounds.height);

	/* Get y-numbers' width */
	if (numbersY.size())
	{
		gbounds = numbersY[0].getGlobalBounds();
		yLabel.setPosition(yAxis.getPosition().x - numberIndentX - gbounds.width - yLabelIndent,
						   indentTop);
	}
	else
	{
		yLabel.setPosition(yAxis.getPosition().x - yLabelIndent,
						   indentTop);
	}

	yLabel.setRotation(-90);

	/* Graph label setup */
	graphLabel.setCharacterSize(graphLabelSize);
	graphLabel.setFillColor(sf::Color(0, 0, 0));
	graphLabel.setFont(font);
	graphLabel.setString(graphLabelString);
	graphLabel.setPosition(graphLabelPosX, graphLabelPosY);
}

gr::Window &sa::operator <<(gr::Window &window, sa::Histogram &histogram)
{
	for (std::vector <sf::RectangleShape>::iterator iter = histogram.boxes.begin();
		 iter != histogram.boxes.end(); iter++)
	{
		window << *iter;
	}

	for (std::vector <sf::RectangleShape>::iterator iter = histogram.gaussian.begin();
		 iter != histogram.gaussian.end(); iter++)
	{
		window << *iter;
	}

	window << histogram.xAxis << histogram.yAxis << histogram.xArrow << histogram.yArrow <<
		histogram.xLabel << histogram.yLabel << histogram.zeroLabel << histogram.graphLabel;

	for (std::vector <sf::RectangleShape>::iterator iter = histogram.hatchesX.begin();
		 iter != histogram.hatchesX.end(); iter++)
	{
		window << *iter;
	}

	for (std::vector <sf::RectangleShape>::iterator iter = histogram.hatchesY.begin();
		 iter != histogram.hatchesY.end(); iter++)
	{
		window << *iter;
	}

	for (std::vector <sf::Text>::iterator iter = histogram.numbersX.begin();
		 iter != histogram.numbersX.end(); iter++)
	{
		window << *iter;
	}

	for (std::vector <sf::Text>::iterator iter = histogram.numbersY.begin();
		 iter != histogram.numbersY.end(); iter++)
	{
		window << *iter;
	}

	return window;
}

void sa::Histogram::drawHistogram(sa::Sequence &seq)
{
	/* Draw axes and, if necessary, a gaussian */
	drawAxes();
	
	if (doGaussianBuilding)
	{
		drawGaussian(seq, gaussianSegmentCount);
	}
	else
	{
		gaussian.clear();
	}

	/* Draw histogram */
	boxes.clear();

	long n = round(sqrt(seq.getCount()));  // Box count

	/* If the plot is empty */
	if (seq.getCount() < 2)
	{
		return;
	}

	double min = seq.getMin();
	double max = seq.getMax();
	std::vector <long> valueDistribution;

	double deltaX = (max - min) / n;  // Range width
	
	/* For each range count the number of values inside */
	long i = 0;
	for (double x = min; i < n; x = min + deltaX * i)
	{
		long deltaN = 0;

		for (long j = 0; j < seq.size(); j++)
		{
			/* If the value is inside the range */
			if (seq[j] >= x && seq[j] < x + deltaX)
			{
				deltaN++;
			}
		}

		valueDistribution.push_back(deltaN);
		i++;
	}
	
	/* The max value is on the right bound of the right box and
	is not included into the range because of strict inequality.
	We have to include this value into the range manually. */
	valueDistribution[n - 1]++;

	/* Setup boxes */
	for (long i = 0; i < n; i++)
	{
		sf::RectangleShape box;
		box.setSize(sf::Vector2f(deltaX * scaleX,
					valueDistribution[i] / (seq.size() * deltaX) * scaleY));
		box.setOrigin(0, box.getSize().y);
		box.setOutlineThickness(lineWidth);
		box.setOutlineColor(lineColor);
		box.setFillColor(fillColor);
		box.setPosition(originX + (min + deltaX * i) * scaleX, originY);
		
		boxes.push_back(box);
	}
}

void sa::Histogram::drawGaussian(sa::Sequence &seq, long segmentCount)
{
	// If a sequence is empty
	if (seq.getCount() < 2)
		return;

	gaussian.clear();

	double deltaX = (rightBound - leftBound) / segmentCount;  // Segment width
	double pixDX = deltaX * scaleX;  // Segment width in pixels
	double nxtX = leftBound + deltaX;  // Next argument
	double avg = seq.getAverage();
	double dev = seq.getNormalDeviation();
	double curY = exp( -((leftBound - avg) * (leftBound - avg)) / (2 * dev * dev) ) / 
		(dev * sqrt(2 * PI));  // Current function

	for (long i = 0; i < segmentCount; i++, nxtX += deltaX)
	{
		// Calculate the next function value
		double nxtY = exp( -((nxtX - avg) * (nxtX - avg)) / (2 * dev * dev) ) / 
			(dev * sqrt(2 * PI));

		// Use rectangle as a line (height = 0)
		double pixDY = (nxtY - curY) * scaleY;  // Function difference in pixels
		sf::RectangleShape line(sf::Vector2f( sqrt(pixDX * pixDX + pixDY * pixDY), 0 ));
		line.setOutlineThickness(gaussianWidth);
		line.setOutlineColor(gaussianColor);
		double angle = -atan(pixDY/pixDX) * 180 / PI;
		line.setRotation(angle);
		line.setPosition(originX + (nxtX - deltaX) * scaleX, originY - curY * scaleY);

		// Save the rectangle to the vector
		gaussian.push_back(line);

		// Update curY
		curY = nxtY;
	}
}

std::wofstream &sa::operator <<(std::wofstream &s, sa::Histogram &h)
{
	std::wstring xlstr, ylstr, glstr;

	/* Insert settings into the stream */
	s << "width " << h.width << '\n' <<
		"height " << h.height << '\n' <<
		"indentLeft " << h.indentLeft << '\n' <<
		"indentRight " << h.indentRight << '\n' <<
		"indentTop " << h.indentTop << '\n' <<
		"indentBottom " << h.indentBottom << '\n' <<
		"numberSize " << h.numberSize << '\n' <<
		"labelSize " << h.labelSize << '\n' <<
		"graphLabelSize " << h.graphLabelSize << '\n' <<
		"arrowSize " << h.arrowSize << '\n' <<
		"axisWidth " << h.axisWidth<< '\n' <<
		"hatchLength " << h.hatchLength << '\n' <<
		"hatchWidth " << h.hatchWidth << '\n' <<
		"lineWidth " << h.lineWidth << '\n' <<
		"numberIndentX " << h.numberIndentX << '\n' <<
		"numberIndentY " << h.numberIndentY << '\n' <<
		"verticalNumberCorrection " << h.verticalNumberCorrection << '\n' <<
		"precisionX " << h.precisionX << '\n' <<
		"precisionY " << h.precisionY << '\n' <<
		"xLabelIndent " << h.xLabelIndent << '\n' <<
		"yLabelIndent " << h.yLabelIndent << '\n' <<
		"graphLabelPosX " << h.graphLabelPosX << '\n' <<
		"graphLabelPosY " << h.graphLabelPosY << '\n' <<
		"leftBound " << h.leftBound << '\n' <<
		"rightBound " << h.rightBound << '\n' <<
		"bottomBound " << h.bottomBound << '\n' <<
		"topBound " << h.topBound << '\n' <<
		"scaleDivX " << h.scaleDivX << '\n' <<
		"scaleDivY " << h.scaleDivY << '\n' <<
		"fillColorR " << h.fillColor.r << '\n' <<
		"fillColorG " << h.fillColor.g << '\n' <<
		"fillColorB " << h.fillColor.b << '\n' <<
		"lineColorR " << h.lineColor.r << '\n' <<
		"lineColorG " << h.lineColor.g << '\n' <<
		"lineColorB " << h.lineColor.b << '\n' <<
		"doGaussianBuilding " << h.doGaussianBuilding << '\n' <<
		"gaussianSegmentCount " << h.gaussianSegmentCount << '\n';

	return s;
}

std::wifstream &sa::operator >>(std::wifstream &s, sa::Histogram &h)
{
	std::wstring useless;
	int fillR, fillG, fillB, lineR, lineG, lineB;


	/* Get settings from the stream */
	s >> useless >> h.width >>
		useless >> h.height >>
		useless >> h.indentLeft >>
		useless >> h.indentRight >>
		useless >> h.indentTop >>
		useless >> h.indentBottom >>
		useless >> h.numberSize >>
		useless >> h.labelSize >>
		useless >> h.graphLabelSize >>
		useless >> h.arrowSize  >>
		useless >> h.axisWidth >>
		useless >> h.hatchLength >>
		useless >> h.hatchWidth >>
		useless >> h.lineWidth >>
		useless >> h.numberIndentX >>
		useless >> h.numberIndentY >>
		useless >> h.verticalNumberCorrection >>
		useless >> h.precisionX >>
		useless >> h.precisionY >>
		useless >> h.xLabelIndent >>
		useless >> h.yLabelIndent >>
		useless >> h.graphLabelPosX  >>
		useless >> h.graphLabelPosY >>
		useless >> h.leftBound >>
		useless >> h.rightBound >>
		useless >> h.bottomBound >>
		useless >> h.topBound >>
		useless >> h.scaleDivX >>
		useless >> h.scaleDivY >>
		useless >> fillR >>
		useless >> fillG >>
		useless >> fillB >>
		useless >> lineR >>
		useless >> lineG >>
		useless >> lineB >>
		useless >> h.doGaussianBuilding >>
		useless >> h.gaussianSegmentCount;

	/* Set colors */
	h.fillColor.r = fillR;
	h.fillColor.g = fillG;
	h.fillColor.b = fillB;
	h.lineColor.r = lineR;
	h.lineColor.g = lineG;
	h.lineColor.b = lineB;

	return s;
}

void sa::Histogram::clear()
{
	/* Delete boxes, gaussian, numbers and hatches */
	boxes.clear();
	gaussian.clear();
	numbersX.clear();
	numbersY.clear();
	hatchesX.clear();
	hatchesY.clear();

	/* Reset settings to default */
	indentLeft = 50;
	indentRight = 20;
	indentTop = 20;
	indentBottom = 50;
	labelSize = 16;
	leftBound = -10;
	rightBound = 10;
	bottomBound = -10;
	topBound = 10;
	scaleDivX = 1;
	scaleDivY = 1;
	axisWidth = 1;
	hatchWidth = 1;
	lineWidth = 1;
	hatchLength = 7;
	arrowSize = 10;
	numberIndentX = 7;
	numberIndentY = 3;
	precisionX = 2;
	precisionY = 2;
	verticalNumberCorrection = 3;
	lineColor = sf::Color(0, 0, 0);
	fillColor = sf::Color(255, 64, 64);
	numberSize = 16;
	xLabelString = "";
	yLabelString = "";
	graphLabelString = "";
	xLabelIndent = 5;
	yLabelIndent = 10;
	graphLabelPosX = width / 2;
	graphLabelPosY = height / 2;
	graphLabelSize = 16;
	doGaussianBuilding = false;
	gaussianSegmentCount = 100;
	gaussianWidth = 1;
	gaussianColor = sf::Color(0, 0, 0);
}

/* Other */

std::string sa::str(double arg, long precision)
{
	std::ostringstream mem;
	mem.setf(std::ios::fixed);
	mem << std::setprecision(precision) << arg;
	return mem.str();
}

std::wstring sa::formatted(const std::wstring &arg)
{
	std::wstring str = arg;

	/* Insert '\n' symbols */
	while (str.find(L'\\') != -1)
	{
		long pos = str.find(L'\\');

		if (pos + 1 < str.size())
		{
			if (str.substr(pos + 1, 1) == L"n")
			{
				str.replace(pos, 2, L"\n");
			}
			else
			{
				str.erase(pos, 2);
			}
		}
		else
		{
			str.erase(pos, 1);
		}
	}

	return str;
}

std::wstring sa::disformatted(const std::wstring &arg)
{
	std::wstring str = arg;

	/* Replace '\n' with "\\n" */
	while (str.find(L'\n') != -1)
	{
		long pos = str.find(L'\n');
		str.replace(pos, 1, L"\\n");
	}

	return str;
}