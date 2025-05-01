#include <sstream>
#include "GreenRayWindow.h"
#include "GreenRayInterface.h"
#include "Sequence.h"
#include "Histogram.h"

std::wstring str(float);
bool isNumber(std::string);
void eraseExtension(std::string&);
void eraseExtension(std::wstring&);
std::string toASCIIString(const std::wstring&);

int main()
{
	// Константы
	const long WIDTH = 800, HEIGHT = 600;

	// Начальные настройки
	gr::Window::hideConsole();
	gr::Window window(WIDTH, HEIGHT, L"Анализатор последовательностей", sf::Style::Close);
	gr::Window graphWin(WIDTH, HEIGHT, L"Гистограмма", sf::Style::Titlebar);
	window.setPosition(200, 250);
	graphWin.setPosition(1000, 250);
	sf::Font font;
	font.loadFromFile("Arial.ttf");
	sf::Clock operationTime;
	
	// Интерфейс
	gr::Label sourceFileLabel(10, 10, 20, L"Входной файл", font);
	gr::Label resultLabel(600, 10, 20, L"Результаты анализа", font, gr::Label::Center);
	gr::Label countLabel(410, 40, 20, L"Всего элементов:", font);
	gr::Label minLabel(410, 70, 20, L"Минимальный: ", font);
	gr::Label maxLabel(410, 100, 20, L"Максимальный: ", font);
	gr::Label avgLabel(410, 130, 20, L"Среднее значение: ", font);
	gr::Label normalDeviationLabel(410, 160, 20, L"Ср. кв. отклонение: ", font);
	gr::Label dispersionLabel(410, 190, 20, L"Дисперсия: ", font);
	gr::Label rangeCountLabel(410, 220, 20, L"Кол-во диапазонов: ", font);
	gr::Label rangeWidthLabel(410, 250, 20, L"Ширина диапазона: ", font);
	gr::Label lrBoundsLabel(10, 40, 20, L"Гор. границы", font);
	gr::Label btBoundsLabel(10, 70, 20, L"Верт. границы", font);
	gr::Label scaleDivLabel(10, 100, 20, L"Ц. деления X/Y", font);
	gr::Label precisionLabel(10, 130, 20, L"Точность X/Y", font);
	gr::Label histogramColorLabel(10, 190, 20, L"Цвет заливки", font);
	gr::Label documentSizeLabel(10, 280, 20, L"Размер документа", font);
	gr::Label lrIndentLabel(10, 310, 20, L"Отступ лев./прав.", font);
	gr::Label btIndentLabel(10, 340, 20, L"Отступ ниж./верх.", font);
	gr::Label xLabelLabel(10, 400, 20, L"Подпись оси Х", font);
	gr::Label yLabelLabel(10, 430, 20, L"Подпись оси Y", font);
	gr::Label graphLabelLabel(10, 460, 20, L"Подпись графика", font);
	gr::Label formatLabel(10, 490, 20, L"Формат итогового документа", font);
	gr::Label statusBar(400, 560, 20, "", font, gr::Label::Center);
	gr::Label gaussianSegmentCountLabel(410, 310, 20, L"Сегменты кривой", font);
	gr::Label numberSizeLabel(410, 340, 20, L"Размер чисел", font);
	gr::Label labelSizeLabel(410, 370, 20, L"Подписей осей", font);
	gr::Label graphLabelSizeLabel(410, 400, 20, L"Подписи графика", font);
	gr::Label graphLabelCoordLabel(10, 370, 20, L"Позиция подписи", font);
	gr::Label projectNameLabel(410, 490, 20, L"Имя проекта", font);
	gr::TextField fileNameField(200, 10, 200, 20, L"Имя файла", font);
	gr::TextField leftBoundField(200, 40, 95, 20, L"Л. граница", font);
	gr::TextField rightBoundField(305, 40, 95, 20, L"П. граница", font);
	gr::TextField bottomBoundField(200, 70, 95, 20, L"Н. граница", font);
	gr::TextField topBoundField(305, 70, 95, 20, L"В. граница", font);
	gr::TextField scaleDivXField(200, 100, 95, 20, L"Ц. деления X", font);
	gr::TextField scaleDivYField(305, 100, 95, 20, L"Ц. деления Y", font);
	gr::TextField precisionXField(200, 130, 95, 20, L"Точность X", font);
	gr::TextField precisionYField(305, 130, 95, 20, L"Точность Y", font);
	gr::TextField documentWidthField(200, 280, 95, 20, L"Ширина", font);
	gr::TextField documentHeightField(305, 280, 95, 20, L"Высота", font);
	gr::TextField leftIndentField(200, 310, 95, 20, L"Лев. отступ", font);
	gr::TextField rightIndentField(305, 310, 95, 20, L"Прав. отступ", font);
	gr::TextField botIndentField(200, 340, 95, 20, L"Ниж. отступ", font);
	gr::TextField topIndentField(305, 340, 95, 20, L"Верх. отступ", font);
	gr::TextField xLabelField(200, 400, 200, 20, L"Подпись оси Х", font);
	gr::TextField yLabelField(200, 430, 200, 20, L"Подпись оси Y", font);
	gr::TextField graphLabelField(200, 460, 590, 20, L"Подпись графика", font);
	gr::TextField graphLabelXPosField(200, 370, 95, 20, L"X", font);
	gr::TextField graphLabelYPosField(305, 370, 95, 20, L"Y", font);
	gr::TextField projectNameField(600, 490, 190, 20, L"Имя проекта", font);
	gr::DropdownList formatList(305, 490, 95, 20, 3, L"Формат", L"PNG\nJPEG\n\BMP\n", font);
	gr::Slider redSlider(200, 190, 200, 0, 255, 1, font);
	gr::Slider greenSlider(200, 220, 200, 0, 255, 1, font);
	gr::Slider blueSlider(200, 250, 200, 0, 255, 1, font);
	gr::Slider gaussianSegmentCountSlider(600, 310, 190, 100, 500, 50, font);
	gr::Slider numberSizeSlider(600, 340, 190, 1, 72, 1, font);
	gr::Slider labelSizeSlider(600, 370, 190, 1, 72, 1, font);
	gr::Slider graphLabelSizeSlider(600, 400, 190, 1, 72, 1, font);
	gr::Button analyzeButton(20, 530, 110, 20, L"Анализ", font);
	gr::Button plotButton(150, 530, 120, 20, L"Построить", font);
	gr::Button exportButton(290, 530, 110, 20, L"Экспорт", font);
	gr::Button loadButton(420, 530, 110, 20, L"Загрузить", font);
	gr::Button saveButton(550, 530, 110, 20, L"Сохранить", font);
	gr::Button resetButton(680, 530, 110, 20, L"Сбросить", font);
	gr::Checkbox drawGaussianCheckbox(10, 160, 20, L"Построить кривую распределения", font);
	sf::RectangleShape colorSample(sf::Vector2f(120, 60));

	// Начальные параметры интерфейса
	redSlider.setFillColor(sf::Color(255, 0, 0));
	redSlider.setArrowColor(sf::Color(255, 0, 0));
	redSlider.setValue(255);
	greenSlider.setFillColor(sf::Color(0, 255, 0));
	greenSlider.setArrowColor(sf::Color(0, 255, 0));
	greenSlider.setValue(255);
	blueSlider.setFillColor(sf::Color(0, 0, 255));
	blueSlider.setArrowColor(sf::Color(0, 0, 255));
	blueSlider.setValue(255);
	gaussianSegmentCountSlider.setValue(100);
	numberSizeSlider.setValue(16);
	labelSizeSlider.setValue(16);
	graphLabelSizeSlider.setValue(16);
	std::ostringstream mem;
	mem << WIDTH / 2;
	graphLabelXPosField.setText(mem.str());
	mem.str("");
	mem << HEIGHT / 2;
	graphLabelYPosField.setText(mem.str());
	mem.str("");
	leftIndentField.setText("100");
	rightIndentField.setText("50");
	botIndentField.setText("50");
	topIndentField.setText("50");
	colorSample.setOutlineThickness(0);
	colorSample.setPosition(10, 220);
	mem << WIDTH;
	documentWidthField.setText(mem.str());
	mem.str("");
	mem << HEIGHT;
	documentHeightField.setText(mem.str());
	mem.str("");
	
	window << redSlider << greenSlider << blueSlider << formatList << sourceFileLabel <<
		resultLabel << fileNameField << graphLabelCoordLabel <<
		analyzeButton << minLabel << maxLabel << avgLabel << normalDeviationLabel <<
		countLabel << leftBoundField << rightBoundField <<
		bottomBoundField <<	topBoundField << scaleDivXField << scaleDivYField <<
		precisionXField << precisionYField << plotButton << exportButton << lrBoundsLabel <<
		btBoundsLabel << scaleDivLabel << precisionLabel << histogramColorLabel <<
		lrIndentLabel << btIndentLabel << leftIndentField <<
		rightIndentField << botIndentField << topIndentField << xLabelLabel <<
		yLabelLabel << graphLabelLabel << xLabelField << yLabelField << graphLabelField <<
		formatLabel << statusBar << graphLabelXPosField << graphLabelYPosField <<
		numberSizeLabel << labelSizeLabel << graphLabelSizeLabel << projectNameLabel <<
		projectNameField << loadButton << saveButton << resetButton << numberSizeSlider <<
		labelSizeSlider << graphLabelSizeSlider << colorSample << rangeCountLabel <<
		rangeWidthLabel << dispersionLabel << documentSizeLabel << documentWidthField <<
		documentHeightField << drawGaussianCheckbox << gaussianSegmentCountLabel <<
		gaussianSegmentCountSlider;

	sa::Histogram histogram(800, 600, font);
	sa::Sequence seq;

	while (window.work())
	{
		graphWin.work();  // Работа вспомогательного окна

		/* Обновить цвет образца */
		colorSample.setFillColor(sf::Color(
			redSlider.getValue(),
			greenSlider.getValue(),
			blueSlider.getValue()));

		/* Если нажата кнопка анализа */
		if (analyzeButton.is_triggered())
		{
			operationTime.restart();  // Запустить счётчик времени

			/* Если данные удалось считать */
			if (seq.loadFromFile(fileNameField.getText()))
			{
				/* Вывести данные */
				countLabel.setText(L"Всего элементов: " + str(seq.getCount()));
				minLabel.setText(L"Минимальный: " + str(seq.getMin()));
				maxLabel.setText(L"Максимальный: " + str(seq.getMax()));
				avgLabel.setText(L"Среднее значение: " + str(seq.getAverage()));
				normalDeviationLabel.setText(L"Ср. кв. отклонение: " + str(seq.getNormalDeviation()));
				dispersionLabel.setText(L"Дисперсия: " + str(seq.getDispersion()));
				rangeCountLabel.setText(L"Кол-во диапазонов: " + str(seq.getRangeCount()));
				rangeWidthLabel.setText(L"Ширина диапазона: " + str(seq.getRangeWidth()));

				/* Вывести затраченное время */
				statusBar.setTextColor(sf::Color::Black);
				statusBar.setText(L"Затраченное время: " +
								   str(operationTime.getElapsedTime().asMicroseconds()) +
								   L" мкс");
			}
			/* Если данные не удалось считать */
			else
			{
				statusBar.setTextColor(sf::Color::Red);
				statusBar.setText(L"Невозможно открыть файл");
			}
		}

		/* Если нажата кнопка построения */
		if (plotButton.is_triggered())
		{
			operationTime.restart();

			/* Если данные введены верно */
			if (isNumber(leftBoundField.getText()) &&
				isNumber(rightBoundField.getText()) &&
				isNumber(bottomBoundField.getText()) &&
				isNumber(topBoundField.getText()) &&
				isNumber(scaleDivXField.getText()) &&
				isNumber(scaleDivYField.getText()) &&
				isNumber(precisionXField.getText()) &&
				isNumber(precisionYField.getText()) &&
				isNumber(leftIndentField.getText()) &&
				isNumber(rightIndentField.getText()) &&
				isNumber(botIndentField.getText()) &&
				isNumber(topIndentField.getText()) &&
				isNumber(graphLabelXPosField.getText()) &&
				isNumber(graphLabelYPosField.getText()) &&
				isNumber(documentWidthField.getText()) &&
				isNumber(documentHeightField.getText()))
			{
				/* Поменять размер окна */

				graphWin.setSize(std::atol(documentWidthField.getText().toAnsiString().c_str()),
								 std::atol(documentHeightField.getText().toAnsiString().c_str()));

				/* Поменять размер гистограммы */
				histogram.setSize(std::atol(documentWidthField.getText().toAnsiString().c_str()),
								  std::atol(documentHeightField.getText().toAnsiString().c_str()));

				/* Построить гистограмму */
				histogram.setLeftBound(std::atof(leftBoundField.getText().toAnsiString().c_str()));
				histogram.setRightBound(std::atof(rightBoundField.getText().toAnsiString().c_str()));
				histogram.setBottomBound(std::atof(bottomBoundField.getText().toAnsiString().c_str()));
				histogram.setTopBound(std::atof(topBoundField.getText().toAnsiString().c_str()));
				histogram.setScaleDivision(std::atof(scaleDivXField.getText().toAnsiString().c_str()),
										   std::atof(scaleDivYField.getText().toAnsiString().c_str()));
				histogram.setPrecision(std::atof(precisionXField.getText().toAnsiString().c_str()),
									   std::atof(precisionYField.getText().toAnsiString().c_str()));
				histogram.setLeftIndent(std::atof(leftIndentField.getText().toAnsiString().c_str()));
				histogram.setRightIndent(std::atof(rightIndentField.getText().toAnsiString().c_str()));
				histogram.setBottomIndent(std::atof(botIndentField.getText().toAnsiString().c_str()));
				histogram.setTopIndent(std::atof(topIndentField.getText().toAnsiString().c_str()));
				histogram.setXLabel(xLabelField.getText());
				histogram.setYLabel(yLabelField.getText());
				histogram.setGraphLabel(graphLabelField.getText());
				histogram.setFillColor(sf::Color(
					redSlider.getValue(),
					greenSlider.getValue(),
					blueSlider.getValue()));
				histogram.setGraphLabelPos(std::atof(graphLabelXPosField.getText().toAnsiString().c_str()),
										   std::atof(graphLabelYPosField.getText().toAnsiString().c_str()));
				histogram.setNumberSize(numberSizeSlider.getValue());
				histogram.setLabelSize(labelSizeSlider.getValue());
				histogram.setGraphLabelSize(graphLabelSizeSlider.getValue());
				histogram.setGaussianBuilding(drawGaussianCheckbox.is_on());
				histogram.setGaussianSegmentCount(gaussianSegmentCountSlider.getValue());
				histogram.drawHistogram(seq);

				/* Вывести в окно */
				graphWin.clearSpriteBuffer();  // Обязательно очистить буфер спрайтов
				graphWin << histogram;  // Загрузить новые спрайты в окно
				
				/* Вывести затраченное время */
				statusBar.setTextColor(sf::Color::Black);
				statusBar.setText(L"Затраченное время: " +
								   str(operationTime.getElapsedTime().asMicroseconds()) +
								   L" мкс");
			}
			/* Если данные введены ошибочно */
			else
			{
				statusBar.setTextColor(sf::Color::Red);
				statusBar.setText(L"Числовые параметры в настройках введены неверно");
			}			
		}

		/* Если нажата кнопка экспорта документа */
		if (exportButton.is_triggered())
		{
			operationTime.restart();

			sf::Texture screenShot;
			sf::Vector2u size = graphWin.getSfmlWindow().getSize();
			screenShot.create(size.x, size.y);
			screenShot.update(graphWin.getSfmlWindow());
			sf::Image document = screenShot.copyToImage();
			std::string fileName = toASCIIString(fileNameField.getText());
			eraseExtension(fileName);

			switch (formatList.getSelectedOption())
			{
				case 0:
					fileName += ".png";
					break;
				case 1:
					fileName += ".jpeg";
					break;
				case 2:
					fileName += ".bmp";
					break;
				default:
					fileName += ".png";
					break;
			}

			document.saveToFile(std::string(fileName));

			/* Вывести затраченное время */
			statusBar.setTextColor(sf::Color::Black);
			statusBar.setText(L"Затраченное время: " +
							  str(operationTime.getElapsedTime().asMicroseconds()) +
							  L" мкс");
		}

		/* Если нажата кнопка сохранения проекта */
		if (saveButton.is_triggered())
		{
			operationTime.restart();

			if (projectNameField.getText() != "")
			{
				std::wstring fileName = projectNameField.getText().toWideString();
				eraseExtension(fileName);
				std::wofstream file(fileName + L".sap");
				file << histogram << seq;
				file.close();

				/* Вывести затраченное время */
				statusBar.setTextColor(sf::Color::Black);
				statusBar.setText(L"Затраченное время: " +
								  str(operationTime.getElapsedTime().asMicroseconds()) +
								  L" мкс");
			}
			else
			{
				statusBar.setTextColor(sf::Color::Red);
				statusBar.setText(L"Указано пустое имя проекта");
			}
		}

		/* Если нажата кнопка загрузки проекта */
		if (loadButton.is_triggered())
		{
			operationTime.restart();

			std::wstring fileName = projectNameField.getText().toWideString();
			eraseExtension(fileName);
			std::wifstream file(fileName + L".sap");

			/* Если файл успешно открылся */
			if (file)
			{
				/* Прочитать */
				statusBar.setText(L"");
				file >> histogram;
				file >> seq;
				file.close();

				/* Установить значения в поля ввода */
				leftBoundField.setText(str(histogram.leftBound));
				rightBoundField.setText(str(histogram.rightBound));
				bottomBoundField.setText(str(histogram.bottomBound));
				topBoundField.setText(str(histogram.topBound));
				scaleDivXField.setText(str(histogram.scaleDivX));
				scaleDivYField.setText(str(histogram.scaleDivY));
				precisionXField.setText(str(histogram.precisionX));
				precisionYField.setText(str(histogram.precisionY));
				redSlider.setValue(histogram.fillColor.r);
				greenSlider.setValue(histogram.fillColor.g);
				blueSlider.setValue(histogram.fillColor.b);
				documentWidthField.setText(str(histogram.width));
				documentHeightField.setText(str(histogram.height));
				leftIndentField.setText(str(histogram.indentLeft));
				rightIndentField.setText(str(histogram.indentRight));
				botIndentField.setText(str(histogram.indentBottom));
				topIndentField.setText(str(histogram.indentTop));
				xLabelField.setText(histogram.xLabelString);
				yLabelField.setText(histogram.yLabelString);
				graphLabelField.setText(histogram.graphLabelString);
				numberSizeSlider.setValue(histogram.numberSize);
				labelSizeSlider.setValue(histogram.labelSize);
				graphLabelSizeSlider.setValue(histogram.graphLabelSize);
				graphLabelXPosField.setText(str(histogram.graphLabelPosX));
				graphLabelYPosField.setText(str(histogram.graphLabelPosY));
				drawGaussianCheckbox.setState(histogram.doGaussianBuilding);
				gaussianSegmentCountSlider.setValue(histogram.gaussianSegmentCount);

				/* Вывести данные */
				countLabel.setText(L"Всего элементов: " + str(seq.getCount()));
				minLabel.setText(L"Минимальный: " + str(seq.getMin()));
				maxLabel.setText(L"Максимальный: " + str(seq.getMax()));
				avgLabel.setText(L"Среднее значение: " + str(seq.getAverage()));
				normalDeviationLabel.setText(L"Ср. кв. отклонение: " + str(seq.getNormalDeviation()));
				dispersionLabel.setText(L"Дисперсия: " + str(seq.getDispersion()));
				rangeCountLabel.setText(L"Кол-во диапазонов: " + str(seq.getRangeCount()));
				rangeWidthLabel.setText(L"Ширина диапазона: " + str(seq.getRangeWidth()));

				/* Изменить размер окна */
				graphWin.setSize(std::atol(documentWidthField.getText().toAnsiString().c_str()),
								 std::atol(documentHeightField.getText().toAnsiString().c_str()));

				/* Построить гистограмму и гауссиану */
				histogram.drawHistogram(seq);

				/* Вывести в окно */
				graphWin.clearSpriteBuffer();  // Обязательно очистить буфер спрайтов
				graphWin << histogram;  // Загрузить новые спрайты в окно

				/* Вывести затраченное время */
				statusBar.setTextColor(sf::Color::Black);
				statusBar.setText(L"Затраченное время: " +
								  str(operationTime.getElapsedTime().asMicroseconds()) +
								  L" мкс");
			}
			else
			{
				statusBar.setTextColor(sf::Color::Red);
				statusBar.setText(L"Невозможно открыть файл проекта");
			}
		}

		/* Если нажата кнопка сброса данных */
		if (resetButton.is_triggered())
		{
			operationTime.restart();

			/* Сбросить данные последовательности и гистограммы */
			seq.clear();
			histogram.clear();

			/* Сбросить интерфейс */
			fileNameField.setText("");
			leftBoundField.setText("");
			rightBoundField.setText("");
			bottomBoundField.setText("");
			topBoundField.setText("");
			scaleDivXField.setText("");
			scaleDivYField.setText("");
			precisionXField.setText("");
			precisionYField.setText("");
			redSlider.setValue(255);
			greenSlider.setValue(255);
			blueSlider.setValue(255);
			mem << WIDTH;
			documentWidthField.setText(mem.str());
			mem.str("");
			mem << HEIGHT;
			documentHeightField.setText(mem.str());
			mem.str("");
			leftIndentField.setText("100");
			rightIndentField.setText("50");
			botIndentField.setText("50");
			topIndentField.setText("50");
			xLabelField.setText("");
			yLabelField.setText("");
			graphLabelField.setText("");
			numberSizeSlider.setValue(16);
			labelSizeSlider.setValue(16);
			graphLabelSizeSlider.setValue(16);
			mem << WIDTH / 2;
			graphLabelXPosField.setText(mem.str());
			mem.str("");
			mem << HEIGHT / 2;
			graphLabelYPosField.setText(mem.str());
			mem.str("");
			drawGaussianCheckbox.setState(false);
			gaussianSegmentCountSlider.setValue(100);

			/* Стереть данные анализа */
			countLabel.setText(L"Всего элементов: ");
			minLabel.setText(L"Минимальный: ");
			maxLabel.setText(L"Максимальный: ");
			avgLabel.setText(L"Среднее значение: ");
			normalDeviationLabel.setText(L"Ср. кв. отклонение: ");
			dispersionLabel.setText(L"Дисперсия: ");
			rangeCountLabel.setText(L"Кол-во диапазонов: ");
			rangeWidthLabel.setText(L"Ширина диапазона: ");

			/* Очистить окно и изменить его размер на первоначальный */
			graphWin.clearSpriteBuffer();
			graphWin.setSize(WIDTH, HEIGHT);

			/* Вывести затраченное время */
			statusBar.setTextColor(sf::Color::Black);
			statusBar.setText(L"Затраченное время: " +
							  str(operationTime.getElapsedTime().asMicroseconds()) +
							  L" мкс");
		}
	}
}

std::wstring str(float arg)
{
	std::wostringstream mem;
	mem << arg;
	return mem.str();
}

bool isNumber(std::string arg)
{
	if (arg.size() == 0)
	{
		return false;
	}

	if (arg[0] == '-')
	{
		arg.erase(0, 1);
	}

	if (arg.size() == 0)
	{
		return false;
	}

	if (arg.find_first_not_of("0123456789.") != -1)
	{
		return false;
	}

	return true;
}

void eraseExtension(std::string &arg)
{
	if (arg.rfind('.') != -1)
	{
		arg.erase(arg.rfind('.'), -1);
	}
}

void eraseExtension(std::wstring &arg)
{
	if (arg.rfind('.') != -1)
	{
		arg.erase(arg.rfind('.'), -1);
	}
}

std::string toASCIIString(const std::wstring& wide)
{
	std::string result;
	for (long i = 0; i < wide.size(); i++)
	{
		if (wide[i] < 128)
		{
			result += wide[i];
		}
		else
		{
			result += '_';
		}
	}

	return result;
}