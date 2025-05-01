/* Green Ray interface library */

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

namespace gr  // This is the main Green Ray namespace
{

/* A base class for all interactive stuff you can place into a window */
class Interactive
{
protected:
	static long existingObjectsNum;
	static long targetedObjectId;

	const long id;
	long xPos, yPos;
	long width, height;
	bool enabled;
public:
	/* Default conctructor */
	Interactive();

	/* Constructor
	x: x-coordinate of the left top corner of the object in pixels
	y: y-coordinate of the left top corner of the object in pixels
	w: width of the object in pixels
	h: height of the object in pixels */
	Interactive(long x, long y, long w, long h);

	/* Destructor */
	virtual ~Interactive();

	/* Enables the object, so it becomes interactive
	(the object is enabled by default) */
	void enable();

	/* Disables the object, so
	it becomes inaccessible */
	void disable();

	/* Picks an event from a window and refreshes the object data
	depending on its type and a current event
	w: a window object */
	virtual void refresh(sf::RenderWindow &w) =0;

	/* Renders the object at the window
	w: a window object */
	virtual void render(sf::RenderWindow &w) const =0;

	/* Returns a pointer to
	a copy of the object */
	virtual Interactive *clone() =0;

	/* Resets the object data to default.
	Should be called if the object is no longer
	in use. */
	virtual void reset();
};

/* A base class for colored objects */
class Colored
{
protected:
	sf::Color backgroundColor;
	sf::Color borderColor;
	sf::Color textColor;
public:
	/* Constructor
	bgc: background color
	bc: border color
	tc: text color */
	Colored(sf::Color bgc, sf::Color bc, sf::Color tc);

	/* Sets the object's background color
	bgc: background color */
	void setBackgroundColor(sf::Color bgc);

	/* Sets the object's border color
	bc: border color */
	void setBorderColor(sf::Color bc);

	/* Sets the object's text color */
	void setTextColor(sf::Color tc);
};

/* An SFML-dependent class which allows you to
create and manage a simple button in any window */
class Button: public Interactive, public Colored
{
protected:
	static const long SHADOW_OFFSET;
	static const long BORDER_THICKNESS;
	static const float ANIMATION_TIME;
	static const float FADE_TIME;
	static sf::Clock moveClock;
	static const float PI;
	
	sf::Clock fadeClock;
	sf::Text text;
	sf::Color selectColor;
	sf::RectangleShape main;
	sf::Vertex mainGradient[4], shadowGradient[4];
	sf::FloatRect textBounds;
	sf::Keyboard::Key boundKey;
	bool isPressed;
	bool isTriggered;
	bool isMoving;
	bool isFading;
	bool isTargeted;
	bool isMisclicked;
	bool isKeyBound;
	bool isBoundKeyPressed;
	int keyTriggeringPhase;

	/* Sets up fields that cannot be set up by
	the constructor (for system use only) */
	void setup();
public:
	/* Constructor
	x: x-coordinate of the left top corner of the object in pixels
	y: y-coordinate of the left top corner of the object in pixels
	w: width of the object in pixels
	h: height of the object in pixels
	t: text on the button
	f: sfml font object
	bgc: background color
	bc: border color
	tc: text color
	sc: select color */
	Button(long x, long y, long w, long h, sf::String t, sf::Font &f, sf::Color bgc,
		   sf::Color bc, sf::Color tc, sf::Color sc);

	/* Simplified constructor (default colors)
	x: x-coordinate
	y: y-coordinate
	w: width of the button in pixels
	h: height of the button in pixels 
	t: text on the button
	f: sfml font object */
	Button(long x, long y, long w, long h, sf::String t, sf::Font &f);

	/* Copy constructor
	b: class Button object */
	Button(Button &b);

	/* Renders the button at the window
	w: a window object */
	void render(sf::RenderWindow &w) const;

	/* Picks an event from a window
	and refreshes the button data
	a window object */
	void refresh(sf::RenderWindow &w);

	/* Returns a pointer to
	a copy of the object */
	Interactive *clone();

	/* Binds the button to a key
	k: sfml key object */
	void bindKey(sf::Keyboard::Key k);

	/* Unbindes the button from a key */
	void unbindKey();

	/* Sets the color with which the
	button is filled when the cursor
	is on it
	sc: select color */
	void setSelectColor(sf::Color sc);

	/* Sets the object's border color
	bc: border color */
	void setBorderColor(sf::Color bc);

	/* Sets the object's text color */
	void setTextColor(sf::Color tc);

	/* Resets the object data to default.
	Should be called if the object is no longer
	in use. */
	void reset();

	/* If the button is pressed, returns true,
	otherwise - false */
	inline bool is_pressed() const
	{return isPressed;}

	/* If the button has just been pressed and
	released, returns true, otherwise - false. */
	inline bool is_triggered() const
	{return isTriggered;}

	/* Changes the button text. */
	inline void setText(sf::String newText)
	{text.setString(newText);}
};

/* An SFML-dependent class which allows you to
create and manage a checkbox in any window */
class Checkbox: public Interactive, public Colored
{
private:
	static const long BORDER_THICKNESS;
	static const float SQUARE_RELATIVE_SIZE;
	static const long TEXT_INDENT;
	static const float FADE_TIME;

	bool state;
	bool isPressed;
	bool isMisclicked;
	bool isFading;
	bool isTargeted;
	sf::Text text;
	sf::Color squareColor;
	sf::RectangleShape main, square;
	sf::Vertex mainGradient[4], squareGradient[4];
	sf::Clock fadeClock;

	/* Sets up fields that cannot be set up by
	the constructor (for system use only) */
	void setup();
public:
	/* Constructor
	x: x-coordinate of the left top corner of the checkbox in pixels
	y: y-coordinate of the left top corner of the checkbox in pixels
	s: size of a side of the checkbox in pixels
	t: text near the checkbox
	f: sfml font object
	bgc: background color
	bc: border color
	tc: text color
	sc: square color */
	Checkbox(long x, long y, long s, sf::String t, sf::Font &f, sf::Color bgc,
			 sf::Color bc, sf::Color tc, sf::Color sc);

	/* Simplified constructor (default colors)
	x: x-coordinate of the left top corner of the checkbox in pixels
	y: y-coordinate of the left top corner of the checkbox in pixels
	s: size of a side of the checkbox in pixels
	t: text near the checkbox
	f: sfml font object */
	Checkbox(long x, long y, long s, sf::String t, sf::Font &f);

	/* Copy constructor
	c: class Checkbox object */
	
	Checkbox(Checkbox &c);

	/* Picks an event from a window
	and	refreshes the checkbox data
	w: a window object */
	void refresh(sf::RenderWindow &w);

	/* Renders the checkbox at the window
	w: a window object */
	void render(sf::RenderWindow &w) const;

	/* Returns a pointer to
	a copy of the object */
	Interactive *clone();

	/* Sets the object's background color
	bgc: background color */
	void setBackgroundColor(sf::Color bgc);

	/* Sets the object's border color
	bc: border color */
	void setBorderColor(sf::Color bc);

	/* Sets the object's text color */
	void setTextColor(sf::Color tc);

	/* Sets the color of the box's square
	dc: dot color */
	void setSquareColor(sf::Color sc);

	/* Resets the object data to default.
	Should be called if the object is no longer
	in use. */
	void reset();

	/* Sets a state of the box
	s: state */
	void setState(bool s);

	/* Returns true if the box is on,
	false otherwise */
	inline bool is_on() const
	{return state;}

	/* Changes the text near the checkbox */
	inline void setText(sf::String newText)
	{text.setString(newText);}
};

/* An SFML-dependent class which allows you to
create and manage a progressbar in any window */
class ProgressBar: public Interactive, public Colored
{
private:
	static const long BORDER_THICKNESS;
	static const float ANIMATION_TIME;
	static const float PI;

	float value, previousValue;
	sf::Color fillColor;
	sf::RectangleShape main, fill;
	sf::Vertex mainGradient[4];
	sf::Clock clock;
	bool isMoving;

	/* Sets up fields that cannot be set up by
	the constructor (for system use only) */
	void setup();
public:
	/* Constructor
	x: x-coordinate
	y: y-coordinate
	w: width of the bar in pixels
	h: height of the bar in pixels
	bgc: background color
	fc: fill color
	bc: border color */
	ProgressBar(long x, long y, long w, long h, sf::Color bgc, sf::Color fc, sf::Color bc);

	/* Simplified constructor (default colors)
	x: x-coordinate
	y: y-coordinate
	w: width of the button in pixels
	h: height of the button in pixels */
	ProgressBar(long x, long y, long w, long h);

	/* Copy constructor
	r: ProgressBar class object */
	ProgressBar(ProgressBar &r);

	/* Picks an event from a window
	and	refreshes the progress bar data
	w: a window object */
	void refresh(sf::RenderWindow &w);

	/* Renders the progress bar at the window
	w: a window object */
	void render(sf::RenderWindow &w) const;

	/* Returns a pointer to
	a copy of the object */
	Interactive *clone();

	/* Returns a value [0; 1] which is equal
	to the ratio of the length of the filled part 
	of the bar and the length of the whole bar */
	inline float getValue() const
	{return value;}

	/* Sets a ratio of the filled bar
	part length	and the whole bar length */
	void setValue(float newValue);

	/* Sets the object's background color
	bgc: background color */
	void setBackgroundColor(sf::Color bgc);

	/* Sets the object's border color
	bc: border color */
	void setBorderColor(sf::Color bc);

	/* Sets the color of the filled
	part of the progress bar
	fc: fill color*/
	void setFillColor(sf::Color fc);

	/* Resets the object data to default.
	Should be called if the object is no longer
	in use. */
	void reset();
};

/* An SFML-dependent class which allows you to
create and manage radiobuttons in any window */
class RadioButtonSet: public Interactive, public Colored
{
private:
	static const float RELATIVE_DOT_SIZE;
	static const long BORDER_THICKNESS;
	static const long TEXT_INDENT;
	static const float FADE_TIME;

	sf::Color dotColor;
	sf::CircleShape *main, *dot;
	sf::Text *text;
	sf::Clock fadeClock;
	bool isMisclicked;
	bool isFading;
	bool isSomeButtonTargeted;
	long numOfButtons;
	long indent;
	long value;
	long previousValue;
	long pressedButtonNum;
	long targetedButtonNum;

	/* Sets up fields that cannot be set up by
	the constructor (for system use only) */
	void setup(sf::String atext, sf::Font &afont);
public:
	/* Constructor
	x: x-coordinate
	y: y-coordinate
	n: number of buttons
	i: indent (distance between buttons)
	d: diameter of each button
	t: text
	bgc: background color
	bc: border color
	dc: dot color
	tc: text color
	f: font */
	RadioButtonSet(long x, long y, long n, long i, long d, sf::String t, sf::Font &f,
				   sf::Color bgc, sf::Color bc, sf::Color dc, sf::Color tc);

	/* Simplified constructor (default colors)
	x: x-coordinate
	y: y-coordinate
	n: number of buttons
	i: indent (distance between buttons)
	d: diameter of each button
	t: text
	f: font */
	RadioButtonSet(long x, long y, long n, long i, long d, sf::String t, sf::Font &f);

	/* Copy constructor
	r: RadioButtonSet class object */
	RadioButtonSet(RadioButtonSet &r);

	/* Destructor */
	~RadioButtonSet();

	/* Picks an event from a window
	and refreshes the object data
	w: a window object */
	void refresh(sf::RenderWindow &w);
	
	/* Renders the object at the window
	w: a window object */
	void render(sf::RenderWindow &w) const;

	/* Returns a number of the clicked button.
	The	first button's number is 0. */
	inline long getClickedButton() const
	{return value;}

	/* Returns a pointer to
	a copy of the object */
	Interactive *clone();

	/* Sets a number of button
	that should be clicked. The
	first buttons's number is 0.
	v: string value */
	inline void setClickedButton(long newClickedButton)
	{value = newClickedButton;}

	/* Sets the object's background color
	bgc: background color */
	void setBackgroundColor(sf::Color bgc);

	/* Sets the object's border color
	bc: border color */
	void setBorderColor(sf::Color bc);

	/* Sets the object's text color
	tc: text color */
	void setTextColor(sf::Color tc);

	/* Sets the buttons' dot color
	dc: dot color */
	void setDotColor(sf::Color dc);

	/* Resets the object data to default.
	Should be called if the object is no longer
	in use. */
	void reset();
};

/* An SFML-dependent class which allows you to
create and manage text fields in any window */

class TextField: public Interactive, public Colored
{
public:
	enum Mode
	{
		None       = 0b00000000,
		NoGradient = 0b00000001,
		NoFrame    = 0b00000010
	};
private:
	static const long BORDER_THICKNESS;
	static long activeFieldId;
	static sf::Uint32 enteredChar, previousEnteredChar;

	sf::String value;
	sf::Color selectColor;
	sf::RectangleShape main, highlightRectangle;
	sf::Vertex mainGradient[4];
	sf::Text text;
	sf::String defaultText;
	sf::Font &font;
	bool isPressed;
	bool isTargeted;
	bool isMisclicked;
	bool isEnterPressed;
	bool isEmpty;
	bool isBackspacePressed;
	short mode;

	/* Sets up fields that cannot be set up by
	the constructor (for system use only) */
	void setup();
public:
	/* Constructor 
	x: x-coordinate
	y: y-coordinate
	w: width of the field in pixels
	h: height of the field in pixels
	dt: text set in the field by default
	f: font
	bgc: background color
	bc: border color
	tc: text color
	sc: select color */
	TextField(long x, long y, long w, long h, sf::String dt, sf::Font &f, 
			  sf::Color bgc, sf::Color bc, sf::Color tc, sf::Color sc);

	/* Simplified constructor (default colors)
	x: x-coordinate
	y: y-coordinate
	w: width of the field in pixels
	h: height of the field in pixels
	dt: text set in the field by default
	f: font */
	TextField(long x, long y, long w, long h, sf::String dt, sf::Font &f);

	/* Copy constructor */
	TextField(TextField &r);

	/* Destructor */
	~TextField();

	/* Picks an event from a window
	and refreshes the field data
	w: a window object */
	void refresh(sf::RenderWindow &w);

	/* Renders the text field at the window
	w: a window object */
	void render(sf::RenderWindow &w) const;

	/* Returns the field's text */
	sf::String getText() const;

	/* Returns a pointer to
	a copy of the object */
	Interactive *clone();

	/* Sets a string to the field
	nt: new text */
	void setText(sf::String nt);

	/* Sets the field's background color
	bgc: background color */
	void setBackgroundColor(sf::Color bgc);

	/* Sets the field's border color
	bc: border color */
	void setBorderColor(sf::Color bc);

	/* Sets the color with which the
	field is filled when it's active
	sc: select color */
	void setSelectColor(sf::Color sc);

	/* Sets the entered character
	c: code of a key */
	static void setEnteredCharacter(sf::Uint32 c);

	/* Converts sfml key code
	into an ASCII-character */
	static char sfmlToAscii(long keyCode);

	/* Resets the object data to default.
	Should be called if the object is no longer
	in use. */
	void reset();

	/* Highlights a part of the string with color.
	start: index of the beginning of the highlighted substring
	length: length of the highlighted substring.
	color: color of the highlighted part of the string.
	If the length = -1,	highlights to the end of the string. */
	void highlight(long start, long length, sf::Color color);

	/* Removes the highlight of the text */
	void resetHighlight();

	/* Sets a new position of the text field */
	void setPosition(int x, int y);
	
	/* Sets mode flags */
	void setMode(short newFlags);

	inline bool is_empty()
	{return isEmpty;}

	inline bool is_enter_pressed()
	{return isEnterPressed;}

	inline bool is_backspace_pressed()
	{return isBackspacePressed;}

	inline void focus()
	{
		activeFieldId = id;
	}
};

/* An SFML-dependent class which allows you to
create and manage text labels in any window */
class Label: public Interactive, public Colored
{
public:
	enum Alignment {Left, Center, Right};
private:
	const Alignment alignment;
	sf::Text text;

	/* Sets up fields that cannot be set up by
	the constructor (for system use only) */
	void setup();
public:

	/* Constructor 
	x: x-coordinate
	y: y-coordinate
	s: character size
	t: text
	f: font
	tc: text color
	a: alignment */
	Label(long x, long y, long s, sf::String t, sf::Font &f, sf::Color tc, Alignment a=Left);

	/* Simplified constructor (default colors)
	x: x-coordinate
	y: y-coordinate
	s: character size
	t: text
	a: alignment
	f: font */
	Label(long x, long y, long s, sf::String t, sf::Font &f, Alignment a=Left);

	/* Do nothing
	w: a window object */
	void refresh(sf::RenderWindow &w);

	/* Renders the object at the window
	w: a window object */
	void render(sf::RenderWindow &w) const;

	/* Returns the string */
	inline sf::String getText() const
	{return text.getString();}

	/* Returns a pointer to
	a copy of the object */
	Interactive *clone();

	/* Sets new label text
	nt: new text */
	void setText(sf::String nt);

	/* Sets the object's text color
	tc: text color */
	void setTextColor(sf::Color tc);
};

/* An SFML-dependent class which allows you to
create and manage a textured button in any window */
class AdvancedButton: public Button
{
public:
	enum Mode
	{
		None              = 0b000000000,
		NoShadow          = 0b000000001,
		NoGradient        = 0b000000010,
		NoOffset          = 0b000000100,
		NoColorChange     = 0b000001000,
		NoTexture         = 0b000010000,
		NoText            = 0b000100000,
		Invisible         = 0b001000000,
		InstantTriggering = 0b010000000,
		DragClick         = 0b100000000,
	};
private:
	Mode mode;
	sf::Sprite sprite;

	/* Sets up fields that cannot be set up by
	the constructor (for system use only) */
	void setup();
public:
	/* Constructor
	x: x-coordinate of the left top corner of the button
	y: y-coordinate of the left top corner of the button
	w: width of the button in pixels
	h: height of the button in pixels
	t: text on the button
	f: sfml font object
	bgc: background color
	bc: border color
	tc: text color
	sc: select color
	tx: sfml texture object
	m: button mode parameter */
	AdvancedButton(long x, long y, long w, long h, sf::String t, sf::Font &f, sf::Color bgc,
				   sf::Color bc, sf::Color tc, sf::Color sc, sf::Texture &tx, long m=0);

	/* Copy constructor
	r: AdvancedButton class object */
	AdvancedButton(AdvancedButton &r);

	/* Renders the button at the window
	w: a window object */
	void render(sf::RenderWindow &w) const;

	/* Picks an event from a window
	and refreshes the button data
	a window object */
	void refresh(sf::RenderWindow &w);

	/* Returns a pointer to
	a copy of the object */
	Interactive *clone();

	/* Sets the object's background color
	bgc: background color */
	void setBackgroundColor(sf::Color bgc);
};

/* An SFML-dependent class which allows you to
create and manage a drop-down list in any window */

class DropdownList:public Interactive, public Colored
{
private:
	static const float RELATIVE_ARROW_SIZE;
	static const float ANIMATION_TIME;
	static const long BORDER_THICKNESS;
	static const float PI;

	sf::Color selectColor, arrowColor;
	sf::CircleShape arrow;
	sf::RectangleShape titleMain, *main;
	sf::Text titleText, *text;
	sf::Vertex **mainGradient, titleMainGradient[4];
	sf::Clock clock;
	bool isHidden;
	bool isArrowPressed;
	bool isMoving;
	bool isSomeButtonTargeted;
	bool isArrowMisclicked;
	bool isOptionMisclicked;
	long numOfOptions;
	long pressedButtonNum;
	long selectedOptionNum;

	/* Sets up fields that cannot be set up by
	the constructor (for system use only)
	aoptions: '\n'-separated and '\n'-terminated string
	which contains names of each option
	afont: an sfml font object */
	void setup(sf::String aoptions, sf::Font &afont);
public:
	/* Constructor
	x: x-coordinate
	y: y-coordinate
	w: width of each line
	h: height of each line
	n: number of options
	ti: title of the list
	tx: '\n'-separated and '\n'-terminated string
	which contains names of each option
	f: sfml font object
	bgc: background color
	bc: border color
	tc: text color
	sc: select color
	ac: arrow color
	*/
	DropdownList(long x, long y, long w, long h, long n, sf::String ti, sf::String tx,
				 sf::Font &f, sf::Color bgc, sf::Color bc, sf::Color tc, sf::Color sc,
				 sf::Color ac);

	/* Simplified constructor (default colors)
	x: x-coordinate
	y: y-coordinate
	w: width of each line
	h: height of each line
	n: number of options
	ti: title of the list
	tx: '\n'-separated and '\n'-terminated string
	which contains names of each option
	f: sfml font object */
	DropdownList(long x, long y, long w, long h, long n, sf::String ti, sf::String tx,
				 sf::Font &f);

	/* Copy constructor
	r: DropdownList class object */
	DropdownList(DropdownList &r);

	/* Destructor */
	~DropdownList();

	/* Picks an event from a window
	and refreshes the list's data
	w: a window object */
	void refresh(sf::RenderWindow &w);

	/* Renders the object at the window
	w: a window object */
	void render(sf::RenderWindow &w) const;

	/* Returns a string which contains
	information about interaction with user */
	inline long getSelectedOption() const
	{return selectedOptionNum;}

	/* Returns a pointer to
	a copy of the object */
	Interactive *clone();

	/* Sets a selected option number
	nn: new option number */
	void setSelectedOption(long nn);

	/* Sets the object's background color
	bgc: background color */
	void setBackgroundColor(sf::Color bgc);

	/* Sets the object's border color
	bc: border color */
	void setBorderColor(sf::Color bc);

	/* Sets the object's text color */
	void setTextColor(sf::Color tc);

	/* Sets the color of the triangle
	ac: arrow color */
	void setArrowColor(sf::Color ac);

	/* Sets the color of the
	selected option button
	sc: select color */
	void setSelectColor(sf::Color sc);

	/* Resets the object data to default.
	Should be called if the object is no longer
	in use. */
	void reset();
};

class Slider: public Interactive, public Colored
{
public:
	enum Mode
	{
		None         = 0b00,
		MaxPresicion = 0b01,
	};
private:
	static const long LINE_THICKNESS;
	static const long ARROW_RADIUS;
	static const long ARROW_BORDER_THICKNESS;
	static const long CHARACTER_SIZE;

	Mode mode;
	sf::Color fillColor, arrowColor;
	sf::CircleShape arrow;
	sf::RectangleShape main, fill;
	sf::Vertex mainGradient[4];
	sf::Text leftText, rightText, valueText;
	bool isSeized;
	bool isMisclicked;
	bool isTargeted;
	float left;
	float right;
	float value;
	float precision;

	/* Sets up fields that cannot be set up by
	the constructor (for system use only)
	aoptions: '\n'-separated and '\n'-terminated string
	which contains names of each option
	afont: an sfml font object */
	void setup(sf::Font &afont);
public:
	/* Constructor
	x: x-position
	y: y-position
	w: width (length) of the slider
	l: left slider value
	r: right slider value
	p: presicion (ignored if Int of MaxPresicion flags are up)
	f: sfml Font class object
	bgc: background color
	fc: fill color
	ac: arrow color
	bc: border color
	tc: text color
	m: mode of the slider */
	Slider(long x, long y, long w, float l, float r, float p, sf::Font &f,
		   sf::Color bgc, sf::Color fc, sf::Color ac, sf::Color bc, sf::Color tc,
		   long m=None);

	/* Simplified constructor
	x: x-position
	y: y-position
	w: width (length) of the slider
	l: left slider value
	r: right slider value
	p: presicion (ignored if Int of MaxPresicion flags are up)
	f: sfml Font class object
	m: mode of the slider */
	Slider(long x, long y, long w, float l, float r, float p, sf::Font &f, long m=None);

	/* Copy constructor
	r: Slider class object */
	Slider(Slider &r);

	/* Picks an event from a window
	and refreshes the slider data
	w: a window object */
	void refresh(sf::RenderWindow &w);

	/* Renders the object at the window
	w: a window object */
	void render(sf::RenderWindow &w) const;

	/* Returns a current value of the slider */
	inline float getValue() const
	{return value;}

	/* Returns a pointer to
	a copy of the object */
	Interactive *clone();

	/* Sets a value to the slider
	nv: new value */
	void setValue(float nv);

	/* Sets the object's background color
	bgc: background color */
	void setBackgroundColor(sf::Color bgc);

	/* Sets the object's border color
	bc: border color */
	void setBorderColor(sf::Color bc);

	/* Sets the object's text color */
	void setTextColor(sf::Color tc);

	/* Sets the color of the arrow
	ac: arrow color */
	void setArrowColor(sf::Color ac);

	/* Sets the color with which the line
	on the left from the slider is filled
	fc: fill color */
	void setFillColor(sf::Color fc);

	/* Resets the object data to default.
	Should be called if the object is no longer
	in use. */
	void reset();
};

class ScrollBar: public Interactive, public Colored
{
public: 
	enum Orientation {Horizontal, Vertical};
protected:
	static const long INDENT;
	static const long ANIMATION_TIME;
	static const long SCROLL_STEP;
	static const long SCROLL_STEP_TIME;
	static const long SCROLL_START_DELAY;
	static const float RELATIVE_ARROW_SIZE;

	sf::RectangleShape main, bar;
	sf::Color barColor, barBorderColor, barSelectColor;
	sf::Vertex mainGradient[4], barGradient[4];
	sf::Clock fadeClock, scrollClock, scrollStartClock;
	sf::Texture useless;

	float scrollDistance;
	float docPosition;
	long barWidth, barHeight;
	float relativeBarPos;
	float relativeCurX;
	float relativeCurY;

	bool orientation;
	bool isSeized;
	bool isMisclicked;
	bool isTargeted;
	bool isBarTargeted;
	bool isFading;
	bool isScrollingStarted;
	bool isScrolling;

	/* Sets up fields that cannot be set up by
	the constructor (for system use only) */
	void setup();
public:
	/* Constructor
	x: x-position of the empty space rectangle
	y: y-position of -//-
	w: width of -//-
	h: height of -//-
	sd: scroll distance
	orientation: false = horizontal, true = vertical */
	ScrollBar(long x, long y, long w, long h, float sd, bool orientation);

	/* Copy constructor */
	ScrollBar(ScrollBar &right);

	/* Renders the scroll bar at the window
	w: a window object */
	void render(sf::RenderWindow &w) const;

	/* Detects grabbing the scrollbar
	w: a window object */
	void refresh(sf::RenderWindow &w);

	/* Changes the color of a piece of interface
	when targeted */
	void updateColor();

	/* Resets the object data to default.
	Should be called if the object is no longer
	in use. */
	void reset();

	/* Moves the bar and its gradient to a
	specified position */
	void setBarPosition(long x, long y);

	/* Returns a position of the upper border
	of the visible part of the scrollable
	content in pixels */
	inline long getDocumentPosition()
	{return docPosition;}

	/* Set a new document position */
	void setDocumentPosition(long newDocPos);
};

class VerticalScrollBar: public ScrollBar
{
private:
	AdvancedButton scrollUp, scrollDown;
	sf::CircleShape arrowUp, arrowDown;

	/* Sets up fields that cannot be set up by
	the constructor (for system use only) */
	void setup(float arrow_radius);
public:
	/* Constructor
	x: x-position of the scroll up button
	y: y-position of -//-
	w: width of the whole object
	l: length of -//-
	sd: scroll distance
	f: sfml font object */
	VerticalScrollBar(long x, long y, long l, long w, float sd, sf::Font &f);

	/* Copy constructor */
	VerticalScrollBar(VerticalScrollBar &right);

	/* Returns a pointer to
	a copy of the object */
	Interactive *clone();

	/* Refreshes the object */
	void refresh(sf::RenderWindow &w);

	/* Renders the scroll bar and its buttons at the window
	w: a window object */
	void render(sf::RenderWindow &w) const;

	/* Moves the scrollbar as if the scrollable
	content	were shifted by the specified value.
	Does not allow the bar to leave the outer rectangle. */
	void moveDocument(long offset);

	/* Set a new document position and,
	according to it, moves the scrollbar */
	void setDocumentPosition(long newDocPos);

	/* Sets the bar absolute position.
	Does not allow the bar to leave the outer rectangle. */
	void setBarAbsolutePosition(long absolutePos);

	/* Sets the document position value, according
	to the position of the scroll bar */
	void updateDocPosition();

	/* Returns a coordinate relative to the background
	rectangle at which the scroll bar should be */
	long getBarRelativePosition(); 
};

class HorizontalScrollBar: public ScrollBar
{
private:
	AdvancedButton scrollLeft, scrollRight;
	sf::CircleShape arrowLeft, arrowRight;

	/* Sets up fields that cannot be set up by
	the constructor (for system use only) */
	void setup(float arrow_radius);
public:
	/* Constructor
	x: x-position of the scroll up button
	y: y-position of -//-
	w: width of the background empty space
	l: length of -//-
	sd: scroll distance
	f: sfml font object */
	HorizontalScrollBar(long x, long y, long l, long w, float sd, sf::Font &f);

	/* Copy constructor */
	HorizontalScrollBar(HorizontalScrollBar &right);

	/* Returns a pointer to
	a copy of the object */
	Interactive *clone();

	/* Refreshes the object */
	void refresh(sf::RenderWindow &w);

	/* Renders the scroll bar and its buttons at the window
	w: a window object */
	void render(sf::RenderWindow &w) const;

	/* Moves the scrollbar by the specified offset.
	Does not allow the bar to leave the outer rectangle. */
	void moveDocument(long offset);

	/* Set a new document position and,
	according to it, moves the scrollbar */
	void setDocumentPosition(long newDocPos);

	/* Sets the bar absolute position.
	Does not allow the bar to leave the outer rectangle. */
	void setBarAbsolutePosition(long relativePos);

	/* Sets the document position value, according
	to the position of the scroll bar */
	void updateDocPosition();

	/* Returns a coordinate relative to the background
	rectangle at which the scroll bar should be */
	long getBarRelativePosition(); 
};

/* Functions */

/* Returns an intermediate
color between two given
c1: color 1
c2: color 2
v: value [0.0; 1.0] */
sf::Color blend(sf::Color c1, sf::Color c2, float v);

/* Converts a numeric value to string */
sf::String str(double val);

}