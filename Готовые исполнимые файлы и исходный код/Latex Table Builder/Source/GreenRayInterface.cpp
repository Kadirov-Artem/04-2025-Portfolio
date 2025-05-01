#include "GreenRayInterface.h"

/* Interactive */

long gr::Interactive::existingObjectsNum = 0;
long gr::Interactive::targetedObjectId = -1;

gr::Interactive::Interactive():
	xPos(0), yPos(0), width(100), height(100), enabled(true), id(existingObjectsNum++)
{}

gr::Interactive::Interactive(long axPos, long ayPos, long awidth, long aheight):
	xPos(axPos), yPos(ayPos), width(awidth), height(aheight), enabled(true), 
	id(existingObjectsNum++)
{}

gr::Interactive::~Interactive()
{existingObjectsNum--;}

void gr::Interactive::enable()
{enabled = true;}

void gr::Interactive::disable()
{enabled = false;}

void gr::Interactive::reset()
{targetedObjectId = -1;}

/* Colored */
gr::Colored::Colored(sf::Color abackgroundColor,
					 sf::Color aborderColor, sf::Color atextColor):
	backgroundColor(abackgroundColor), borderColor(aborderColor), textColor(atextColor)
{}

void gr::Colored::setBackgroundColor(sf::Color abackgroundColor)
{backgroundColor = abackgroundColor;}

void gr::Colored::setBorderColor(sf::Color aborderColor)
{borderColor = aborderColor;}

void gr::Colored::setTextColor(sf::Color atextColor)
{textColor = atextColor;}

/* Button */

const long gr::Button::SHADOW_OFFSET = 8;
const long gr::Button::BORDER_THICKNESS = 1;
const float gr::Button::FADE_TIME = 300;
const float gr::Button::ANIMATION_TIME = 80;
sf::Clock gr::Button::moveClock;
const float gr::Button::PI = 3.141;

void gr::Button::setup()
{
	/* Text setup */

	text.setFillColor(textColor);
	text.setCharacterSize(height * 0.8);
	textBounds = text.getGlobalBounds();
	text.setPosition(xPos + width / 2 - textBounds.width / 2, yPos);

	/* Main button rectangle setup */
	main.setSize(sf::Vector2f(width, height));
	main.setFillColor(backgroundColor);
	main.setOutlineColor(borderColor);
	main.setOutlineThickness(BORDER_THICKNESS);
	main.setPosition(xPos, yPos);

	/* Gradient initialization */
	mainGradient[0] = sf::Vertex(sf::Vector2f(xPos, yPos),
								 sf::Color(255, 255, 255, 128));
	mainGradient[1] = sf::Vertex(sf::Vector2f(xPos + width - 1, yPos),
								 sf::Color(255, 255, 255, 128));
	mainGradient[2] = sf::Vertex(sf::Vector2f(xPos + width - 1, yPos + height - 1),
								 sf::Color(255, 255, 255, 0));
	mainGradient[3] = sf::Vertex(sf::Vector2f(xPos, yPos + height - 1),
								 sf::Color(255, 255, 255, 0));

	long sxPos = xPos + SHADOW_OFFSET, syPos = yPos + SHADOW_OFFSET;

	shadowGradient[0] = sf::Vertex(sf::Vector2f(sxPos, syPos),
								   sf::Color(0, 0, 0, 0));
	shadowGradient[1] = sf::Vertex(sf::Vector2f(sxPos + width - 1, syPos),
								   sf::Color(0, 0, 0, 128));
	shadowGradient[2] = sf::Vertex(sf::Vector2f(sxPos + width - 1, syPos + height - 1),
								   sf::Color(0, 0, 0, 255));
	shadowGradient[3] = sf::Vertex(sf::Vector2f(sxPos, syPos + height - 1),
								   sf::Color(0, 0, 0, 128));
}

gr::Button::Button(long axPos, long ayPos, long awidth, long aheight,
				   sf::String atext, sf::Font &afont,
				   sf::Color abackgroundColor, sf::Color aborderColor,
				   sf::Color atextColor, sf::Color aselectColor):
	Interactive(axPos, ayPos, awidth, aheight),
	Colored(abackgroundColor, aborderColor, atextColor), selectColor(aselectColor),
	isPressed(false), isTriggered(false), text(atext, afont), isMoving(false),
	isFading(false), isTargeted(false), isMisclicked(false), isKeyBound(false),
	isBoundKeyPressed(false), keyTriggeringPhase(0)
{setup();}

gr::Button::Button(long axPos, long ayPos, long awidth, long aheight,
				   sf::String atext, sf::Font &afont):
	Interactive(axPos, ayPos, awidth, aheight),
	Colored(sf::Color(200, 200, 200), sf::Color(64, 64, 64), sf::Color(0, 0, 0)),
	selectColor(41, 235, 56), isPressed(false), isTriggered(false), text(atext, afont),
	isMoving(false), isFading(false), isTargeted(false), isMisclicked(false),
	isKeyBound(false), isBoundKeyPressed(false), keyTriggeringPhase(0)
{setup();}

gr::Button::Button(Button &right):
	Interactive(right.xPos, right.yPos, right.width, right.height),
	Colored(right.backgroundColor, right.borderColor, right.textColor),
	text(right.text), selectColor(right.selectColor), main(right.main),
	isPressed(false), isTriggered(false), isMoving(false), isFading(false), isMisclicked(false), isTargeted(false),
	isKeyBound(false), isBoundKeyPressed(false), keyTriggeringPhase(0)
{
	for (long i = 0; i < 4; i++)
	{
		mainGradient[i] = right.mainGradient[i];
		shadowGradient[i] = right.shadowGradient[i];
	}
}

void gr::Button::render(sf::RenderWindow &window) const
{
	window.draw(shadowGradient, 4, sf::Quads);
	window.draw(main);
	window.draw(mainGradient, 4, sf::Quads);
	window.draw(text);
}

void gr::Button::refresh(sf::RenderWindow &window)
{
	/* We must reset the
	click trigger */
	isTriggered = false;

	sf::Vector2i curPos = sf::Mouse::getPosition(window);

	/* If the button is enabled */
	if (enabled)
	{
		if(!isPressed)
		{
			/* If this object is targeted or none targeted */
			if (targetedObjectId == id || targetedObjectId == -1)
			{
				/* If the cursor is on the button */
				if(curPos.x >= xPos && curPos.x <= xPos + width - 1 &&
				   curPos.y >= yPos && curPos.y <= yPos + height - 1)
				{
					targetedObjectId = id;

					if (!isTargeted)
					{
						isTargeted = true;
						isFading = true;
						fadeClock.restart();
					}
	
					/* If the left mouse key is pressed on
					the button or a bound key is pressed */
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMisclicked)
					{
						isPressed = true;
						isMoving = true;
						moveClock.restart();
					}
					/* If the left mouse key is released */
					if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						isMisclicked = false;
					}
				}
				/* If the cursor is away from the button */
				else
				{
					targetedObjectId = -1;

					if (isTargeted)
					{
						isTargeted = false;
						isFading = true;
						fadeClock.restart();
					}
	
					isMisclicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
				}
			}
			/* If another object is targeted */
			else
			{
				isMisclicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
			}

			/* If a bound key is pressed */
			if (isKeyBound && sf::Keyboard::isKeyPressed(boundKey))
			{
				if (!isBoundKeyPressed)
				{
					isTriggered = true;
					isBoundKeyPressed = true;
					keyTriggeringPhase = 1;  // Phase 1 = the button is being pressed
				}
			}
			/* If a bound key released */
			else
			{
				isBoundKeyPressed = false;
			}
		}
		else
		{
			/* If the cursor is not on the button, reset it.
			Notice that the button trigger zone is changed
			because of the main rectangle offset. */
			if(curPos.x < xPos || curPos.x > xPos + width - 1 + SHADOW_OFFSET ||
			   curPos.y < yPos || curPos.y > yPos + height - 1 + SHADOW_OFFSET)
			{
				targetedObjectId = -1;

				/* If the button is not triggered by a key
				but is triggered by the mouse cursor */
				if (!keyTriggeringPhase)
				{
					isPressed = false;
					isMoving = true;
					moveClock.restart();
				}
	
				if (isTargeted)
				{
					isTargeted = false;
					isFading = true;
					fadeClock.restart();
				}	
			}
			/* If the left mouse key is not pressed, reset it
			and activate the click trigger */
			else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				/* If the button is not triggered by a key
				but is triggered by the mouse cursor */
				if (!keyTriggeringPhase)
				{
					isPressed = false;
					isMoving = true;
					moveClock.restart();
	
					/* We must set the value of
					the	click trigger to true */
					isTriggered = true;
				}
			}
		}

		/* Fade stuff */
		long fadeElapsedTime = fadeClock.getElapsedTime().asMilliseconds();

		/* Check wether the fade is over */
		if (fadeElapsedTime > FADE_TIME)
			isFading = false;

		/* If fading is over */
		if (!isFading)
			main.setFillColor(isTargeted ? selectColor : backgroundColor);
		/* If fading is still running */
		else
		{
			/* If the cursor is on the button */
			if (isTargeted)
			{
				main.setFillColor(blend(backgroundColor, selectColor,
								  fadeElapsedTime / FADE_TIME));
			}
			/* If the cursor is away from button */
			else
			{
				main.setFillColor(blend(selectColor, backgroundColor, 
								  fadeElapsedTime / FADE_TIME));
			}
		}
	}
	/* If the button is disabled and none of the objects are targeted */
	else if (targetedObjectId == -1)
	{
		main.setFillColor(backgroundColor);
		sf::Cursor cursor;

		if (curPos.x >= xPos && curPos.x <= xPos + width - 1 &&
			curPos.y >= yPos && curPos.y <= yPos + height - 1)
		{
			if (!isTargeted)
			{
				isTargeted = true;
				cursor.loadFromSystem(sf::Cursor::NotAllowed);
				window.setMouseCursor(cursor);
			}
		}
		else
		{
			if (isTargeted)
			{
				isTargeted = false;
				cursor.loadFromSystem(sf::Cursor::Arrow);
				window.setMouseCursor(cursor);
			}
		}
	}

	/* Key-triggered button animation */
	switch (keyTriggeringPhase)
	{
		case 1: // The button is being pressed
			if (!isPressed)
			{
				isPressed = true;
				isMoving = true;
				moveClock.restart();
			}
			break;
		case 2: // The button is being released
			if (isPressed)
			{
				isPressed = false;
				isMoving = true;
				moveClock.restart();
			}
			break;
		default:  // The button has been released
			keyTriggeringPhase = 0;
			break;
	}

	/* Animation stuff */
	long moveElapsedTime = moveClock.getElapsedTime().asMilliseconds();

	/* Check wether the animation is over */
	if (moveElapsedTime > ANIMATION_TIME)
	{
		isMoving = false;

		/* If the button is triggered by a key */
		if (keyTriggeringPhase)
		{
			keyTriggeringPhase++;
		}
	}

	/* If animation is over */
	if (!isMoving)
	{
		if (isPressed)
		{
			main.setPosition(xPos + SHADOW_OFFSET, yPos + SHADOW_OFFSET);
			text.setPosition(xPos + width / 2 - textBounds.width / 2 + SHADOW_OFFSET,
							 yPos + SHADOW_OFFSET);

			mainGradient[0].position = sf::Vector2f(xPos + SHADOW_OFFSET,
													yPos + SHADOW_OFFSET);
			mainGradient[1].position = sf::Vector2f(xPos + width - 1 + SHADOW_OFFSET,
													yPos + SHADOW_OFFSET);
			mainGradient[2].position = sf::Vector2f(xPos + width - 1 + SHADOW_OFFSET,
													yPos + height - 1 + SHADOW_OFFSET);
			mainGradient[3].position = sf::Vector2f(xPos + SHADOW_OFFSET,
													yPos + height - 1 + SHADOW_OFFSET);
			
		}
		else
		{
			main.setPosition(xPos, yPos);
			text.setPosition(xPos + width / 2 - textBounds.width / 2, yPos);

			mainGradient[0].position = sf::Vector2f(xPos, yPos);
			mainGradient[1].position = sf::Vector2f(xPos + width - 1, yPos);
			mainGradient[2].position = sf::Vector2f(xPos + width - 1, yPos + height - 1);
			mainGradient[3].position = sf::Vector2f(xPos, yPos + height - 1);
		}
	}
	/* If animation is still running */
	else
	{
		/* Calculate offset */
		long offset =  SHADOW_OFFSET * std::sin(PI / 2 * moveElapsedTime / ANIMATION_TIME);

		if (isPressed)
		{
			main.setPosition(xPos + offset, yPos + offset);
			text.setPosition(xPos + width / 2 - textBounds.width / 2 + offset, yPos + offset);

			mainGradient[0].position = sf::Vector2f(xPos + offset,
													yPos + offset);
			mainGradient[1].position = sf::Vector2f(xPos + width - 1 + offset,
													yPos + offset);
			mainGradient[2].position = sf::Vector2f(xPos + width - 1 + offset,
													yPos + height - 1 + offset);
			mainGradient[3].position = sf::Vector2f(xPos + offset,
													yPos + height - 1 + offset);

		}
		else
		{
			main.setPosition(xPos + SHADOW_OFFSET - offset, yPos + SHADOW_OFFSET - offset);
			text.setPosition(xPos + width / 2 - textBounds.width / 2 + SHADOW_OFFSET - offset,
							 yPos + SHADOW_OFFSET - offset);

			mainGradient[0].position = sf::Vector2f(xPos + SHADOW_OFFSET - offset,
													yPos + SHADOW_OFFSET - offset);
			mainGradient[1].position = sf::Vector2f(xPos + width - 1 + SHADOW_OFFSET - offset,
													yPos + SHADOW_OFFSET - offset);
			mainGradient[2].position = sf::Vector2f(xPos + width - 1 + SHADOW_OFFSET - offset,
													yPos + height - 1 + SHADOW_OFFSET - offset);
			mainGradient[3].position = sf::Vector2f(xPos + SHADOW_OFFSET - offset,
													yPos + height - 1 + SHADOW_OFFSET - offset);
		}
	}
}

gr::Interactive *gr::Button::clone()
{return new Button(*this);}

void gr::Button::bindKey(sf::Keyboard::Key aboundKey)
{
	isKeyBound = true;
	boundKey = aboundKey;
}

void gr::Button::unbindKey()
{isKeyBound = false;}

void gr::Button::setSelectColor(sf::Color aselectColor)
{selectColor = aselectColor;}

void gr::Button::setBorderColor(sf::Color aborderColor)
{
	Colored::setBorderColor(aborderColor);
	main.setOutlineColor(borderColor);
}

void gr::Button::setTextColor(sf::Color atextColor)
{
	Colored::setTextColor(atextColor);
	text.setFillColor(textColor);
}

void gr::Button::reset()
{
	Interactive::reset();
	isTriggered = false;
	isPressed = false;
	isMoving = false;
	isFading = false;
	isMisclicked = false;
	isBoundKeyPressed = false;
	keyTriggeringPhase = 0;
}

/* Checkbox */

const long gr::Checkbox::BORDER_THICKNESS = 1;
const float gr::Checkbox::SQUARE_RELATIVE_SIZE = 0.7;
const long gr::Checkbox::TEXT_INDENT = 3;
const float gr::Checkbox::FADE_TIME = 100;

void gr::Checkbox::setup()
{
	/* Text setup */
	text.setFillColor(textColor);
	text.setCharacterSize(height);
	text.setPosition(xPos + width + TEXT_INDENT, yPos);

	/* Main checkbox rectangle setup */
	main.setSize(sf::Vector2f(width, height));
	main.setFillColor(backgroundColor);
	main.setOutlineColor(borderColor);
	main.setOutlineThickness(BORDER_THICKNESS);
	main.setPosition(xPos, yPos);

	/* Square shape setup */
	long sxPos = xPos + width * (1 - SQUARE_RELATIVE_SIZE) / 2;
	long syPos = yPos + height * (1 - SQUARE_RELATIVE_SIZE) / 2;

	square.setSize(sf::Vector2f(width * SQUARE_RELATIVE_SIZE, height * SQUARE_RELATIVE_SIZE));
	square.setFillColor(squareColor);
	square.setOutlineThickness(0);
	square.setPosition(sxPos, syPos);

	/* Gradient initialization */
	mainGradient[0] = sf::Vertex(sf::Vector2f(xPos, yPos),
								 sf::Color(255, 255, 255, 128));
	mainGradient[1] = sf::Vertex(sf::Vector2f(xPos + width - 1, yPos),
								 sf::Color(255, 255, 255, 128));
	mainGradient[2] = sf::Vertex(sf::Vector2f(xPos + width - 1, yPos + height - 1),
								 sf::Color(255, 255, 255, 0));
	mainGradient[3] = sf::Vertex(sf::Vector2f(xPos, yPos + height - 1),
								 sf::Color(255, 255, 255, 0));

	squareGradient[0] = sf::Vertex(sf::Vector2f(sxPos, syPos),
								   sf::Color(255, 255, 255, 0));
	squareGradient[1] = sf::Vertex(sf::Vector2f(sxPos + width * SQUARE_RELATIVE_SIZE - 1, syPos),
								   sf::Color(255, 255, 255, 0));
	squareGradient[2] = sf::Vertex(sf::Vector2f(sxPos + width * SQUARE_RELATIVE_SIZE - 1,
								   syPos + height * SQUARE_RELATIVE_SIZE - 1),
								   sf::Color(255, 255, 255, 128));
	squareGradient[3] = sf::Vertex(sf::Vector2f(sxPos, syPos + height * SQUARE_RELATIVE_SIZE - 1),
								   sf::Color(255, 255, 255, 128));
}

gr::Checkbox::Checkbox(long axPos, long ayPos, long asize, sf::String atext, sf::Font &afont,
					   sf::Color abackgroundColor, sf::Color aborderColor,
					   sf::Color atextColor, sf::Color asquareColor):
	Interactive(axPos, ayPos, asize, asize),
	Colored(abackgroundColor, aborderColor, atextColor), text(atext, afont),
	isFading(false), isMisclicked(false), squareColor(asquareColor), state(false),
	isPressed(false), isTargeted(false)
{setup();}

gr::Checkbox::Checkbox(long axPos, long ayPos, long asize, sf::String atext, sf::Font &afont):
	Interactive(axPos, ayPos, asize, asize), 
	Colored(sf::Color(128, 128, 128), sf::Color(64, 64, 64), sf::Color(0, 0, 0)),
	text(atext, afont), isFading(false), isMisclicked(false), squareColor(41, 235, 56),
	state(false), isPressed(false), isTargeted(false)
{setup();}

gr::Checkbox::Checkbox(Checkbox &right):
	Interactive(right.xPos, right.yPos, right.width, right.height),
	Colored(right.backgroundColor, right.borderColor, right.textColor),
	text(right.text), squareColor(right.squareColor), main(right.main),
	square(right.square), isPressed(right.isPressed), state(right.state),
	isFading(false), isMisclicked(false), isTargeted(false)
{
	for (long i = 0; i < 4; i++)
	{
		mainGradient[i] = right.mainGradient[i];
		squareGradient[i] = right.squareGradient[i];
	}
}

void gr::Checkbox::refresh(sf::RenderWindow &window)
{
	sf::Vector2i curPos = sf::Mouse::getPosition(window);

	/* If checkbox is enabled */
	if (enabled)
	{	
		if(!isPressed)
		{
			/* If this object is targeted or none targeted */
			if (targetedObjectId == id || targetedObjectId == -1)
			{
				/* If the cursor is on the checkbox */
				if(curPos.x >= xPos && curPos.x <= xPos + width - 1 &&
				   curPos.y >= yPos && curPos.y <= yPos + height - 1)
				{
					targetedObjectId = id;

					/* If the left mouse key is pressed on the checkbox */
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMisclicked)
					{
					    isPressed = true;
					}
				
					/* If the left mouse key is released on the checkbox */
					if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						isMisclicked = false;
					}
				}
				/* If the cursor is away from the checkbox */
				else
				{
					targetedObjectId = -1;
					isMisclicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
				}
			}
			else
			{
				isMisclicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
			}
		}	
		else
		{
			/* If the cursor is not on the checkbox */
			if(curPos.x < xPos || curPos.x > xPos + width - 1 ||
			   curPos.y < yPos || curPos.y > yPos + height - 1)
			{
				targetedObjectId = -1;
				isPressed = false;
			}
			/* If the left mouse key is released on the box */
			else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				isPressed = false;
				state = !state;
				isFading = true;
				fadeClock.restart();
			}
		}
	}
	/* If the checkbox is disabled and none of the objects are targeted */
	else if (targetedObjectId == -1)
	{
		sf::Cursor cursor;

		if (curPos.x >= xPos && curPos.x <= xPos + width - 1 &&
			curPos.y >= yPos && curPos.y <= yPos + height - 1)
		{
			if (!isTargeted)
			{
				isTargeted = true;
				cursor.loadFromSystem(sf::Cursor::NotAllowed);
				window.setMouseCursor(cursor);
			}
		}
		else
		{
			if (isTargeted)
			{
				isTargeted = false;
				cursor.loadFromSystem(sf::Cursor::Arrow);
				window.setMouseCursor(cursor);
			}
		}
	}

	/* Fading stuff */

	/* Check if the fading is over */
	if (fadeClock.getElapsedTime().asMilliseconds() > FADE_TIME)
	{
		isFading = false;
	}

	/* If fading is over */
	if (!isFading)
	{
		/* If checkbox is active */
		if (state)
		{
			square.setFillColor(squareColor);
		}
		else
		{
			square.setFillColor(backgroundColor);
		}
	}
	/* If fading is still running */
	else
	{
		float progress = fadeClock.getElapsedTime().asMilliseconds() / FADE_TIME;

		/* If the box is active */
		if (state)
		{
			square.setFillColor(blend(backgroundColor, squareColor, progress));
		}
		/* If the box is inactive */
		else
		{
			square.setFillColor(blend(squareColor, backgroundColor, progress));
		}
	}
}

void gr::Checkbox::render(sf::RenderWindow &window) const
{
	window.draw(main);
	window.draw(mainGradient, 4, sf::Quads);
	window.draw(square);
	window.draw(squareGradient, 4, sf::Quads);
	window.draw(text);
}

gr::Interactive *gr::Checkbox::clone()
{return new Checkbox(*this);}

void gr::Checkbox::setBackgroundColor(sf::Color abackgroundColor)
{
	Colored::setBackgroundColor(abackgroundColor);
	main.setFillColor(backgroundColor);
}

void gr::Checkbox::setBorderColor(sf::Color aborderColor)
{
	
	Colored::setBorderColor(aborderColor);
	main.setOutlineColor(borderColor);
}

void gr::Checkbox::setTextColor(sf::Color atextColor)
{
	Colored::setTextColor(atextColor);
	text.setFillColor(textColor);
}

void gr::Checkbox::setSquareColor(sf::Color asquareColor)
{squareColor = asquareColor;}

void gr::Checkbox::reset()
{
	Interactive::reset();
	isTargeted = false;
	isPressed = false;
	isMisclicked = false;
	isFading = false;
}

void gr::Checkbox::setState(bool s)
{state = s;}

/* ProgressBar */

const long gr::ProgressBar::BORDER_THICKNESS = 1;
const float gr::ProgressBar::ANIMATION_TIME = 200;
const float gr::ProgressBar::PI = 3.141;

void gr::ProgressBar::setup()
{
	/* Main progress bar rectangle setup */
	main.setSize(sf::Vector2f(width, height));
	main.setFillColor(backgroundColor);
	main.setOutlineColor(borderColor);
	main.setOutlineThickness(BORDER_THICKNESS);
	main.setPosition(xPos, yPos);

	/* A bar rectangle setup */
	fill.setFillColor(fillColor);
	fill.setOutlineThickness(0);
	fill.setPosition(xPos, yPos);

	/* Gradient initialization */
	mainGradient[0] = sf::Vertex(sf::Vector2f(xPos, yPos),
								 sf::Color(255, 255, 255, 0));
	mainGradient[1] = sf::Vertex(sf::Vector2f(xPos + width - 1, yPos),
								 sf::Color(255, 255, 255, 0));
	mainGradient[2] = sf::Vertex(sf::Vector2f(xPos + width - 1, yPos + height / 3),
								 sf::Color(255, 255, 255, 128));
	mainGradient[3] = sf::Vertex(sf::Vector2f(xPos, yPos + height / 3),
								 sf::Color(255, 255, 255, 128));
}

gr::ProgressBar::ProgressBar(long axPos, long ayPos, long awidth, long aheight,
							 sf::Color abackgroundColor, sf::Color afillColor,
							 sf::Color aborderColor):
	Interactive(axPos, ayPos, awidth, aheight), 
	Colored(abackgroundColor, aborderColor, sf::Color()),
	fillColor(afillColor), value(0.0f), previousValue(0.0f), isMoving(false)
{setup();}

gr::ProgressBar::ProgressBar(long axPos, long ayPos, long awidth, long aheight):
	Interactive(axPos, ayPos, awidth, aheight),
	Colored(sf::Color(128, 128, 128), sf::Color(0, 328, 0), sf::Color()),
	fillColor(0, 238, 0), value(0.0f), previousValue(0.0f), isMoving(false)
{setup();}

void gr::ProgressBar::setValue(float newValue)
{
	/* Set a new value */
	previousValue = value;
	value = newValue;

	/* Restart animation */
	clock.restart();
	isMoving = true;
}

void gr::ProgressBar::refresh(sf::RenderWindow &window)
{
	float visualValue;
	long elapsedTime = clock.getElapsedTime().asMilliseconds();

	/* Check wether the animation is over */
	if (elapsedTime >= ANIMATION_TIME)
		isMoving = false;

	/* Calculate the visual value */
	if (!isMoving)
	{
		visualValue = value;
	}
	else
	{
		/* Calculate phase*/
		float phase = (PI / 2) * (elapsedTime / ANIMATION_TIME);
		
		/* Calculate animation share */
		float share = sin(phase);

		/* Calculate visual bar's value */
		visualValue = previousValue + (value - previousValue) * share;
	}

	/* Set the bar length */
	fill.setSize(sf::Vector2f(width * visualValue, height));
}

void gr::ProgressBar::render(sf::RenderWindow &window) const
{
	window.draw(main);
	window.draw(fill);
	window.draw(mainGradient, 4, sf::Quads);
}

gr::Interactive *gr::ProgressBar::clone()
{return new ProgressBar(*this);}

gr::ProgressBar::ProgressBar(ProgressBar &right):
	Interactive(right.xPos, right.yPos, right.width, right.height),
	Colored(right.backgroundColor, right.borderColor, sf::Color()),
	value(right.value), fillColor(right.fillColor), main(right.main),
	fill(right.fill), isMoving(right.isMoving),	previousValue(right.previousValue)
{
	for (long i = 0; i < 4; i++)
		mainGradient[i] = right.mainGradient[i];
}

void gr::ProgressBar::setBackgroundColor(sf::Color abackgroundColor)
{
	Colored::setBackgroundColor(abackgroundColor);
	main.setFillColor(backgroundColor);
}

void gr::ProgressBar::setBorderColor(sf::Color aborderColor)
{
	Colored::setBorderColor(aborderColor);
	main.setOutlineColor(borderColor);
}

void gr::ProgressBar::setFillColor(sf::Color afillColor)
{
	fillColor = afillColor;
	fill.setFillColor(fillColor);
}

void gr::ProgressBar::reset()
{
	Interactive::reset();
	isMoving = false;
}

/* RadioButtonSet */

const float gr::RadioButtonSet::RELATIVE_DOT_SIZE = 0.7;
const long gr::RadioButtonSet::BORDER_THICKNESS = 1;
const long gr::RadioButtonSet::TEXT_INDENT = 3;
const float gr::RadioButtonSet::FADE_TIME = 100;

void gr::RadioButtonSet::setup(sf::String atext, sf::Font &afont)
{
	/* Memory allocation */
	main = new sf::CircleShape[numOfButtons];
	dot = new sf::CircleShape[numOfButtons];
	text = new sf::Text[numOfButtons];

	/* Circles, dots and text setup */
	for (long i = 0; i < numOfButtons; i++)
	{
		main[i].setFillColor(backgroundColor);
		main[i].setOutlineColor(borderColor);
		main[i].setOutlineThickness(BORDER_THICKNESS);
		main[i].setRadius(height / 2.0);
		main[i].setPosition(xPos, yPos + i * (height + indent));

		dot[i].setFillColor(backgroundColor);
		dot[i].setOutlineThickness(0);
		dot[i].setRadius(height / 2.0 * RELATIVE_DOT_SIZE);
		dot[i].setPosition(xPos + width * (1 - RELATIVE_DOT_SIZE) / 2,
						   height * (1 - RELATIVE_DOT_SIZE) / 2.0 + yPos + i * (height + indent));

		/* Get position of the '\n'
		symbol and calculate length
		of the substring before it */
		long subLen = atext.find('\n');

		/* Get a substring and setup
		an sf::Text class object */
		sf::String subStr;
		subStr = atext.substring(0, subLen);
		text[i].setFont(afont);
		text[i].setString(subStr);
		text[i].setCharacterSize(height * 0.8);
		text[i].setFillColor(textColor);
		text[i].setPosition(xPos + width + TEXT_INDENT, yPos + i * (height + indent));

		/* Delete used substring from source */
		atext.erase(0, subLen + 1);
	}
}

gr::RadioButtonSet::RadioButtonSet(long axPos, long ayPos, long anumOfButtons,
								   long aindent, long adiameter, sf::String atext, sf::Font &afont,
								   sf::Color abackgroundColor, sf::Color aborderColor,
								   sf::Color adotColor, sf::Color atextColor):
	Interactive(axPos, ayPos, adiameter, adiameter),
	Colored(abackgroundColor, aborderColor, atextColor),
	indent(aindent), numOfButtons(anumOfButtons), dotColor(adotColor), value(0),
	pressedButtonNum(-1), isMisclicked(false), previousValue(0),
	isFading(false), isSomeButtonTargeted(false), targetedButtonNum(-1)
{setup(atext, afont);}

gr::RadioButtonSet::RadioButtonSet(long axPos, long ayPos, long anumOfButtons, long aindent,
								   long adiameter, sf::String atext, sf::Font &afont):
	Interactive(axPos, ayPos, adiameter, adiameter),
	Colored(sf::Color(128, 128, 128), sf::Color(64, 64, 64), sf::Color(0, 0, 0)),
	indent(aindent), numOfButtons(anumOfButtons), dotColor(41, 235, 56), value(0),
	pressedButtonNum(-1), isMisclicked(false), previousValue(0),
	isFading(false), isSomeButtonTargeted(false), targetedButtonNum(-1)
{setup(atext, afont);}

gr::RadioButtonSet::RadioButtonSet(RadioButtonSet &right):
	Interactive(right.xPos, right.yPos, right.width, right.height),
	Colored(right.backgroundColor, right.borderColor, right.textColor),
	numOfButtons(right.numOfButtons), indent(right.indent), value(right.value),
	dotColor(right.dotColor), pressedButtonNum(right.pressedButtonNum),
	isMisclicked(false), previousValue(right.previousValue), isFading(false),
	isSomeButtonTargeted(false), targetedButtonNum(-1)
{
	/* Memory allocation */
	main = new sf::CircleShape[numOfButtons];
	dot = new sf::CircleShape[numOfButtons];
	
	/* Copy data */
	for (long i = 0; i < numOfButtons; i++)
	{
		main[i] = right.main[i];
		dot[i] = right.dot[i];
		text[i] = right.text[i];
	}
}

gr::RadioButtonSet::~RadioButtonSet()
{
	delete[] main;
	delete[] dot;
	delete[] text;
}

void gr::RadioButtonSet::refresh(sf::RenderWindow &window)
{
	sf::Vector2i curPos = sf::Mouse::getPosition(window);

	/* If the radiobutton set is enabled */
	if (enabled)
	{
		/* If none of the buttons are pressed */
		if (pressedButtonNum == -1)
		{
			/* If this object is targeted or none is targeted */
			if (targetedObjectId == id || targetedObjectId == -1)
			{
				targetedButtonNum = -1;

				/* Check if some button is targeted */
				for (long i = 0; i < numOfButtons; i++)
				{
					/* Get coordinates of
					the center of the button */
					sf::Vector2f butPos;
					butPos.x = main[i].getPosition().x + width / 2.0;
					butPos.y = main[i].getPosition().y + height / 2.0;
					/* If the cursor is on the button */
					if (std::abs(curPos.x - butPos.x) * std::abs(curPos.x - butPos.x) +
						std::abs(curPos.y - butPos.y) * std::abs(curPos.y - butPos.y) <=
						width * width / 4.0)
					{
						targetedButtonNum = i;
						break;
					}
				}
				
				/* If some button is targeted */
				if (targetedButtonNum != -1)
				{
					targetedObjectId = id;

					/* If the left mouse key is pressed and we didn't misclicked */
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMisclicked)
					{	
						pressedButtonNum = targetedButtonNum;
					}
					/* If the left mouse key is released */
					if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						isMisclicked = false;
					}
				}
				/* If none of the buttons are targeted */
				else
				{
					targetedObjectId = -1;
					isMisclicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
				}
			}
			else
			{
				isMisclicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
			}
		}
		/* If some button is pressed */
		else
		{
			/* Get coordinates of the
			center of the pressed button */
			sf::Vector2f butPos;
			butPos.x = main[pressedButtonNum].getPosition().x + width / 2.0;
			butPos.y = main[pressedButtonNum].getPosition().y + height / 2.0;
	
			/* If the cursor is not on the button,
			reset the pressed button number */
			if (std::abs(curPos.x - butPos.x) * std::abs(curPos.x - butPos.x) +
				std::abs(curPos.y - butPos.y) * std::abs(curPos.y - butPos.y) >
				width * width / 4.0)
			{
				targetedObjectId = -1;
				pressedButtonNum = -1;
			}
			/* If the left mouse button is not pressed
			while cursor is on the pressed button,
			this button should be triggered and the
			pressed button number should be reset */
			else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				previousValue = value;
				value = pressedButtonNum;
				pressedButtonNum = -1;
				isFading = true;
				fadeClock.restart();
			}
		}
	}
	/* If the radiobutton set is disabled and none of the objects are targeted */
	else if (targetedObjectId == -1)
	{
		sf::Cursor cursor;
		bool isNowSomeButtonTargeted = false;

		/* Check if some button is targeted */
		for (long i = 0; i < numOfButtons; i++)
		{
			/* Get coordinates of
			the center of the button */
			sf::Vector2f butPos;
			butPos.x = main[i].getPosition().x + width / 2.0;
			butPos.y = main[i].getPosition().y + height / 2.0;

			/* If the cursor is on the button */
			if (std::abs(curPos.x - butPos.x) * std::abs(curPos.x - butPos.x) +
				std::abs(curPos.y - butPos.y) * std::abs(curPos.y - butPos.y) <=
				width * width / 4.0)
			{
				isNowSomeButtonTargeted = true;
				break;
			}
		}

		/* If some button is targeted */
		if (isNowSomeButtonTargeted)
		{
			if (!isSomeButtonTargeted)
			{
				isSomeButtonTargeted = true;
				cursor.loadFromSystem(sf::Cursor::NotAllowed);
				window.setMouseCursor(cursor);
			}
		}
		/* If none of the buttons are targeted */
		else
		{
			if (isSomeButtonTargeted)
			{
				isSomeButtonTargeted = false;
				cursor.loadFromSystem(sf::Cursor::Arrow);
				window.setMouseCursor(cursor);
			}
		}
	}

	/* Fade stuff */
	float elapsedTime = fadeClock.getElapsedTime().asMilliseconds();

	/* Check if the fading is over */
	if (elapsedTime > FADE_TIME)
	{
		isFading = false;
	}

	/* If the fading is over */
	if (!isFading)
	{
		dot[previousValue].setFillColor(backgroundColor);
		dot[value].setFillColor(dotColor);
	}
	/* If fading is still running */
	else
	{
		float progress = elapsedTime / FADE_TIME;

		dot[previousValue].setFillColor(blend(dotColor, backgroundColor, progress));
		dot[value].setFillColor(blend(backgroundColor, dotColor, progress));
	}
}

void gr::RadioButtonSet::render(sf::RenderWindow &window) const
{
	/* Draw all circles, dots and text */
	for (long i = 0; i < numOfButtons; i++)
	{
		window.draw(main[i]);
		window.draw(dot[i]);
		window.draw(text[i]);
	}
}

gr::Interactive *gr::RadioButtonSet::clone()
{return new RadioButtonSet(*this);}

void gr::RadioButtonSet::setBackgroundColor(sf::Color abackgroundColor)
{
	Colored::setBackgroundColor(abackgroundColor);

	for (long i = 0; i < numOfButtons; i++)
	{
		main[i].setFillColor(backgroundColor);
		dot[i].setFillColor(backgroundColor);
	}
}

void gr::RadioButtonSet::setBorderColor(sf::Color aborderColor)
{
	Colored::setBorderColor(aborderColor);

	for (long i = 0; i < numOfButtons; i++)
	{
		main[i].setOutlineColor(borderColor);
	}
}

void gr::RadioButtonSet::setTextColor(sf::Color atextColor)
{
	Colored::setTextColor(atextColor);

	for (long i = 0; i < numOfButtons; i++)
	{
		text[i].setFillColor(textColor);
	}
}

void gr::RadioButtonSet::setDotColor(sf::Color adotColor)
{
	dotColor = adotColor;
	dot[value].setFillColor(dotColor);
}

void gr::RadioButtonSet::reset()
{
	Interactive::reset();
	isMisclicked = false;
	isFading = false;
	isSomeButtonTargeted = false;
	pressedButtonNum = -1;
	targetedButtonNum = -1;
}

/* TextField */

const long gr::TextField::BORDER_THICKNESS = 1;
long gr::TextField::activeFieldId = -1;
sf::Uint32 gr::TextField::enteredChar = -1, gr::TextField::previousEnteredChar = -1;

void gr::TextField::setup()
{
	/* Main rectangle setup */
	main.setFillColor(backgroundColor);
	main.setOutlineColor(borderColor);
	main.setOutlineThickness(BORDER_THICKNESS);
	main.setPosition(xPos, yPos);
	main.setSize(sf::Vector2f(width, height));

	/* Gradient setup */
	mainGradient[0] = sf::Vertex(sf::Vector2f(xPos, yPos),
								 sf::Color(255, 255, 255, 128));
	mainGradient[1] = sf::Vertex(sf::Vector2f(xPos + width - 1, yPos),
								 sf::Color(255, 255, 255, 128));
	mainGradient[2] = sf::Vertex(sf::Vector2f(xPos + width - 1, yPos + height - 1),
								 sf::Color(255, 255, 255, 0));
	mainGradient[3] = sf::Vertex(sf::Vector2f(xPos, yPos + height - 1),
								 sf::Color(255, 255, 255, 0));

	/* Text setup */
	text.setCharacterSize(height * 0.8);
	text.setPosition(sf::Vector2f(xPos, yPos));
	text.setFillColor(textColor);

	/* Highlight rectangle setup */
	highlightRectangle.setOutlineThickness(0);
	highlightRectangle.setFillColor(sf::Color(0, 0, 0, 0));
}

gr::TextField::TextField(long axPos, long ayPos, long awidth, long aheight,
						 sf::String adefaultText, sf::Font &afont, sf::Color abackgroundColor,
						 sf::Color aborderColor, sf::Color atextColor, sf::Color aselectColor):
	Interactive(axPos, ayPos, awidth, aheight),
	Colored(abackgroundColor, aborderColor, atextColor),
	defaultText(adefaultText), text(adefaultText, afont), isPressed(false), value(""),
	selectColor(aselectColor), isTargeted(false), isMisclicked(false), font(afont), mode(0),
	isBackspacePressed(false), isEnterPressed(false), isEmpty(true)
{setup();}

gr::TextField::TextField(long axPos, long ayPos, long awidth, long aheight,
						 sf::String adefaultText, sf::Font &afont):
	Interactive(axPos, ayPos, awidth, aheight),
	Colored(sf::Color(200, 200, 200), sf::Color(64, 64, 64), sf::Color(128, 128, 128)),
	defaultText(adefaultText), text(adefaultText, afont), isPressed(false), value(""),
	selectColor(0, 0, 0), isTargeted(false), isMisclicked(false), font(afont), mode(0),
	isBackspacePressed(false), isEnterPressed(false), isEmpty(true)
{setup();}

gr::TextField::TextField(TextField &right):
	Interactive(right.xPos, right.yPos, right.width, right.height),
	Colored(right.backgroundColor, right.borderColor, right.textColor),
	isPressed(false), value(right.value), main(right.main), text(right.text),
	defaultText(right.defaultText), selectColor(right.selectColor),
	isTargeted(false), isMisclicked(false), font(right.font), mode(right.mode),
	isBackspacePressed(false), isEnterPressed(false), isEmpty(true)
{
	for (long i = 0; i < 4; i++)
		mainGradient[i] = right.mainGradient[i];
}

gr::TextField::~TextField()
{
	reset();
}

void gr::TextField::refresh(sf::RenderWindow &window)
{
	sf::Vector2i curPos = sf::Mouse::getPosition(window);

	/* If the text field is enabled */
	if (enabled)
	{
		/* If the field is not pressed */
		if (!isPressed)
		{
			/* If this object is targeted or none are targeted */
			if (targetedObjectId == id || targetedObjectId == -1)
			{
				/* If the cursor is on the field */
				if (curPos.x >= xPos && curPos.x <= xPos + width - 1 &&
					curPos.y >= yPos && curPos.y <= yPos + height - 1)
				{
					targetedObjectId = id;

					if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMisclicked)
					{
						isPressed = true;
					}
					
					if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						isMisclicked = false;
					}
				}
				/* If the cursor is away from the field */
				else
				{
					targetedObjectId = -1;

					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						isMisclicked = true;
						activeFieldId = -1;
					}
					else
					{
						isMisclicked = false;
					}
				}
			}
			else
			{
				isMisclicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
			}
		}
		/* If the field is pressed */
		else
		{
			/* If the cursor is not on the field */
			if (curPos.x < xPos || curPos.x > xPos + width - 1 ||
				curPos.y < yPos || curPos.y > yPos + height - 1)
			{
				targetedObjectId = -1;
				isPressed = false;
			}
			/* If the cursor is on the field
			and the left mouse key is up */
			else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				isPressed = false;
				activeFieldId = id;
			}
		}

		/* Refresh color */
		text.setFillColor(activeFieldId == id ? selectColor : textColor);
	
		/* Reset flags */
		isEnterPressed = false;
		isBackspacePressed = false;
		isEmpty = !text.getString().getSize();

		/* If this field is active */
		if (activeFieldId == id)
		{
			if (enteredChar != previousEnteredChar)
			{
				//std::cout << enteredChar << std::endl;
				if (enteredChar == 8)
				{
					isBackspacePressed = true;

					/* Erase last symbol  */
					if (value.getSize())
					{
						value.erase(value.getSize() - 1, 1);

						// If there is an '\n'-symbol at the end of the string
						if (value.getSize() && value[value.getSize() - 1] == '\n')
						{
							value.erase(value.getSize() - 1, 1);  // Delete it
						}
					}

					if (value.getSize() == 0)
						text.setString(defaultText);
					else
						text.setString(value);
				}
				else if (enteredChar != -1 && enteredChar != 13)
				{
					value += enteredChar;  // Add a new symbol
					text.setString(value);
				}
				else if (enteredChar == 13)
				{
					isEnterPressed = true;
				}
			}
		}
	}
	/* If the text field is disabled and none of the objects are targeted */
	else if (targetedObjectId == -1)
	{
		sf::Cursor cursor;

		/* If the object is targeted */
		if (curPos.x >= xPos && curPos.x <= xPos + width - 1 &&
			curPos.y >= yPos && curPos.y <= yPos + height - 1)
		{
			if (!isTargeted)
			{
				isTargeted = true;
				cursor.loadFromSystem(sf::Cursor::NotAllowed);
				window.setMouseCursor(cursor);
			}
		}
		else
		{
			if (isTargeted)
			{
				isTargeted = false;
				cursor.loadFromSystem(sf::Cursor::Arrow);
				window.setMouseCursor(cursor);
			}
		}
	}
}

void gr::TextField::setEnteredCharacter(sf::Uint32 character)
{
	previousEnteredChar = enteredChar;
	enteredChar = character;
}

char gr::TextField::sfmlToAscii(long keyCode)
{
	if (keyCode <= 25)
		return keyCode + 97;
	if (keyCode <= 35)
		return keyCode + 22;
	switch (keyCode)
	{
		case 50:
			return '.';
			break;
		case 49:
			return ',';
			break;
		case 56:
			return '-';
			break;
		case 55:
			return '=';
			break;
		case 52:
			return '/';
			break;
		case 53:
			return '\\';
			break;
		case 51:
			return '\'';
			break;
		case 48:
			return ';';
			break;
		case 57:
			return ' ';
			break;
		case 59:
			return 8;
			break;
		default:
			return '?';
			break;
	}
}

void gr::TextField::setMode(short newFlags)
{
	mode = newFlags;
	
	if (NoFrame & mode)
	{
		main.setOutlineThickness(0);
	}
	else
	{
		main.setOutlineThickness(BORDER_THICKNESS);
	}
}

void gr::TextField::render(sf::RenderWindow &window) const
{
	window.draw(main);

	if (!(NoGradient & mode))
	{
		window.draw(mainGradient, 4, sf::Quads);
	}

	window.draw(text);
	window.draw(highlightRectangle);
}

sf::String gr::TextField::getText() const
{
	std::wstring str = value;
	long start = 0;

	/* Insert '\n' symbols */
	while (str.find(start, '\\') != -1)
	{
		long pos = str.find('\\');

		if (pos + 1 < str.size())
		{
			if (str.substr(pos + 1, 1) == L"n")
			{
				str.replace(pos, 2, L"\n");
			}
			else
			{
				start = pos + 1;
			}
		}
		else
		{
			break;
		}
	}

	return str;
}

gr::Interactive *gr::TextField::clone()
{return new TextField(*this);}

void gr::TextField::setText(sf::String newText)
{
	value = newText;

	if (value.getSize() == 0)
		text.setString(defaultText);
	else
		text.setString(value);
}

void gr::TextField::setBackgroundColor(sf::Color abackgroundColor)
{
	Colored::setBackgroundColor(abackgroundColor);
	main.setFillColor(backgroundColor);
}

void gr::TextField::setBorderColor(sf::Color aborderColor)
{
	Colored::setBorderColor(aborderColor);
	main.setOutlineColor(borderColor);
}

void gr::TextField::setSelectColor(sf::Color aselectColor)
{selectColor = aselectColor;}

void gr::TextField::reset()
{
	Interactive::reset();
	isPressed = false;
	isTargeted = false;
	isMisclicked = false;
	
	if (activeFieldId == id)
	{
		activeFieldId = -1;
	}
}

void gr::TextField::highlight(long start, long length, sf::Color color)
{
	// Если границы подстроки лежат внутри строки
	if (start >= 0 && start < value.getSize() && length > 0 && start + length <= value.getSize())
	{
		sf::Text left(value.substring(0, start), font);
		sf::Text highlighted(value.substring(start, length), font);

		left.setCharacterSize(text.getCharacterSize());
		highlighted.setCharacterSize(text.getCharacterSize());

		long width = highlighted.getGlobalBounds().width;
		long height = main.getSize().y;
		long x = text.getPosition().x + left.getGlobalBounds().width;
		long y = main.getPosition().y;

		highlightRectangle.setFillColor(color);
		highlightRectangle.setSize(sf::Vector2f(width, height));
		highlightRectangle.setPosition(sf::Vector2f(x, y));
	}
}

void gr::TextField::resetHighlight()
{highlightRectangle.setFillColor(sf::Color(0, 0, 0, 0));}

void gr::TextField::setPosition(int newX, int newY)
{
	highlightRectangle.move(newX - xPos, newY - yPos);

	xPos = newX;
	yPos = newY;

	main.setPosition(xPos, yPos);
	
	mainGradient[0].position = sf::Vector2f(xPos, yPos);
	mainGradient[1].position = sf::Vector2f(xPos + width, yPos);
	mainGradient[2].position = sf::Vector2f(xPos + width, yPos + height);
	mainGradient[3].position = sf::Vector2f(xPos, yPos + height);

	text.setPosition(xPos, yPos);
}

/* Label */

void gr::Label::setup()
{
	sf::FloatRect gbounds;

	text.setFillColor(textColor);
	text.setCharacterSize(width);

	switch (alignment)
	{
		case Left:
			text.setPosition(xPos, yPos);
			break;
		case Center:
			gbounds = text.getGlobalBounds();
			text.setPosition(xPos - gbounds.width / 2.0, yPos);
			break;
		case Right:
			gbounds = text.getGlobalBounds();
			text.setPosition(xPos - gbounds.width, yPos);
			break;
	}
}

gr::Label::Label(long axPos, long ayPos, long asize, sf::String atext,
				 sf::Font &afont, sf::Color atextColor, Alignment aalignment):
	Interactive(axPos, ayPos, asize, asize),
	Colored(sf::Color(), sf::Color(), atextColor),
	text(atext, afont), alignment(aalignment)
{setup();}

gr::Label::Label(long axPos, long ayPos, long asize, sf::String atext,
				 sf::Font &afont, Alignment aalignment):
	Interactive(axPos, ayPos, asize, asize),
	Colored(sf::Color(), sf::Color(), sf::Color(0, 0, 0)),
	text(atext, afont), alignment(aalignment)
{setup();}

void gr::Label::refresh(sf::RenderWindow &window)
{}

void gr::Label::render(sf::RenderWindow &window) const
{window.draw(text);}

gr::Interactive *gr::Label::clone()
{return new Label(*this);}

void gr::Label::setText(sf::String newText)
{
	sf::FloatRect gbounds;
	text.setString(newText);

	switch (alignment)
	{
		case Left:
			text.setPosition(xPos, yPos);
			break;
		case Center:
			gbounds = text.getGlobalBounds();
			text.setPosition(xPos - gbounds.width / 2.0, yPos);
			break;
		case Right:
			gbounds = text.getGlobalBounds();
			text.setPosition(xPos - gbounds.width, yPos);
			break;
	}
}

void gr::Label::setTextColor(sf::Color atextColor)
{
	Colored::setTextColor(atextColor);
	text.setFillColor(textColor);
}

/* AdvancedButton */

void gr::AdvancedButton::setup()
{
	Button::setup();
	sprite.setColor(backgroundColor);
	sprite.setPosition(xPos, yPos);
}

gr::AdvancedButton::AdvancedButton(long axPos, long ayPos, long awidth, long aheight,
								   sf::String atext, sf::Font &afont,
								   sf::Color abackgroundColor, sf::Color aborderColor,
								   sf::Color atextColor, sf::Color aselectColor,
								   sf::Texture &atexture, long amode):
	Button(axPos, ayPos, awidth, aheight, atext, afont, abackgroundColor, aborderColor,
	atextColor, aselectColor), mode(Mode(amode)),
	sprite(atexture, sf::IntRect(0, 0, atexture.getSize().x, atexture.getSize().y))
{setup();}

gr::AdvancedButton::AdvancedButton(AdvancedButton &right):
	Button(right), mode(right.mode), sprite(right.sprite)
{}

void gr::AdvancedButton::render(sf::RenderWindow &window) const
{
	if (!(Invisible & mode))
	{
		if (!(NoShadow & mode))
			window.draw(shadowGradient, 4, sf::Quads);

		if (!(NoTexture & mode))
			window.draw(sprite);
		else
			window.draw(main);

		if (!(NoGradient & mode))
			window.draw(mainGradient, 4, sf::Quads);

		if (!(NoText & mode))
			window.draw(text);
	}
}

void gr::AdvancedButton::refresh(sf::RenderWindow &window)
{
	/* We must reset the
	click trigger */
	isTriggered = false;

	sf::Vector2i curPos = sf::Mouse::getPosition(window);

	/* If the button is enabled */
	if (enabled)
	{	
		if (!isPressed)
		{
			/* If this object is targeted or none targeted */
			if (targetedObjectId == id || targetedObjectId == -1)
			{
				/* If the cursor is on the button */
				if (curPos.x >= xPos && curPos.x <= xPos + width - 1 &&
					curPos.y >= yPos && curPos.y <= yPos + height - 1)
				{
					targetedObjectId = id;

					if (!(NoColorChange & mode))
					{
						if (!isTargeted)
						{
							isTargeted = true;
							isFading = true;
							fadeClock.restart();
						}
					}
			
					/* If the left mouse key is down on the button */
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
						(!isMisclicked || DragClick & mode))
					{
	
						if (!(NoOffset & mode))
						{
							isMoving = true;
							moveClock.restart();
						}
			
						isPressed = true;
			
						if (InstantTriggering & mode)
							isTriggered = true;
					}
			
					/* If the left mouse key is released */
					if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						isMisclicked = false;
					}
				}
				/* If the cursor is away from the button */
				else
				{
					targetedObjectId = -1;

					if (!(NoColorChange & mode))
					{
						if (isTargeted)
						{
							isTargeted = false;
							isFading = true;
							fadeClock.restart();
						}
					}
			
					isMisclicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
				}
			}
			else
			{
				isMisclicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
			}
			/* If a bound key is pressed */
			if (isKeyBound && sf::Keyboard::isKeyPressed(boundKey))
			{
				if (!isBoundKeyPressed)
				{
					isTriggered = true;
					isBoundKeyPressed = true;
					
					if (!(NoOffset & mode))
					{
						keyTriggeringPhase = 1;  // Phase 1 = the button is being pressed
					}
				}
			}
			/* If a bound key released */
			else
			{
				isBoundKeyPressed = false;
			}
		}
		else
		{
			bool cursorGoawayCondition;
	
			/* Choose a condition of cursor goaway */
			if (!(NoOffset & mode))
			{
				cursorGoawayCondition =
				curPos.x < xPos || curPos.x > xPos + width - 1 + SHADOW_OFFSET ||
				curPos.y < yPos || curPos.y > yPos + height - 1 + SHADOW_OFFSET;
			}
			else
			{
				cursorGoawayCondition =
				curPos.x < xPos || curPos.x > xPos + width - 1 ||
				curPos.y < yPos || curPos.y > yPos + height - 1;
			}
	
			/* If cursor is away from the button */
			if (cursorGoawayCondition)
			{
				targetedObjectId = -1;

				/* If the button is not triggered by a key
				but is triggered by the mouse cursor */
				if (!keyTriggeringPhase)
				{
					/* Reset it */
					if (!(NoOffset & mode))
					{
						isMoving = true;
						moveClock.restart();
					}

					isPressed = false;
				}

				if (!(NoColorChange & mode))
				{
					if (isTargeted)
					{
						isTargeted = false;
						isFading = true;
						fadeClock.restart();
					}
				}
			}
			/* If the left mouse button is up */
			else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				/* If the button is not triggered by a key
				but is triggered by the mouse cursor */
				if (!keyTriggeringPhase)
				{
					/* Reset the button and activate click trigger */
					if (!(NoOffset & mode))
					{
						isMoving = true;
						moveClock.restart();
					}
		
					isPressed = false;
	
					if (!(InstantTriggering & mode))
						isTriggered = true;
				}
			}
		}
	}
	/* If the button is disabled and none of the objects are targeted */
	else if (targetedObjectId == -1)
	{
		sf::Cursor cursor;

		if (curPos.x >= xPos && curPos.x <= xPos + width - 1 &&
			curPos.y >= yPos && curPos.y <= yPos + height - 1)
		{
			if (!isTargeted)
			{
				isTargeted = true;
				cursor.loadFromSystem(sf::Cursor::NotAllowed);
				window.setMouseCursor(cursor);
			}
		}
		else
		{
			if (isTargeted)
			{
				isTargeted = false;
				cursor.loadFromSystem(sf::Cursor::Arrow);
				window.setMouseCursor(cursor);
			}
		}
	}

	if (!(NoColorChange & mode))
	{
		/* Fade stuff */
		long fadeElapsedTime = fadeClock.getElapsedTime().asMilliseconds();
	
		/* Check wether the fade is over */
		if (fadeElapsedTime > FADE_TIME)
			isFading = false;

		/* If fading is over */
		if (!isFading)
		{
			main.setFillColor(isTargeted ? selectColor : backgroundColor);
			sprite.setColor(isTargeted ? selectColor : backgroundColor);
		}
		/* If fading is still running */
		else
		{
			/* If the cursor is on the button */
			if (isTargeted)
			{
				main.setFillColor(blend(backgroundColor, selectColor,
								  fadeElapsedTime / FADE_TIME));
				sprite.setColor(blend(backgroundColor, selectColor,
								fadeElapsedTime / FADE_TIME));
			}
			/* If the cursor is away from button */
			else
			{
				main.setFillColor(blend(selectColor, backgroundColor, 
								  fadeElapsedTime / FADE_TIME));
				sprite.setColor(blend(selectColor, backgroundColor, 
								fadeElapsedTime / FADE_TIME));
			}
		}
	}

	if (!(NoOffset & mode))
	{
		/* Key-triggered button animation */
		switch (keyTriggeringPhase)
		{
			case 1: // The button is being pressed
				if (!isPressed)
				{
					isPressed = true;
					isMoving = true;
					moveClock.restart();
				}
				break;
			case 2: // The button is being released
				if (isPressed)
				{
					isPressed = false;
					isMoving = true;
					moveClock.restart();
				}
				break;
			default:  // The button has been released
				keyTriggeringPhase = 0;
				break;
		}

		/* Animation stuff */
		long moveElapsedTime = moveClock.getElapsedTime().asMilliseconds();

		/* Check wether the animation is over */
		if (moveElapsedTime > ANIMATION_TIME)
		{
			/* If the button is triggered by a key */
			if (keyTriggeringPhase)
			{
				keyTriggeringPhase++;
			}

			isMoving = false;
		}

		/* If animation is over */
		if (!isMoving)
		{
			if (isPressed)
			{
				main.setPosition(xPos + SHADOW_OFFSET, yPos + SHADOW_OFFSET);
				sprite.setPosition(xPos + SHADOW_OFFSET, yPos + SHADOW_OFFSET);
				text.setPosition(xPos + width / 2 - textBounds.width / 2 + SHADOW_OFFSET,
								 yPos + SHADOW_OFFSET);

				mainGradient[0].position = sf::Vector2f(xPos + SHADOW_OFFSET,
														yPos + SHADOW_OFFSET);
				mainGradient[1].position = sf::Vector2f(xPos + width - 1 + SHADOW_OFFSET,
														yPos + SHADOW_OFFSET);
				mainGradient[2].position = sf::Vector2f(xPos + width - 1 + SHADOW_OFFSET,
														yPos + height - 1 + SHADOW_OFFSET);
				mainGradient[3].position = sf::Vector2f(xPos + SHADOW_OFFSET,
														yPos + height - 1 + SHADOW_OFFSET);

			}
			else
			{
				main.setPosition(xPos, yPos);
				sprite.setPosition(xPos, yPos);
				text.setPosition(xPos + width / 2 - textBounds.width / 2, yPos);

				mainGradient[0].position = sf::Vector2f(xPos, yPos);
				mainGradient[1].position = sf::Vector2f(xPos + width - 1, yPos);
				mainGradient[2].position = sf::Vector2f(xPos + width - 1, yPos + height - 1);
				mainGradient[3].position = sf::Vector2f(xPos, yPos + height - 1);
			}
		}
		/* If animation is still running */
		else
		{
			/* Calculate offset */
			long offset =  SHADOW_OFFSET * std::sin(PI / 2 * moveElapsedTime / ANIMATION_TIME);

			if (isPressed)
			{
				main.setPosition(xPos + offset, yPos + offset);
				sprite.setPosition(xPos + offset, yPos + offset);
				text.setPosition(xPos + width / 2 - textBounds.width / 2 + offset, yPos + offset);

				mainGradient[0].position = sf::Vector2f(xPos + offset,
														yPos + offset);
				mainGradient[1].position = sf::Vector2f(xPos + width - 1 + offset,
														yPos + offset);
				mainGradient[2].position = sf::Vector2f(xPos + width - 1 + offset,
														yPos + height - 1 + offset);
				mainGradient[3].position = sf::Vector2f(xPos + offset,
														yPos + height - 1 + offset);

			}
			else
			{
				main.setPosition(xPos + SHADOW_OFFSET - offset, yPos + SHADOW_OFFSET - offset);
				sprite.setPosition(xPos + SHADOW_OFFSET - offset, yPos + SHADOW_OFFSET - offset);
				text.setPosition(xPos + width / 2 - textBounds.width / 2 + SHADOW_OFFSET - offset,
								 yPos + SHADOW_OFFSET - offset);

				mainGradient[0].position = sf::Vector2f(xPos + SHADOW_OFFSET - offset,
														yPos + SHADOW_OFFSET - offset);
				mainGradient[1].position = sf::Vector2f(xPos + width - 1 + SHADOW_OFFSET - offset,
														yPos + SHADOW_OFFSET - offset);
				mainGradient[2].position = sf::Vector2f(xPos + width - 1 + SHADOW_OFFSET - offset,
														yPos + height - 1 + SHADOW_OFFSET - offset);
				mainGradient[3].position = sf::Vector2f(xPos + SHADOW_OFFSET - offset,
														yPos + height - 1 + SHADOW_OFFSET - offset);
			}
		}
	}
}

gr::Interactive *gr::AdvancedButton::clone()
{return new AdvancedButton(*this);}

void gr::AdvancedButton::setBackgroundColor(sf::Color abackgroundColor)
{
	Button::setBackgroundColor(abackgroundColor);
	sprite.setColor(backgroundColor);
}

/* DropdownList */

const float gr::DropdownList::RELATIVE_ARROW_SIZE = 0.7;
const float gr::DropdownList::ANIMATION_TIME = 300;
const long gr::DropdownList::BORDER_THICKNESS = 1;
const float gr::DropdownList::PI = 3.141;

void gr::DropdownList::setup(sf::String aoptions, sf::Font &afont)
{
	/* Setup the title rectangle */
	titleMain.setFillColor(backgroundColor);
	titleMain.setOutlineColor(borderColor);
	titleMain.setOutlineThickness(BORDER_THICKNESS);
	titleMain.setPosition(xPos, yPos);
	titleMain.setSize(sf::Vector2f(width, height));

	/* Setup the title main gradient */
	titleMainGradient[0] = sf::Vertex(sf::Vector2f(xPos, yPos),
								      sf::Color(255, 255, 255, 128));
	titleMainGradient[1] = sf::Vertex(sf::Vector2f(xPos + width - 1, yPos),
									  sf::Color(255, 255, 255, 128));
	titleMainGradient[2] = sf::Vertex(sf::Vector2f(xPos + width - 1, yPos + height - 1),
									  sf::Color(255, 255, 255, 0));
	titleMainGradient[3] = sf::Vertex(sf::Vector2f(xPos, yPos + height - 1),
									  sf::Color(255, 255, 255, 0));

	/* Setup the title text */
	titleText.setFillColor(textColor);
	titleText.setCharacterSize(height * 0.8);
	titleText.setPosition(xPos, yPos);
	
	/* Setup the arrow */
	arrow.setOrigin(height * RELATIVE_ARROW_SIZE / 2, height * RELATIVE_ARROW_SIZE / 2);
	arrow.setFillColor(arrowColor);
	arrow.setOutlineColor(borderColor);
	arrow.setOutlineThickness(BORDER_THICKNESS);
	arrow.setPosition(xPos + width - height / 2, yPos + height / 2);

	/* Memory allocation */
	main = new sf::RectangleShape[numOfOptions];
	text = new sf::Text[numOfOptions];
	mainGradient = new sf::Vertex*[numOfOptions];

	for (long i = 0; i < numOfOptions; i++)
		mainGradient[i] = new sf::Vertex[4];

	/* Options' rectangles, text and gradients setup */
	for (long i = 0; i < numOfOptions; i++)
	{
		/* Setup rectangle */
		main[i].setFillColor(backgroundColor);
		main[i].setOutlineColor(borderColor);
		main[i].setOutlineThickness(BORDER_THICKNESS);
		main[i].setPosition(xPos, yPos);
		main[i].setSize(sf::Vector2f(width, height));

		/* Setup the main gradient */
		mainGradient[i][0] = sf::Vertex(sf::Vector2f(xPos, yPos),
										sf::Color(255, 255, 255, 128));
		mainGradient[i][1] = sf::Vertex(sf::Vector2f(xPos + width - 1, yPos),
										sf::Color(255, 255, 255, 128));
		mainGradient[i][2] = sf::Vertex(sf::Vector2f(xPos + width - 1, yPos + height - 1),
										sf::Color(255, 255, 255, 0));
		mainGradient[i][3] = sf::Vertex(sf::Vector2f(xPos, yPos + height - 1),
										sf::Color(255, 255, 255, 0));

		/* Get position of the '\n'
		symbol and calculate length
		of the substring before it */
		long subLen = aoptions.find('\n');

		/* Get a substring and setup
		an sf::Text class object */
		sf::String subStr;
		subStr = aoptions.substring(0, subLen);
		text[i].setFont(afont);
		text[i].setString(subStr);
		text[i].setCharacterSize(height * 0.8);
		text[i].setFillColor(textColor);
		text[i].setPosition(xPos, yPos);

		/* Delete used substring from source */
		aoptions.erase(0, subLen + 1);
	}
}

gr::DropdownList::DropdownList(long axPos, long ayPos, long awidth, long aheight,
							   long anumOfOptions, sf::String atitle, sf::String aoptions,
							   sf::Font &afont, sf::Color abackgroundColor,
							   sf::Color aborderColor, sf::Color atextColor,
							   sf::Color aselectColor, sf::Color aarrowColor):
	Interactive(axPos, ayPos, awidth, aheight),
	Colored(abackgroundColor, aborderColor, atextColor),
	numOfOptions(anumOfOptions), isMoving(false),
	selectColor(aselectColor), arrowColor(aarrowColor), pressedButtonNum(-1), 
	selectedOptionNum(-1), isHidden(true), isArrowPressed(false), titleText(atitle, afont),
	text(NULL), main(NULL), mainGradient(NULL), arrow(aheight * RELATIVE_ARROW_SIZE / 2, 3),
	isSomeButtonTargeted(false), isArrowMisclicked(false), isOptionMisclicked(false)
{setup(aoptions, afont);}

gr::DropdownList::DropdownList(long axPos, long ayPos, long awidth, long aheight,
							   long anumOfOptions, sf::String atitle, sf::String aoptions,
							   sf::Font &afont):
	Interactive(axPos, ayPos, awidth, aheight),
	Colored(sf::Color(200, 200, 200), sf::Color(64, 64, 64), sf::Color(0, 0, 0)),
	numOfOptions(anumOfOptions), isMoving(false),
	selectColor(41, 235, 56), arrowColor(41, 235, 56), pressedButtonNum(-1),
	selectedOptionNum(-1), isHidden(true), isArrowPressed(false), titleText(atitle, afont),
	text(NULL), main(NULL), mainGradient(NULL), arrow(aheight * RELATIVE_ARROW_SIZE / 2, 3),
	isSomeButtonTargeted(false), isArrowMisclicked(false), isOptionMisclicked(false)
{setup(aoptions, afont);}

gr::DropdownList::DropdownList(DropdownList &right):
	Interactive(right.xPos, right.yPos, right.width, right.height),
	Colored(right.backgroundColor, right.borderColor, right.textColor),
	isHidden(true), isArrowPressed(false), numOfOptions(right.numOfOptions),
	pressedButtonNum(-1), selectedOptionNum(-1), arrow(right.arrow),
	selectColor(right.selectColor), arrowColor(right.arrowColor), titleMain(right.titleMain),
	titleText(right.titleText), isMoving(false), isSomeButtonTargeted(false),
	isOptionMisclicked(false), isArrowMisclicked(false)
{
	/* Memory allocation */
	main = new sf::RectangleShape[numOfOptions];
	text = new sf::Text[numOfOptions];
	mainGradient = new sf::Vertex*[numOfOptions];

	for (long i = 0; i < numOfOptions; i++)
		mainGradient[i] = new sf::Vertex[4];

	/* Copy all data */
	for (long i = 0; i < 4; i++)
		titleMainGradient[i] = right.titleMainGradient[i];

	for (long i = 0; i < numOfOptions; i++)
	{
		main[i] = right.main[i];
		text[i] = right.text[i];
		
		for (long j = 0; j < 4; j++)
			mainGradient[i][j] = right.mainGradient[i][j];
	}
}

gr::DropdownList::~DropdownList()
{
	delete[] main;
	delete[] text;
	
	for (long i = 0; i < numOfOptions; i++)
		delete[] mainGradient[i];

	delete[] mainGradient;
}

void gr::DropdownList::refresh(sf::RenderWindow &window)
{
	sf::Vector2i curPos = sf::Mouse::getPosition(window);

	/* If the object is enabled */
	if (enabled)
	{	
		/* Arrow triggering */
		if (!isArrowPressed)
		{
			/* If this object is targeted or none targeted */
			if (targetedObjectId == id || targetedObjectId == -1)
			{
				if (curPos.x >= xPos && curPos.x <= xPos + width - 1 &&
					curPos.y >= yPos && curPos.y <= yPos + height - 1)
				{
					targetedObjectId = id;

					if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isArrowMisclicked)
					{
						isArrowPressed = true;
					}
		
					if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						isArrowMisclicked = false;
					}
				}
				else
				{
					targetedObjectId = -1;
					isArrowMisclicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
				}
			}
			else
			{
				isArrowMisclicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
			}
		}
		else
		{
			if (curPos.x < xPos || curPos.x > xPos + width - 1 ||
				curPos.y < yPos || curPos.y > yPos + height - 1)
			{
				targetedObjectId = -1;
				isArrowPressed = false;
			}
			else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				isArrowPressed = false;
				isHidden = !isHidden;
				isMoving = true;
				clock.restart();
			}
		}	
		/* List buttons triggering */
		if (!isHidden)
		{
			/* If none of buttons are pressed */
			if (pressedButtonNum == -1)
			{
				/* If this object is targeted or none targeted */
				if (targetedObjectId == id || targetedObjectId == -1)
				{
					/* If cursor is in the zone of buttons */
					if (curPos.x >= xPos && curPos.x <= xPos + width - 1 &&
						curPos.y >= yPos + height &&
						curPos.y <= yPos + height * (numOfOptions + 1) - 1)
					{
						targetedObjectId = id;

						/* If the left mouse button is down and not misclicked */
						if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isOptionMisclicked)
						{
							/* Get pressed button number (first button is designated 0) */
							pressedButtonNum = (curPos.y - (yPos + height)) / height;
						}
						
						if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
						{
							isOptionMisclicked = false;
						}
					}
					else
					{
						targetedObjectId = -1;
						isOptionMisclicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
					}
				}
				else
				{
					isOptionMisclicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
				}
			}
			/* If some button is pressed */
			else 
			{
				targetedObjectId = id;

				/* if the cursor is away from pressed button */
				if (curPos.x < xPos || curPos.x > xPos + width - 1 ||
					 curPos.y < yPos + height * (pressedButtonNum + 1) ||
					 curPos.y > yPos + height * (pressedButtonNum + 2) - 1)
				{
					/* Reset the pressed button */
					pressedButtonNum = -1;
				}
				/* If the cursor is on the button and the left mouse key is up */
				else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					/* Change the color of previous selected button */
					if (selectedOptionNum != -1)
						main[selectedOptionNum].setFillColor(backgroundColor);
	
					/* Change the selected option number */
					selectedOptionNum = pressedButtonNum;
	
					/* Change the color of selected button */
					main[selectedOptionNum].setFillColor(selectColor);
	
					/* Change the text of the title */
					titleText.setString(text[selectedOptionNum].getString());
	
					/* Reset the pressed button */
					pressedButtonNum = -1;
				}
			}
		}
	}
	/* If the drop-down list is disabled and none of the objects are targeted */
	else if (targetedObjectId == -1)
	{
		sf::Cursor cursor;
		bool isObjectTargeted;

		/* Select a condition of object targeting */
		if (isHidden)
		{
			isObjectTargeted = curPos.x >= xPos && curPos.x <= xPos + width - 1 &&
				curPos.y >= yPos && curPos.y <= yPos + height - 1;
		}
		else
		{
			isObjectTargeted = curPos.x >= xPos && curPos.x <= xPos + width - 1 &&
				curPos.y >= yPos && curPos.y <= yPos + height * (numOfOptions + 1) - 1;
		}

		if (isObjectTargeted)
		{
			if (!isSomeButtonTargeted)
			{
				isSomeButtonTargeted = true;
				cursor.loadFromSystem(sf::Cursor::NotAllowed);
				window.setMouseCursor(cursor);
			}
		}
		else
		{
			if (isSomeButtonTargeted)
			{
				isSomeButtonTargeted = false;
				cursor.loadFromSystem(sf::Cursor::Arrow);
				window.setMouseCursor(cursor);
			}
		}
	}
	
	/* Animation stuff */
	long elapsedTime = clock.getElapsedTime().asMilliseconds();

	/* Check if animation is over */
	if (elapsedTime > ANIMATION_TIME)
		isMoving = false;

	/* If animation is over */
	if (!isMoving)
	{
		if (isHidden)
		{
			for (long i = 0; i < numOfOptions; i++)
			{
				main[i].setPosition(xPos, yPos);
				text[i].setPosition(xPos, yPos);
				arrow.setRotation(0);

				mainGradient[i][0] = sf::Vertex(sf::Vector2f(xPos, yPos),
												sf::Color(255, 255, 255, 128));
				mainGradient[i][1] = sf::Vertex(sf::Vector2f(xPos + width - 1, yPos),
												sf::Color(255, 255, 255, 128));
				mainGradient[i][2] = sf::Vertex(sf::Vector2f(xPos + width - 1,
												             yPos + height - 1),
												sf::Color(255, 255, 255, 0));
				mainGradient[i][3] = sf::Vertex(sf::Vector2f(xPos,
												             yPos + height -1),
												sf::Color(255, 255, 255, 0));
			}
		}
		else
		{
			for (long i = 0; i < numOfOptions; i++)
			{
				main[i].setPosition(xPos, yPos + height * (i + 1));
				text[i].setPosition(xPos, yPos + height * (i + 1));
				arrow.setRotation(180);

				mainGradient[i][0] = sf::Vertex(sf::Vector2f(xPos,
												yPos + height * (i + 1)),
												sf::Color(255, 255, 255, 128));
				mainGradient[i][1] = sf::Vertex(sf::Vector2f(xPos + width - 1,
												yPos + height * (i + 1)),
												sf::Color(255, 255, 255, 128));
				mainGradient[i][2] = sf::Vertex(sf::Vector2f(xPos + width - 1,
												yPos + height * (i + 2) - 1),
												sf::Color(255, 255, 255, 0));
				mainGradient[i][3] = sf::Vertex(sf::Vector2f(xPos,
												yPos + height * (i + 2) - 1),
												sf::Color(255, 255, 255, 0));
			}
		}
	}
	/* If animation is still running */
	else
	{
		float phase = (PI / 2) * (elapsedTime / ANIMATION_TIME);
		float progress = sin(phase);

		if (isHidden)
		{
			float leftAngle = 180 * (1 - progress);
			arrow.setRotation(-leftAngle);

			for (long i = 0; i < numOfOptions; i++)
			{
				/* Calculate the passed distance */
				long leftDist = height * (i + 1) * (1 - progress);

				/* Set objects' position */
				main[i].setPosition(xPos, yPos + leftDist);
				text[i].setPosition(xPos, yPos + leftDist);

				mainGradient[i][0] = sf::Vertex(sf::Vector2f(xPos,
												yPos + leftDist),
												sf::Color(255, 255, 255, 128));
				mainGradient[i][1] = sf::Vertex(sf::Vector2f(xPos + width - 1,
												yPos + leftDist),
												sf::Color(255, 255, 255, 128));
				mainGradient[i][2] = sf::Vertex(sf::Vector2f(xPos + width - 1,
												yPos + height - 1 + leftDist),
												sf::Color(255, 255, 255, 0));
				mainGradient[i][3] = sf::Vertex(sf::Vector2f(xPos,
												yPos + height - 1 + leftDist),
												sf::Color(255, 255, 255, 0));
			}
		}
		else
		{
			float passedAngle = 180 * progress;
			arrow.setRotation(-passedAngle);

			for (long i = 0; i < numOfOptions; i++)
			{
				/* Calculate the passed distance */
				long passedDist = height * (i + 1) * progress;

				/* Set objects' position */
				main[i].setPosition(xPos, yPos + passedDist);
				text[i].setPosition(xPos, yPos + passedDist);

				mainGradient[i][0] = sf::Vertex(sf::Vector2f(xPos,
												yPos + passedDist),
												sf::Color(255, 255, 255, 128));
				mainGradient[i][1] = sf::Vertex(sf::Vector2f(xPos + width - 1,
												yPos + passedDist),
												sf::Color(255, 255, 255, 128));
				mainGradient[i][2] = sf::Vertex(sf::Vector2f(xPos + width - 1,
												yPos + height - 1 + passedDist),
												sf::Color(255, 255, 255, 0));
				mainGradient[i][3] = sf::Vertex(sf::Vector2f(xPos,
												yPos + height - 1 + passedDist),
												sf::Color(255, 255, 255, 0));
			}
		}
	}
}

void gr::DropdownList::render(sf::RenderWindow &window) const
{
	/* If option buttons are visible */
	if (!isHidden || isMoving)
	{
		/* Render them */
		for (long i = numOfOptions - 1; i >= 0; i--)
		{
			window.draw(main[i]);
			window.draw(mainGradient[i], 4, sf::Quads);
			window.draw(text[i]);
		}
	}

	/* Render the title and the arrow */
	window.draw(titleMain);
	window.draw(titleMainGradient, 4, sf::Quads);
	window.draw(titleText);
	window.draw(arrow);
}

gr::Interactive *gr::DropdownList::clone()
{return new DropdownList(*this);}

void gr::DropdownList::setSelectedOption(long newOptionNum)
{
	/* Change the color of previous selected button */
	if (selectedOptionNum != -1)
		main[selectedOptionNum].setFillColor(backgroundColor);

	/* Change the selected option number */
	selectedOptionNum = newOptionNum;

	/* Change the color of selected button */
	main[selectedOptionNum].setFillColor(selectColor);

	/* Change the text of the title */
	titleText.setString(text[selectedOptionNum].getString());
}

void gr::DropdownList::setBackgroundColor(sf::Color abackgroundColor)
{
	Colored::setBackgroundColor(abackgroundColor);

	for (long i = 0; i < numOfOptions; i++)
	{
		if (i != selectedOptionNum)
		{
			main[i].setFillColor(backgroundColor);
		}
	}

	titleMain.setFillColor(backgroundColor);
}

void gr::DropdownList::setBorderColor(sf::Color aborderColor)
{
	Colored::setBorderColor(aborderColor);

	for (long i = 0; i < numOfOptions; i++)
	{
		main[i].setOutlineColor(borderColor);
	}

	titleMain.setOutlineColor(borderColor);
	arrow.setOutlineColor(borderColor);
}

void gr::DropdownList::setTextColor(sf::Color atextColor)
{
	Colored::setTextColor(atextColor);

	for (long i = 0; i < numOfOptions; i++)
	{
		text[i].setFillColor(textColor);
	}

	titleText.setFillColor(textColor);
}

void gr::DropdownList::setArrowColor(sf::Color aarrowColor)
{
	arrowColor = aarrowColor;
	arrow.setFillColor(arrowColor);
}

void gr::DropdownList::setSelectColor(sf::Color aselectColor)
{
	selectColor = aselectColor;
	main[selectedOptionNum].setFillColor(selectColor);
}

void gr::DropdownList::reset()
{
	Interactive::reset();
	isHidden = true;
	isArrowPressed = false;
	isMoving = false;
	isSomeButtonTargeted = false;
	isArrowMisclicked = false;
	isOptionMisclicked = false;
	pressedButtonNum = -1;
}

/* Slider */

const long gr::Slider::LINE_THICKNESS = 5;
const long gr::Slider::ARROW_RADIUS = 10;
const long gr::Slider::ARROW_BORDER_THICKNESS = 1;
const long gr::Slider::CHARACTER_SIZE = 16;

void gr::Slider::setup(sf::Font &afont)
{
	/* Arrow setup */
	arrow.setRotation(180);
	arrow.setOrigin(ARROW_RADIUS, ARROW_RADIUS);
	arrow.setPosition(xPos, yPos + height / 2);
	arrow.setFillColor(arrowColor);
	arrow.setOutlineColor(borderColor);
	arrow.setOutlineThickness(ARROW_BORDER_THICKNESS);

	/* Background rectangle setup */
	main.setPosition(xPos, yPos);
	main.setSize(sf::Vector2f(width, height));
	main.setOutlineThickness(0);
	main.setFillColor(backgroundColor);

	/* Fill rectangle setup */
	fill.setPosition(xPos, yPos);
	fill.setSize(sf::Vector2f(0, 0));
	fill.setOutlineThickness(0);
	fill.setFillColor(fillColor);

	/* Gradient setup */
	mainGradient[0] = sf::Vertex(sf::Vector2f(xPos, yPos),
								 sf::Color(255, 255, 255, 128));
	mainGradient[1] = sf::Vertex(sf::Vector2f(xPos + width - 1, yPos),
								 sf::Color(255, 255, 255, 128));
	mainGradient[2] = sf::Vertex(sf::Vector2f(xPos + width - 1, yPos + height - 1),
								 sf::Color(255, 255, 255, 0));
	mainGradient[3] = sf::Vertex(sf::Vector2f(xPos, yPos + height - 1),
								 sf::Color(255, 255, 255, 0));

	/* Text setup */
	std::ostringstream memLeft, memRight;
	sf::String leftString, rightString;

	memLeft << left;
	memRight << right;

	leftString = memLeft.str();
	rightString = memRight.str();

	leftText.setFont(afont);
	leftText.setString(leftString);
	leftText.setCharacterSize(CHARACTER_SIZE);
	leftText.setFillColor(textColor);
	leftText.setPosition(xPos, yPos + height);

	rightText.setFont(afont);
	rightText.setString(rightString);
	rightText.setCharacterSize(CHARACTER_SIZE);
	rightText.setFillColor(textColor);
	rightText.setPosition(xPos + width, yPos + height);

	valueText.setFont(afont);
	valueText.setString(leftString);
	valueText.setCharacterSize(CHARACTER_SIZE);
	valueText.setFillColor(textColor);
	sf::FloatRect gbounds = valueText.getGlobalBounds();
	valueText.setPosition(xPos + width / 2 - gbounds.width / 2, yPos + height);

	gbounds = rightText.getGlobalBounds();
	rightText.move(-gbounds.width, 0);
}

gr::Slider::Slider(long axPos, long ayPos, long awidth, float aleft, float aright,
	               float aprecision, sf::Font &afont, sf::Color abackgroundColor,
				   sf::Color afillColor, sf::Color aarrowColor, sf::Color aborderColor,
				   sf::Color atextColor, long amode):
	Interactive(axPos, ayPos, awidth, LINE_THICKNESS),
	Colored(abackgroundColor, aborderColor, atextColor),
	left(aleft), right(aright), precision(aprecision), mode(Mode(amode)),
	isSeized(false), isMisclicked(false), value(left), arrow(ARROW_RADIUS, 3),
	fillColor(afillColor), arrowColor(aarrowColor), isTargeted(false)
{setup(afont);}

gr::Slider::Slider(long axPos, long ayPos, long awidth, float aleft, float aright,
				   float aprecision, sf::Font &afont, long amode):
	Interactive(axPos, ayPos, awidth, LINE_THICKNESS),
	Colored(sf::Color(128, 128, 128), sf::Color(64, 64, 64), sf::Color(0, 0, 0)),
	left(aleft), right(aright), precision(aprecision), mode(Mode(amode)),
	isSeized(false), isMisclicked(false), value(left), arrow(ARROW_RADIUS, 3),
	fillColor(0, 238, 0), arrowColor(41, 235, 56), isTargeted(false)
{setup(afont);}

gr::Slider::Slider(Slider &r):
	Interactive(r.xPos, r.yPos, r.width, r.height),
	Colored(r.backgroundColor, r.borderColor, r.textColor),
	mode(r.mode), isSeized(false), isMisclicked(false), left(r.left), right(r.right),
	value(r.value), precision(r.precision), fillColor(r.fillColor), arrowColor(r.arrowColor),
	arrow(r.arrow), main(r.main), fill(r.fill),	leftText(r.leftText), rightText(r.rightText),
	isTargeted(false)
{
	for (long i = 0; i < 4; i++)
		mainGradient[i] = r.mainGradient[i];
}

void gr::Slider::refresh(sf::RenderWindow &window)
{
	sf::Vector2i curPos = sf::Mouse::getPosition(window);

	/* If the slider is enabled */
	if (enabled)
	{	
		if (!isSeized)
		{
			/* If this object is targeted of none are targeted */
			if (targetedObjectId == id || targetedObjectId == -1)
			{
				/* If the cursor is on the slider or on the line */
				if (curPos.x >= xPos && curPos.x <= xPos + width - 1 &&
					curPos.y >= yPos && curPos.y <= yPos + height - 1 ||
					std::abs(curPos.x - arrow.getPosition().x) *
					std::abs(curPos.x - arrow.getPosition().x) +
					std::abs(curPos.y - arrow.getPosition().y) *
					std::abs(curPos.y - arrow.getPosition().y) <=
					ARROW_RADIUS * ARROW_RADIUS)
				{
					targetedObjectId = id;

					if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMisclicked)
					{
						isSeized = true;
					}
					
					if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						isMisclicked = false;
					}
				}
				else
				{
					targetedObjectId = -1;
					isMisclicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
				}
			}
			else
			{
				isMisclicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
			}
		}
		else
		{
			if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				isSeized = false;
			}
		}
	
		/* Position calculating */
		if (isSeized)
		{
			if (curPos.x < xPos)
			{
				value = left;
				arrow.setPosition(xPos, yPos + height / 2);
				fill.setSize(sf::Vector2f(0, height));
			}
			else if (curPos.x > xPos + width - 1)
			{
				value = right;
				arrow.setPosition(xPos + width - 1, yPos + height / 2);
				fill.setSize(sf::Vector2f(width, height));
			}
			else
			{
				if (MaxPresicion & mode)
				{
					/* Position calculating */
					float relativePosX = curPos.x - xPos;
					arrow.setPosition(curPos.x, yPos + height / 2);
					fill.setSize(sf::Vector2f(relativePosX, height));
	
					/* Value calculating */
					value = left + (right - left) * relativePosX / width;
				}
				else
				{
					/* Position calculating */
					float sectorWidth = width * (precision / (right - left));
					float relativePosX = curPos.x - xPos;
					float relativeArrowPosX = std::round(relativePosX / sectorWidth) * sectorWidth;
					arrow.setPosition(xPos + relativeArrowPosX, yPos + height / 2);
					fill.setSize(sf::Vector2f(relativeArrowPosX, height));
	
					/* Value calculating */
					value = left + (right - left) * relativeArrowPosX / width;

					//std::cout << "relativeArrowPosX" << relativeArrowPosX << std::endl;
					//std::cout << "curPos.x " << curPos.x << std::endl;
					//std::cout << "sectorWidth " << sectorWidth << std::endl;
				}
			}

			valueText.setString(str(getValue()));
			sf::FloatRect gbounds = valueText.getGlobalBounds();
			valueText.setPosition(xPos + width / 2 - gbounds.width / 2, yPos + height);
		}
	}
	/* If the slider is disabled and none of the objects are targeted */
	else if (targetedObjectId == -1)
	{
		sf::Cursor cursor;

		if (curPos.x >= xPos && curPos.x <= xPos + width - 1 &&
			curPos.y >= yPos && curPos.y <= yPos + height - 1 ||
			std::abs(curPos.x - arrow.getPosition().x) *
			std::abs(curPos.x - arrow.getPosition().x) +
			std::abs(curPos.y - arrow.getPosition().y) *
			std::abs(curPos.y - arrow.getPosition().y) <=
			ARROW_RADIUS * ARROW_RADIUS)
		{
			if (!isTargeted)
			{
				isTargeted = true;
				cursor.loadFromSystem(sf::Cursor::NotAllowed);
				window.setMouseCursor(cursor);
			}
		}
		else
		{
			if (isTargeted)
			{
				isTargeted = false;
				cursor.loadFromSystem(sf::Cursor::Arrow);
				window.setMouseCursor(cursor);
			}
		}
	}
}

void gr::Slider::render(sf::RenderWindow &window) const
{
	window.draw(main);
	window.draw(fill);
	window.draw(mainGradient, 4, sf::Quads);
	window.draw(arrow);
	window.draw(leftText);
	window.draw(rightText);
	window.draw(valueText);
}

gr::Interactive *gr::Slider::clone()
{return new Slider(*this);}

void gr::Slider::setValue(float newValue)
{
	double visualValue = value = newValue;

	if (value < left)
	{
		visualValue = left;
	}
	else if (value > right)
	{
		visualValue = right;
	}

	arrow.setPosition(xPos + (visualValue - left) / (right - left) * width, arrow.getPosition().y);
	fill.setSize(sf::Vector2f((visualValue - left) / (right - left) * width, height));
	valueText.setString(str(getValue()));
	sf::FloatRect gbounds = valueText.getGlobalBounds();
	valueText.setPosition(xPos + width / 2 - gbounds.width / 2, yPos + height);
}

void gr::Slider::setBackgroundColor(sf::Color abackgroundColor)
{
	Colored::setBackgroundColor(abackgroundColor);
	main.setFillColor(backgroundColor);
}

void gr::Slider::setBorderColor(sf::Color aborderColor)
{
	Colored::setBorderColor(aborderColor);
	arrow.setOutlineColor(aborderColor);
}

void gr::Slider::setTextColor(sf::Color atextColor)
{
	Colored::setTextColor(atextColor);
	leftText.setFillColor(textColor);
	rightText.setFillColor(textColor);
}

void gr::Slider::setArrowColor(sf::Color aarrowColor)
{
	arrowColor = aarrowColor;
	arrow.setFillColor(arrowColor);
}

void gr::Slider::setFillColor(sf::Color afillColor)
{
	fillColor = afillColor;
	fill.setFillColor(fillColor);
}

void gr::Slider::reset()
{
	Interactive::reset();
	isSeized = false;
	isMisclicked = false;
	isTargeted = false;
}

/* ScrollBar */

const long gr::ScrollBar::INDENT = 3;
const long gr::ScrollBar::ANIMATION_TIME = 300;
const long gr::ScrollBar::SCROLL_STEP = 50;
const long gr::ScrollBar::SCROLL_STEP_TIME = 10;
const long gr::ScrollBar::SCROLL_START_DELAY = 500;
const float gr::ScrollBar::RELATIVE_ARROW_SIZE = 0.2;

void gr::ScrollBar::setup()
{
	barWidth = orientation ? width - 2 * INDENT : (width - 2 * (INDENT + 1)) * (width - 2 * (INDENT + 1)) /
		scrollDistance;
	barHeight = orientation ? (height - 2 * (INDENT)) * (height - 2 * (INDENT)) /
		scrollDistance : height - 2 * INDENT;

	main.setSize(sf::Vector2f(width, height));
	main.setFillColor(backgroundColor);
	main.setOutlineColor(borderColor);
	main.setOutlineThickness(1);
	main.setPosition(sf::Vector2f(xPos, yPos));

	bar.setSize(sf::Vector2f(barWidth, barHeight));
	bar.setFillColor(barColor);
	bar.setOutlineColor(sf::Color(64, 64, 64));
	bar.setOutlineThickness(1);
	bar.setPosition(orientation ? sf::Vector2f(xPos + INDENT, yPos + relativeBarPos) :
					sf::Vector2f(xPos + relativeBarPos, yPos + INDENT));

	if (orientation == false)  // If the scrollbar is horizontal
	{
		mainGradient[0] = sf::Vertex(sf::Vector2f(xPos, yPos),
									 sf::Color(255, 255, 255, 128));
		mainGradient[1] = sf::Vertex(sf::Vector2f(xPos + width - 1, yPos),
									 sf::Color(255, 255, 255, 128));
		mainGradient[2] = sf::Vertex(sf::Vector2f(xPos + width - 1, yPos + height - 1),
									 sf::Color(255, 255, 255, 0));
		mainGradient[3] = sf::Vertex(sf::Vector2f(xPos, yPos + height - 1),
									 sf::Color(255, 255, 255, 0));

		long barX = bar.getPosition().x;
		long barY = bar.getPosition().y;

		barGradient[0] = sf::Vertex(sf::Vector2f(barX, barY),
									sf::Color(255, 255, 255, 128));
		barGradient[1] = sf::Vertex(sf::Vector2f(barX + barWidth - 1, barY),
									sf::Color(255, 255, 255, 128));
		barGradient[2] = sf::Vertex(sf::Vector2f(barX + barWidth - 1, barY + barHeight - 1),
									sf::Color(255, 255, 255, 0));
		barGradient[3] = sf::Vertex(sf::Vector2f(barX, barY + barHeight - 1),
									sf::Color(255, 255, 255, 0));
	}
	else  // If the scrollbar is vertical
	{
		mainGradient[0] = sf::Vertex(sf::Vector2f(xPos, yPos),
									 sf::Color(255, 255, 255, 128));
		mainGradient[1] = sf::Vertex(sf::Vector2f(xPos + width - 1, yPos),
									 sf::Color(255, 255, 255, 0));
		mainGradient[2] = sf::Vertex(sf::Vector2f(xPos + width - 1, yPos + height - 1),
									 sf::Color(255, 255, 255, 0));
		mainGradient[3] = sf::Vertex(sf::Vector2f(xPos, yPos + height - 1),
									 sf::Color(255, 255, 255, 128));

		long barX = bar.getPosition().x;
		long barY = bar.getPosition().y;

		barGradient[0] = sf::Vertex(sf::Vector2f(barX, barY),
									sf::Color(255, 255, 255, 128));
		barGradient[1] = sf::Vertex(sf::Vector2f(barX + barWidth - 1, barY),
									sf::Color(255, 255, 255, 0));
		barGradient[2] = sf::Vertex(sf::Vector2f(barX + barWidth - 1, barY + barHeight - 1),
									sf::Color(255, 255, 255, 0));
		barGradient[3] = sf::Vertex(sf::Vector2f(barX, barY + barHeight - 1),
									sf::Color(255, 255, 255, 128));
	}
}

gr::ScrollBar::ScrollBar(long x, long y, long w, long h, float sd, bool orient):
	gr::Interactive(x, y, w, h),
	scrollDistance(sd), relativeBarPos(INDENT + 1),
	gr::Colored(sf::Color(128, 128, 128), sf::Color(64, 64, 64), sf::Color()),
	barColor(sf::Color(200, 200, 200)), barBorderColor(sf::Color(64, 64, 64)),
	barSelectColor(sf::Color(41, 235, 56)), isSeized(false), isMisclicked(false),
	isTargeted(false), isFading(false), orientation(orient), isBarTargeted(false),
	relativeCurX(-1), relativeCurY(-1), isScrollingStarted(false), isScrolling(false),
	docPosition(0)
{setup();}

gr::ScrollBar::ScrollBar(ScrollBar &r):
	Interactive(r.xPos, r.yPos, r.width, r.height),
	Colored(r.backgroundColor, r.borderColor, r.textColor),
	main(r.main), bar(r.bar), barColor(r.barColor), barBorderColor(r.barBorderColor),
	barSelectColor(r.barSelectColor), scrollDistance(r.scrollDistance), barWidth(r.barWidth),
	barHeight(r.barHeight), relativeBarPos(r.relativeBarPos), orientation(r.orientation),
	isSeized(false), isMisclicked(false), isTargeted(false), isFading(false),
	isBarTargeted(false), relativeCurX(-1), relativeCurY(-1), isScrollingStarted(false),
	isScrolling(false), docPosition(r.docPosition)
{
	for (int i = 0; i < 4; i++)
	{
		mainGradient[i] = r.mainGradient[i];
		barGradient[i] = r.barGradient[i];
	}
}

void gr::ScrollBar::render(sf::RenderWindow &window) const
{
	window.draw(main);
	//window.draw(mainGradient, 4, sf::Quads);
	window.draw(bar);
	window.draw(barGradient, 4, sf::Quads);
}

void gr::ScrollBar::refresh(sf::RenderWindow &window)
{
	sf::Vector2i curPos = sf::Mouse::getPosition(window);

	if (enabled)
	{
		if (!isSeized)
		{
			if (targetedObjectId == id || targetedObjectId == -1)
			{
				// The the cursor is in the area of the object
				if (curPos.x >= xPos && curPos.x < xPos + width &&
					curPos.y >= yPos && curPos.y < yPos + height)
				{
					targetedObjectId = id;

					if (!isTargeted)
					{
						isTargeted = true;
					}
				}
				else
				{
					targetedObjectId = -1;
				}

				// If the cursor is on the bar
				if (curPos.x >= bar.getPosition().x && curPos.x < bar.getPosition().x + barWidth &&
					curPos.y >= bar.getPosition().y && curPos.y < bar.getPosition().y + barHeight)
				{
					if (!isBarTargeted)
					{
						isBarTargeted = true;
						isFading = true;
						fadeClock.restart();
					}

					if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMisclicked)
					{
						isSeized = true;
						relativeCurX = curPos.x - bar.getPosition().x;
						relativeCurY = curPos.y - bar.getPosition().y;
					}

					if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						isMisclicked = false;
					}
				}
				else
				{
					isMisclicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);

					if (isBarTargeted)
					{
						isBarTargeted = false;
						isFading = true;
						fadeClock.restart();
					}
				}
			}
			else
			{
				isMisclicked = sf::Mouse::isButtonPressed(sf::Mouse::Left);
			}
		}
		else
		{
			if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				isSeized = false;

				// If the cursor is away from the scrollbar
				if (curPos.x < bar.getPosition().x || curPos.x >= bar.getPosition().x + barWidth ||
					curPos.y < bar.getPosition().y || curPos.y >= bar.getPosition().y + barHeight)
				{
					isBarTargeted = false;
					isFading = true;
					fadeClock.restart();

					// If the cursor is away from the object
					if (curPos.x < xPos || curPos.x >= xPos + width ||
						curPos.y < yPos || curPos.y >= yPos + height)
					{
						targetedObjectId = -1;
						isTargeted = false;
					}
				}
			}
		}

		updateColor();
	}
}

void gr::ScrollBar::updateColor()
{
	float time = fadeClock.getElapsedTime().asMilliseconds();

	if (isFading)  // If animation is running
	{
		if (time > ANIMATION_TIME)  // If animation is over
		{
			isFading = false;  // Stop it
			bar.setFillColor(isBarTargeted ? barSelectColor : barColor);
		}
	}

	if (isFading)  // If animation is running
	{
		float progress = time / ANIMATION_TIME;

		if (isBarTargeted)
		{
			bar.setFillColor(blend(barColor, barSelectColor, progress));
		}
		else
		{
			bar.setFillColor(blend(barSelectColor, barColor, progress));
		}
	}
}

void gr::ScrollBar::reset()
{
	Interactive::reset();
	isSeized = false;
	isMisclicked = false;
	isTargeted = false;
	isFading = false;
}

void gr::ScrollBar::setBarPosition(long x, long y)
{
	bar.setPosition(x, y);

	barGradient[0].position = sf::Vector2f(x, y);
	barGradient[1].position = sf::Vector2f(x + barWidth - 1, y);
	barGradient[2].position = sf::Vector2f(x + barWidth - 1, y + barHeight - 1);
	barGradient[3].position = sf::Vector2f(x, y + barHeight - 1);
}

void gr::ScrollBar::setDocumentPosition(long newDocPos)
{
	docPosition = newDocPos;

	if (docPosition < 0)
	{
		docPosition = 0;
	}
	else if (docPosition > scrollDistance)
	{
		docPosition = scrollDistance;
	}
}

/* VerticalScrollBar */

void gr::VerticalScrollBar::setup(float arrow_radius)
{
	arrowUp.setOutlineThickness(0);
	arrowUp.setFillColor(sf::Color(64, 64, 64));
	arrowUp.setOrigin(arrow_radius, arrow_radius);
	arrowUp.setPosition(xPos + width / 2, yPos - width / 2);
	arrowUp.setRotation(0); 
	arrowUp.setScale(1.5, 1);

	arrowDown.setOutlineThickness(0);
	arrowDown.setOrigin(arrow_radius, arrow_radius);
	arrowDown.setFillColor(sf::Color(64, 64, 64));
	arrowDown.setPosition(xPos + width / 2, yPos + height + width / 2);
	arrowDown.setRotation(180); 
	arrowDown.setScale(1.5, 1);
}

gr::VerticalScrollBar::VerticalScrollBar(long x, long y, long l, long w, float sd, sf::Font &f):
	ScrollBar(x, y + w, w, l - 2 * w, sd, Vertical),
	scrollUp(x, y, w, w, "", f, sf::Color(128, 128, 128), sf::Color(64, 64, 64),
			 sf::Color(64, 64, 64), sf::Color(41, 235, 56), useless,
			 gr::AdvancedButton::NoOffset | gr::AdvancedButton::NoShadow |
			 gr::AdvancedButton::NoTexture | gr::AdvancedButton::NoGradient),
	scrollDown(x, y + w + height, w, w, "", f, sf::Color(128, 128, 128), sf::Color(64, 64, 64),
			   sf::Color(64, 64, 64), sf::Color(41, 235, 56), useless,
			   gr::AdvancedButton::NoOffset | gr::AdvancedButton::NoShadow |
			   gr::AdvancedButton::NoTexture | gr::AdvancedButton::NoGradient),
	arrowUp(w * RELATIVE_ARROW_SIZE, 3),
	arrowDown(w * RELATIVE_ARROW_SIZE, 3)
{
	setup(w * RELATIVE_ARROW_SIZE);
}

gr::VerticalScrollBar::VerticalScrollBar(VerticalScrollBar &r):
	ScrollBar(r),
	scrollUp(r.scrollUp), scrollDown(r.scrollDown), arrowUp(r.arrowUp), arrowDown(r.arrowDown)
{}

gr::Interactive *gr::VerticalScrollBar::clone()
{return new VerticalScrollBar(*this);}

void gr::VerticalScrollBar::refresh(sf::RenderWindow &window)
{
	ScrollBar::refresh(window);  // Detect the scrollbar brabbing
	scrollUp.refresh(window);  // Refresh buttons
	scrollDown.refresh(window);

	if (scrollUp.is_pressed())
	{
		if (!isScrolling)
		{
			if (!isScrollingStarted)
			{
				moveDocument(-SCROLL_STEP);
				isScrollingStarted = true;
				scrollStartClock.restart();
			}

			if (scrollStartClock.getElapsedTime().asMilliseconds() > SCROLL_START_DELAY)
			{
				isScrolling = true;
				isScrollingStarted = false;
			}
		}

		if (isScrolling && scrollClock.getElapsedTime().asMilliseconds() > SCROLL_STEP_TIME)
		{
			scrollClock.restart();
			moveDocument(-SCROLL_STEP);			
		}
	}

	if (scrollDown.is_pressed())
	{
		if (!isScrolling)
		{
			if (!isScrollingStarted)
			{
				moveDocument(SCROLL_STEP);
				isScrollingStarted = true;
				scrollStartClock.restart();
			}

			if (scrollStartClock.getElapsedTime().asMilliseconds() > SCROLL_START_DELAY)
			{
				isScrolling = true;
				isScrollingStarted = false;
			}
		}

		if (isScrolling && scrollClock.getElapsedTime().asMilliseconds() > SCROLL_STEP_TIME)
		{
			scrollClock.restart();
			moveDocument(SCROLL_STEP);
		}
	}

	if (!scrollDown.is_pressed() && !scrollUp.is_pressed())
	{
		isScrollingStarted = false;
		isScrolling = false;
	}

	if (isSeized)  // Bar dragging
	{
		sf::Vector2i curPos = sf::Mouse::getPosition(window);
		setBarAbsolutePosition(curPos.y - relativeCurY);
		updateDocPosition();
	}
}

void gr::VerticalScrollBar::render(sf::RenderWindow &window) const
{
	ScrollBar::render(window);  // Render the scrollbar
	scrollUp.render(window);	// Render buttons
	scrollDown.render(window);  
	window.draw(arrowUp);  // Render arrows
	window.draw(arrowDown);
}

void gr::VerticalScrollBar::updateDocPosition()
{
	docPosition = scrollDistance * (bar.getPosition().y - yPos - INDENT - 1) / (height - 2 * INDENT - barHeight);
}

long gr::VerticalScrollBar::getBarRelativePosition()
{
	return docPosition / scrollDistance * (height - 2 * INDENT - barHeight);
}

void gr::VerticalScrollBar::setDocumentPosition(long newDocPos)
{
	ScrollBar::setDocumentPosition(newDocPos);
	long newBarYPos = getBarRelativePosition() + yPos + INDENT + 1;
	//std::cout << docPosition << std::endl;
	setBarAbsolutePosition(newBarYPos);
}

void gr::VerticalScrollBar::moveDocument(long offset)
{
	setDocumentPosition(docPosition + offset);
}

void gr::VerticalScrollBar::setBarAbsolutePosition(long absolutePos)
{
	if (absolutePos < yPos + INDENT + 1)
	{
		setBarPosition(bar.getPosition().x, yPos + INDENT + 1);
	}
	else if (absolutePos >= yPos + height - INDENT - 1 - barHeight)
	{
		setBarPosition(bar.getPosition().x, yPos + height - barHeight - INDENT - 1);
	}
	else
	{
		setBarPosition(bar.getPosition().x, absolutePos);
	}
}

/* Horizontal scrollbar */

void gr::HorizontalScrollBar::setup(float arrow_radius)
{
	arrowLeft.setOutlineThickness(0);
	arrowLeft.setFillColor(sf::Color(64, 64, 64));
	arrowLeft.setOrigin(arrow_radius, arrow_radius);
	arrowLeft.setPosition(xPos - height / 2, yPos + height / 2);
	arrowLeft.setRotation(-90); 
	arrowLeft.setScale(1.5, 1);

	arrowRight.setOutlineThickness(0);
	arrowRight.setOrigin(arrow_radius, arrow_radius);
	arrowRight.setFillColor(sf::Color(64, 64, 64));
	arrowRight.setPosition(xPos + width + height / 2, yPos + height / 2);
	arrowRight.setRotation(90); 
	arrowRight.setScale(1.5, 1);
}

gr::HorizontalScrollBar::HorizontalScrollBar(long x, long y, long l, long w, float sd, sf::Font &f):
	ScrollBar(x + w, y, l - 2 * w, w, sd, Horizontal),

	scrollLeft(x, y, w, w, "", f, sf::Color(128, 128, 128), sf::Color(64, 64, 64),
			 sf::Color(64, 64, 64), sf::Color(41, 235, 56), useless,
			 gr::AdvancedButton::NoOffset | gr::AdvancedButton::NoShadow |
			 gr::AdvancedButton::NoTexture | gr::AdvancedButton::NoGradient),
	scrollRight(x + w + width,y,w,w, "", f,sf::Color(128,128,128),sf::Color(64,64,64),
			   sf::Color(64, 64, 64), sf::Color(41, 235, 56), useless,
			   gr::AdvancedButton::NoOffset | gr::AdvancedButton::NoShadow |
			   gr::AdvancedButton::NoTexture | gr::AdvancedButton::NoGradient),
	arrowLeft(w * RELATIVE_ARROW_SIZE, 3), arrowRight(w * RELATIVE_ARROW_SIZE, 3)
{
	setup(w * RELATIVE_ARROW_SIZE);
}

gr::HorizontalScrollBar::HorizontalScrollBar(HorizontalScrollBar &r):
	ScrollBar(r),
	scrollLeft(r.scrollLeft), scrollRight(r.scrollRight)
{}

gr::Interactive *gr::HorizontalScrollBar::clone()
{return new HorizontalScrollBar(*this);}

void gr::HorizontalScrollBar::refresh(sf::RenderWindow &window)
{
	ScrollBar::refresh(window);  // Detect the scrollbar brabbing
	scrollLeft.refresh(window);  // Refresh buttons
	scrollRight.refresh(window);

	if (scrollLeft.is_pressed())
	{
		if (!isScrolling)
		{
			if (!isScrollingStarted)
			{
				moveDocument(-SCROLL_STEP);
				isScrollingStarted = true;
				scrollStartClock.restart();
			}

			if (scrollStartClock.getElapsedTime().asMilliseconds() > SCROLL_START_DELAY)
			{
				isScrolling = true;
				isScrollingStarted = false;
			}
		}

		if (isScrolling && scrollClock.getElapsedTime().asMilliseconds() > SCROLL_STEP_TIME)
		{
			scrollClock.restart();
			moveDocument(-SCROLL_STEP);			
		}
	}

	if (scrollRight.is_pressed())
	{
		if (!isScrolling)
		{
			if (!isScrollingStarted)
			{
				moveDocument(SCROLL_STEP);
				isScrollingStarted = true;
				scrollStartClock.restart();
			}

			if (scrollStartClock.getElapsedTime().asMilliseconds() > SCROLL_START_DELAY)
			{
				isScrolling = true;
				isScrollingStarted = false;
			}
		}

		if (isScrolling && scrollClock.getElapsedTime().asMilliseconds() > SCROLL_STEP_TIME)
		{
			scrollClock.restart();
			moveDocument(SCROLL_STEP);
		}
	}

	if (!scrollLeft.is_pressed() && !scrollRight.is_pressed())
	{
		isScrollingStarted = false;
		isScrolling = false;
	}

	if (isSeized)  // Bar dragging
	{
		sf::Vector2i curPos = sf::Mouse::getPosition(window);
		setBarAbsolutePosition(curPos.x - relativeCurX);
		updateDocPosition();
	}
}

void gr::HorizontalScrollBar::render(sf::RenderWindow &window) const
{
	ScrollBar::render(window);  // Render the scrollbar
	scrollLeft.render(window);	// Render buttons
	scrollRight.render(window);  
	window.draw(arrowLeft);  // Render arrows
	window.draw(arrowRight);
}

void gr::HorizontalScrollBar::setDocumentPosition(long newDocPos)
{
	ScrollBar::setDocumentPosition(newDocPos);
	long newBarXPos = getBarRelativePosition() + xPos + INDENT + 1;
	//std::cout << docPosition << std::endl;
	setBarAbsolutePosition(newBarXPos);
}

void gr::HorizontalScrollBar::moveDocument(long offset)
{
	setDocumentPosition(docPosition + offset);
}

void gr::HorizontalScrollBar::setBarAbsolutePosition(long absolutePos)
{
	if (absolutePos < xPos + INDENT + 1)
	{
		setBarPosition(xPos + INDENT + 1, bar.getPosition().y);
	}
	else if (absolutePos >= xPos + width - INDENT - 1 - barWidth)
	{
		setBarPosition(xPos + width - INDENT - 1 - barWidth, bar.getPosition().y);
	}
	else
	{
		setBarPosition(absolutePos, bar.getPosition().y);
	}
}

void gr::HorizontalScrollBar::updateDocPosition()
{
	docPosition = scrollDistance * (bar.getPosition().x - xPos - INDENT - 1) / (width - 2 * INDENT - barWidth);
}

long gr::HorizontalScrollBar::getBarRelativePosition()
{
	return docPosition / scrollDistance * (width - 2 * INDENT - barWidth);
}

/* Functions */

sf::Color gr::blend(sf::Color color1, sf::Color color2, float coefficient)
{
	unsigned char r, g, b, a;

	r = color1.r + (color2.r - color1.r) * coefficient;
	g = color1.g + (color2.g - color1.g) * coefficient;
	b = color1.b + (color2.b - color1.b) * coefficient;
	a = color1.a + (color2.a - color1.a) * coefficient;

	return sf::Color(r, g, b, a);
}

sf::String gr::str(double val)
{
	std::wostringstream mem;
	mem << val;
	return mem.str();
}